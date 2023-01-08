#include "General.h"
#include "structure/r_node.h"
#include "structure/relational_graph.h"
#include <sys/time.h>

/**
 * @brief                       Algorithm Combination
 * @param t_set                 The tuple set
 * @param u                     The utility vector
 * @param categorical_value     The numerical value of each categorical value
 * @param Qcount                Number of questions
 * @param ROUND
 * @param mode
 */
void general(tuple_set *t_set, double *u, std::map<std::string, double> &categorical_value, double u_range,
                       int &Qcount, int ROUND, double selectRatio, int mode)
{
    timeval t1; gettimeofday(&t1, 0);
    ofstream out_cp("../result.txt");
    double totalSize = t_set->tuples.size();
    if(t_set->tuples.size() <= 0)
        return;
    int d_num = t_set->tuples[0]->d_num, x = 0, sid; Qcount = 0;
    cluster_t *c = new cluster_t(t_set);
    hyperplane_set *R = new hyperplane_set(d_num, u_range);
    double t_count = 10;
    rnode_tree *tree = new rnode_tree();
    relational_graph *RG = new relational_graph(c, tree);

    while(t_count > 1)
    {
        ++Qcount;
        //randomly select two tuples
        tuple_t *tp1, *tp2; int cid_1, cid_2, tid_1, tid_2;
        int tsize = c->count_cluster();
        if(tsize >= c->clusters.size())
            c->select_tuple(tp1, tp2, cid_1, cid_2, tid_1, tid_2);
        else
        {
            tuple_t *tpnum1, *tpnum2; int cidnum_1, cidnum_2, tidnum_1, tidnum_2;
            tuple_t *tpcat1, *tpcat2; int cidcat_1, cidcat_2, tidcat_1, tidcat_2;
            c->select_tuple_sameset(tpnum1, tpnum2, cidnum_1, cidnum_2, tidnum_1, tidnum_2);
            c->select_tuple(tpcat1, tpcat2, cidcat_1, cidcat_2, tidcat_1, tidcat_2);
            std::string *s1 = tpcat1->attr_cat;
            std::string *s2 = tpcat2->attr_cat;
            int d_num = tpnum1->d_num;

            //find the node needed to update
            int direction;
            double numericalPrune = 0, categoricalPrune = 0, numOriginal = 0;
            int RGindex = RG->find_node(s1, s2, direction);
            r_node *nd;
            if(RGindex != -1)
                nd = RG->list[RGindex];
            else
                nd = RG->list[0];
            for (int i = 0; i < nd->sets.size(); ++i)
            {
                numOriginal += nd->sets[i].first->tuples.size();
                numOriginal += nd->sets[i].second->tuples.size();
            }

            //numerical approximate
            r_node *newnd = new r_node(nd);
            hyperplane_set *R1 = new hyperplane_set(R);
            R1->hyperplanes.push_back(new hyperplane(tpnum2, tpnum1, 0));
            R1->set_ext_pts(u_range);
            for(int i = 0; i < newnd->sets.size(); ++i)
            {
                numericalPrune += R1->num_prune_same_cat(newnd->sets[i].first);
                numericalPrune += R1->num_prune_same_cat(newnd->sets[i].second);
            }

            newnd = new r_node(nd);
            hyperplane_set *R2 = new hyperplane_set(R);
            R2->hyperplanes.push_back(new hyperplane(tpnum1, tpnum2, 0));
            R2->set_ext_pts(u_range);
            for(int i = 0; i < newnd->sets.size(); ++i)
            {
                numericalPrune += R2->num_prune_same_cat(newnd->sets[i].first);
                numericalPrune += R2->num_prune_same_cat(newnd->sets[i].second);
            }
            numericalPrune /= 2;



            //categorical approximate
            newnd = new r_node(nd);
            double *v = new double[d_num];
            if (direction == -1)
            {
                for (int i = 0; i < d_num; ++i)
                    v[i] = tpcat2->attr_num[i] - tpcat1->attr_num[i];
            } else if (direction == 1)
            {
                for (int i = 0; i < d_num; ++i)
                    v[i] = tpcat1->attr_num[i] - tpcat2->attr_num[i];
            }
            std::vector<double> coeff;
            for (int i = 0; i < x; ++i)
                coeff.push_back(0);
            coeff.push_back(1);
            hyperplane_set *R3 = new hyperplane_set(R);
            newnd->update_without_check_round(v, direction, R3, coeff, u_range);
            newnd->tuple_prune_all_rnode_aggre(R);
            for (int i = 0; i < newnd->sets.size(); ++i)
            {
               categoricalPrune += newnd->sets[i].first->tuples.size();
               categoricalPrune += newnd->sets[i].second->tuples.size();
            }

            newnd = new r_node(nd);
            if (direction == -1)
                direction = 1;
            else
                direction = -1;
            if (direction == -1)
            {
                for (int i = 0; i < d_num; ++i)
                    v[i] = tpcat2->attr_num[i] - tpcat1->attr_num[i];
            } else if (direction == 1)
            {
                for (int i = 0; i < d_num; ++i)
                    v[i] = tpcat1->attr_num[i] - tpcat2->attr_num[i];
            }
            std::vector<double> coeff1;
            for (int i = 0; i < x; ++i)
                coeff1.push_back(0);
            coeff1.push_back(1);
            hyperplane_set *R4 = new hyperplane_set(R);
            newnd->update_without_check_round(v, direction, R4, coeff1, u_range);
            newnd->tuple_prune_all_rnode_aggre(R);
            for (int i = 0; i < newnd->sets.size(); ++i)
            {
                categoricalPrune += newnd->sets[i].first->tuples.size();
                categoricalPrune += newnd->sets[i].second->tuples.size();
            }
            categoricalPrune /= 2;
            categoricalPrune = numOriginal - categoricalPrune;


            //cout << categoricalPrune << "    " << numericalPrune << "\n";
            if(categoricalPrune / numericalPrune > selectRatio)
            {
                tp1 = tpcat1; tp2 = tpcat2;
                cid_1 = cidcat_1; cid_2 = cidcat_2;
                tid_1 = tidcat_1, tid_2 = tidcat_2;
            }
            else
            {
                tp1 = tpnum1; tp2 = tpnum2;
                cid_1 = cidnum_1; cid_2 = cidnum_2;
                tid_1 = tidnum_1, tid_2 = tidnum_2;
            }

        }


        //user's feedback
        double score1 = tp1->score(u, categorical_value);
        double score2 = tp2->score(u, categorical_value);
        //tp1->print(); tp2->print();
        //std::cout<<"score1: "<<score1<<"  score2: "<<score2<<"\n";

        //update information
        if(!tp1->is_same_cat(tp2))
        {
            if(score1 >= score2)//update the relational graph
            {
                RG->update_all_round(tp2, tp1, R, u_range, x++, tree, ROUND);
                c->clusters[cid_2]->tuples.erase(c->clusters[cid_2]->tuples.begin() + tid_2);
            }
            else
            {
                RG->update_all_round(tp1, tp2, R, u_range, x++, tree, ROUND);
                c->clusters[cid_1]->tuples.erase(c->clusters[cid_1]->tuples.begin() + tid_1);
            }
            //RG->print_list();
        }
        else
        {
            if(score1 >= score2)
            {
                R->hyperplanes.push_back(new hyperplane(tp2, tp1, 0));
                c->clusters[cid_2]->tuples.erase(c->clusters[cid_2]->tuples.begin() + tid_2);
            }
            else
            {
                R->hyperplanes.push_back(new hyperplane(tp1, tp2, 0));
                c->clusters[cid_1]->tuples.erase(c->clusters[cid_1]->tuples.begin() + tid_1);
            }
            R->set_ext_pts(u_range);
            ///R->print();
        }
        if(d_num > 1)
            RG->update_basedR(R);

        //prune tuples
        c->prune(R);
        for(int i = 0; i < RG->list.size(); ++i)
        {
            //RG->list[i]->tuple_prune_all_rnode(R);
            RG->list[i]->tuple_prune_all_rnode_aggre(R);
            RG->list[i]->clean();
        }
        t_count = c->count_tuple();//count the tuples
        printMidResult(out_cp, t_count/totalSize * 100, Qcount, t1, mode);
    }
    c->clusters[0]->tuples[0]->p->printResult(out_cp, "GE-Graph", Qcount, t1, mode);
    out_cp.close();
}






