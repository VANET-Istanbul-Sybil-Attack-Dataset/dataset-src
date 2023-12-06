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
#include <iostream>
using namespace std;

#include "VeReMiPrintable.h"
#include <veins/modules/messages/BasicSafetyMessage_m.h>

static bool folderCreated = false;
static int prevSlice = 0;
static char directoryPathGen[1024] = "";

void VeReMiPrintable::initVeReMiPrintable(std::string basePath, std::string serial, LAddress::L2Type& realId, int parentId, int attackerType, const std::string curDate,double sliceTime, double sliceStartTime, double simTime) {

    int curSlice = (simTime-sliceStartTime)/sliceTime;
    if(prevSlice != curSlice || !folderCreated){
        folderCreated = true;
        prevSlice = curSlice;
        strcat(directoryPathGen, basePath.c_str());
        strcat(directoryPathGen, serial.c_str());
        struct stat info;
        if (stat(directoryPathGen, &info) != 0) {
            mkdir(directoryPathGen, 0777);
        } else if (info.st_mode & S_IFDIR) {
        } else {
            mkdir(directoryPathGen, 0777);
        }


        directoryPathGen[0] = '\0';
        char filePathGen[1024] = "";
        strcat(filePathGen, "/VeReMi_");
        strcat(filePathGen, std::to_string((int)(sliceTime*curSlice + sliceStartTime)).c_str());
        strcat(filePathGen, "_");
        strcat(filePathGen, std::to_string((int)(sliceTime*(curSlice+1) + sliceStartTime)).c_str());
        strcat(filePathGen, "_");
        strcat(filePathGen, curDate.c_str());

        strcat(directoryPathGen, basePath.c_str());
        strcat(directoryPathGen, serial.c_str());
        strcat(directoryPathGen, filePathGen);

        if (stat(directoryPathGen, &info) != 0) {
            mkdir(directoryPathGen, 0777);
        } else if (info.st_mode & S_IFDIR) {
        } else {
            mkdir(directoryPathGen, 0777);
        }
    }

    std::ostringstream out_json;
    out_json <<directoryPathGen<<"/"<< "traceJSON"<<"-"<< realId << "-" << parentId<< "-A" << attackerType <<"-"<< simTime <<"-"<< curSlice << ".json";
    traceJSONFile = out_json.str();

    std::ostringstream out_gt_json;
    out_gt_json << directoryPathGen<<"/"<< "traceGroundTruthJSON" <<"-"<< curSlice<< ".json";
    traceGroundTruthJSONFile = out_gt_json.str();
}

const void VeReMiPrintable::traceJSON(std::string file, std::string JSONObject) const {
    std::ofstream out_stream;
    out_stream.open(file, std::ios_base::app);
    if(out_stream.is_open())
        out_stream << JSONObject << std::endl;
    else
        EV << "Warning, tracing stream is closed";
    out_stream.close();
}


void VeReMiPrintable::serializeGroundTruth(BasicSafetyMessage *bsm) {

    const veins::Coord *pos = &bsm->getSenderPos();
    const veins::Coord *posc = &bsm->getSenderPosConfidence();
    const veins::Coord *spd = &bsm->getSenderSpeed();
    const veins::Coord *spdc = &bsm->getSenderSpeedConfidence();
    const veins::Coord *hed = &bsm->getSenderHeading();
    const veins::Coord *hedc = &bsm->getSenderHeadingConfidence();
    const veins::Coord *acl = &bsm->getSenderAccel();
    const veins::Coord *aclc = &bsm->getSenderAccelConfidence();

    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartObject();

    writer.Key("type");
    writer.Uint(TYPE_TRUTH_BEACON);
    writer.Key("sendTime");
    writer.Double(simTime().dbl());
    writer.Key("sender");
    writer.Uint(bsm->getSenderRealId());
    writer.Key("senderPseudo");
    writer.Uint(bsm->getSenderPseudonym());
    writer.Key("messageID");
    writer.Uint(bsm->getTreeId());

    writer.Key("pos");
    writer.StartArray();
    writer.Double(pos->x);
    writer.Double(pos->y);
    writer.Double(pos->z);
    writer.EndArray();

    writer.Key("pos_noise");
    writer.StartArray();
    writer.Double(posc->x);
    writer.Double(posc->y);
    writer.Double(posc->z);
    writer.EndArray();

    writer.Key("spd");
    writer.StartArray();
    writer.Double(spd->x);
    writer.Double(spd->y);
    writer.Double(spd->z);
    writer.EndArray();

    writer.Key("spd_noise");
    writer.StartArray();
    writer.Double(spdc->x);
    writer.Double(spdc->y);
    writer.Double(spdc->z);
    writer.EndArray();

    writer.Key("acl");
    writer.StartArray();
    writer.Double(acl->x);
    writer.Double(acl->y);
    writer.Double(acl->z);
    writer.EndArray();

    writer.Key("acl_noise");
    writer.StartArray();
    writer.Double(aclc->x);
    writer.Double(aclc->y);
    writer.Double(aclc->z);
    writer.EndArray();

    writer.Key("hed");
    writer.StartArray();
    writer.Double(hed->x);
    writer.Double(hed->y);
    writer.Double(hed->z);
    writer.EndArray();

    writer.Key("hed_noise");
    writer.StartArray();
    writer.Double(hedc->x);
    writer.Double(hedc->y);
    writer.Double(hedc->z);
    writer.EndArray();
    //writer.Key("RSSI");
    //writer.Double(bsm->getRSSI());

    writer.EndObject();

    traceJSON(traceGroundTruthJSONFile, s.GetString());

}

