#include "structure/data_utility.h"
#include "structure/data_struct.h"
#include "structure/point_set.h"
#include "structure/tuple_set.h"
#include "structure/define.h"
#include <vector>
#include <ctime>
#include <sys/time.h>
#include "UH/maxUtility.h"
#include "structure/cluster_t.h"
#include "Groundtruth/Groundtruth.h"
#include "ActiveRanking/active_ranking.h"
#include "Adaptive/adaptive.h"
#include "Special/Tree.h"
#include "General/General.h"

int main(int argc, char *argv[])
{
    ifstream config("../config.txt");
    char data_name[MAX_FILENAME_LENG]; double Para, selectRatio, differRatio; int mode;
    config >> data_name >> Para >> selectRatio >> differRatio >> mode;
    //cout << "  " << data_name << "  " << Para << "  " << selectRatio << "  " << differRatio << "  " << mode << "\n";
    //initialization
    std::map<std::string, double> categorical_value;
    std::vector<std::string> categorical;
    tuple_set *t_set = new tuple_set(data_name);
    point_set *p_set = t_set->transform(categorical);
    point_set *p_skyline = p_set->skyline();
    tuple_set *t_skyline = t_set->skyline_based_num();
    t_set->find_categorical(categorical_value);
    int d_num = t_set->tuples[0]->d_num; //number of numerical dimension
    int dim_p = p_skyline->points[0]->d; //dimensionality of transformed points
    int d_new_attr = dim_p - t_set->tuples[0]->d_num; //number of newly generated dimension


    // obtain the utility vector
    point_t *up = new point_t(dim_p);
    for(int i = 0; i < dim_p; ++i)
        config >> up->attr[i];
    config.close();
    point_t *ut = up->extract_num(d_num, d_new_attr);//for numerical attributes of tuples
    double u_range = 0;
    for (int i = 0; i < d_num; i++)
        u_range += up->attr[d_new_attr + i];
    std::map<std::string, double>::iterator it;
    for (int i = 0; i < d_new_attr; i++)
        categorical_value[categorical[i]] = up->attr[i];

    printf("-----------------------------------------------------------------\n");
    printf("|%15s |%15s |%15s |%10s |\n", "Method", "# of Questions", "Time Cost", "Point #ID");
    printf("-----------------------------------------------------------------\n");
    ground_truth(p_skyline, up); // look for the ground truth maximum utility point

    double Csize;
    int Qcount = 0;

    if (d_num <= 1)
    {
        //The SP-Tree Algorithm
        special(t_skyline, ut->attr, categorical_value, Qcount);
    }

    //The GE-Graph algorithm
    general(t_skyline, ut->attr, categorical_value, 1, Qcount, Para, selectRatio, mode);

    //The GE-Graph algorithm for the confidence study
    general_confidence(t_skyline, ut->attr, categorical_value, 1, Qcount, Para, selectRatio, differRatio, mode);

    //The UH-Simplex algorithm
    //max_utility(p_skyline, up, 2, 0.0, 11, Qcount, Csize, SIMPLEX, EXACT_BOUND, RTREE, HYPER_PLANE, mode);

    //The UH-Random algorithm
    max_utility(p_skyline, up, 2, 0.0, 1000, Qcount, Csize, RANDOM, EXACT_BOUND, RTREE, HYPER_PLANE, mode);

    //The ActiveRanking Algorithm
    Active_Ranking(p_skyline, up, Qcount, mode);

    //The Adaptive Algorithm
    Adaptive(p_skyline, up, Qcount, mode);

    //The Adaptive Algorithm
    Adaptive_prune(p_skyline, up, Qcount, mode);


    delete p_skyline;
    return 0;
}
