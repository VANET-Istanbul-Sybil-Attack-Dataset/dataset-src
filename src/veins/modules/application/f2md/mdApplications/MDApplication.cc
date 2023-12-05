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
#include <veins/modules/application/f2md/mdApplications/MDApplication.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace boost;

MDApplication::MDApplication(int version) {

    this->version = version;

    if (version == 1) {
        this->prntApp = &prntAppV1;
        this->prntAppInst = &prntAppInstV1;

        this->prntApp->setName(AppV1Name);
        this->prntAppInst->setName(AppV1Name);
    } else {
        this->prntApp = &prntAppV2;
        this->prntAppInst = &prntAppInstV2;

        this->prntApp->setName(AppV2Name);
        this->prntAppInst->setName(AppV2Name);
    }
}

double MDApplication::getMinFactor() {
    return minFactor;
}

void MDApplication::calculateMinFactor(BsmCheck * bsmCheck) {
    minFactor = 1;
    if (bsmCheck->getProximityPlausibility() < minFactor) {
        minFactor = bsmCheck->getProximityPlausibility();
    }
    if (bsmCheck->getRangePlausibility() < minFactor) {
        minFactor = bsmCheck->getRangePlausibility();
    }
    if (bsmCheck->getPositionConsistancy() < minFactor) {
        minFactor = bsmCheck->getPositionConsistancy();
    }
    if (bsmCheck->getPositionSpeedConsistancy() < minFactor) {
        minFactor = bsmCheck->getPositionSpeedConsistancy();
    }
    if (bsmCheck->getPositionSpeedMaxConsistancy() < minFactor) {
        minFactor = bsmCheck->getPositionSpeedMaxConsistancy();
    }
    if (bsmCheck->getSpeedConsistancy() < minFactor) {
        minFactor = bsmCheck->getSpeedConsistancy();
    }
    if (bsmCheck->getSpeedPlausibility() < minFactor) {
        minFactor = bsmCheck->getSpeedPlausibility();
    }
    if (bsmCheck->getPositionPlausibility() < minFactor) {
        minFactor = bsmCheck->getPositionPlausibility();
    }
    if (bsmCheck->getBeaconFrequency() < minFactor) {
        minFactor = bsmCheck->getBeaconFrequency();
    }
    if (bsmCheck->getSuddenAppearence() < minFactor) {
        //minFactor = bsmCheck->getSuddenAppearence();
    }
    if (bsmCheck->getPositionHeadingConsistancy() < minFactor) {
        minFactor = bsmCheck->getPositionHeadingConsistancy();
    }
    if (bsmCheck->getKalmanPSCP() < minFactor) {
        minFactor = bsmCheck->getKalmanPSCP();
    }
    if (bsmCheck->getKalmanPSCS() < minFactor) {
        minFactor = bsmCheck->getKalmanPSCS();
    }
    if (bsmCheck->getKalmanPSCSP() < minFactor) {
        minFactor = bsmCheck->getKalmanPSCSP();
    }
    if (bsmCheck->getKalmanPSCSS() < minFactor) {
        minFactor = bsmCheck->getKalmanPSCSS();
    }
    if (bsmCheck->getKalmanPCC() < minFactor) {
        minFactor = bsmCheck->getKalmanPCC();
    }
    if (bsmCheck->getKalmanPACS() < minFactor) {
        minFactor = bsmCheck->getKalmanPACS();
    }
    if (bsmCheck->getKalmanSCC() < minFactor) {
        minFactor = bsmCheck->getKalmanSCC();
    }
    InterTest inter = bsmCheck->getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {
        double IT = inter.getInterValue(var);
        if (IT < minFactor) {
            minFactor = IT;
        }
    }

}

