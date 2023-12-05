#ifndef __DATASET_LOG_H__
#define __DATASET_LOG_H__

#include <omnetpp.h>
#include <veins/modules/messages/BasicSafetyMessage_m.h>
#include <fstream>

//#include "rapidjson/writer.h"
//#include "rapidjson/stringbuffer.h"
//using namespace rapidjson;

using namespace veins;

#define TYPE_GPS 2
#define TYPE_BEACON 3
#define TYPE_TRUTH_BEACON 4

class BsmLogger {

private:

public:

    std::string traceFile;
    std::string traceGroundTruthFile;

    void init(std::string basePath, std::string serial,
            LAddress::L2Type &realId, int parentId, int attackerType,
            const std::string curDate, double sliceTime, double sliceStartTime,
            double simTime);

    virtual void writeTrace(std::string file, std::string JSONObject) const;
    void saveGroundTruth(
            long myId,
            BasicSafetyMessage *bsm,
            long messageId,
            const std::pair<double, double> &curLonLat,
            const veins::Coord &curPosition,
            const veins::Coord &curPositionConfidence, const veins::Coord &curSpeed,
            const veins::Coord &curSpeedConfidence, const veins::Coord &curHeading,
            const veins::Coord &curHeadingConfidence, const veins::Coord &curAccel,
            const veins::Coord &curAccelConfidence, double curWidth, double curLength);

    void saveBeacon(long myId, BasicSafetyMessage *bsm, double rssi,
            const std::pair<double, double> &curLonLat,
            const veins::Coord &curPosition,
            const veins::Coord &curPositionConfidence,
            const veins::Coord &curSpeed,
            const veins::Coord &curSpeedConfidence,
            const veins::Coord &curHeading,
            const veins::Coord &curHeadingConfidence,
            const veins::Coord &curAccel,
            const veins::Coord &curAccelConfidence, double curWidth,
            double curLength);

};

#endif /* __DATASET_LOG_H__ */
