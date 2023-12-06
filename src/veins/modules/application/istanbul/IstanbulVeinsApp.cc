
#include <stdexcept>

#include <veins/modules/phy/DeciderResult80211.h>
#include <veins/base/phyLayer/PhyToMacControlInfo.h>

#include <veins/modules/application/istanbul/IstanbulVeinsApp.h>
#include "IstanbulDsAttack.h"
#include "IstanbulDsParams.h"

Define_Module(IstanbulVeinsApp);

static const char* disabledF2MDParams[] = {
    "randomConf",
    "variableConf",

    "EnableV1",
    "EnableV2",
    "SaveStatsV1",
    "SaveStatsV2",

    "writeSelfMsg",
    "writeListSelfMsg",

    "writeBsmsV1",
    "writeBsmsV2",
    "writeListBsmsV1",
    "writeListBsmsV2",

    "writeReportsV1",
    "writeReportsV2",
    "writeListReportsV1",
    "writeListReportsV2",

    "sendReportsV1",
    "sendReportsV2",

    "enableVarThreV1",
    "enableVarThreV2",

    "SelfSybil",

    "UseAttacksServer",
    "writeReportsV1",

};


void IstanbulVeinsApp::initialize(int stage)
{
    for (auto p : disabledF2MDParams) {
        bool b = par(p);
        if (b) {
            const string& s = string("disabled F2MD parameter: ") + p;
            EV_ERROR << s << endl;
            getSimulation()->getActiveEnvir()->alert(s.c_str());
            endSimulation();
        }
    }

    JosephVeinsApp::initialize(stage);

    istanbulDsParams.probGridSybil = par("probGridSybil");
    istanbulDsParams.probRandomSybil = par("probRandomSybil");
    istanbulDsParams.probReplaySybil = par("probReplaySybil");
    istanbulDsParams.probSimilarRandomSybil = par("probSimilarRandomSybil");
    istanbulDsParams.probPowerControl = par("probPowerControl");
    istanbulDsParams.minSybilNodeCount = par("minSybilNodeCount");
    istanbulDsParams.maxSybilNodeCount = par("maxSybilNodeCount");

    if (stage == 1) {
        logger.init(params.savePath, params.serialNumber, myId,
            getParentModule()->getId(), myAttackType, curDate,
            params.VeReMiSliceTime, VeReMiSliceStartTime, simTime().dbl());
    }
}

void IstanbulVeinsApp::localAttackerInit() {

    if (params.LOCAL_ATTACK_TYPE == attackTypes::Attacks::_IstanbulDsRandomSelect) {
        attackTypes::Attacks attacks[] = { attackTypes::Attacks::_ReplaySybil,
                attackTypes::Attacks::_RandomSybil,
                attackTypes::Attacks::_SimilarRandomSybil,
                attackTypes::Attacks::_GridSybil };
        double probs[] = { istanbulDsParams.probReplaySybil,
                istanbulDsParams.probRandomSybil,
                istanbulDsParams.probSimilarRandomSybil,
                istanbulDsParams.probGridSybil };
        double total = 0;
        for (int i = 0 ; i < 4 ; i++)
            total += probs[i];
        double val = genLib.RandomDouble(0, total);
        cout << "val " << val << endl;
        double s = 0;
        int i = 0;
        for (i = 0 ; i < 4 ; i++) {
            s += probs[i];
            if (val < s + 0.000001)
                break;
        }
        if (i == 4) {
            getSimulation()->getActiveEnvir()->alert("IstanbulVeinsApp::localAttackerInit wrong attack config");
            exit(0);
        }
        cout << "selected attack " << i << " " << attacks[i] << endl;
        myAttackType =  attacks[i];
    } else {
        myAttackType = params.LOCAL_ATTACK_TYPE;
    }

    std::cout
        << "=+#=+#=+#=+#=+#=+#=+#=+#+#=+#=+#=+#=+#=+#=+#=+#=+#=+#=+#=+#=+# "
        << "\n";
    std::cout
        << "=+#=+#=+#=+#=+#=+#=+#=+# NEW ATTACKER 2 #=+#=+#=+#=+#=+#=+#=+# "
        << myPseudonym << " : "
        << attackTypes::AttackNames[myAttackType] << "\n";
    std::cout
        << "=+#=+#=+#=+#=+#=+#=+#=+#+#=+#=+#=+#=+#=+#=+#=+#=+#=+#=+#=+#=+# "
        << "\n";

    mdAttack->setBeaconInterval(&beaconInterval);
    mdAttack->setCurHeading(&curHeading);
    mdAttack->setCurHeadingConfidence(&curHeadingConfidence);
    mdAttack->setCurPosition(&curPosition);
    mdAttack->setCurPositionConfidence(&curPositionConfidence);
    mdAttack->setCurSpeed(&curSpeed);
    mdAttack->setCurSpeedConfidence(&curSpeedConfidence);
    mdAttack->setCurAccel(&curAccel);
    mdAttack->setCurAccelConfidence(&curAccelConfidence);
    mdAttack->setDetectedNodes(&detectedNodes);
    mdAttack->setMyBsm(myBsm);
    mdAttack->setMyBsmNum(&myBsmNum);
    mdAttack->setMyLength(&myLength);
    mdAttack->setMyPseudonym(&myPseudonym);
    mdAttack->setMyWidth(&myWidth);
    mdAttack->setPcPolicy(&pcPolicy);

    mdAttack->init(myAttackType, MaxRandomPosX, MaxRandomPosY, &params);
}

