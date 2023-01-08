#ifndef RUN_GENERAL_H
#define RUN_GENERAL_H

#include "structure/cluster_t.h"
#include "structure/rnode_tree.h"
#include "QuadProg++.hh"
using namespace quadprogpp;


void general(tuple_set *t_set, double *u, std::map<std::string, double> &categorical_value, double u_range,
             int &Qcount, int ROUND, double selectRatio, int mode);



void general_confidence(tuple_set *t_set, double *u, std::map<std::string, double> &categorical_value, double u_range,
                        int &Qcount, int ROUND, double selectRatio, double diffRatio, int mode);

void general_error(tuple_set *t_set, double *u, std::map<std::string, double> &categorical_value, double u_range,
                   int &Qcount, int ROUND, double selectRatio, double probability, int mode, double maxValue);

#endif //RUN_GENERAL_H
