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

#ifndef __VEINS_LegacyChecks_H_
#define __VEINS_LegacyChecks_H_

#include <tuple>

#include <omnetpp.h>
#include <veins/modules/application/f2md/mdBase/NodeTable.h>
#include <veins/modules/application/f2md/mdReport/MDReport.h>
#include <veins/modules/application/f2md/mdStats/MDStatistics.h>
#include <veins/modules/application/f2md/mdSupport/networkLinksLib/LinkControl.h>
#include "../mdSupport/MDMLib.h"
#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/Obstacle.h"
#include <unordered_map>

using namespace veins;
using namespace omnetpp;

class LegacyChecks {

private:
    int version = 0;
    unsigned long myPseudonym;
    veins::Coord myPosition;
    veins::Coord mySpeed;
    veins::Coord mySize;
    veins::Coord myHeading;

    MDMLib mdmLib;

    double MAX_PLAUSIBLE_SPEED = 0;
    double MAX_PLAUSIBLE_ACCEL = 0;
    double MAX_PLAUSIBLE_DECEL = 0;

    LinkControl* LinkC;

    LAddress::L2Type myId;

    F2MDParameters * params;

    std::unordered_map<LAddress::L2Type,veins::Coord> *realDynamicMap;

    double ProximityPlausibilityCheck(const veins::Coord*testPosition, const veins::Coord*myPosition, const veins::Coord* myHeading);

    double RangePlausibilityCheck(const veins::Coord*, const veins::Coord*);
    double PositionConsistancyCheck(const veins::Coord*, const veins::Coord*, double);
    double SpeedConsistancyCheck(double, double, double);
    double PositionSpeedConsistancyCheck(const veins::Coord*, const veins::Coord *, double, double,
            double);
    double PositionSpeedMaxConsistancyCheck(const veins::Coord*, const veins::Coord *, double, double,
            double);
    double SpeedPlausibilityCheck(double);
    double IntersectionCheck(veins::Coord nodePosition1, veins::Coord nodeSize1, veins::Coord head1,
            veins::Coord nodePosition2, veins::Coord nodeSize2, veins::Coord head2,
            double deltaTime);
    double SuddenAppearenceCheck(const veins::Coord*, const veins::Coord*);
    double BeaconFrequencyCheck(double, double);
    double PositionPlausibilityCheck(const veins::Coord*, double);
    double PositionHeadingConsistancyCheck(const veins::Coord* curHeading,
            const veins::Coord *curPosition, const veins::Coord *oldPosition, double deltaTime,
            double curSpeed);

    void KalmanPositionSpeedConsistancyCheck(const veins::Coord * curPosition,
            const veins::Coord * curPositionConfidence, const veins::Coord * curSpeed, const veins::Coord * oldSpeed,
            const veins::Coord * curSpeedConfidence, double time, Kalman_SVI * kalmanSVI,
            double retVal[]);

    void KalmanPositionSpeedScalarConsistancyCheck(const veins::Coord * curPosition,const veins::Coord * oldPosition,
            const veins::Coord * curPositionConfidence, const veins::Coord * curSpeed, const veins::Coord * oldSpeed,
            const veins::Coord * curSpeedConfidence, double time, Kalman_SC * kalmanSC,
            double retVal[]);

    double KalmanPositionConsistancyCheck(const veins::Coord * curPosition, const veins::Coord * oldPosition, const veins::Coord * curPosConfidence,
             double time, Kalman_SI * kalmanSI);

    double KalmanPositionAccConsistancyCheck(const veins::Coord * curPosition, const veins::Coord * curSpeed, const veins::Coord * curPosConfidence,
             double time, Kalman_SI * kalmanSI);

    double KalmanSpeedConsistancyCheck(const veins::Coord * curSpeed, const veins::Coord * oldSpeed, const veins::Coord * curSpeedConfidence,
            double time, Kalman_SI * kalmanSI);

    InterTest MultipleIntersectionCheck(NodeTable * detectedNodes,
            BasicSafetyMessage * bsm);

    void PrintBsmCheck(unsigned long senderPseudonym, BsmCheck bsmCheck);

public:

    LegacyChecks(int version, unsigned long myPseudonym, veins::Coord myPosition, veins::Coord mySpeed,
            veins::Coord myHeading, veins::Coord mySize, veins::Coord myLimits, LinkControl* LinkC, std::unordered_map<LAddress::L2Type,veins::Coord> *realDynamicMap, LAddress::L2Type myId, F2MDParameters * params);
    BsmCheck CheckBSM(BasicSafetyMessage * bsm, NodeTable * detectedNodes);

};

#endif