/**
 * @brief                       Algorithm Combination
 * @param t_set                 The tuple set
 * @param u                     The utility vector
 * @param categorical_value     The numerical value of each categorical value
 * @param Qcount                Number of questions
 * @param ROUND
 * @param mode
 */
void general_confidence(tuple_set *t_set, double *u, std::map<std::string, double> &categorical_value, double u_range,
             int &Qcount, int ROUND, double selectRatio, double diffRatio, int mode)
{
    timeval t1; gettimeofday(&t1, 0);
    ofstream out_cp("../result.txt");
    double totalSize = t_set->tuples.size();
    if(t_set->tuples.size() <= 0)
        return;
    int d_num = t_set->tuples[0]->d_num, x = 0, sid;
    double difficultQcount = 0; Qcount = 0;
    cluster_t *c = new cluster_t(t_set);
    hyperplane_set *R = new hyperplane_set(d_num, u_range);
    double t_count = 10;
    rnode_tree *tree = new rnode_tree();
    relational_graph *RG = new relational_graph(c, tree);

    while(t_count > 1)
    {
        ++Qcount;
        //randomly select two tuples
        tuple_t *tp1, *tp2; int cid_1, cid_2, tid_1, tid_2;
        int tsize = c->count_cluster();
        if(tsize >= c->clusters.size())
            c->select_tuple(tp1, tp2, cid_1, cid_2, tid_1, tid_2);
        else
        {
            tuple_t *tpnum1, *tpnum2; int cidnum_1, cidnum_2, tidnum_1, tidnum_2;
            tuple_t *tpcat1, *tpcat2; int cidcat_1, cidcat_2, tidcat_1, tidcat_2;
            c->select_tuple_sameset(tpnum1, tpnum2, cidnum_1, cidnum_2, tidnum_1, tidnum_2);
            c->select_tuple(tpcat1, tpcat2, cidcat_1, cidcat_2, tidcat_1, tidcat_2);
            std::string *s1 = tpcat1->attr_cat;
            std::string *s2 = tpcat2->attr_cat;
            int d_num = tpnum1->d_num;

            //find the node needed to update
            int direction;
            double numericalPrune = 0, categoricalPrune = 0, numOriginal = 0;
            int RGindex = RG->find_node(s1, s2, direction);
            r_node *nd;
            if(RGindex != -1)
                nd = RG->list[RGindex];
            else
                nd = RG->list[0];
            for (int i = 0; i < nd->sets.size(); ++i)
            {
                numOriginal += nd->sets[i].first->tuples.size();
                numOriginal += nd->sets[i].second->tuples.size();
            }

            //numerical approximate
            r_node *newnd = new r_node(nd);
            hyperplane_set *R1 = new hyperplane_set(R);
            R1->hyperplanes.push_back(new hyperplane(tpnum2, tpnum1, 0));
            R1->set_ext_pts(u_range);
            for(int i = 0; i < newnd->sets.size(); ++i)
            {
                numericalPrune += R1->num_prune_same_cat(newnd->sets[i].first);
                numericalPrune += R1->num_prune_same_cat(newnd->sets[i].second);
            }

            newnd = new r_node(nd);
            hyperplane_set *R2 = new hyperplane_set(R);
            R2->hyperplanes.push_back(new hyperplane(tpnum1, tpnum2, 0));
            R2->set_ext_pts(u_range);
            for(int i = 0; i < newnd->sets.size(); ++i)
            {
                numericalPrune += R2->num_prune_same_cat(newnd->sets[i].first);
                numericalPrune += R2->num_prune_same_cat(newnd->sets[i].second);
            }
            numericalPrune /= 2;

            //categorical approximate
            newnd = new r_node(nd);
            double *v = new double[d_num];
            if (direction == -1)
            {
                for (int i = 0; i < d_num; ++i)
                    v[i] = tpcat2->attr_num[i] - tpcat1->attr_num[i];
            } else if (direction == 1)
            {
                for (int i = 0; i < d_num; ++i)
                    v[i] = tpcat1->attr_num[i] - tpcat2->attr_num[i];
            }
            std::vector<double> coeff;
            for (int i = 0; i < x; ++i)
                coeff.push_back(0);
            coeff.push_back(1);
            hyperplane_set *R3 = new hyperplane_set(R);
            newnd->update_without_check_round(v, direction, R3, coeff, u_range);
            newnd->tuple_prune_all_rnode_aggre(R);
            for (int i = 0; i < newnd->sets.size(); ++i)
            {
                categoricalPrune += newnd->sets[i].first->tuples.size();
                categoricalPrune += newnd->sets[i].second->tuples.size();
            }

            newnd = new r_node(nd);
            if (direction == -1)
                direction = 1;
            else
                direction = -1;
            if (direction == -1)
            {
                for (int i = 0; i < d_num; ++i)
                    v[i] = tpcat2->attr_num[i] - tpcat1->attr_num[i];
            } else if (direction == 1)
            {
                for (int i = 0; i < d_num; ++i)
                    v[i] = tpcat1->attr_num[i] - tpcat2->attr_num[i];
            }
            std::vector<double> coeff1;
            for (int i = 0; i < x; ++i)
                coeff1.push_back(0);
            coeff1.push_back(1);
            hyperplane_set *R4 = new hyperplane_set(R);
            newnd->update_without_check_round(v, direction, R4, coeff1, u_range);
            newnd->tuple_prune_all_rnode_aggre(R);
            for (int i = 0; i < newnd->sets.size(); ++i)
            {
                categoricalPrune += newnd->sets[i].first->tuples.size();
                categoricalPrune += newnd->sets[i].second->tuples.size();
            }
            categoricalPrune /= 2;
            categoricalPrune = numOriginal - categoricalPrune;

            //cout << categoricalPrune << "    " << numericalPrune << "\n";
            if(categoricalPrune / numericalPrune > selectRatio)
            {
                tp1 = tpcat1; tp2 = tpcat2;
                cid_1 = cidcat_1; cid_2 = cidcat_2;
                tid_1 = tidcat_1, tid_2 = tidcat_2;
            }
            else
            {
                tp1 = tpnum1; tp2 = tpnum2;
                cid_1 = cidnum_1; cid_2 = cidnum_2;
                tid_1 = tidnum_1, tid_2 = tidnum_2;
            }

        }


        //user's feedback
        double score1 = tp1->score(u, categorical_value);
        double score2 = tp2->score(u, categorical_value);
        //tp1->print(); tp2->print();
        //std::cout<<"score1: "<<score1<<"  score2: "<<score2<<"\n";


        if(differenceRatio(score1, score2) < diffRatio)
            ++difficultQcount;
        if(differenceRatio(score1, score2) > diffRatio || rand() % 10 >= 5)//judge whether the user will answer the question
        {

            //update information
            if (!tp1->is_same_cat(tp2))
            {
                if (score1 >= score2)//update the relational graph
                {
                    RG->update_all_round(tp2, tp1, R, u_range, x++, tree, ROUND);
                    c->clusters[cid_2]->tuples.erase(c->clusters[cid_2]->tuples.begin() + tid_2);
                } else
                {
                    RG->update_all_round(tp1, tp2, R, u_range, x++, tree, ROUND);
                    c->clusters[cid_1]->tuples.erase(c->clusters[cid_1]->tuples.begin() + tid_1);
                }
                //RG->print_list();
            } else
            {
                if (score1 >= score2)
                {
                    R->hyperplanes.push_back(new hyperplane(tp2, tp1, 0));
                    c->clusters[cid_2]->tuples.erase(c->clusters[cid_2]->tuples.begin() + tid_2);
                } else
                {
                    R->hyperplanes.push_back(new hyperplane(tp1, tp2, 0));
                    c->clusters[cid_1]->tuples.erase(c->clusters[cid_1]->tuples.begin() + tid_1);
                }
                R->set_ext_pts(u_range);
                ///R->print();
            }
            if (d_num > 1)
                RG->update_basedR(R);

            //prune tuples
            c->prune(R);
            for (int i = 0; i < RG->list.size(); ++i)
            {
                //RG->list[i]->tuple_prune_all_rnode(R);
                RG->list[i]->tuple_prune_all_rnode_aggre(R);
                RG->list[i]->clean();
            }
        }
        t_count = c->count_tuple();//count the tuples
        printMidResult(out_cp, t_count/totalSize * 100, Qcount, t1, mode);
    }
    c->clusters[0]->tuples[0]->p->printResult(out_cp, "GE-Graph(Conf)", Qcount, t1, mode);
    //cout << difficultQcount << "\n";
    out_cp << difficultQcount/Qcount << "\n";
    out_cp.close();
}



