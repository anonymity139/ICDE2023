#include "inequality.h"


/**
 * @brief   Constructor
 */
inequality::inequality(){}

/**
 * @brief   Constructor
 */
inequality::inequality(int ver, double *v, std::vector<double> &c)
{
    version = ver;
    norm = v;
    for(int i = 0; i < c.size(); ++i)
        coeff.push_back(c[i]);
}

/**
 * @brief   Deconstructor
 */
inequality::~inequality()
{
    //delete[] norm;
    //std::vector<double>().swap(coeff);
    //coeff.swap(std::vector<double>());
}

/**
 * @brief   Constructor
 */
inequality::inequality(inequality const &ineq)
{
    version = ineq.version;
    norm = ineq.norm;
    for(int i = 0; i < ineq.coeff.size(); ++i)
        coeff.push_back(ineq.coeff[i]);
}

