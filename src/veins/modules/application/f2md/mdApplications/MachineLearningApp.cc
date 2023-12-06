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

#include <veins/modules/application/f2md/mdApplications/MachineLearningApp.h>

using namespace std;
using namespace boost;

MachineLearningApp::MachineLearningApp(int version, int port, std::string host) :
        MDApplication(version) {
    this->port = port;
    this->host = host;
    httpr = HTTPRequest(port, host);
    bsmPrint = BsmPrintable();
}

void MachineLearningApp::setMyId(LAddress::L2Type myId) {
    this->myId = myId;
}

bool MachineLearningApp::CheckNodeForReport(unsigned long myPseudonym,
        BasicSafetyMessage * bsm, BsmCheck * bsmCheck,
        NodeTable * detectedNodes) {

    prntApp->incAll(mbTypes::intMbs[bsm->getSenderMbType()]);
    prntAppInst->incAll(mbTypes::intMbs[bsm->getSenderMbType()]);
    calculateMinFactor(bsmCheck);
    incrementDetailedFlags(bsm,bsmCheck,0.5);

    bsmPrint.setReceiverId(myId);
    bsmPrint.setReceiverPseudo(myPseudonym);
    bsmPrint.setBsm(*bsm);
    bsmPrint.setBsmCheck(*bsmCheck);

    std::string s = bsmPrint.getBsmPrintableJson();

    s.erase(std::remove(s.begin(), s.end(), '\t'), s.end());
    s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());

    std::string response = httpr.Request(s);

    //std::cout << "response:" << response << "\n";

    if (!response.compare("True")) {
        prntApp->incCumulFlags(mbTypes::intMbs[bsm->getSenderMbType()]);
        prntAppInst->incCumulFlags(mbTypes::intMbs[bsm->getSenderMbType()]);
        return true;
    }

    return false;
}

double MachineLearningApp::getMinFactor() {
    return minFactor;
}
