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

#ifndef __VEINS_VeReMiPrintable_H_
#define __VEINS_VeReMiPrintable_H_

#include <omnetpp.h>
#include <veins/modules/messages/BasicSafetyMessage_m.h>
#include <fstream>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace veins;

#define TYPE_GPS 2
#define TYPE_BEACON 3
#define TYPE_TRUTH_BEACON 4

class VeReMiPrintable {

private:

public:

    std::string traceJSONFile;
    std::string traceGroundTruthJSONFile;

    void initVeReMiPrintable(std::string basePath, std::string serial, LAddress::L2Type& realId, int parentId, int attackerType, const std::string curDate,double sliceTime, double sliceStartTime, double simTime);


    virtual const void traceJSON(std::string file, std::string JSONObject) const;
    void serializeGroundTruth(BasicSafetyMessage *bsm);
    void serializeBeacon(BasicSafetyMessage *bsm);
    void serializeRawData(veins::Coord*curPosition, veins::Coord*curPositionConfidence,
            veins::Coord*curSpeed, veins::Coord*curSpeedConfidence, veins::Coord*curHeading,
            veins::Coord*curHeadingConfidence, veins::Coord*curAccel,
            veins::Coord*curAccelConfidence);

};

#endif
