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

#ifndef __VEINS_LocalAttackServer_H_
#define __VEINS_LocalAttackServer_H_

#include <tuple>
#include <omnetpp.h>
#include <stdio.h>
#include <stdlib.h>     /* atof */
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <veins/modules/application/f2md/mdSupport/HTTPRequest.h>
#include <iostream>
#include <string>
#include <vector>

#include "../mdEnumTypes/AttackTypes.h"

using namespace omnetpp;

#define ATTACK_LIST_SIZE 1024

class LocalAttackServer {
private:

    int port = 8888;
    std::string host = "localhost";

    HTTPRequest httpr = HTTPRequest(8888, "localhost");

    attackTypes::Attacks attackTypes[ATTACK_LIST_SIZE];
    bool attackPowers[ATTACK_LIST_SIZE];
    int attackSize = 0;

    void addAttack(attackTypes::Attacks attackType, bool attackPower);
    void removeAttack();

public:
    LocalAttackServer();
    LocalAttackServer(int port, std::string host);
    void pingAttackServer();

    bool isQueuedAttack();
    bool isInstantAttack();
    attackTypes::Attacks getNextAttack();

};

#endif
