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

#include <stdio.h>
#include <stdlib.h> /* atof */
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <veins/modules/application/f2md/mdChecks/ExperiChecks.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace boost;
#define AUG_FACTOR 20

ExperiChecks::ExperiChecks(int version, unsigned long myPseudonym,
    veins::Coord myPosition, veins::Coord myPositionConfidence, veins::Coord myHeading,
    veins::Coord myHeadingConfidence, veins::Coord mySize, veins::Coord myLimits,
    LinkControl* LinkC, std::unordered_map<LAddress::L2Type, Coord>* realDynamicMap, LAddress::L2Type myId, F2MDParameters* params)
{
    this->version = version;
    this->myPseudonym = myPseudonym;
    this->myPosition = myPosition;
    this->myPositionConfidence = myPositionConfidence;
    this->myHeading = myHeading;
    this->myHeadingConfidence = myHeadingConfidence;
    this->mySize = mySize;
    this->MAX_PLAUSIBLE_SPEED = myLimits.x;
    this->MAX_PLAUSIBLE_ACCEL = myLimits.y;
    this->MAX_PLAUSIBLE_DECEL = myLimits.z;
    this->LinkC = LinkC;
    this->realDynamicMap = realDynamicMap;
    this->params = params;
    this->myId = myId;
}

double ExperiChecks::ProximityPlausibilityCheck(const veins::Coord* testPosition, const veins::Coord* myPosition, const veins::Coord* myHeading)
{
    double distance = mdmLib.calculateDistancePtr(testPosition, myPosition);
    veins::Coord relativePos = veins::Coord(testPosition->x - myPosition->x,
        testPosition->y - myPosition->y,
        testPosition->z - myPosition->z);
    double positionAngle = fabs(mdmLib.calculateHeadingAngle(relativePos));
    if (distance < params->MAX_PROXIMITY_RANGE_L) {
        if (distance < params->MAX_PROXIMITY_RANGE_W * 2 || (positionAngle < 90 && distance < (params->MAX_PROXIMITY_RANGE_W / cos((90 - positionAngle) * M_PI / 180)))) {
            double minDistance = 9999;

            for (std::pair<LAddress::L2Type, veins::Coord> x : *realDynamicMap) {
                if (x.first != myId) {
                    double testDistance = mdmLib.calculateDistancePtr(testPosition, &x.second);
                    if (minDistance > testDistance) {
                        minDistance = testDistance;
                    }
                }
            }

            double returnVal = 1 - minDistance / (2 * params->MAX_PROXIMITY_DISTANCE);

            if (returnVal < -20) {
                returnVal = -20;
            }
            return returnVal;
        }
    }
    return 1;
}

double ExperiChecks::RangePlausibilityCheck(const veins::Coord* receiverPosition,
    const veins::Coord* receiverPositionConfidence, const veins::Coord* senderPosition,
    const veins::Coord* senderPositionConfidence)
{

    double distance = mdmLib.calculateDistancePtr(senderPosition,
        receiverPosition);

    double factor = mdmLib.CircleCircleFactor(distance,
        senderPositionConfidence->x, receiverPositionConfidence->x,
        params->MAX_PLAUSIBLE_RANGE);

    if (factor <= 0) {
        factor = -AUG_FACTOR * (2 * mdmLib.gaussianSum(distance - senderPositionConfidence->x - receiverPositionConfidence->x, 2 * params->MAX_PLAUSIBLE_RANGE / 3) - 1);
    }

    return factor;
}

double ExperiChecks::PositionConsistancyCheck(const veins::Coord* curPosition,
    const veins::Coord* curPositionConfidence, const veins::Coord* oldPosition,
    const veins::Coord* oldPositionConfidence, double time)
{
    double distance = mdmLib.calculateDistancePtr(curPosition, oldPosition);
    double factor = mdmLib.CircleCircleFactor(distance,
        curPositionConfidence->x, oldPositionConfidence->x,
        MAX_PLAUSIBLE_SPEED * time);
    if (factor <= 0) {
        factor = -AUG_FACTOR * (2 * mdmLib.gaussianSum(distance - curPositionConfidence->x - oldPositionConfidence->x, 2 * (MAX_PLAUSIBLE_SPEED * time) / 3) - 1);
    }
    return factor;
}

double ExperiChecks::SpeedConsistancyCheck(double curSpeed,
    double curSpeedConfidence, double oldspeed, double oldSpeedConfidence,
    double time)
{

    double speedDelta = curSpeed - oldspeed;

    //    double attFact = mdmLib.gaussianSum(1, 1 / 3);
    //    if (time >= 1) {
    //        attFact = time;
    //    }

    double factor = 1;
    if (speedDelta > 0) {
        factor = mdmLib.SegmentSegmentFactor(speedDelta, curSpeedConfidence,
            oldSpeedConfidence, MAX_PLAUSIBLE_ACCEL * time);
        if (factor <= 0) {
            factor = -AUG_FACTOR * (2 * mdmLib.gaussianSum(speedDelta - curSpeedConfidence - oldSpeedConfidence, 2 * (MAX_PLAUSIBLE_ACCEL * time) / 3) - 1);
        }
    }
    else {
        factor = mdmLib.SegmentSegmentFactor(fabs(speedDelta),
            curSpeedConfidence, oldSpeedConfidence,
            MAX_PLAUSIBLE_DECEL * time);
        if (factor <= 0) {
            factor = -AUG_FACTOR * (2 * mdmLib.gaussianSum(fabs(speedDelta) - curSpeedConfidence - oldSpeedConfidence, 2 * (MAX_PLAUSIBLE_DECEL * time) / 3) - 1);
        }
    }

    return factor;
}

double ExperiChecks::SpeedPlausibilityCheck(double speed,
    double speedConfidence)
{
    if ((fabs(speed) + fabs(speedConfidence) / 2) < MAX_PLAUSIBLE_SPEED) {
        return 1;
    }
    else if ((fabs(speed) - fabs(speedConfidence) / 2) > MAX_PLAUSIBLE_SPEED) {
        double factor = -AUG_FACTOR * (2 * mdmLib.gaussianSum((fabs(speed) - fabs(speedConfidence) / 2), 2 * (MAX_PLAUSIBLE_SPEED) / 3) - 1);
        return factor;
    }
    else {
        double factor = (fabs(speedConfidence) / 2 + (MAX_PLAUSIBLE_SPEED - fabs(speed))) / fabs(speedConfidence);

        return factor;
    }
}

