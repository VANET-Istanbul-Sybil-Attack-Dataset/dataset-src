#include <veins/modules/application/istanbul/BsmLogger.h>
#include <iomanip>

static bool folderCreated = false;
static int prevSlice = 0;

static std::string directoryPath = "";

void BsmLogger::init(
        std::string basePath,
        std::string serial,
        LAddress::L2Type& realId,
        int parentId,
        int attackerType,
        const std::string curDate,
        double sliceTime,
        double sliceStartTime,
        double simTime)
{
    int curSlice = (simTime - sliceStartTime) / sliceTime;

    if (prevSlice != curSlice || !folderCreated) {
        directoryPath = basePath + serial;
        struct stat info;
        int r = stat(directoryPath.c_str(), &info);
        if (r != 0 || info.st_mode & S_IFDIR != S_IFDIR)
            mkdir(directoryPath.c_str(), 0777);

        directoryPath += "/dataset"
            + std::string("_") + std::to_string((int)(sliceTime*curSlice + sliceStartTime))
            + std::string("_") + std::to_string((int)(sliceTime*(curSlice+1) + sliceStartTime))
            + std::string("_") + curDate;

        r = stat(directoryPath.c_str(), &info);
        if (r != 0 || info.st_mode & S_IFDIR != S_IFDIR)
            mkdir(directoryPath.c_str(), 0777);
    }

    std::ostringstream out;
    out << directoryPath << "/trace" << "-" << realId << "-" << parentId << "-A"
            << attackerType << "-" << simTime << "-" << curSlice << ".txt";
    traceFile = out.str();

    std::ostringstream out_gt;
    out_gt << directoryPath << "/traceGroundTruth" << "-" << curSlice
            << ".txt";
    traceGroundTruthFile = out_gt.str();
}


void BsmLogger::writeTrace(std::string file, std::string data) const {
    std::ofstream out_stream;
    out_stream.open(file, std::ios_base::app);
    if(out_stream.is_open())
        out_stream << data << std::endl;
    else
        EV << "Warning, tracing stream is closed";
    out_stream.close();
}

static inline void writeCoord(std::ostringstream& out, const veins::Coord& coord) {
    out << "|" << coord.x << "," << coord.y << "," << coord.z;
}

static inline std::string bsm_to_string(long vehicleId, long messageId, const std::pair<double, double>& gps,
        const Coord& pos, const Coord& posc, const Coord& spd, const Coord& spdc,
        const Coord& hed, const Coord& hedc, const Coord& acl, const Coord& aclc,
        double vehicleWidth, double vehicleLength
) {
    std::ostringstream out;

    out << std::fixed << std::setprecision(12);

    out << vehicleId;
    out << "|" << messageId;
    out << "|" << gps.first << "," << gps.second;
    writeCoord(out, pos);
    writeCoord(out, posc);
    writeCoord(out, spd);
    writeCoord(out, spdc);
    writeCoord(out, hed);
    writeCoord(out, hedc);
    writeCoord(out, acl);
    writeCoord(out, aclc);
    out << "|" << vehicleWidth;
    out << "|" << vehicleLength;

    return out.str();
}

static inline std::string bsm_to_string(const BasicSafetyMessage *bsm) {
    return bsm_to_string(bsm->getSenderPseudonym(),
            bsm->getTreeId(), std::pair<double, double>(bsm->getSenderGpsCoordinates().x, bsm->getSenderGpsCoordinates().y),
            bsm->getSenderPos(), bsm->getSenderPosConfidence(),
            bsm->getSenderSpeed(), bsm->getSenderSpeedConfidence(),
            bsm->getSenderHeading(), bsm->getSenderHeadingConfidence(),
            bsm->getSenderAccel(), bsm->getSenderAccelConfidence(),
            bsm->getSenderWidth(), bsm->getSenderLength());
}

void BsmLogger::saveGroundTruth(
        long myId,
        BasicSafetyMessage *bsm,
        long messageId,
        const std::pair<double, double> &curLonLat,
        const veins::Coord &curPosition,
        const veins::Coord &curPositionConfidence, const veins::Coord &curSpeed,
        const veins::Coord &curSpeedConfidence, const veins::Coord &curHeading,
        const veins::Coord &curHeadingConfidence, const veins::Coord &curAccel,
        const veins::Coord &curAccelConfidence, double curWidth, double curLength)
{
    std::ostringstream out;

    // type
    out << 0 << "#";

    // recv info (empty)
    out << "#";

    double time = simTime().dbl();

    // bsm
    out << time;
    out << "|" << bsm_to_string(bsm);
    out << "#";

    // labels
    out << bsm->getSenderAttackType();
    out << "|" << bsm->getPowerControlType();
    out << "|" << bsm->getPowerControlTxPower();
    out << "|" << time;
    out << "|" << bsm_to_string(bsm->getSenderRealId(), messageId, curLonLat, curPosition, curPositionConfidence,
            curSpeed, curSpeedConfidence, curHeading, curHeadingConfidence, curAccel, curAccelConfidence, curWidth, curLength);

    writeTrace(traceGroundTruthFile, out.str());
}

void BsmLogger::saveBeacon(long myId,
        BasicSafetyMessage *bsm,
        double rssi,
        const std::pair<double, double>& curLonLat,
        const veins::Coord &curPosition,
        const veins::Coord &curPositionConfidence, const veins::Coord &curSpeed,
        const veins::Coord &curSpeedConfidence, const veins::Coord &curHeading,
        const veins::Coord &curHeadingConfidence, const veins::Coord &curAccel,
        const veins::Coord &curAccelConfidence, double curWidth, double curLength)
{

    std::ostringstream out;

    // type
    out << 1 << "#";

    // recv info
    out << rssi;
    out << "|" << bsm->getArrivalTime().dbl();
    out << "|" << bsm_to_string(myId, 0, curLonLat, curPosition, curPositionConfidence,
                curSpeed, curSpeedConfidence, curHeading, curHeadingConfidence, curAccel, curAccelConfidence, curWidth, curLength);
    out << "#";

    // bsm
    out << bsm->getSendingTime().dbl();
    out << "|" << bsm_to_string(bsm);
    out << "#";

    // labels (empty)

    writeTrace(traceFile, out.str());
}