void IstanbulVeinsApp::onBSM(BasicSafetyMessage* bsm) {
    std::pair<double, double> curLonLat = traci->getLonLat(curPosition);

    double rssi = check_and_cast<DeciderResult80211*>(check_and_cast<PhyToMacControlInfo*>(bsm->getControlInfo())->getDeciderResult())->getRecvPower_dBm();
    logger.saveBeacon(myId, bsm, rssi, curLonLat, curPosition, curPositionConfidence, curSpeed, curSpeedConfidence,
            curHeading, curHeadingConfidence, curAccel, curAccelConfidence, myWidth, myLength);

    JosephVeinsApp::onBSM(bsm);
}

void IstanbulVeinsApp::onWSM(BaseFrame1609_4* wsm) {
    JosephVeinsApp::onWSM(wsm);
}

void IstanbulVeinsApp::onWSA(DemoServiceAdvertisment* wsa) {
    JosephVeinsApp::onWSA(wsa);
}

void IstanbulVeinsApp::populateWSM(BaseFrame1609_4* wsm, LAddress::L2Type rcvId, int serial) {
    F2MDBaseApplLayer::populateWSM(wsm, rcvId, serial);

    if (BasicSafetyMessage *bsm = dynamic_cast<BasicSafetyMessage*>(wsm)) {
        bsm->setSenderPseudonym(myPseudonym);
        bsm->setSenderMbType(myMdType);
        bsm->setSenderAttackType(myAttackType);

        // Genuine BSM
        bsm->setSenderPos(curPosition);
        bsm->setSenderPosConfidence(curPositionConfidence);
        std::pair<double, double> curLonLat = traci->getLonLat(curPosition);
        bsm->setSenderGpsCoordinates(Coord(curLonLat.first, curLonLat.second));
        bsm->setSenderSpeed(curSpeed);
        bsm->setSenderSpeedConfidence(curSpeedConfidence);
        bsm->setSenderHeading(curHeading);
        bsm->setSenderHeadingConfidence(curHeadingConfidence);
        bsm->setSenderAccel(curAccel);
        bsm->setSenderAccelConfidence(curAccelConfidence);
        bsm->setSenderWidth(myWidth);
        bsm->setSenderLength(myLength);
        bsm->setSenderRealId(myId);

        // cout << "speed " << curSpeed.x << " " << curSpeed.y << endl;
        // cout << "heading" << curHeading.x << " " << curHeading.y << endl;

        if (myMdType == mbTypes::Genuine || attackBsm.getSenderPseudonym() == 0) {
            addMyBsm(*bsm);
            // cout << "my bsm " << bsm->getSenderSpeed().x << " " << bsm->getSenderPos().x << endl;
            logger.saveGroundTruth((int)myId, bsm, bsm->getTreeId(), curLonLat, curPosition, curPositionConfidence, curSpeed, curSpeedConfidence,
                    curHeading, curHeadingConfidence, curAccel, curAccelConfidence, myWidth, myLength);
        }

        if (myMdType == mbTypes::LocalAttacker && attackBsm.getSenderPseudonym() != 0) {
            bsm->setSenderPseudonym(attackBsm.getSenderPseudonym());
            bsm->setSenderPos(attackBsm.getSenderPos());
            bsm->setSenderPosConfidence(attackBsm.getSenderPosConfidence());
            std::pair<double, double> attackLonLat = traci->getLonLat(attackBsm.getSenderPos());
            bsm->setSenderGpsCoordinates(Coord(attackLonLat.first, attackLonLat.second));
            bsm->setSenderSpeed(attackBsm.getSenderSpeed());
            bsm->setSenderSpeedConfidence(attackBsm.getSenderSpeedConfidence());
            bsm->setSenderHeading(attackBsm.getSenderHeading());
            bsm->setSenderHeadingConfidence(attackBsm.getSenderHeadingConfidence());
            bsm->setSenderAccel(attackBsm.getSenderAccel());
            bsm->setSenderAccelConfidence(attackBsm.getSenderAccelConfidence());
            bsm->setSenderWidth(attackBsm.getSenderWidth());
            bsm->setSenderLength(attackBsm.getSenderLength());
            bsm->setPowerControlTxPower(attackBsm.getPowerControlTxPower());
            bsm->setPowerControlType(attackBsm.getPowerControlType());

            logger.saveGroundTruth((int)myId, bsm, bsm->getTreeId(), curLonLat, curPosition, curPositionConfidence, curSpeed, curSpeedConfidence,
                    curHeading, curHeadingConfidence, curAccel, curAccelConfidence, myWidth, myLength);

            // cout << bsm->getTreeId() << " " << attackBsm.getTreeId() << endl;
//            const Coord& p = bsm->getSenderPos();
//            std::list<Coord> points = { p, p + Coord(1, 0, 0), p + Coord(1, 1, 0), p + Coord(0, 1, 0) };
//            const string id = std::to_string(bsm->getTreeId());
//            traci->addPolygon(id, "test", TraCIColor(255, 0, 0, 255), true, 5, points);
        }
    }
}

