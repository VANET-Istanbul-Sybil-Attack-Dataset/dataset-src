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

#include <veins/modules/application/f2md/mdBase/NodeTable.h>

NodeTable::NodeTable() {
    nodesNum = 0;
}

int NodeTable::getNodesNum() {
    return nodesNum;
}

NodeHistory* NodeTable::getNodeHistoryList() {
    return nodeHistoryList;
}

unsigned long NodeTable::getNodePseudo(int index) {
    return nodePseudos[index];
}

void NodeTable::put(unsigned long pseudo, NodeHistory nodeHistory,
        MDMHistory mdmHistory, ProtocolEnforcer * reportProtocolEnforcerV1,
        ProtocolEnforcer * reportProtocolEnforcerV2) {

    bool included = false;
    int nodeKey;

    for (int var = 0; var < nodesNum; ++var) {
        if (pseudo == nodePseudos[var]) {
            included = true;
            nodeKey = var;
            break;
        }
    }

    if (included) {
        nodeHistoryList[nodeKey] = nodeHistory;
        mdmHistoryList[nodeKey] = mdmHistory;
    } else {
        if (nodesNum < MAX_NODES_LENGTH) {
            nodePseudos[nodesNum] = pseudo;
            nodeHistoryList[nodesNum] = nodeHistory;
            mdmHistoryList[nodesNum] = mdmHistory;
            nodesNum++;
        } else {
            nodeKey = getOldestNode(reportProtocolEnforcerV1,
                    reportProtocolEnforcerV2);
            nodePseudos[nodeKey] = pseudo;
            nodeHistoryList[nodeKey] = nodeHistory;
            mdmHistoryList[nodeKey] = mdmHistory;
        }
    }
}

int NodeTable::getOldestNode(ProtocolEnforcer * reportProtocolEnforcerV1,
        ProtocolEnforcer * reportProtocolEnforcerV2) {
    int oldestNodeIndex = 0;
    double oldestNodeTime =
            nodeHistoryList[0].getLatestBSMAddr()->getSendingTime().dbl();
    double currentNodeTime = 0;

    double currentPseudo =
            nodeHistoryList[0].getLatestBSMAddr()->getSenderPseudonym();

    for (int var = 0; var < nodesNum; ++var) {
        currentNodeTime =
                nodeHistoryList[var].getLatestBSMAddr()->getSendingTime().dbl();
        currentPseudo =
                nodeHistoryList[var].getLatestBSMAddr()->getSenderPseudonym();

        if ((currentNodeTime < oldestNodeTime)
                && !reportProtocolEnforcerV1->isReported(currentPseudo)
                && !reportProtocolEnforcerV2->isReported(currentPseudo)) {
            oldestNodeTime = currentNodeTime;
            oldestNodeIndex = var;
        }

    }
    return oldestNodeIndex;
}

NodeHistory* NodeTable::getNodeHistoryAddr(unsigned long nodePseudo) {
    for (int var = 0; var < nodesNum; ++var) {
        if (nodePseudo == nodePseudos[var]) {
            return &nodeHistoryList[var];
        }
    }
    std::cout<<"nodePseudonym:"<<nodePseudo<<"\n";
    std::cout<<"ERROR: getNodeHistoryAddr no node found \n";
    exit(0);

    return &nullNode;
}

MDMHistory* NodeTable::getMDMHistoryAddr(unsigned long nodePseudonym) {
    for (int var = 0; var < nodesNum; ++var) {
        if (nodePseudonym == nodePseudos[var]) {
            return &mdmHistoryList[var];
        }
    }
    std::cout<<"nodePseudonym:"<<nodePseudonym<<"\n";
    std::cout<<"ERROR: getMDMHistoryAddr no node found \n";
    exit(0);

    return &nullMDMNode;
}

bool NodeTable::includes(unsigned long nodePseudonym) {
    for (int var = 0; var < nodesNum; ++var) {
        if (nodePseudonym == nodePseudos[var]) {
            return true;
        }
    }
    return false;
}

double NodeTable::getDeltaTime(unsigned long nodePseudo1,
        unsigned long nodePseudo2) {
    bool includes1 = includes(nodePseudo1);
    bool includes2 = includes(nodePseudo2);
    if(includes1&&includes2){
        return fabs(
                getNodeHistoryAddr(nodePseudo1)->getArrivalTime(0)
                        - getNodeHistoryAddr(nodePseudo2)->getArrivalTime(0));
    }else if(includes1){
        return fabs(getNodeHistoryAddr(nodePseudo1)->getArrivalTime(0));
    }else if(includes2){
        return fabs(getNodeHistoryAddr(nodePseudo2)->getArrivalTime(0));
    }else{
        return 0;
    }
}

BasicSafetyMessage* NodeTable::getRandomBSM() {
    GeneralLib genLib = GeneralLib();
    int randNode = genLib.RandomInt(0, nodesNum - 1);
    int randBSM = genLib.RandomInt(0,
            nodeHistoryList[randNode].getBSMNum() - 1);
    return nodeHistoryList[randNode].getBSMAddr(randBSM);
}

BasicSafetyMessage* NodeTable::getNextAttackedBsm(veins::Coord myPosition,
        unsigned long bsmNode, double bsmTime) {
    if (bsmNode == 0 || (simTime().dbl() - bsmTime) > 1.1) {
        double minDistance = 10000000;
        int index = -1;
        MDMLib mdmLib = MDMLib();
        for (int var = 0; var < nodesNum; ++var) {
            double distance = mdmLib.calculateDistancePtr(&myPosition,
                    &nodeHistoryList[var].getLatestBSMAddr()->getSenderPos());
            if (minDistance > distance) {
                minDistance = distance;
                index = var;
            }
        }
        return nodeHistoryList[index].getLatestBSMAddr();
    } else {
        return getNodeHistoryAddr(bsmNode)->getLatestBSMAddr();
    }
}

BasicSafetyMessage* NodeTable::getLastBSM(unsigned long bsmNode) {
    for (int var = 0; var < nodesNum; ++var) {
        if (nodePseudos[var] == bsmNode)
            return nodeHistoryList[var].getLatestBSMAddr();
    }
    return NULL;
}

vector<tuple<double, unsigned long>> NodeTable::getNearVehcilesSorted(const veins::Coord& myPosition) {
    int index = -1;
    MDMLib mdmLib = MDMLib();
    vector<tuple<double, unsigned long>> l;
    for (int var = 0; var < nodesNum; ++var) {
        double distance = mdmLib.calculateDistancePtr(&myPosition, &nodeHistoryList[var].getLatestBSMAddr()->getSenderPos());
        l.push_back(tuple<double, unsigned long>(distance, nodeHistoryList[var].getLatestBSMAddr()->getSenderPseudonym()));
    }
    sort(l.begin(), l.end());
    return l;
}