double ExperiChecks::PositionSpeedMaxConsistancyCheck(const veins::Coord* curPosition,
    const veins::Coord* curPositionConfidence, const veins::Coord* oldPosition,
    const veins::Coord* oldPositionConfidence, double curSpeed,
    double curSpeedConfidence, double oldspeed, double oldSpeedConfidence,
    double time)
{

    if (time < params->MAX_TIME_DELTA) {

        double distance = mdmLib.calculateDistancePtr(curPosition, oldPosition);
        double theoreticalSpeed = distance / time;
        double maxspeed = std::max(curSpeed, oldspeed);
        double minspeed = std::min(curSpeed, oldspeed);

        double curR = curPositionConfidence->x / time + curSpeedConfidence;
        double oldR = oldPositionConfidence->x / time + oldSpeedConfidence;

        double maxfactor = mdmLib.OneSidedCircleSegmentFactor(
            maxspeed - theoreticalSpeed, curR, oldR,
            (MAX_PLAUSIBLE_DECEL + params->MAX_MGT_RNG) * time);

        if (maxfactor <= 0) {
            maxfactor = -AUG_FACTOR * (2 * mdmLib.gaussianSum((maxspeed - theoreticalSpeed - curR - oldR), 2 * ((MAX_PLAUSIBLE_DECEL + params->MAX_MGT_RNG) * time) / 3) - 1);
        }

        double minfactor = mdmLib.OneSidedCircleSegmentFactor(
            theoreticalSpeed - minspeed, curR, oldR,
            (MAX_PLAUSIBLE_ACCEL + params->MAX_MGT_RNG) * time);
        if (minfactor <= 0) {
            minfactor = -AUG_FACTOR * (2 * mdmLib.gaussianSum((theoreticalSpeed - minspeed - curR - oldR), 2 * ((MAX_PLAUSIBLE_ACCEL + params->MAX_MGT_RNG) * time) / 3) - 1);
        }
        double factor = 1;

        if (minfactor < maxfactor) {
            factor = minfactor;
        }
        else {
            factor = maxfactor;
        }

        //        factor = (factor - 0.5) * 2;
        //        factor = mdmLib.gaussianSum(factor, (1.0 / 4.5));
        //        if (factor > 0.75) {
        //            factor = 1;
        //        }
        //
        //        if (factor <0.001) {
        //            factor = 0;
        //        }

        return factor;
    }
    else {
        return 1;
    }
}

double ExperiChecks::PositionSpeedConsistancyCheck(const veins::Coord* curPosition,
    const veins::Coord* curPositionConfidence, const veins::Coord* oldPosition,
    const veins::Coord* oldPositionConfidence, double curSpeed,
    double curSpeedConfidence, double oldspeed, double oldSpeedConfidence,
    double time)
{

    if (time < params->MAX_TIME_DELTA) {
        double distance = mdmLib.calculateDistancePtr(curPosition, oldPosition);

        double curTest_1 = curSpeed + curSpeedConfidence;
        double oldTest_1 = oldspeed - oldSpeedConfidence;

        double curTest_2 = curSpeed - curSpeedConfidence;
        double oldTest_2 = oldspeed + oldSpeedConfidence;

        if (curTest_2 < oldTest_2) {
            curTest_2 = (curSpeed + oldspeed) / 2;
            oldTest_2 = (curSpeed + oldspeed) / 2;
        }
        double minSpeed = std::min(curSpeed, oldspeed);

        double addon_mgt_range = params->MAX_MGT_RNG_DOWN + 0.3571 * minSpeed - 0.01694 * minSpeed * minSpeed;
        if (addon_mgt_range < 0) {
            addon_mgt_range = 0;
        }

        double retDistance_1[2];
        mdmLib.calculateMaxMinDist(curTest_1, oldTest_1, time,
            MAX_PLAUSIBLE_ACCEL, MAX_PLAUSIBLE_DECEL, MAX_PLAUSIBLE_SPEED,
            retDistance_1);
        double factorMin_1 = 1 - mdmLib.CircleCircleFactor(distance, curPositionConfidence->x, oldPositionConfidence->x, retDistance_1[0]);
        double factorMax_1 = mdmLib.OneSidedCircleSegmentFactor(distance,
            curPositionConfidence->x, oldPositionConfidence->x,
            retDistance_1[1] + params->MAX_MGT_RNG_UP);

        double retDistance_2[2];
        mdmLib.calculateMaxMinDist(curTest_2, oldTest_2, time,
            MAX_PLAUSIBLE_ACCEL, MAX_PLAUSIBLE_DECEL, MAX_PLAUSIBLE_SPEED,
            retDistance_2);
        double factorMin_2 = 1 - mdmLib.OneSidedCircleSegmentFactor(distance, curPositionConfidence->x, oldPositionConfidence->x, retDistance_2[0] - addon_mgt_range);
        double factorMax_2 = mdmLib.OneSidedCircleSegmentFactor(distance,
            curPositionConfidence->x, oldPositionConfidence->x,
            retDistance_2[1] + params->MAX_MGT_RNG_UP);

        double retDistance_0[2];
        mdmLib.calculateMaxMinDist(curSpeed, oldspeed, time,
            MAX_PLAUSIBLE_ACCEL, MAX_PLAUSIBLE_DECEL, MAX_PLAUSIBLE_SPEED,
            retDistance_0);
        double factorMin_0 = 1 - mdmLib.OneSidedCircleSegmentFactor(distance, curPositionConfidence->x, oldPositionConfidence->x, retDistance_0[0] - addon_mgt_range);
        double factorMax_0 = mdmLib.OneSidedCircleSegmentFactor(distance,
            curPositionConfidence->x, oldPositionConfidence->x,
            retDistance_0[1] + params->MAX_MGT_RNG_UP);

        //return std::min(factorMin_0, factorMax_0);

        double factorMin = (factorMin_1 + factorMin_0 + factorMin_2) / 3.0;
        double factorMax = (factorMax_1 + factorMax_0 + factorMax_2) / 3.0;

        return std::min(factorMin, factorMax);
    }
    else {
        return 1;
    }
}

