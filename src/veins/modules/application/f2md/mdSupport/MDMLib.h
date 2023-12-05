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

#ifndef __VEINS_MDMLib_H_
#define __VEINS_MDMLib_H_

#include <omnetpp.h>
#include <veins/modules/messages/BasicSafetyMessage_m.h>
#include <veins/modules/application/f2md/mdSupport/ellipseIntLib/EllipseIntLib.h>
#include <veins/modules/application/f2md/mdSupport/rectIntLib/RectIntLib.h>

using namespace veins;

class MDMLib {

private:
    void countCircles(double rc, double rl, double rs);
    double calculateCircles(double dl, double ds);

    double importanceFactor(double r1, double r2, double d);

public:
    double gaussianSum(double x, double sig);
    double boundedGaussianSum(double x1, double x2, double sig);

    double calculateDistancePtr(const veins::Coord *, const veins::Coord *);
    double calculateSpeedPtr(const veins::Coord * Speed);
    double calculateHeadingAnglePtr(const veins::Coord * heading);

    double calculateDistance(veins::Coord, veins::Coord);
    double calculateSpeed(veins::Coord Speed);
    double calculateHeadingAngle(veins::Coord heading);

    double calculatePolynom(long double coof[], const int coofNum, double x);

    void calculateMaxMinDist(double curSpeed, double oldspeed, double time,
            double MAX_PLAUSIBLE_ACCEL, double MAX_PLAUSIBLE_DECEL,
            double MAX_PLAUSIBLE_SPEED, double * returnDistance);

    double calculateDeltaTime(BasicSafetyMessage * bsm1,
            BasicSafetyMessage * bsm2);
    double calculateCircleSegment(double radius, double intDistance);
    double calculateCircleCircleIntersection(double r1, double r2, double d);
    double SegmentSegmentFactor(double d, double r1, double r2, double range);
    double CircleSegmentFactor(double d, double r1, double r2, double range);
    double CircleCircleFactor(double d, double r1, double r2, double range);
    double OneSidedCircleSegmentFactor(double d, double r1, double r2,
            double range);
    double CircleIntersectionFactor(double conf1, double conf2, double d,
            double initRadius);

    double RectRectFactor(veins::Coord c1, veins::Coord c2, double heading1, double heading2,
            veins::Coord size1, veins::Coord size2);

    double EllipseEllipseIntersectionFactor(veins::Coord pos1, veins::Coord posConf1,
            veins::Coord pos2, veins::Coord posConf2, double heading1, double heading2,
            veins::Coord size1, veins::Coord size2);

    double SafeAcos(double x);

};

#endif
