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

#ifndef __VEINS_MDAttack_H_
#define __VEINS_MDAttack_H_

#include "MDAttackBase.h"

class MDAttack: public MDAttackBase {
protected:
    BasicSafetyMessage saveAttackBsm = BasicSafetyMessage();

    BasicSafetyMessage staleMessagesBsm[MAX_STALE_NUM];
    int staleMessagesNum = 0;

    double localStopProb = 0;

    BasicSafetyMessage StopBsm;
    bool StopInitiated;

    bool DoSInitiated;

    unsigned long SybilMyOldPseudo = 0;
    unsigned long SybilPseudonyms[MAX_SYBIL_NUM];
    int SybilVehSeq = 0;

    double saveHeading = 0;

    unsigned long targetNode = 0;

    double MaxRandomPosX;
    double MaxRandomPosY;

    double ConstPosX;
    double ConstPosY;

    double ConstPosOffsetX;
    double ConstPosOffsetY;

    double ConstSpeedX;
    double ConstSpeedY;

    double ConstSpeedOffsetX;
    double ConstSpeedOffsetY;


    int localReplaySeqNum = 0;
    int ReplaySeq = 0;

    int localDosMultipleFreq = 0;
    int localDosMultipleFreqSybil = 0;

    int localSybilVehNumber = 0;

    double localSybilDistanceX = 0;
    double localSybilDistanceY = 0;

    F2MDParameters *params;

public:

    MDAttack();

    void init(attackTypes::Attacks myAttackType, double MaxRandomPosX,double MaxRandomPosY,F2MDParameters *params);

    BasicSafetyMessage launchAttack(attackTypes::Attacks myAttackType, LinkControl* LinkC);

    unsigned long getTargetNode();
};

#endif
