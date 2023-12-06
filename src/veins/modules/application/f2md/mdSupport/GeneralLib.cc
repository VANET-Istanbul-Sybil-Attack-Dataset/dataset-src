/*******************************************************************************
* @author  Joseph Kamel 
* @email   josephekamel@gmail.com 
* @date    28/11/2018
* @version 2.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#include <veins/modules/application/f2md/mdSupport/GeneralLib.h>

GeneralLib::GeneralLib(){
}

double GeneralLib::RandomDouble(double fMin, double fMax)
{
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);

    random::mt19937 rng(tm.tv_nsec);
    std::uniform_real_distribution<> one(fMin,fMax);

    double f = one(rng);
    return f;
}

int GeneralLib::RandomInt(int min, int max)
{
    std::random_device rd;
    
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> one(min, max);

    int guess = one(rng);
    return guess;
}
double GeneralLib::GaussianRandomDouble(double mean, double stddev) {

    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);

    random::mt19937 rng(tm.tv_nsec);

    std::normal_distribution<> d{mean,stddev};
    return d(rng);
}
