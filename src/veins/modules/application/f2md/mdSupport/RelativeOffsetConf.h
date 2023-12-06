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

#ifndef __VEINS_RelativeOffsetConf_H_
#define __VEINS_RelativeOffsetConf_H_

#include <omnetpp.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <veins/modules/application/f2md/mdSupport/GeneralLib.h>
#include <veins/modules/application/f2md/mdSupport/MDMLib.h>
using namespace std;
using namespace boost;

using namespace omnetpp;
using namespace veins;

class RelativeOffsetConf {
private:
    veins::Coord* ConfPosMax = 0;
    veins::Coord* ConfSpeedMax = 0;
    veins::Coord* ConfHeadingMax = 0;
    veins::Coord* ConfAccelMax = 0;

    double* deltaConfPos = 0;
    double* deltaConfSpeed = 0;
    double* deltaConfHeading = 0;
    double* deltaConfAccel = 0;

    double getGaussianRand(double mean, double stddev);

public:

    RelativeOffsetConf(veins::Coord* ConfPosMax, veins::Coord* ConfSpeedMax,
            veins::Coord *ConfHeadingMax,veins::Coord*ConfAccelMax, double* deltaConfPos,
            double* deltaConfSpeed, double *deltaConfHeading,
            double *deltaConfAccel);

    veins::Coord OffsetPosConf(veins::Coord curPosConf);
    veins::Coord OffsetSpeedConf(veins::Coord curSpeedConf);
    veins::Coord OffsetAccelConf(veins::Coord curAccelConf);
    veins::Coord OffsetHeadingConf(veins::Coord curHeadingConf);

};

#endif
