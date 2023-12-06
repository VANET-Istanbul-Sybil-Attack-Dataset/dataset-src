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

#include "LocalAttackServer.h"
using namespace std;
using namespace boost;

LocalAttackServer::LocalAttackServer() {
}

LocalAttackServer::LocalAttackServer(int port, std::string host) {
    this->port = port;
    this->host = host;
    httpr = HTTPRequest(port, host);
}

void LocalAttackServer::pingAttackServer() {

    std::string s = "helloworld";
    std::string response = httpr.Request(s);
    std::string delimiter = ";";
    bool attack = false;

    bool saveInstant;
    attackTypes::Attacks saveAttack;

    size_t pos = 0;
    std::string token;
    while ((pos = response.find(delimiter)) != std::string::npos) {
        token = response.substr(0, pos);
        if (!attack) {
            if (token.compare("instant") == 0) {
                saveInstant = true;
            } else {
                saveInstant = false;
            }
            attack = true;
        } else {
            for (int i = 0; i < sizeof(attackTypes::intAttacks); i++) {
                if (token.compare(attackTypes::AttackNames[i]) == 0) {
                    saveAttack = attackTypes::intAttacks[i];
                    break;
                }
            }
            addAttack(saveAttack,saveInstant);
            attack = false;
        }

        response.erase(0, pos + delimiter.length());
    }

}

void LocalAttackServer::addAttack(attackTypes::Attacks attackType,
        bool attackPower) {
    if (attackSize < ATTACK_LIST_SIZE) {
        attackTypes[attackSize] = attackType;
        attackPowers[attackSize] = attackPower;
        attackSize++;

    }
}

void LocalAttackServer::removeAttack() {
    for (int i = 0; i < attackSize - 1; i++) {
        attackTypes[i] = attackTypes[i + 1];
        attackPowers[i] = attackPowers[i + 1];
    }
    attackSize--;
}

bool LocalAttackServer::isInstantAttack() {
    if (attackSize > 0) {
        return attackPowers[0];
    } else {
        return false;
    }
}

bool LocalAttackServer::isQueuedAttack() {
    if (attackSize > 0) {
        return true;
    } else {
        return false;
    }
}

attackTypes::Attacks LocalAttackServer::getNextAttack() {
    if (attackSize > 0) {
        attackTypes::Attacks save = attackTypes[0];
        removeAttack();
        return attackTypes[0];
    } else {
        std::cout << "LocalAttackServer::getNextAttack Logic ERROR \n";
        exit(0);
    }
}
