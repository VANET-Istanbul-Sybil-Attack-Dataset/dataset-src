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
#include <veins/modules/application/f2md/mdApplications/ThresholdApp.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace boost;

ThresholdApp::ThresholdApp(int version, double Threshold) :
        MDApplication(version) {
    this->Threshold = Threshold;
}

bool ThresholdApp::CheckNodeForReport(unsigned long myPseudonym,
        BasicSafetyMessage * bsm, BsmCheck * bsmCheck,
        NodeTable * detectedNodes) {

    bool checkFailed = false;

    prntApp->incAll(mbTypes::intMbs[bsm->getSenderMbType()]);
    prntAppInst->incAll(mbTypes::intMbs[bsm->getSenderMbType()]);

    calculateMinFactor(bsmCheck);
    incrementDetailedFlags(bsm,bsmCheck,Threshold);

    InterTest inter = bsmCheck->getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {
        if (inter.getInterValue(var) <= Threshold) {
            checkFailed = true;
            break;
        }
    }

    if (checkFailed || bsmCheck->getProximityPlausibility() <= Threshold
    || bsmCheck->getRangePlausibility() <= Threshold
            || bsmCheck->getPositionConsistancy() <= Threshold
            || bsmCheck->getPositionSpeedConsistancy() <= Threshold
            || bsmCheck->getPositionSpeedMaxConsistancy() <= Threshold
            || bsmCheck->getSpeedConsistancy() <= Threshold
            || bsmCheck->getSpeedPlausibility() <= Threshold
            || bsmCheck->getPositionPlausibility() <= Threshold
            || bsmCheck->getBeaconFrequency() <= Threshold
            || bsmCheck->getPositionHeadingConsistancy() <= Threshold
            || bsmCheck->getKalmanPSCP() <= Threshold
            || bsmCheck->getKalmanPSCS() <= Threshold
            || bsmCheck->getKalmanPSCSP() <= Threshold
            || bsmCheck->getKalmanPSCSS() <= Threshold
            || bsmCheck->getKalmanPCC() <= Threshold
            || bsmCheck->getKalmanPACS() <= Threshold
            || bsmCheck->getKalmanSCC() <= Threshold) {
        checkFailed = true;
    }


    if (checkFailed) {
        prntApp->incCumulFlags(mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incCumulFlags(mbTypes::intMbs[bsm->getSenderMbType()]);

//        if(bsm->getSenderMbType() == 0){
//            prntApp->printOutDebug();
//        }

    }

    return checkFailed;
}

double ThresholdApp::getMinFactor() {
    return minFactor;
}