double ExperiChecks::IntersectionCheck(const veins::Coord* nodePosition1,
    const veins::Coord* nodePositionConfidence1, const veins::Coord* nodePosition2,
    const veins::Coord* nodePositionConfidence2, const veins::Coord* nodeHeading1,
    const veins::Coord* nodeHeading2, const veins::Coord* nodeSize1, const veins::Coord* nodeSize2,
    double deltaTime)
{

    //    double distance = mdmLib.calculateDistancePtr(nodePosition1, nodePosition2);
    //    double intFactor = mdmLib.CircleIntersectionFactor(
    //            nodePositionConfidence1->x, nodePositionConfidence2->x, distance,
    //            MIN_INT_DIST);
    //
    //    intFactor = intFactor *  ((MAX_DELTA_INTER - deltaTime) / MAX_DELTA_INTER);
    //
    //    intFactor = 1 - intFactor;
    //    return intFactor;

    double intFactor2 = mdmLib.EllipseEllipseIntersectionFactor(*nodePosition1,
        *nodePositionConfidence1, *nodePosition2, *nodePositionConfidence2,
        mdmLib.calculateHeadingAnglePtr(nodeHeading1),
        mdmLib.calculateHeadingAnglePtr(nodeHeading2), *nodeSize1,
        *nodeSize2);

    intFactor2 = 1.01 - intFactor2 * ((params->MAX_DELTA_INTER - deltaTime) / params->MAX_DELTA_INTER);

    if (intFactor2 > 1) {
        intFactor2 = 1;
    }

    return intFactor2;
}

InterTest ExperiChecks::MultipleIntersectionCheck(NodeTable* detectedNodes,
    BasicSafetyMessage* bsm)
{
    unsigned long senderPseudonym = bsm->getSenderPseudonym();
    veins::Coord senderPos = bsm->getSenderPos();
    veins::Coord senderPosConfidence = bsm->getSenderPosConfidence();
    veins::Coord senderHeading = bsm->getSenderHeading();

    veins::Coord senderSize = veins::Coord(bsm->getSenderWidth(), bsm->getSenderLength());

    NodeHistory* varNode;

    double INTScore = 0;
    InterTest intertTest = InterTest();

    INTScore = IntersectionCheck(&myPosition, &myPositionConfidence, &senderPos,
        &senderPosConfidence, &myHeading, &senderHeading, &mySize,
        &senderSize, 0.11);
    if (INTScore < 1) {
        intertTest.addInterValue(myPseudonym, INTScore);
    }

    for (int var = 0; var < detectedNodes->getNodesNum(); ++var) {
        if (detectedNodes->getNodePseudo(var) != senderPseudonym) {
            varNode = detectedNodes->getNodeHistoryAddr(
                detectedNodes->getNodePseudo(var));

            double deltaTime = mdmLib.calculateDeltaTime(
                varNode->getLatestBSMAddr(), bsm);

            if (deltaTime < params->MAX_DELTA_INTER) {

                veins::Coord varSize = veins::Coord(
                    varNode->getLatestBSMAddr()->getSenderWidth(),
                    varNode->getLatestBSMAddr()->getSenderLength());

                INTScore = IntersectionCheck(
                    &varNode->getLatestBSMAddr()->getSenderPos(),
                    &varNode->getLatestBSMAddr()->getSenderPosConfidence(),
                    &senderPos, &senderPosConfidence,
                    &varNode->getLatestBSMAddr()->getSenderHeading(),
                    &senderHeading, &varSize, &senderSize, deltaTime);
                if (INTScore < 1) {
                    intertTest.addInterValue(detectedNodes->getNodePseudo(var),
                        INTScore);
                }
            }
        }
    }

    return intertTest;
}

double ExperiChecks::SuddenAppearenceCheck(const veins::Coord* receiverPosition,
    const veins::Coord* receiverPositionConfidence, const veins::Coord* senderPosition,
    const veins::Coord* senderPositionConfidence)
{
    double distance = mdmLib.calculateDistancePtr(senderPosition,
        receiverPosition);

    double r2 = params->MAX_SA_RANGE + receiverPositionConfidence->x;

    double factor = 0;
    if (senderPositionConfidence->x <= 0) {
        if (distance < r2) {
            factor = 0;
        }
        else {
            factor = 1;
        }
    }
    else {
        double area = mdmLib.calculateCircleCircleIntersection(
            senderPositionConfidence->x, r2, distance);

        factor = area / (PI * senderPositionConfidence->x * senderPositionConfidence->x);
        factor = 1 - factor;
    }

    return factor;
}

double ExperiChecks::PositionPlausibilityCheck(const veins::Coord* senderPosition,
    const veins::Coord* senderPositionConfidence, double senderSpeed,
    double senderSpeedConfidence)
{

    double speedd = senderSpeed - senderSpeedConfidence;
    if (speedd < 0) {
        speedd = 0;
    }

    if (speedd <= params->MAX_NON_ROUTE_SPEED) {
        return 1;
    }

    double resolution = senderPositionConfidence->x / 10;
    if (resolution < 1) {
        resolution = 1;
    }
    double resolutionDelta = resolution / 10;

    double failedCount = 0;
    double allCount = 1;

    if (LinkC->calculateDistance(*senderPosition, 50,
            50) > params->MAX_DISTANCE_FROM_ROUTE) {
        failedCount++;
    }

    int resolutionTheta = 0;

    for (double r = resolution; r < senderPositionConfidence->x;
         r = r + resolution) {
        resolutionTheta = (int) (PI * r / (resolution));
        //std::cout << r<< "#" << resolution << "^" << resolutionTheta<<"-"<<"\n";
        for (int t = 0; t < resolutionTheta; ++t) {
            veins::Coord p(senderPosition->x + r * cos(2 * PI * t / resolutionTheta),
                senderPosition->y + r * sin(2 * PI * t / resolutionTheta));

            double distLocal = LinkC->calculateDistance(p, 50, 50);

            double lofactor = 1 + (1 - (2 * mdmLib.gaussianSum(distLocal - (senderPositionConfidence->x - r), 2 * params->MAX_DISTANCE_FROM_ROUTE / 3) - 1));

            if (distLocal > params->MAX_DISTANCE_FROM_ROUTE) {
                failedCount = failedCount + lofactor;
            }
            allCount = allCount + lofactor;
        }
        resolution = resolution + resolutionDelta;
    }

    return (1 - (failedCount / allCount));
}

double ExperiChecks::BeaconFrequencyCheck(double timeNew, double timeOld)
{
    double timeDelta = timeNew - timeOld;
    if (timeDelta < params->MAX_BEACON_FREQUENCY) {
        return 0;
    }
    else {
        return 1;
    }
}

