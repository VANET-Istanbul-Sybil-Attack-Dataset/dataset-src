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

#ifndef __VEINS_ExperiChecks_H_
#define __VEINS_ExperiChecks_H_

#include <tuple>

#include <omnetpp.h>
#include <veins/modules/application/f2md/mdBase/NodeMDMHistory.h>
#include <veins/modules/application/f2md/mdBase/NodeTable.h>
#include <veins/modules/application/f2md/mdReport/MDReport.h>
#include <veins/modules/application/f2md/mdStats/MDStatistics.h>
#include <veins/modules/application/f2md/mdSupport/networkLinksLib/LinkControl.h>
#include "../mdBase/NodeTable.h"
#include "../mdBase/InterTest.h"
#include "../mdBase/BsmCheck.h"
#include "../mdBase/InterTest.h"
#include "../mdSupport/MDMLib.h"
#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/Obstacle.h"

#include "../mdSupport/kalmanLib/Kalman_SVI.h"
#include "../mdSupport/kalmanLib/Kalman_SI.h"
#include <unordered_map>


using namespace veins;
using namespace omnetpp;

class ExperiChecks {
private:
    int version = 0;
    unsigned long myPseudonym;
    veins::Coord myPosition;
    veins::Coord myPositionConfidence;

    veins::Coord myHeading;
    veins::Coord myHeadingConfidence;

    veins::Coord mySize;

    LAddress::L2Type myId;

    MDMLib mdmLib;

    LinkControl* LinkC;

    std::unordered_map<LAddress::L2Type,veins::Coord> *realDynamicMap;

    double MAX_PLAUSIBLE_SPEED = 0;
    double MAX_PLAUSIBLE_ACCEL = 0;
    double MAX_PLAUSIBLE_DECEL = 0;
    F2MDParameters * params;

    double ProximityPlausibilityCheck(const veins::Coord*testPosition, const veins::Coord*myPosition, const veins::Coord* myHeading);

    double RangePlausibilityCheck(const veins::Coord*, const veins::Coord*, const veins::Coord*, const veins::Coord*);
    double PositionPlausibilityCheck(const veins::Coord*, const veins::Coord*, double, double);
    double SpeedPlausibilityCheck(double, double);
    double PositionConsistancyCheck(const veins::Coord * curPosition,
            const veins::Coord * curPositionConfidence, const veins::Coord * oldPosition,
            const veins::Coord * oldPositionConfidence, double time);
    double SpeedConsistancyCheck(double, double, double, double, double);
    double IntersectionCheck(const veins::Coord * nodePosition1,
            const veins::Coord * nodePositionConfidence1, const veins::Coord * nodePosition2,
            const veins::Coord* nodePositionConfidence2, const veins::Coord * nodeHeading1,
            const veins::Coord * nodeHeading2, const veins::Coord* nodeSize1, const veins::Coord * nodeSize2,
            double deltaTime);
    InterTest MultipleIntersectionCheck(NodeTable * detectedNodes,
            BasicSafetyMessage * bsm);

    double PositionSpeedMaxConsistancyCheck(const veins::Coord * curPosition,
            const veins::Coord * curPositionConfidence, const veins::Coord * oldPosition,
            const veins::Coord * oldPositionConfidence, double curSpeed,
            double curSpeedConfidence, double oldspeed,
            double oldSpeedConfidence, double time);

    double PositionSpeedConsistancyCheck(const veins::Coord * curPosition,
            const veins::Coord * curPositionConfidence, const veins::Coord * oldPosition,
            const veins::Coord * oldPositionConfidence, double curSpeed,
            double curSpeedConfidence, double oldspeed,
            double oldSpeedConfidence, double time);

    void KalmanPositionSpeedConsistancyCheck(const veins::Coord * curPosition,
            const veins::Coord * curPositionConfidence, const veins::Coord * curSpeed, const veins::Coord * curAccel,
            const veins::Coord * curSpeedConfidence, double time, Kalman_SVI * kalmanSVI,
            double retVal[]);

    void KalmanPositionSpeedScalarConsistancyCheck(const veins::Coord * curPosition,const veins::Coord * oldPosition,
            const veins::Coord * curPositionConfidence, const veins::Coord * curSpeed, const veins::Coord * curAccel,
            const veins::Coord * curSpeedConfidence, double time, Kalman_SC * kalmanSC,
            double retVal[]);

    double KalmanPositionConsistancyCheck(const veins::Coord * curPosition, const veins::Coord * oldPosition, const veins::Coord * curPosConfidence,
             double time, Kalman_SI * kalmanSI);

    double KalmanPositionAccConsistancyCheck(const veins::Coord * curPosition, const veins::Coord * curSpeed, const veins::Coord * curPosConfidence,
             double time, Kalman_SI * kalmanSI);

    double KalmanSpeedConsistancyCheck(const veins::Coord * curSpeed, const veins::Coord * curAccel, const veins::Coord * curSpeedConfidence,
            double time, Kalman_SI * kalmanSI);

    double PositionHeadingConsistancyCheck(const veins::Coord * curHeading,
            const veins::Coord * curHeadingConfidence, const veins::Coord * oldPosition,
            const veins::Coord * oldPositionConfidence, const veins::Coord * curPositionConfidence,
            const veins::Coord * curPosition, double deltaTime, double curSpeed,
            double curSpeedConfidence);

    double BeaconFrequencyCheck(double, double);
    double SuddenAppearenceCheck(const veins::Coord*, const veins::Coord*, const veins::Coord*, const veins::Coord*);

    void PrintBsmCheck(unsigned long senderPseudonym, BsmCheck bsmCheck);

    void resetAll();

public:
    ExperiChecks(int version, unsigned long myPseudonym, veins::Coord myPosition,
            veins::Coord myPositionConfidence, veins::Coord myHeading,
            veins::Coord myHeadingConfidence, veins::Coord mySize, veins::Coord myLimits,
            LinkControl* LinkC,std::unordered_map<LAddress::L2Type,veins::Coord> *realDynamicMap, LAddress::L2Type myId, F2MDParameters * params);
    BsmCheck CheckBSM(BasicSafetyMessage * bsm, NodeTable * detectedNodes);

};

#endif