/**
 * @brief                       Algorithm Combination
 * @param t_set                 The tuple set
 * @param u                     The utility vector
 * @param categorical_value     The numerical value of each categorical value
 * @param Qcount                Number of questions
 * @param ROUND
 * @param mode
 */
void general_error(tuple_set *t_set, double *u, std::map<std::string, double> &categorical_value, double u_range,
             int &Qcount, int ROUND, double selectRatio, double probability, int mode, double maxValue)
{
    timeval t1; gettimeofday(&t1, 0);
    ofstream out_cp("../result.txt");
    double totalSize = t_set->tuples.size();
    if(t_set->tuples.size() <= 0)
        return;
    int d_num = t_set->tuples[0]->d_num, x = 0, sid; Qcount = 0;
    cluster_t *c = new cluster_t(t_set);
    hyperplane_set *R = new hyperplane_set(d_num, u_range);
    double t_count = 10;
    rnode_tree *tree = new rnode_tree();
    relational_graph *RG = new relational_graph(c, tree);

    while(t_count > 1)
    {
        ++Qcount;
        //randomly select two tuples
        tuple_t *tp1, *tp2; int cid_1, cid_2, tid_1, tid_2;
        int tsize = c->count_cluster();
        if(tsize >= c->clusters.size())
            c->select_tuple(tp1, tp2, cid_1, cid_2, tid_1, tid_2);
        else
        {
            tuple_t *tpnum1, *tpnum2; int cidnum_1, cidnum_2, tidnum_1, tidnum_2;
            tuple_t *tpcat1, *tpcat2; int cidcat_1, cidcat_2, tidcat_1, tidcat_2;
            c->select_tuple_sameset(tpnum1, tpnum2, cidnum_1, cidnum_2, tidnum_1, tidnum_2);
            c->select_tuple(tpcat1, tpcat2, cidcat_1, cidcat_2, tidcat_1, tidcat_2);
            std::string *s1 = tpcat1->attr_cat;
            std::string *s2 = tpcat2->attr_cat;
            int d_num = tpnum1->d_num;

            //find the node needed to update
            int direction;
            double numericalPrune = 0, categoricalPrune = 0, numOriginal = 0;
            int RGindex = RG->find_node(s1, s2, direction);
            r_node *nd;
            if(RGindex != -1)
                nd = RG->list[RGindex];
            else
                nd = RG->list[0];
            for (int i = 0; i < nd->sets.size(); ++i)
            {
                numOriginal += nd->sets[i].first->tuples.size();
                numOriginal += nd->sets[i].second->tuples.size();
            }

            //numerical approximate
            r_node *newnd = new r_node(nd);
            hyperplane_set *R1 = new hyperplane_set(R);
            R1->hyperplanes.push_back(new hyperplane(tpnum2, tpnum1, 0));
            R1->set_ext_pts(u_range);
            for(int i = 0; i < newnd->sets.size(); ++i)
            {
                numericalPrune += R1->num_prune_same_cat(newnd->sets[i].first);
                numericalPrune += R1->num_prune_same_cat(newnd->sets[i].second);
            }

            newnd = new r_node(nd);
            hyperplane_set *R2 = new hyperplane_set(R);
            R2->hyperplanes.push_back(new hyperplane(tpnum1, tpnum2, 0));
            R2->set_ext_pts(u_range);
            for(int i = 0; i < newnd->sets.size(); ++i)
            {
                numericalPrune += R2->num_prune_same_cat(newnd->sets[i].first);
                numericalPrune += R2->num_prune_same_cat(newnd->sets[i].second);
            }
            numericalPrune /= 2;



            //categorical approximate
            newnd = new r_node(nd);
            double *v = new double[d_num];
            if (direction == -1)
            {
                for (int i = 0; i < d_num; ++i)
                    v[i] = tpcat2->attr_num[i] - tpcat1->attr_num[i];
            } else if (direction == 1)
            {
                for (int i = 0; i < d_num; ++i)
                    v[i] = tpcat1->attr_num[i] - tpcat2->attr_num[i];
            }
            std::vector<double> coeff;
            for (int i = 0; i < x; ++i)
                coeff.push_back(0);
            coeff.push_back(1);
            hyperplane_set *R3 = new hyperplane_set(R);
            newnd->update_without_check_round(v, direction, R3, coeff, u_range);
            newnd->tuple_prune_all_rnode_aggre(R);
            for (int i = 0; i < newnd->sets.size(); ++i)
            {
                categoricalPrune += newnd->sets[i].first->tuples.size();
                categoricalPrune += newnd->sets[i].second->tuples.size();
            }

            newnd = new r_node(nd);
            if (direction == -1)
                direction = 1;
            else
                direction = -1;
            if (direction == -1)
            {
                for (int i = 0; i < d_num; ++i)
                    v[i] = tpcat2->attr_num[i] - tpcat1->attr_num[i];
            } else if (direction == 1)
            {
                for (int i = 0; i < d_num; ++i)
                    v[i] = tpcat1->attr_num[i] - tpcat2->attr_num[i];
            }
            std::vector<double> coeff1;
            for (int i = 0; i < x; ++i)
                coeff1.push_back(0);
            coeff1.push_back(1);
            hyperplane_set *R4 = new hyperplane_set(R);
            newnd->update_without_check_round(v, direction, R4, coeff1, u_range);
            newnd->tuple_prune_all_rnode_aggre(R);
            for (int i = 0; i < newnd->sets.size(); ++i)
            {
                categoricalPrune += newnd->sets[i].first->tuples.size();
                categoricalPrune += newnd->sets[i].second->tuples.size();
            }
            categoricalPrune /= 2;
            categoricalPrune = numOriginal - categoricalPrune;


            //cout << categoricalPrune << "    " << numericalPrune << "\n";
            if(categoricalPrune / numericalPrune > selectRatio)
            {
                tp1 = tpcat1; tp2 = tpcat2;
                cid_1 = cidcat_1; cid_2 = cidcat_2;
                tid_1 = tidcat_1, tid_2 = tidcat_2;
            }
            else
            {
                tp1 = tpnum1; tp2 = tpnum2;
                cid_1 = cidnum_1; cid_2 = cidnum_2;
                tid_1 = tidnum_1, tid_2 = tidnum_2;
            }

        }


        //user's feedback
        double score1 = tp1->score(u, categorical_value);
        double score2 = tp2->score(u, categorical_value);
        //tp1->print(); tp2->print();
        //std::cout<<"score1: "<<score1<<"  score2: "<<score2<<"\n";

        //update information
        if(!tp1->is_same_cat(tp2))
        {
            if(score1 >= score2)//update the relational graph
            {
                if(rand()%100 > probability * 100)
                {
                    RG->update_all_round(tp2, tp1, R, u_range, x++, tree, ROUND);
                    c->clusters[cid_2]->tuples.erase(c->clusters[cid_2]->tuples.begin() + tid_2);
                }
                else
                {
                    RG->update_all_round(tp1, tp2, R, u_range, x++, tree, ROUND);
                    c->clusters[cid_1]->tuples.erase(c->clusters[cid_1]->tuples.begin() + tid_1);
                }
            }
            else
            {
                if(rand()%100 > probability * 100)
                {
                    RG->update_all_round(tp1, tp2, R, u_range, x++, tree, ROUND);
                    c->clusters[cid_1]->tuples.erase(c->clusters[cid_1]->tuples.begin() + tid_1);
                }
                else
                {
                    RG->update_all_round(tp2, tp1, R, u_range, x++, tree, ROUND);
                    c->clusters[cid_2]->tuples.erase(c->clusters[cid_2]->tuples.begin() + tid_2);
                }
            }
            //RG->print_list();
        }
        else
        {
            if(score1 >= score2)
            {
                if(rand()%100 > probability * 100)
                {
                    R->hyperplanes.push_back(new hyperplane(tp2, tp1, 0));
                    c->clusters[cid_2]->tuples.erase(c->clusters[cid_2]->tuples.begin() + tid_2);
                }
                else
                {
                    R->hyperplanes.push_back(new hyperplane(tp1, tp2, 0));
                    c->clusters[cid_1]->tuples.erase(c->clusters[cid_1]->tuples.begin() + tid_1);
                }
            }
            else
            {
                if(rand()%100 > probability * 100)
                {
                    R->hyperplanes.push_back(new hyperplane(tp1, tp2, 0));
                    c->clusters[cid_1]->tuples.erase(c->clusters[cid_1]->tuples.begin() + tid_1);
                }
                else
                {
                    R->hyperplanes.push_back(new hyperplane(tp2, tp1, 0));
                    c->clusters[cid_2]->tuples.erase(c->clusters[cid_2]->tuples.begin() + tid_2);
                }
            }
            R->set_ext_pts(u_range);
            ///R->print();
        }
        if(d_num > 1)
            RG->update_basedR(R);

        //prune tuples
        c->prune(R);
        for(int i = 0; i < RG->list.size(); ++i)
        {
            //RG->list[i]->tuple_prune_all_rnode(R);
            RG->list[i]->tuple_prune_all_rnode_aggre(R);
            RG->list[i]->clean();
        }
        t_count = c->count_tuple();//count the tuples
        printMidResult(out_cp, t_count/totalSize * 100, Qcount, t1, mode);
    }
    c->clusters[0]->tuples[0]->p->printResult(out_cp, "GE-Graph(Error)", Qcount, t1, mode);
    //cout << c->clusters[0]->tuples[0]->score(u, categorical_value) / maxValue << "\n";
    out_cp << c->clusters[0]->tuples[0]->score(u, categorical_value) / maxValue << "\n";
    out_cp.close();
}