double ExperiChecks::PositionHeadingConsistancyCheck(const veins::Coord* curHeading,
    const veins::Coord* curHeadingConfidence, const veins::Coord* oldPosition,
    const veins::Coord* oldPositionConfidence, const veins::Coord* curPosition,
    const veins::Coord* curPositionConfidence, double deltaTime, double curSpeed,
    double curSpeedConfidence)
{
    if (deltaTime < params->POS_HEADING_TIME) {
        double distance = mdmLib.calculateDistancePtr(curPosition, oldPosition);
        if (distance < 1) {
            return 1;
        }

        if (curSpeed - curSpeedConfidence < 1) {
            return 1;
        }

        double curHeadingAngle = mdmLib.calculateHeadingAnglePtr(curHeading);

        veins::Coord relativePos = veins::Coord(curPosition->x - oldPosition->x,
            curPosition->y - oldPosition->y,
            curPosition->z - oldPosition->z);
        double positionAngle = mdmLib.calculateHeadingAnglePtr(&relativePos);
        double angleDelta = fabs(curHeadingAngle - positionAngle);
        if (angleDelta > 180) {
            angleDelta = 360 - angleDelta;
        }

        double angleLow = angleDelta - curHeadingConfidence->x;
        if (angleLow < 0) {
            angleLow = 0;
        }

        double angleHigh = angleDelta + curHeadingConfidence->x;
        if (angleHigh > 180) {
            angleHigh = 180;
        }

        double xLow = distance * cos(angleLow * PI / 180);

        double curFactorLow = 1;
        if (curPositionConfidence->x == 0) {
            if (angleLow <= params->MAX_HEADING_CHANGE) {
                curFactorLow = 1;
            }
            else {
                curFactorLow = 0;
            }
        }
        else {
            curFactorLow =
                mdmLib.calculateCircleSegment(curPositionConfidence->x,
                    curPositionConfidence->x + xLow) /
                (PI * curPositionConfidence->x * curPositionConfidence->x);
        }

        double oldFactorLow = 1;
        if (oldPositionConfidence->x == 0) {
            if (angleLow <= params->MAX_HEADING_CHANGE) {
                oldFactorLow = 1;
            }
            else {
                oldFactorLow = 0;
            }
        }
        else {
            oldFactorLow = 1 - mdmLib.calculateCircleSegment(oldPositionConfidence->x, oldPositionConfidence->x - xLow) / (PI * oldPositionConfidence->x * oldPositionConfidence->x);
        }

        double xHigh = distance * cos(angleHigh * PI / 180);
        double curFactorHigh = 1;
        if (curPositionConfidence->x == 0) {
            if (angleHigh <= params->MAX_HEADING_CHANGE) {
                curFactorHigh = 1;
            }
            else {
                curFactorHigh = 0;
            }
        }
        else {
            curFactorHigh =
                mdmLib.calculateCircleSegment(curPositionConfidence->x,
                    curPositionConfidence->x + xHigh) /
                (PI * curPositionConfidence->x * curPositionConfidence->x);
        }

        double oldFactorHigh = 1;
        if (oldPositionConfidence->x == 0) {
            if (angleHigh <= params->MAX_HEADING_CHANGE) {
                oldFactorHigh = 1;
            }
            else {
                oldFactorHigh = 0;
            }
        }
        else {
            oldFactorHigh = 1 - mdmLib.calculateCircleSegment(oldPositionConfidence->x, oldPositionConfidence->x - xHigh) / (PI * oldPositionConfidence->x * oldPositionConfidence->x);
        }

        double factor = (curFactorLow + oldFactorLow + curFactorHigh + oldFactorHigh) / 4;

        if (factor <= 0) {
            double factorL = -AUG_FACTOR * (2 * mdmLib.gaussianSum(angleLow, 2 * (params->MAX_HEADING_CHANGE) / 3) - 1);
            double factorH = -AUG_FACTOR * (2 * mdmLib.gaussianSum(angleHigh, 2 * (params->MAX_HEADING_CHANGE) / 3) - 1);
            if (factorH > factorL) {
                factor = factorH;
            }
            else {
                factor = factorL;
            }
            factor = 0;
        }

        //    if(factor<=0.0){
        //
        //        std::cout<<"curPos: "<<curPosition<<'\n';
        //        std::cout<<"oldPos: "<<oldPosition<<'\n';
        //
        //        std::cout<<"relativePos: "<<relativePos<<'\n';
        //
        //        std::cout<<"curFactorLow: "<<curFactorLow<<'\n';
        //        std::cout<<"oldFactorLow: "<<oldFactorLow<<'\n';
        //        std::cout<<"curFactorHigh: "<<curFactorHigh<<'\n';
        //        std::cout<<"oldFactorHigh: "<<oldFactorHigh<<'\n';
        //        std::cout<<"positionAngle: "<<positionAngle<<'\n';
        //        std::cout<<"curHeadingAngle: "<<curHeadingAngle<<'\n';
        //        std::cout<<"angleDelta: "<<angleDelta<<'\n';
        //        std::cout<<"distance: "<<distance<<'\n';
        //        std::cout<<"distance: "<<distance<<'\n';
        //        std::cout<<"xLow: "<<xLow<<'\n';
        //        if(factor == 0){
        //            std::cout<<"ZERO: "<<factor<<'\n';
        //        }else{
        //            std::cout<<"NONZ: "<<factor<<'\n';
        //        }
        //
        //     //   exit(0);
        //    }

        //        factor = (factor - 0.5) * 2;
        //        factor = mdmLib.gaussianSum(factor, (1.0 / 4.5));
        //        if (factor > 0.75) {
        //            factor = 1;
        //        }
        //        if (factor <0.001) {
        //            factor = 0;
        //        }

        return factor;
    }
    else {
        return 1;
    }
}

static double max_f1 = 1;
static double max_f2 = 1;
static double max_f3 = 1;
static double max_f4 = 1;
static double max_f5 = 1;
static double max_f6 = 1;
static double max_f7 = 1;