void VeReMiPrintable::serializeBeacon(BasicSafetyMessage *bsm) {
    const veins::Coord *pos = &bsm->getSenderPos();
    const veins::Coord *posc = &bsm->getSenderPosConfidence();
    const veins::Coord *spd = &bsm->getSenderSpeed();
    const veins::Coord *spdc = &bsm->getSenderSpeedConfidence();
    const veins::Coord *hed = &bsm->getSenderHeading();
    const veins::Coord *hedc = &bsm->getSenderHeadingConfidence();
    const veins::Coord *acl = &bsm->getSenderAccel();
    const veins::Coord *aclc = &bsm->getSenderAccelConfidence();
    BasicSafetyMessage* bsm2 = NULL;
    if ((bsm2 = dynamic_cast<BasicSafetyMessage*>(bsm)) != NULL) {
//        cout << "VeReMiPrintable::serializeBeacon powerControl " << bsm2->getPowerControl() << endl;
    }

    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartObject();

    writer.Key("type");
    writer.Uint(TYPE_BEACON);
    writer.Key("rcvTime");
    writer.Double(bsm->getArrivalTime().dbl());
    writer.Key("sendTime");
    writer.Double(bsm->getSendingTime().dbl());
    writer.Key("sender");
    writer.Uint(bsm->getSenderRealId());
    writer.Key("senderPseudo");
    writer.Uint(bsm->getSenderPseudonym());
    writer.Key("messageID");
    writer.Uint(bsm->getTreeId());

    writer.Key("pos");
    writer.StartArray();
    writer.Double(pos->x);
    writer.Double(pos->y);
    writer.Double(pos->z);
    writer.EndArray();

    writer.Key("pos_noise");
    writer.StartArray();
    writer.Double(posc->x);
    writer.Double(posc->y);
    writer.Double(posc->z);
    writer.EndArray();

    writer.Key("spd");
    writer.StartArray();
    writer.Double(spd->x);
    writer.Double(spd->y);
    writer.Double(spd->z);
    writer.EndArray();

    writer.Key("spd_noise");
    writer.StartArray();
    writer.Double(spdc->x);
    writer.Double(spdc->y);
    writer.Double(spdc->z);
    writer.EndArray();

    writer.Key("acl");
    writer.StartArray();
    writer.Double(acl->x);
    writer.Double(acl->y);
    writer.Double(acl->z);
    writer.EndArray();

    writer.Key("acl_noise");
    writer.StartArray();
    writer.Double(aclc->x);
    writer.Double(aclc->y);
    writer.Double(aclc->z);
    writer.EndArray();

    writer.Key("hed");
    writer.StartArray();
    writer.Double(hed->x);
    writer.Double(hed->y);
    writer.Double(hed->z);
    writer.EndArray();

    writer.Key("hed_noise");
    writer.StartArray();
    writer.Double(hedc->x);
    writer.Double(hedc->y);
    writer.Double(hedc->z);
    writer.EndArray();

    //writer.Key("RSSI");
    //writer.Double(bsm->getRSSI());

    writer.EndObject();

    traceJSON(traceJSONFile, s.GetString());
}

void VeReMiPrintable::serializeRawData(veins::Coord*curPosition,
        veins::Coord*curPositionConfidence, veins::Coord*curSpeed, veins::Coord*curSpeedConfidence,
        veins::Coord*curHeading, veins::Coord*curHeadingConfidence, veins::Coord*curAccel,
        veins::Coord*curAccelConfidence) {

    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartObject();

    writer.Key("type");
    writer.Uint(TYPE_GPS);
    writer.Key("rcvTime");
    writer.Double(simTime().dbl());

    writer.Key("pos");
    writer.StartArray();
    writer.Double(curPosition->x);
    writer.Double(curPosition->y);
    writer.Double(curPosition->z);
    writer.EndArray();

    writer.Key("pos_noise");
    writer.StartArray();
    writer.Double(curPositionConfidence->x);
    writer.Double(curPositionConfidence->y);
    writer.Double(curPositionConfidence->z);
    writer.EndArray();

    writer.Key("spd");
    writer.StartArray();
    writer.Double(curSpeed->x);
    writer.Double(curSpeed->y);
    writer.Double(curSpeed->z);
    writer.EndArray();

    writer.Key("spd_noise");
    writer.StartArray();
    writer.Double(curSpeedConfidence->x);
    writer.Double(curSpeedConfidence->y);
    writer.Double(curSpeedConfidence->z);
    writer.EndArray();

    writer.Key("acl");
    writer.StartArray();
    writer.Double(curAccel->x);
    writer.Double(curAccel->y);
    writer.Double(curAccel->z);
    writer.EndArray();

    writer.Key("acl_noise");
    writer.StartArray();
    writer.Double(curAccelConfidence->x);
    writer.Double(curAccelConfidence->y);
    writer.Double(curAccelConfidence->z);
    writer.EndArray();

    writer.Key("hed");
    writer.StartArray();
    writer.Double(curHeading->x);
    writer.Double(curHeading->y);
    writer.Double(curHeading->z);
    writer.EndArray();

    writer.Key("hed_noise");
    writer.StartArray();
    writer.Double(curHeadingConfidence->x);
    writer.Double(curHeadingConfidence->y);
    writer.Double(curHeadingConfidence->z);
    writer.EndArray();

    writer.EndObject();

    traceJSON(traceJSONFile, s.GetString());
}