void MDApplication::incrementDetailedFlags(BasicSafetyMessage * bsm,BsmCheck * bsmCheck, double Threshold) {
    if (bsmCheck->getProximityPlausibility() <= Threshold) {
        prntApp->incFlags(mdChecksTypes::ProximityPlausibility,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::ProximityPlausibility,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }

    if (bsmCheck->getRangePlausibility() <= Threshold) {
        prntApp->incFlags(mdChecksTypes::RangePlausibility,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::RangePlausibility,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getPositionConsistancy() <= Threshold) {
        prntApp->incFlags(mdChecksTypes::PositionConsistancy,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::PositionConsistancy,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }

    if (bsmCheck->getPositionSpeedConsistancy() <= Threshold) {
        prntApp->incFlags(mdChecksTypes::PositionSpeedConsistancy,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::PositionSpeedConsistancy,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getPositionSpeedMaxConsistancy() <= Threshold) {
        prntApp->incFlags(mdChecksTypes::PositionSpeedMaxConsistancy,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::PositionSpeedMaxConsistancy,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getSpeedConsistancy() <= Threshold) {
        prntApp->incFlags(mdChecksTypes::SpeedConsistancy,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::SpeedConsistancy,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getSpeedPlausibility() <= Threshold) {
        prntApp->incFlags(mdChecksTypes::SpeedPlausibility,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::SpeedPlausibility,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getPositionPlausibility() <= Threshold) {
        prntApp->incFlags(mdChecksTypes::PositionPlausibility,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::PositionPlausibility,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }

    if (bsmCheck->getBeaconFrequency() <= Threshold) {
        prntApp->incFlags(mdChecksTypes::BeaconFrequency,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::BeaconFrequency,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getSuddenAppearence() <= Threshold) {
        prntApp->incFlags(mdChecksTypes::SuddenAppearence,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::SuddenAppearence,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getPositionHeadingConsistancy() <= Threshold) {
        prntApp->incFlags(mdChecksTypes::PositionHeadingConsistancy,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::PositionHeadingConsistancy,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getKalmanPSCP() < Threshold) {
        prntApp->incFlags(mdChecksTypes::kalmanPSCP,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::kalmanPSCP,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getKalmanPSCS() < Threshold) {
        prntApp->incFlags(mdChecksTypes::kalmanPSCS,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::kalmanPSCS,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getKalmanPSCSP() < Threshold) {
        prntApp->incFlags(mdChecksTypes::kalmanPSCSP,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::kalmanPSCSP,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getKalmanPSCSS() < Threshold) {
        prntApp->incFlags(mdChecksTypes::kalmanPSCSS,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::kalmanPSCSS,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getKalmanPCC() < Threshold) {
        prntApp->incFlags(mdChecksTypes::kalmanPCC,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::kalmanPCC,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getKalmanPACS() < Threshold) {
        prntApp->incFlags(mdChecksTypes::kalmanPACS,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::kalmanPACS,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }
    if (bsmCheck->getKalmanSCC() < Threshold) {
        prntApp->incFlags(mdChecksTypes::kalmanSCC,
                mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incFlags(mdChecksTypes::kalmanSCC,
                mbTypes::intMbs[bsm->getSenderMbType()]);
    }

    bool minInterFound = false;
    InterTest inter = bsmCheck->getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {
        double IT = inter.getInterValue(var);
        if (IT < minFactor) {
            minFactor = IT;
        }
        if (IT <= Threshold) {
            if (!minInterFound) {
                prntApp->incFlags(mdChecksTypes::Intersection,
                        mbTypes::intMbs[bsm->getSenderMbType()]);
                prntAppInst->incFlags(mdChecksTypes::Intersection,
                        mbTypes::intMbs[bsm->getSenderMbType()]);
                minInterFound = true;
            }
        }

    }
}

void MDApplication::saveLine(std::string path, std::string serial,
        double density, double deltaT, bool printOut) {

    char fileNameApp[64];
    char fileNameAppInst[64];

    char fileNameAppFlags[64];
    char fileNameAppFlagsInst[64];

    if (version == 1) {
        strcpy(fileNameApp, AppV1Name);
        strcat(fileNameApp, ".dat");
        strcpy(fileNameAppInst, AppV1Name);
        strcat(fileNameAppInst, "Inst.dat");

        strcpy(fileNameAppFlags, AppV1Name);
        strcat(fileNameAppFlags, "Flags.dat");
        strcpy(fileNameAppFlagsInst, AppV1Name);
        strcat(fileNameAppFlagsInst, "FlagsInst.dat");

    } else {
        strcpy(fileNameApp, AppV2Name);
        strcat(fileNameApp, ".dat");
        strcpy(fileNameAppInst, AppV2Name);
        strcat(fileNameAppInst, "Inst.dat");

        strcpy(fileNameAppFlags, AppV2Name);
        strcat(fileNameAppFlags, "Flags.dat");
        strcpy(fileNameAppFlagsInst, AppV2Name);
        strcat(fileNameAppFlagsInst, "FlagsInst.dat");
    }

    char outChar[1024];
    char directoryPathGen[1024] = "";
    char filePathGen[1024] = "";
    const char * pathChar = path.c_str();
    const char * serialChar = serial.c_str();
    strcat(directoryPathGen, pathChar);
    strcat(directoryPathGen, serialChar);

    struct stat info;

    if (stat(directoryPathGen, &info) != 0) {
        mkdir(directoryPathGen, 0777);
    } else if (info.st_mode & S_IFDIR) {
    } else {
        mkdir(directoryPathGen, 0777);
    }

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameApp);

    prntApp->getPrintable(outChar, density, deltaT, printOut);
    prntApp->writeFile(filePathGen, outChar);

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameAppInst);

    prntAppInst->getPrintable(outChar, density, deltaT, printOut);
    prntAppInst->writeFile(filePathGen, outChar);

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameAppFlags);

    prntApp->getFlagsPrintable(outChar, density, deltaT, printOut);
    prntApp->writeFile(filePathGen, outChar);

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameAppFlagsInst);

    prntAppInst->getFlagsPrintable(outChar, density, deltaT, printOut);
    prntAppInst->writeFile(filePathGen, outChar);

}

void MDApplication::resetInstFlags() {
    prntAppInst->resetAll();
}

void MDApplication::resetAllFlags() {
    prntApp->resetAll();
    prntAppInst->resetAll();
}