void ExperiChecks::KalmanPositionSpeedConsistancyCheck(const veins::Coord* curPosition,
    const veins::Coord* curPositionConfidence, const veins::Coord* curSpeed, const veins::Coord* curAccel,
    const veins::Coord* curSpeedConfidence, double time, Kalman_SVI* kalmanSVI,
    double retVal[])
{

    if (!kalmanSVI->isInit()) {
        retVal[0] = 1;
        retVal[1] = 1;
    }
    else {
        if (time < params->MAX_KALMAN_TIME) {
            float Delta[4];

            double Ax = curAccel->x;
            double Ay = curAccel->y;

            double curPosConfX = curPositionConfidence->x;
            if (curPosConfX < params->KALMAN_MIN_POS_RANGE) {
                curPosConfX = params->KALMAN_MIN_POS_RANGE;
            }

            double curPosConfY = curPositionConfidence->y;
            if (curPosConfY < params->KALMAN_MIN_POS_RANGE) {
                curPosConfY = params->KALMAN_MIN_POS_RANGE;
            }

            double curSpdConfX = curSpeedConfidence->x;
            if (curSpdConfX < params->KALMAN_MIN_SPEED_RANGE) {
                curSpdConfX = params->KALMAN_MIN_SPEED_RANGE;
            }

            double curSpdConfY = curSpeedConfidence->y;
            if (curSpdConfY < params->KALMAN_MIN_SPEED_RANGE) {
                curSpdConfY = params->KALMAN_MIN_SPEED_RANGE;
            }

            //            kalmanSVI->kalmanFilterJ_SVI.matrixOp_SVI.printVec("X0a", kalmanSVI->kalmanFilterJ_SVI.X0, 4);
            //            kalmanSVI->kalmanFilterJ_SVI.matrixOp_SVI.printVec("Xa", kalmanSVI->kalmanFilterJ_SVI.X, 4);

            kalmanSVI->getDeltaPos(time, curPosition->x, curPosition->y,
                curSpeed->x, curSpeed->y, Ax, Ay, curPosConfX, curPosConfY,
                curSpdConfX, curSpdConfY, Delta);

            double ret_1 = 1 - sqrt(pow(Delta[0], 2.0) + pow(Delta[2], 2.0)) / (params->KALMAN_POS_RANGE * curPosConfX * time);
            if (isnan(ret_1)) {
                ret_1 = 0;
            }
            if (ret_1 < max_f1) {
                max_f1 = ret_1;
            }

            //            if (ret_1 < 0) {
            //                ret_1 = 0;
            //            }

            double ret_2 = 1 - sqrt(pow(Delta[1], 2.0) + pow(Delta[3], 2.0)) / (params->KALMAN_SPEED_RANGE * curSpdConfX * time);
            if (isnan(ret_2)) {
                ret_2 = 0;
            }
            if (ret_2 < max_f2) {
                max_f2 = ret_2;
            }

            //            if (ret_2 < 0) {
            //                ret_2 = 0;
            //            }

            //            kalmanSVI->kalmanFilterJ_SVI.matrixOp_SVI.printVec("Xb", kalmanSVI->kalmanFilterJ_SVI.X, 4);
            //            kalmanSVI->kalmanFilterJ_SVI.matrixOp_SVI.printVec("Delta", Delta, 4);

            retVal[0] = ret_1;
            retVal[1] = ret_2;
        }
        else {
            retVal[0] = 1;
            retVal[1] = 1;
            kalmanSVI->setInitial(curPosition->x, curPosition->y, curSpeed->x,
                curSpeed->y);
        }
    }
}

void ExperiChecks::KalmanPositionSpeedScalarConsistancyCheck(
    const veins::Coord* curPosition, const veins::Coord* oldPosition, const veins::Coord* curPositionConfidence,
    const veins::Coord* curSpeed, const veins::Coord* curAccel, const veins::Coord* curSpeedConfidence,
    double time, Kalman_SC* kalmanSC, double retVal[])
{

    if (!kalmanSC->isInit()) {
        retVal[0] = 1;
        retVal[1] = 1;
    }
    else {
        if (time < params->MAX_KALMAN_TIME) {

            float Delta[2];

            double distance = mdmLib.calculateDistancePtr(curPosition,
                oldPosition);
            double curspd = mdmLib.calculateSpeedPtr(curSpeed);
            double curacl = mdmLib.calculateSpeedPtr(curAccel);

            double curPosConfX = curPositionConfidence->x;
            if (curPosConfX < params->KALMAN_MIN_POS_RANGE) {
                curPosConfX = params->KALMAN_MIN_POS_RANGE;
            }

            double curSpdConfX = curSpeedConfidence->x;
            if (curSpdConfX < params->KALMAN_MIN_SPEED_RANGE) {
                curSpdConfX = params->KALMAN_MIN_SPEED_RANGE;
            }

            kalmanSC->getDeltaPos(time, distance, curspd, curacl, curacl,
                curPosConfX, curSpdConfX, Delta);

            double ret_1 = 1 - (Delta[0] / (params->KALMAN_POS_RANGE * curPosConfX * time));
            if (isnan(ret_1)) {
                ret_1 = 0;
            }
            if (ret_1 < max_f3) {
                max_f3 = ret_1;
            }

            //            if (ret_1 < 0) {
            //                ret_1 = 0;
            //            }
            double ret_2 = 1 - (Delta[1] / (params->KALMAN_SPEED_RANGE * curSpdConfX * time));
            if (isnan(ret_2)) {
                ret_2 = 0;
            }
            if (ret_2 < max_f4) {
                max_f4 = ret_2;
            }
            //            if (ret_2 < 0) {
            //                ret_2 = 0;
            //            }
            //            kalmanSC->kalmanFilterJ_SC.matrixOp_SC.printVec("Xb", kalmanSC->kalmanFilterJ_SC.X, 2);
            //            kalmanSC->kalmanFilterJ_SC.matrixOp_SC.printVec("Delta", Delta, 2);

            retVal[0] = ret_1;
            retVal[1] = ret_2;
        }
        else {
            retVal[0] = 1;
            retVal[1] = 1;
            double curspd = mdmLib.calculateSpeedPtr(curSpeed);
            kalmanSC->setInitial(0, curspd);
        }
    }
}

