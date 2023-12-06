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

#include <veins/modules/application/f2md/mdReport/ProtocolReport.h>

ProtocolReport::ProtocolReport(MDReport baseReport) {
    setBaseReport(baseReport);
    bsmListNum = 0;
}

void ProtocolReport::setReportedCheck(BsmCheck reportedCheck) {
    this->reportedCheck = reportedCheck;
}

void ProtocolReport::addBsmToList(BasicSafetyMessage bsm, BsmCheck check) {
    if(bsmListNum<MAX_EVI_BSM_PROTO){
        if (bsm.getSenderPseudonym() == 0) {
            std: cout << "addBsmToList Error Found \n";
            exit(0);
        }
        this->bsmList[bsmListNum] = bsm;
        this->checksList[bsmListNum] = check;
        bsmListNum++;
    }
}

void ProtocolReport::addEvidence(BasicSafetyMessage myBsm,
        BsmCheck reportedCheck, BasicSafetyMessage receivedBsm,
        NodeTable * detectedNodes, double curTime, double deltaTime,
        int version) {

    bool myBsmAdded = false;

    int histAdd = 0;
    double addedBsmTime = curTime;

    setReportedCheck(reportedCheck);
    addBsmToList(receivedBsm, reportedCheck);
    addedBsmTime = receivedBsm.getArrivalTime().dbl();

    if (detectedNodes->includes(reportedPseudo)) {
        NodeHistory * NodeHistoryAddr = detectedNodes->getNodeHistoryAddr(
                reportedPseudo);
        MDMHistory * MDMHistoryAddr = detectedNodes->getMDMHistoryAddr(
                reportedPseudo);

        if (NodeHistoryAddr->getBSMNum() > 0) {
            double savedArrivalTime =
                    NodeHistoryAddr->getLatestBSMAddr()->getArrivalTime().dbl();
            if (savedArrivalTime == addedBsmTime) {
                histAdd = 1;
            }
        }

        while (((curTime - addedBsmTime) < deltaTime)
                && histAdd < NodeHistoryAddr->getBSMNum()) {
            addBsmToList(*NodeHistoryAddr->getBSMAddr(histAdd),
                    MDMHistoryAddr->getBsmCheck(histAdd, version));
            addedBsmTime =
                    NodeHistoryAddr->getBSMAddr(histAdd)->getArrivalTime().dbl();
            histAdd++;
        }
    }


    if (reportedCheck.getProximityPlausibility() < 1 || reportedCheck.getRangePlausibility() < 1 || reportedCheck.getProximityPlausibility() < 1) {
        addBsmToList(myBsm, BsmCheck());
        myBsmAdded = true;
    }

    for (int var = 0; var < reportedCheck.getIntersection().getInterNum();
            var++) {

        if (reportedCheck.getIntersection().getInterValue(var) < 1) {
            if (senderPseudonym
                    == reportedCheck.getIntersection().getInterId(var)) {

                if (!myBsmAdded) {
                    addBsmToList(myBsm, BsmCheck());
                }
            } else {

                unsigned long pseudiInt =
                        reportedCheck.getIntersection().getInterId(var);
                if (detectedNodes->includes(pseudiInt)) {
                    NodeHistory * NodeHistoryAddrInt =
                            detectedNodes->getNodeHistoryAddr(pseudiInt);
                    MDMHistory * MDMHistoryAddrInt =
                            detectedNodes->getMDMHistoryAddr(pseudiInt);

                    if (NodeHistoryAddrInt->getBSMNum() > 0
                            && MDMHistoryAddrInt->getMDMNum(version) > 0) {
                        BasicSafetyMessage * BSMInt =
                                NodeHistoryAddrInt->getLatestBSMAddr();
                        BsmCheck * BsmCheckInt =
                                MDMHistoryAddrInt->getLatestBsmCheckAddr(
                                        version);

                        addBsmToList(*BSMInt, *BsmCheckInt);
                    }

                }
            }
        }
    }

    if (bsmListNum == 0) {
        std::cout << "ProtocolReport::addEvidence bsmListNum==0" << "\n";
        ReportPrintable rp;
        std::cout << rp.getBsmJson(receivedBsm) << "\n";
        exit(0);
    }

}

std::string ProtocolReport::getReportPrintableJson() {

    ReportPrintable rp;

    JsonWriter jw;
    jw.writeHeader();
    jw.openJsonElement("Report", false);

    jw.addTagToElement("Report", getBaseReportJson("ProtocolReport"));

    jw.addTagToElement("Report", rp.getCheckJson(reportedCheck));

    jw.openJsonElementList("BSMs");

    for (int var = 0; var < bsmListNum; ++var) {
        if (var < bsmListNum - 1) {
            jw.addTagToElement("BSMs", rp.getBsmJson(bsmList[var]));
        } else {
            jw.addFinalTagToElement("BSMs", rp.getBsmJson(bsmList[var]));
        }
    }
    jw.addTagToElement("Report", jw.getJsonElementList("BSMs"));

    jw.openJsonElementList("BsmChecks");
    for (int var = 0; var < bsmListNum; ++var) {
        if (var < bsmListNum - 1) {
            jw.addTagToElement("BsmChecks",
                    rp.getCheckJsonList(checksList[var]));
        } else {
            jw.addFinalTagToElement("BsmChecks",
                    rp.getCheckJsonList(checksList[var]));
        }
    }

    jw.addFinalTagToElement("Report", jw.getJsonElementList("BsmChecks"));
    jw.addElement(jw.getJsonElement("Report"));
    jw.writeFooter();

    return jw.getOutString();
}
