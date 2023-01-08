#include "quicksort.h"


void quicksort(point_set *ppset, point_t *u, int &Qcount, int mode)
{
    timeval t1, t2;
    std::ofstream out_cp("../result.txt");
    gettimeofday(&t1, 0);

    point_set *pset = new point_set(ppset);
    int dim = pset->points[0]->d;
    Qcount = 0;

    //initial
    hyperplane_set *R = new hyperplane_set(dim, U_RANGE);
    while (pset->points.size() > 1)
    {
        point_set *L = new point_set();
        point_t *pviot = pset->points[0];
        //interaction
        for (int i = 1; i < pset->points.size(); ++i) //compare: p_set contains all the points
        {
            if (!pviot->is_same(pset->points[i])) //if there exist a same point in the current_use, insert p_set[i]
            {
                hyperplane *h = new hyperplane(pviot, pset->points[i], 0);
                int relation = R->check_relation(h);
                if (relation == 0) //if intersect, calculate the distance
                {
                    Qcount++;
                    double v1 = pviot->dot_product(u);
                    double v2 = pset->points[i]->dot_product(u);
                    if (v1 < v2)
                    {
                        R->hyperplanes.push_back(new hyperplane(pviot, pset->points[i], 0));
                        R->set_ext_pts(U_RANGE);
                        L->points.push_back(pset->points[i]);
                    }
                    else
                    {
                        R->hyperplanes.push_back(new hyperplane(pset->points[i], pviot, 0));
                        R->set_ext_pts(U_RANGE);
                        //L->points.push_back(pset->points[i]);
                    }
                    printMidResult(out_cp, 100, Qcount, t1, mode);
                    if(Qcount >= QcountBound)
                    {
                        pset->points[0]->printResult(out_cp, "quicksort", Qcount, t1, mode);
                        out_cp.close();
                        return;
                    }
                }
                else if (relation == -1)
                {
                    L->points.push_back(pset->points[i]);
                }

            }
        }
        if(L->points.size() <= 0)
            L->points.push_back(pviot);
        pset = L;
    }

    pset->points[0]->printResult(out_cp, "QuickSort", Qcount, t1, mode);
    out_cp.close();
}