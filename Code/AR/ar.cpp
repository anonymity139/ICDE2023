#include "ar.h"

double at(double t, double sigma)
{
    double result = log(1.0/sigma) + 0.75 * log(log(1.0 / sigma)) + 1.5 * log(1 + log(t/2.0));
    result = sqrt(2*result);
}

/**
 * @brief Algorithm Active ranking from pairwise comparison
 * @param pset      The point set
 * @param u         The user's utility function
 * @param Qcount    The number of questions asked
 * @param mode
 */
void ar(point_set *ppset, point_t *u, int &Qcount, int mode)
{
    timeval t1, t2;
    std::ofstream out_cp("../result.txt");
    gettimeofday(&t1, 0);

    //initial
    point_set *pset = new point_set(ppset);
    int dim = pset->points[0]->d;
    double M = pset->points.size();
    for (int j = 0; j < M; ++j)
        pset->points[j]->value = 0;
    Qcount = 0;
    double t = 0, sigma = 0.5 / M;
    int k[3]; k[0] = 0, k[1] = 1, k[2] = M;
    point_set S[2];



    hyperplane_set *R = new hyperplane_set(dim, U_RANGE);


    while (M > 1 && S[0].points.size() <= 0)
    {
        t++;
        //interaction
        for (int i = 0; i < M; ++i)
        {
            //cout << "point: " << i <<"\n";
            //uniformly chosen item
            int j = rand() % pset->points.size();
            while (pset->points[i]->is_same(pset->points[j]))
                j = rand() % pset->points.size();

            //pairwise comparison
            hyperplane *h = new hyperplane(pset->points[i], pset->points[j], 0);
            int relation = R->check_relation(h);
            if (relation == 0) //if intersect, calculate the distance
            {
                Qcount++;
                double v1 = pset->points[i]->dot_product(u);
                double v2 = pset->points[j]->dot_product(u);
                if (v1 > v2)
                {
                    R->hyperplanes.push_back(new hyperplane(pset->points[j], pset->points[i], 0));
                    R->set_ext_pts(U_RANGE);
                    pset->points[i]->value = (t - 1.0) / t * pset->points[i]->value + 1.0 / t;
                }
                else
                {
                    R->hyperplanes.push_back(new hyperplane(pset->points[i], pset->points[j], 0));
                    R->set_ext_pts(U_RANGE);
                    pset->points[i]->value = (t - 1) / t * pset->points[i]->value;
                }
                printMidResult(out_cp, 100, Qcount, t1, mode);
                if(Qcount >= QcountBound)
                {
                    pset->points[0]->printResult(out_cp, "AR", Qcount, t1, mode);
                    out_cp.close();
                    return;
                }
            }
            else if (relation == 1)
            {
                pset->points[i]->value = (t - 1.0) / t * pset->points[i]->value + 1.0 / t;
            }
            else
            {
                pset->points[i]->value = (t - 1.0) / t * pset->points[i]->value;
            }
        }
        pset = pset->sort_on_value();
        double At = at(t, sigma);
        for (int j = 0; j < pset->points.size(); ++j)
        {
            for (int l = 1; l <= 2; ++l)
            {
                bool condition1 = (k[l - 1] == 0) ||
                                  (pset->points[j]->value < pset->points[ k[l-1]-1 ]->value - 4 * At);
                bool condition2 = (k[l] == M) ||
                                  (pset->points[j]->value > pset->points[k[l]]->value + 4 * At);
                if (condition1 && condition2)
                {
                    S[l - 1].points.push_back(pset->points[j]);
                    pset->points.erase(pset->points.begin() + j);
                    j--;
                    M = pset->points.size();
                    for (int ll = l; ll <= 2; ++ll)
                        k[ll]--;
                }
            }
        }

    }


    if(S[0].points.size() <= 0)
        S[0].points.push_back(pset->points[0]);
    S[0].points[0]->printResult(out_cp, "AR", Qcount, t1, mode);
    out_cp.close();
}