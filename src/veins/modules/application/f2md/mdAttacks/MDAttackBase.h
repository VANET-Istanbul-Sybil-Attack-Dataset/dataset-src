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

#ifndef __VEINS_MDAttackBase_H_
#define __VEINS_MDAttackBase_H_

#include <tuple>
#include <omnetpp.h>
#include <veins/modules/application/f2md/mdSupport/networkLinksLib/LinkControl.h>

#include "../mdBase/NodeTable.h"
#include "../mdPCPolicies/PCPolicy.h"

#include "../F2MDParameters.h"

using namespace veins;
using namespace omnetpp;

class MDAttackBase {
protected:
    GeneralLib genLib = GeneralLib();

    BasicSafetyMessage* myBsm;
    int* myBsmNum;

    NodeTable* detectedNodes;

    unsigned long* myPseudonym;

    veins::Coord* curPosition;
    veins::Coord* curPositionConfidence;
    veins::Coord* curSpeed;
    veins::Coord* curSpeedConfidence;
    veins::Coord* curHeading;
    veins::Coord* curHeadingConfidence;
    veins::Coord* curAccel;
    veins::Coord* curAccelConfidence;

    simtime_t* beaconInterval;

    double* myWidth;
    double* myLength;

    PCPolicy* pcPolicy;

public:
    virtual void setBeaconInterval(simtime_t* beaconInterval);
    virtual void setCurHeading(veins::Coord* curHeading);
    virtual void setCurHeadingConfidence(veins::Coord* curHeadingConfidence);
    virtual void setCurPosition(veins::Coord* curPosition);
    virtual void setCurPositionConfidence(veins::Coord* curPositionConfidence);
    virtual void setCurSpeed(veins::Coord* curSpeed);
    virtual void setCurSpeedConfidence(veins::Coord* curSpeedConfidence);
    virtual void setCurAccel(veins::Coord* curAccel);
    virtual void setCurAccelConfidence(veins::Coord* curAccelConfidence);

    virtual void setDetectedNodes(NodeTable* detectedNodes);
    virtual void setMyBsm(BasicSafetyMessage* myBsm);
    virtual void setMyBsmNum(int* myBsmNum);
    virtual void setMyLength(double* myLength);
    virtual void setMyPseudonym(unsigned long * myPseudonym);
    virtual void setMyWidth(double* myWidth);
    virtual void setPcPolicy(PCPolicy* pcPolicy);

    inline virtual void init(attackTypes::Attacks myAttackType, double MaxRandomPosX,double MaxRandomPosY,F2MDParameters *params) {}
    virtual BasicSafetyMessage launchAttack(attackTypes::Attacks myAttackType, LinkControl* LinkC);
    inline unsigned long getTargetNode() {return 0; }
};

#endif
