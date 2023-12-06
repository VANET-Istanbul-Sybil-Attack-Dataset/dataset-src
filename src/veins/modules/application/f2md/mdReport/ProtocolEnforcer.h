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

#ifndef __VEINS_ProtocolEnforcer_H_
#define __VEINS_ProtocolEnforcer_H_

#include <omnetpp.h>
#include <time.h>
#include "../mdBase/BsmCheck.h"
#include "../F2MDParameters.h"
using namespace omnetpp;

class ProtocolEnforcer {



protected:
    unsigned long reportedPseudos[MAX_REP_PSEUDOS];
    double reportTime[MAX_REP_PSEUDOS];
    double lastMisbehavingTime[MAX_REP_PSEUDOS];
    int reportedPseudosNum = 0;
    void removeMisbehavingPseudo(int index);
    F2MDParameters * params;

public:
    ProtocolEnforcer();

    void setParams(F2MDParameters * params);

    bool isReported(unsigned long pseudo);

    bool addMisbehavingPseudo(unsigned long pseudo, double curTime);
    int getReportPseudoes(double curTime, unsigned long * pseudosList);
    int getAllReportPseudoes(double curTime, unsigned long * pseudosList);

    void removeReportedPseudo(unsigned long pseudo);

};

#endif
