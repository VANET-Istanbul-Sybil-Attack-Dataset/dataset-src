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

#ifndef __VEINS_MDGlobalAttack_H_
#define __VEINS_MDGlobalAttack_H_

#include <tuple>
#include <omnetpp.h>
#include <veins/modules/mobility/traci/TraCICommandInterface.h>

#include "../mdBase/NodeTable.h"
#include "../mdPCPolicies/PCPolicy.h"

class MDGlobalAttack {
protected:
    GeneralLib genLib = GeneralLib();
    unsigned long* myPseudonym;
    TraCICommandInterface* traci;

    veins::Coord* curPosition;
    veins::Coord* curPositionConfidence;
    veins::Coord* curSpeed;
    veins::Coord* curSpeedConfidence;
    veins::Coord* curHeading;
    veins::Coord* curHeadingConfidence;
    veins::Coord* curAccel;
    veins::Coord* curAccelConfidence;


public:

    MDGlobalAttack();

    void init(attackTypes::Attacks myAttackType);

    MDReport launchAttack(attackTypes::Attacks myAttackType, BasicSafetyMessage* reportedBsm);


    void setMyPseudonym(unsigned long * myPseudonym);
    void setTraci(TraCICommandInterface* traci);

    void setCurHeading(veins::Coord* curHeading);
    void setCurHeadingConfidence(veins::Coord* curHeadingConfidence);
    void setCurPosition(veins::Coord* curPosition);
    void setCurPositionConfidence(veins::Coord* curPositionConfidence);
    void setCurSpeed(veins::Coord* curSpeed);
    void setCurSpeedConfidence(veins::Coord* curSpeedConfidence);
    void setCurAccel(veins::Coord* curAccel);
    void setCurAccelConfidence(veins::Coord* curAccelConfidence);
};

#endif