void IstanbulVeinsApp::handlePositionUpdate(cObject* obj) {
    JosephVeinsApp::handlePositionUpdate(obj);

//    datasetLog.serializeRawData(&curPosition, &curPositionConfidence, &curSpeed,
//                &curSpeedConfidence, &curHeading, &curHeadingConfidence,
//                &curAccel, &curAccelConfidence);

}

void IstanbulVeinsApp::handleSelfMsg(cMessage* msg) {

    // check mac/ieee80211p/Mac1609_4.cc and search bsmWithPowerControl for power control

    // update attacks
    // TODO:
    /*string s;
    for (int i = 0 ; i < myBsmNum ; i++) {
        s += std::to_string(myBsm->getPowerControl()) + " ";
    }
    cout << "myBsm " << s << endl;*/

    // F2MD
    treatAttackFlags();

    // F2MDBaseApplLayer handleSelfMsg
    switch (msg->getKind()) {
    case SEND_BEACON_EVT: {
        BasicSafetyMessage *bsm = new BasicSafetyMessage();
        populateWSM(bsm);
        sendDown(bsm);
        scheduleAt(simTime() + beaconInterval, sendBeaconEvt);
        break;
    }
    case SEND_WSA_EVT: {
        DemoServiceAdvertisment *wsa = new DemoServiceAdvertisment();
        populateWSM(wsa);
        sendDown(wsa);
        scheduleAt(simTime() + wsaInterval, sendWSAEvt);
        break;
    }
    default: {
        if (msg)
            EV_WARN << "APP: Error: Got Self Message of unknown kind! Name: "
                           << msg->getName() << endl;
        break;
    }
    }


    // F2MD
    if (params.sendReportsV1 || params.sendReportsV2) {
        getSimulation()->getActiveEnvir()->alert("F2MD sendReportsV1 & sendReportsV2 disabled");
        exit(1);
    }
    if (params.writeSelfMsg || params.writeListSelfMsg) {
       getSimulation()->getActiveEnvir()->alert("F2MD writeSelfMsg && writeListSelfMsg disabled");
       exit(1);
    }
    if (myReportType == reportTypes::ProtocolReport) {
        if (params.EnableV1 || params.EnableV2) {
            getSimulation()->getActiveEnvir()->alert("F2MD EnableV1 && EnableV2 disabled");
            exit(1);
        }
        handleReportProtocol(false);
    }
    if (params.EnablePC) {
        pcPolicy.checkPseudonymChange(myPcType);
    }



}

