#ifndef USERSTUDY_GENERAL_H
#define USERSTUDY_GENERAL_H

#include "structure/cluster_t.h"
#include "structure/rnode_tree.h"
#include "QuadProg++.hh"
using namespace quadprogpp;


void general(tuple_set *car_set, tuple_set *t_set, double *u, std::map<std::string, double> &categorical_value, double u_range,
             int &Qcount, int ROUND, double selectRatio, int &TID, ofstream &fp);

#endif //USERSTUDY_GENERAL_H