double ExperiChecks::KalmanPositionConsistancyCheck(const veins::Coord* curPosition,
    const veins::Coord* oldPosition, const veins::Coord* curPosConfidence, double time,
    Kalman_SI* kalmanSI)
{
    if (!kalmanSI->isInit()) {
        return 1;
    }
    else {
        if (time < params->MAX_KALMAN_TIME) {
            float Delta[2];
            double Ax = (curPosition->x - oldPosition->x) / time;
            double Ay = (curPosition->y - oldPosition->y) / time;

            double curPosConfX = curPosConfidence->x;
            if (curPosConfX < params->KALMAN_MIN_POS_RANGE) {
                curPosConfX = params->KALMAN_MIN_POS_RANGE;
            }

            double curPosConfY = curPosConfidence->y;
            if (curPosConfY < params->KALMAN_MIN_POS_RANGE) {
                curPosConfY = params->KALMAN_MIN_POS_RANGE;
            }

            kalmanSI->getDeltaPos(time, curPosition->x, curPosition->y,
                curPosConfX, curPosConfY, Delta);

            double ret_1 = 1 - sqrt(pow(Delta[0], 2.0) + pow(Delta[1], 2.0)) / (4 * params->KALMAN_POS_RANGE * curPosConfX * time);
            if (isnan(ret_1)) {
                ret_1 = 0;
            }
            if (ret_1 < max_f5) {
                max_f5 = ret_1;
            }

            //            if (ret_1 < 0) {
            //                ret_1 = 0;
            //            }

            return ret_1;
        }
        else {
            kalmanSI->setInitial(curPosition->x, curPosition->y);
            return 1;
        }
    }
}

double ExperiChecks::KalmanPositionAccConsistancyCheck(const veins::Coord* curPosition,
    const veins::Coord* curSpeed, const veins::Coord* curPosConfidence, double time,
    Kalman_SI* kalmanSI)
{
    if (!kalmanSI->isInit()) {
        return 1;
    }
    else {
        if (time < params->MAX_KALMAN_TIME) {
            float Delta[2];
            double Ax = curSpeed->x;
            double Ay = curSpeed->y;

            double curPosConfX = curPosConfidence->x;
            if (curPosConfX < params->KALMAN_MIN_POS_RANGE) {
                curPosConfX = params->KALMAN_MIN_POS_RANGE;
            }

            double curPosConfY = curPosConfidence->y;
            if (curPosConfY < params->KALMAN_MIN_POS_RANGE) {
                curPosConfY = params->KALMAN_MIN_POS_RANGE;
            }

            kalmanSI->getDeltaPos(time, curPosition->x, curPosition->y, Ax, Ay,
                curPosConfX, curPosConfY, Delta);

            double ret_1 = 1 - sqrt(pow(Delta[0], 2.0) + pow(Delta[1], 2.0)) / (4 * params->KALMAN_POS_RANGE * curPosConfX * time);
            if (isnan(ret_1)) {
                ret_1 = 0;
            }
            if (ret_1 < max_f6) {
                max_f6 = ret_1;
            }

            //            if (ret_1 < 0) {
            //                ret_1 = 0;
            //            }

            return ret_1;
        }
        else {
            kalmanSI->setInitial(curPosition->x, curPosition->y);
            return 1;
        }
    }
}
double ExperiChecks::KalmanSpeedConsistancyCheck(const veins::Coord* curSpeed,
    const veins::Coord* curAccel, const veins::Coord* curSpeedConfidence, double time,
    Kalman_SI* kalmanSI)
{
    if (!kalmanSI->isInit()) {
        return 1;
    }
    else {
        if (time < params->MAX_KALMAN_TIME) {
            float Delta[2];
            double curSpdConfX = curSpeedConfidence->x;
            if (curSpdConfX < params->KALMAN_MIN_SPEED_RANGE) {
                curSpdConfX = params->KALMAN_MIN_SPEED_RANGE;
            }

            double curSpdConfY = curSpeedConfidence->y;
            if (curSpdConfY < params->KALMAN_MIN_SPEED_RANGE) {
                curSpdConfY = params->KALMAN_MIN_SPEED_RANGE;
            }

            kalmanSI->getDeltaPos(time, curSpeed->x, curSpeed->y, curAccel->x,
                curAccel->y, curSpdConfX, curSpdConfY, Delta);

            double ret_1 = 1 - sqrt(pow(Delta[0], 2.0) + pow(Delta[1], 2.0)) / (params->KALMAN_SPEED_RANGE * curSpdConfX * time);
            if (isnan(ret_1)) {
                ret_1 = 0;
            }
            if (ret_1 < max_f7) {
                max_f7 = ret_1;
            }
            //            if (ret_1 < 0) {
            //                ret_1 = 0;
            //            }

            return ret_1;
        }
        else {
            kalmanSI->setInitial(curSpeed->x, curSpeed->y);
            return 1;
        }
    }
}

