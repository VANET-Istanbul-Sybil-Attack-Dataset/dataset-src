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

#ifndef __VEINS_CooperativeApp_H_
#define __VEINS_CooperativeApp_H_

#include <tuple>
#include <omnetpp.h>
#include <veins/modules/application/f2md/mdApplications/MDApplication.h>
#include "../mdEnumTypes/MdChecksTypes.h"
#include "../mdEnumTypes/MbTypes.h"

#include "../F2MDParameters.h"

using namespace veins;
using namespace omnetpp;

class CooperativeApp: public MDApplication {
public:

    double Threshold = 0.5;

    unsigned long PseudonymsToTMO[MAX_DETECTED_NODES_COOP];
    double TimeOut[MAX_DETECTED_NODES_COOP];
    double UpdatedTMO[MAX_DETECTED_NODES_COOP];
    int TimeOutNum = 0;

    CooperativeApp(int version, double Threshold);

    bool CheckNodeForReport(unsigned long myPseudonym, BasicSafetyMessage * bsm,
            BsmCheck * bsmCheck, NodeTable * detectedNodes);

    int addPseudoTMO(unsigned long pseudo);
    void removeOldestPseudoTMO();
    void removePseudoTMO(int index);
    int getIndexTMO(unsigned long pseudo);

    double getMinFactor();
};

#endif
