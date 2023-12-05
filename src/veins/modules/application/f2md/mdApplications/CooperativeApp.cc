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
#include <stdlib.h>     /* atof */
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <veins/modules/application/f2md/mdApplications/CooperativeApp.h>
#include <iostream>
#include <string>
#include <vector>


using namespace std;
using namespace boost;

CooperativeApp::CooperativeApp(int version,
        double Threshold) :
        MDApplication(version) {
    this->Threshold = Threshold;
}

bool CooperativeApp::CheckNodeForReport(unsigned long myPseudonym,
        BasicSafetyMessage * bsm, BsmCheck * bsmCheck, NodeTable * detectedNodes) {
    double tempFactor = 0;

    prntApp->incAll(mbTypes::intMbs[bsm->getSenderMbType()]);
    prntAppInst->incAll(mbTypes::intMbs[bsm->getSenderMbType()]);
    calculateMinFactor(bsmCheck);
    incrementDetailedFlags(bsm,bsmCheck,Threshold);

    unsigned long senderId = bsm->getSenderPseudonym();

    bool checkFailed = false;

    int indexTMO = getIndexTMO(senderId);

    if (minFactor < 1) {

        if (indexTMO == -1) {
            indexTMO = addPseudoTMO(senderId);
        }

        double TMOadd = 0.0005*exp(10 * (1 - minFactor));

//        for (double var = 1; var >= 0; var = var - 0.1) {
//            double expAdd = 10 * (1 - var);
//            double expV = expAdd;
//            double TMOadd = 0.0005*exp(expV);
//            std::cout << var << " " << expAdd << " " << expV << " " << TMOadd
//                    << "\n";
//        }
//        exit(0);

        TimeOut[indexTMO] = TimeOut[indexTMO] + TMOadd;
        UpdatedTMO[TimeOutNum] = simTime().dbl();
    }


    if (indexTMO >= 0) {
        if (TimeOut[indexTMO] > 0) {
            if(TimeOut[indexTMO] > 5.5066){
                prntApp->incCumulFlags(mbTypes::intMbs[bsm->getSenderMbType()]);
                prntAppInst->incCumulFlags(mbTypes::intMbs[bsm->getSenderMbType()]);
                bsmCheck->setReported(true);
                checkFailed = true;
            }
            TimeOut[indexTMO] = TimeOut[indexTMO] - 1;
            if(TimeOut[indexTMO]<=0){
                TimeOut[indexTMO] == 0;
            }
        }
        if (TimeOut[indexTMO] <=0) {
            removePseudoTMO(indexTMO);
        }
    }
    return checkFailed;
}

int CooperativeApp::addPseudoTMO(unsigned long pseudo) {
    if (TimeOutNum >= MAX_DETECTED_NODES_COOP) {
        removeOldestPseudoTMO();
    }
    TimeOut[TimeOutNum] = 0;
    UpdatedTMO[TimeOutNum] = simTime().dbl();
    PseudonymsToTMO[TimeOutNum] = pseudo;
    TimeOutNum++;

    //std::cout<<"TimeOutNum:"<<TimeOutNum<<"\n";

    return TimeOutNum - 1;
}

void CooperativeApp::removeOldestPseudoTMO() {
    int oldestIndex = 0;
    double oldestTime = UpdatedTMO[oldestIndex];
    for (int var = 0; var < TimeOutNum; ++var) {
        if (oldestTime > UpdatedTMO[var]) {
            oldestTime = UpdatedTMO[var];
            oldestIndex = var;
        }
    }
    removePseudoTMO(oldestIndex);
}

void CooperativeApp::removePseudoTMO(int index) {
    for (int var = index; var < TimeOutNum; ++var) {
        TimeOut[var] = TimeOut[var + 1];
    }
    TimeOutNum--;
}

int CooperativeApp::getIndexTMO(unsigned long pseudo) {
    for (int var = 0; var < TimeOutNum; ++var) {
        if (PseudonymsToTMO[var] == pseudo) {
            return var;
        }
    }
    return -1;
}

double CooperativeApp::getMinFactor() {
    return minFactor;
}

