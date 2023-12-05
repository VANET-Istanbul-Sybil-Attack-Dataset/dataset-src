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

#ifndef __VEINS_RelativeOffset_H_
#define __VEINS_RelativeOffset_H_

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

class RelativeOffset {
private:
    veins::Coord* curPosConfidence;
    veins::Coord* curSpeedConfidence;
    veins::Coord* curHeadingConfidence;
    veins::Coord* curAccelConfidence;

    double* deltaRPosition = 0;
    double* deltaThetaPosition = 0;
    double* deltaSpeed = 0;
    double* deltaHeading = 0;
    double* deltaAccel = 0;

    double getGaussianRand(double mean, double stddev);

public:

    RelativeOffset(veins::Coord* curPosConfidence, veins::Coord *curSpeedConfidence,
            veins::Coord *curHeadingConfidence,veins::Coord *curAccelConfidence, double* deltaRPosition,
            double* deltaThetaPosition, double* deltaSpeed,
            double* deltaHeadin,double* deltaAccel);
    veins::Coord OffsetPosition(veins::Coord curPosition);
    veins::Coord OffsetSpeed(veins::Coord curSpeed);
    veins::Coord OffsetHeading(veins::Coord curHeading);
    veins::Coord OffsetAccel(veins::Coord curAccel);

};

#endif