BsmCheck ExperiChecks::CheckBSM(BasicSafetyMessage* bsm,
    NodeTable* detectedNodes)
{
    BsmCheck bsmCheck = BsmCheck();

    unsigned long senderPseudonym = bsm->getSenderPseudonym();
    veins::Coord senderPos = bsm->getSenderPos();
    veins::Coord senderPosConfidence = bsm->getSenderPosConfidence();
    NodeHistory* senderNode;
    MDMHistory* senderMDM;
    bool newNode = false;

    if (detectedNodes->includes(senderPseudonym)) {
        senderNode = detectedNodes->getNodeHistoryAddr(
            senderPseudonym);
        senderMDM = detectedNodes->getMDMHistoryAddr(senderPseudonym);
    }
    else {
        newNode = true;
    }

    bsmCheck.setProximityPlausibility(ProximityPlausibilityCheck(&bsm->getSenderPos(), &myPosition, &myHeading));

    bsmCheck.setRangePlausibility(
        RangePlausibilityCheck(&myPosition, &myPositionConfidence,
            &senderPos, &senderPosConfidence));

    bsmCheck.setSpeedPlausibility(
        SpeedPlausibilityCheck(
            mdmLib.calculateSpeedPtr(&bsm->getSenderSpeed()),
            mdmLib.calculateSpeedPtr(
                &bsm->getSenderSpeedConfidence())));

    bsmCheck.setIntersection(MultipleIntersectionCheck(detectedNodes, bsm));

    bsmCheck.setPositionPlausibility(
        PositionPlausibilityCheck(&senderPos, &senderPosConfidence,
            mdmLib.calculateSpeedPtr(&bsm->getSenderSpeed()),
            mdmLib.calculateSpeedPtr(
                &bsm->getSenderSpeedConfidence())));

    if (!newNode) {
        bsmCheck.setPositionConsistancy(
            PositionConsistancyCheck(&senderPos, &senderPosConfidence,
                &senderNode->getLatestBSMAddr()->getSenderPos(),
                &senderNode->getLatestBSMAddr()->getSenderPosConfidence(),
                mdmLib.calculateDeltaTime(bsm,
                    senderNode->getLatestBSMAddr())));

        bsmCheck.setSpeedConsistancy(
            SpeedConsistancyCheck(
                mdmLib.calculateSpeedPtr(&bsm->getSenderSpeed()),
                mdmLib.calculateSpeedPtr(
                    &bsm->getSenderSpeedConfidence()),
                mdmLib.calculateSpeedPtr(
                    &senderNode->getLatestBSMAddr()->getSenderSpeed()),
                mdmLib.calculateSpeedPtr(
                    &senderNode->getLatestBSMAddr()->getSenderSpeedConfidence()),
                mdmLib.calculateDeltaTime(bsm,
                    senderNode->getLatestBSMAddr())));

        bsmCheck.setBeaconFrequency(
            BeaconFrequencyCheck(bsm->getArrivalTime().dbl(),
                senderNode->getLatestBSMAddr()->getArrivalTime().dbl()));

        bsmCheck.setPositionHeadingConsistancy(
            PositionHeadingConsistancyCheck(&bsm->getSenderHeading(),
                &bsm->getSenderHeadingConfidence(),
                &senderNode->getLatestBSMAddr()->getSenderPos(),
                &senderNode->getLatestBSMAddr()->getSenderPosConfidence(),
                &bsm->getSenderPos(), &bsm->getSenderPosConfidence(),
                mdmLib.calculateDeltaTime(bsm,
                    senderNode->getLatestBSMAddr()),
                mdmLib.calculateSpeedPtr(&bsm->getSenderSpeed()),
                mdmLib.calculateSpeedPtr(
                    &bsm->getSenderSpeedConfidence())));

        bsmCheck.setPositionSpeedConsistancy(
            PositionSpeedConsistancyCheck(&senderPos, &senderPosConfidence,
                &senderNode->getLatestBSMAddr()->getSenderPos(),
                &senderNode->getLatestBSMAddr()->getSenderPosConfidence(),
                mdmLib.calculateSpeedPtr(&bsm->getSenderSpeed()),
                mdmLib.calculateSpeedPtr(
                    &bsm->getSenderSpeedConfidence()),
                mdmLib.calculateSpeedPtr(
                    &senderNode->getLatestBSMAddr()->getSenderSpeed()),
                mdmLib.calculateSpeedPtr(
                    &senderNode->getLatestBSMAddr()->getSenderSpeedConfidence()),
                mdmLib.calculateDeltaTime(bsm,
                    senderNode->getLatestBSMAddr())));

        bsmCheck.setPositionSpeedMaxConsistancy(
            PositionSpeedMaxConsistancyCheck(&senderPos,
                &senderPosConfidence,
                &senderNode->getLatestBSMAddr()->getSenderPos(),
                &senderNode->getLatestBSMAddr()->getSenderPosConfidence(),
                mdmLib.calculateSpeedPtr(&bsm->getSenderSpeed()),
                mdmLib.calculateSpeedPtr(
                    &bsm->getSenderSpeedConfidence()),
                mdmLib.calculateSpeedPtr(
                    &senderNode->getLatestBSMAddr()->getSenderSpeed()),
                mdmLib.calculateSpeedPtr(
                    &senderNode->getLatestBSMAddr()->getSenderSpeedConfidence()),
                mdmLib.calculateDeltaTime(bsm,
                    senderNode->getLatestBSMAddr())));

        if (mdmLib.calculateDeltaTime(bsm,
                senderNode->getLatestBSMAddr()) > params->MAX_SA_TIME) {
            bsmCheck.setSuddenAppearence(
                SuddenAppearenceCheck(&senderPos, &senderPosConfidence,
                    &myPosition, &myPositionConfidence));
        }

        double retVal[2];
        double retValSC[2];

        Kalman_SVI* kalmanSVI;
        if (version == 2) {
            kalmanSVI = senderMDM->getKalmanSviv2();
        }
        else {
            kalmanSVI = senderMDM->getKalmanSviv1();
        }

        Kalman_SI* kalmanSI;
        if (version == 2) {
            kalmanSI = senderMDM->getKalmanSiv2();
        }
        else {
            kalmanSI = senderMDM->getKalmanSiv1();
        }

        Kalman_SC* kalmanSC;
        if (version == 2) {
            kalmanSC = senderMDM->getKalmanSvsiv2();
        }
        else {
            kalmanSC = senderMDM->getKalmanSvsiv1();
        }

        Kalman_SI* kalmanSAI;
        if (version == 2) {
            kalmanSAI = senderMDM->getKalmanSaiv2();
        }
        else {
            kalmanSAI = senderMDM->getKalmanSaiv1();
        }

        Kalman_SI* kalmanVI;
        if (version == 2) {
            kalmanVI = senderMDM->getKalmanViv2();
        }
        else {
            kalmanVI = senderMDM->getKalmanViv1();
        }

        KalmanPositionSpeedConsistancyCheck(&senderPos, &senderPosConfidence,
            &bsm->getSenderSpeed(), &bsm->getSenderAccel(),
            &bsm->getSenderSpeedConfidence(),
            mdmLib.calculateDeltaTime(bsm, senderNode->getLatestBSMAddr()),
            kalmanSVI, retVal);
        bsmCheck.setKalmanPSCP(retVal[0]);
        bsmCheck.setKalmanPSCS(retVal[1]);

        KalmanPositionSpeedScalarConsistancyCheck(&senderPos,
            &senderNode->getLatestBSMAddr()->getSenderPos(),
            &senderPosConfidence, &bsm->getSenderSpeed(),
            &bsm->getSenderAccel(), &bsm->getSenderSpeedConfidence(),
            mdmLib.calculateDeltaTime(bsm, senderNode->getLatestBSMAddr()),
            kalmanSC, retValSC);

        bsmCheck.setKalmanPSCSP(retValSC[0]);
        bsmCheck.setKalmanPSCSS(retValSC[1]);

        bsmCheck.setKalmanPCC(
            KalmanPositionConsistancyCheck(&senderPos,
                &senderNode->getLatestBSMAddr()->getSenderPos(),
                &senderPosConfidence,
                mdmLib.calculateDeltaTime(bsm,
                    senderNode->getLatestBSMAddr()),
                kalmanSI));

        bsmCheck.setKalmanPACS(
            KalmanPositionAccConsistancyCheck(&senderPos,
                &senderNode->getLatestBSMAddr()->getSenderSpeed(),
                &senderPosConfidence,
                mdmLib.calculateDeltaTime(bsm,
                    senderNode->getLatestBSMAddr()),
                kalmanSI));

        bsmCheck.setKalmanSCC(
            KalmanSpeedConsistancyCheck(&bsm->getSenderSpeed(),
                &bsm->getSenderAccel(),
                &bsm->getSenderSpeedConfidence(),
                mdmLib.calculateDeltaTime(bsm,
                    senderNode->getLatestBSMAddr()),
                kalmanVI));
    }
    else {
        bsmCheck.setSuddenAppearence(
            SuddenAppearenceCheck(&senderPos, &senderPosConfidence,
                &myPosition, &myPositionConfidence));
    }
    //    std::cout << "======================================="<<"\n";
    //    std::cout << "max_f1 => " << max_f1 << '\n';
    //    std::cout << "max_f2 => " << max_f2 << '\n';
    //    std::cout << "max_f3 => " << max_f3 << '\n';
    //    std::cout << "max_f4 => " << max_f4 << '\n';
    //    std::cout << "max_f5 => " << max_f5 << '\n';
    //    std::cout << "max_f6 => " << max_f6 << '\n';
    //    std::cout << "max_f7 => " << max_f7 << '\n';

    //    if(bsm->getSenderMbType() == 1){
    //    PrintBsmCheck(senderPseudonym, bsmCheck);
    //    }

    return bsmCheck;
}

void ExperiChecks::PrintBsmCheck(unsigned long senderPseudonym,
    BsmCheck bsmCheck)
{

    double assignedThre = 0.5;

    if (bsmCheck.getProximityPlausibility() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "RAD FAILED => "
                  << bsmCheck.getProximityPlausibility() << " A:" << myPseudonym
                  << " B:" << senderPseudonym << '\n';
    }

    if (bsmCheck.getRangePlausibility() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "ART FAILED => "
                  << bsmCheck.getRangePlausibility() << " A:" << myPseudonym
                  << " B:" << senderPseudonym << '\n';
    }
    if (bsmCheck.getPositionConsistancy() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "MGTD FAILED => "
                  << bsmCheck.getPositionConsistancy() << " A:" << myPseudonym
                  << " B:" << senderPseudonym << '\n';
    }
    if (bsmCheck.getSpeedConsistancy() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "MGTS FAILED => "
                  << bsmCheck.getSpeedConsistancy() << " A:" << myPseudonym
                  << " B:" << senderPseudonym << '\n';
    }

    if (bsmCheck.getPositionSpeedConsistancy() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "MGTSV FAILED => "
                  << bsmCheck.getPositionSpeedConsistancy() << " A:"
                  << myPseudonym << " B:" << senderPseudonym << '\n';
    }

    if (bsmCheck.getPositionSpeedMaxConsistancy() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "MGTSVM FAILED => "
                  << bsmCheck.getPositionSpeedMaxConsistancy() << " A:"
                  << myPseudonym << " B:" << senderPseudonym << '\n';
    }

    if (bsmCheck.getSpeedPlausibility() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "MAXS FAILED => "
                  << bsmCheck.getSpeedPlausibility() << " A:" << myPseudonym
                  << " B:" << senderPseudonym << '\n';
    }
    if (bsmCheck.getPositionPlausibility() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "MAP FAILED => "
                  << bsmCheck.getPositionPlausibility() << " A:" << myPseudonym
                  << " B:" << senderPseudonym << '\n';
    }

    //    if (bsmCheck.getSuddenAppearence() < assignedThre) {
    //        std::cout << "^^^^^^^^^^^V2 " << "SAW FAILED => "
    //                << bsmCheck.getSuddenAppearence() << " A:" << myPseudonym
    //                << " B:" << senderPseudonym << '\n';
    //    }

    if (bsmCheck.getPositionHeadingConsistancy() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "PHC FAILED => "
                  << bsmCheck.getPositionHeadingConsistancy() << " A:"
                  << myPseudonym << " B:" << senderPseudonym << '\n';
    }

    if (bsmCheck.getBeaconFrequency() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "FREQ FAILED => "
                  << bsmCheck.getBeaconFrequency() << " A:" << myPseudonym
                  << " B:" << senderPseudonym << '\n';
    }
    if (bsmCheck.getKalmanSCC() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "KalmanSCC FAILED => "
                  << bsmCheck.getKalmanSCC() << " A:" << myPseudonym << " B:"
                  << senderPseudonym << '\n';
    }
    if (bsmCheck.getKalmanSCC() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "KalmanPACS FAILED => "
                  << bsmCheck.getKalmanSCC() << " A:" << myPseudonym << " B:"
                  << senderPseudonym << '\n';
    }
    if (bsmCheck.getKalmanPCC() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "KalmanPCC FAILED => "
                  << bsmCheck.getKalmanPCC() << " A:" << myPseudonym << " B:"
                  << senderPseudonym << '\n';
    }
    if (bsmCheck.getKalmanPSCP() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "KalmanPSCP FAILED => "
                  << bsmCheck.getKalmanPSCP() << " A:" << myPseudonym << " B:"
                  << senderPseudonym << '\n';
    }
    if (bsmCheck.getKalmanPSCS() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "KalmanPSCS FAILED => "
                  << bsmCheck.getKalmanPSCS() << " A:" << myPseudonym << " B:"
                  << senderPseudonym << '\n';
    }
    if (bsmCheck.getKalmanPSCSP() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "KalmanPSCSP FAILED => "
                  << bsmCheck.getKalmanPSCSP() << " A:" << myPseudonym << " B:"
                  << senderPseudonym << '\n';
    }
    if (bsmCheck.getKalmanPSCSP() < assignedThre) {
        std::cout << "^^^^^^^^^^^V2 "
                  << "KalmanPSCSS FAILED => "
                  << bsmCheck.getKalmanPSCSP() << " A:" << myPseudonym << " B:"
                  << senderPseudonym << '\n';
    }
    InterTest inter = bsmCheck.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {
        if (inter.getInterValue(var) < assignedThre) {
            std::cout << "^^^^^^^^^^^V2 "
                      << "INT FAILED => "
                      << inter.getInterValue(var) << " A:" << myPseudonym << " B:"
                      << senderPseudonym << " C:" << inter.getInterId(var)
                      << '\n';
        }
    }
}
