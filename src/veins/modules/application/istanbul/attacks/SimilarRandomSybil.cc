#include <list>

#include <veins/modules/application/istanbul/attacks/SimilarRandomSybil.h>
#include <veins/modules/application/istanbul/IstanbulDsParams.h>


SimilarRandomSybil::SimilarRandomSybil() {
    for (int i = 0 ; i < MAX_SYBIL_NUM ; i++)
        powerControl[i] = NULL;
}

SimilarRandomSybil::~SimilarRandomSybil() {
    for (int i = 0 ; i < MAX_SYBIL_NUM ; i++) {
        if (powerControl[i]) {
            delete powerControl[i];
            powerControl[i] = NULL;
        }
    }
}

void SimilarRandomSybil::init() {
    std::cout << "SimilarRandomSybil::init START" << endl;

    sybilNodeCount = genLib.RandomInt(istanbulDsParams.minSybilNodeCount, istanbulDsParams.maxSybilNodeCount);
    if (sybilNodeCount < 2)
        sybilNodeCount = 2;
    seq = 0;
    for (int i = 0 ; i < sybilNodeCount ; i++) {
        pseudonyms[i] = pcPolicy->getNextPseudonym();
        cout << "pseudonyms " << pseudonyms[i] << endl;
    }
    PowerControlAttackBase::init(sybilNodeCount);

    std::cout << "SimilarRandomSybil::init END" << endl;
}


static void update_min_max(const Coord& value, Coord& min, Coord& max) {
    if (value.x < min.x)
        min.x = value.x;
    if (value.y < min.y)
        min.y = value.y;
    if (value.z < min.z)
        min.z = value.z;
    if (value.x > max.x)
        max.x = value.x;
    if (value.y > max.y)
        max.y = value.y;
    if (value.z > max.z)
        max.z = value.z;
}

Coord SimilarRandomSybil::randomCoord(const Coord &min, const Coord &max) {
    double x = genLib.RandomDouble(min.x, max.x);
    double y = genLib.RandomDouble(min.y, max.y);
    double z = genLib.RandomDouble(min.z, max.z);
    return Coord(x, y, z);
}


BasicSafetyMessage SimilarRandomSybil::launchAttack(LinkControl *LinkC) {

    if (!initCompleted) {
        beaconInterval->setRaw(beaconInterval->raw() / sybilNodeCount);
        initCompleted = true;
    }

    if (++seq >= sybilNodeCount)
        seq = 0;

    Coord minPos = *curPosition;
    Coord maxPos = *curPosition;
    Coord minPosC = *curPositionConfidence;
    Coord maxPosC = *curPositionConfidence;

    Coord minSpd = *curSpeed;
    Coord maxSpd = *curSpeed;
    Coord minSpdC = *curSpeedConfidence;
    Coord maxSpdC = *curSpeedConfidence;

    Coord minAcl = *curAccel;
    Coord maxAcl = *curAccel;
    Coord minAclC = *curAccelConfidence;
    Coord maxAclC = *curAccelConfidence;

    Coord minHed = *curHeading;
    Coord maxHed = *curHeading;
    Coord minHedC = *curHeadingConfidence;
    Coord maxHedC = *curHeadingConfidence;

    double minWidth = *myWidth;
    double maxWidth = *myWidth;
    double minLength = *myLength;
    double maxLength = *myLength;

    double targetDistance = 0;
    unsigned long nextTarget = 0;
    auto x = detectedNodes->getNearVehcilesSorted(*curPosition);

    // find maxDistance contains min 2 vehicles
    // search with exponential backoff
    double maxDistance = 10;
    while (maxDistance < 200) {
        int count = 0;
        for (int i = 0; i < x.size(); i++) {
            double distance = get<0>(x.at(i));
            unsigned int node = get<1>(x.at(i));
            if (distance > maxDistance)
                break;
            count += 1;
        }
        if (count >= 2)
            break;
        maxDistance *= 2;
    }

    // calculate limits in maxDistance
    for (int i = 0; i < x.size(); i++) {
        double distance = get<0>(x.at(i));
        unsigned int node = get<1>(x.at(i));
        // cout << distance << " " << node << endl;
        if (distance > maxDistance)
            break;

        BasicSafetyMessage lastBsm = *detectedNodes->getLastBSM(node);
//        cout << "!!!  " << lastBsm.getSenderPos().x << " "
//                << lastBsm.getSenderPos().y << " " << lastBsm.getSenderSpeed().x
//                << " " << lastBsm.getSenderSpeed().y << endl;

        update_min_max(lastBsm.getSenderPos(), minPos, maxPos);
        update_min_max(lastBsm.getSenderPosConfidence(), minPosC, maxPosC);
        update_min_max(lastBsm.getSenderSpeed(), minSpd, maxSpd);
        update_min_max(lastBsm.getSenderSpeedConfidence(), minSpdC, maxSpdC);
        update_min_max(lastBsm.getSenderAccel(), minAcl, maxAcl);
        update_min_max(lastBsm.getSenderAccelConfidence(), minAclC, maxAclC);
        update_min_max(lastBsm.getSenderHeading(), minHed, maxHed);
        update_min_max(lastBsm.getSenderHeadingConfidence(), minHedC, maxHedC);
        if (lastBsm.getSenderWidth() > maxWidth)
            maxWidth = lastBsm.getSenderWidth();
        if (lastBsm.getSenderWidth() < minWidth)
            minWidth = lastBsm.getSenderWidth();
        if (lastBsm.getSenderLength() > maxLength)
            maxLength = lastBsm.getSenderLength();
        if (lastBsm.getSenderLength() < minLength)
            minLength = lastBsm.getSenderLength();
    }

/*    cout << "X: " << minPos.x << "-" << maxPos.x << "  Y: " << minPos.y << "-" << maxPos.y << endl;
    cout << "sX: " << minSpd.x << "-" << maxSpd.x << "  sY: " << minSpd.y << "-" << maxSpd.y << endl;*/

    BasicSafetyMessage attackBsm;
    attackBsm.setSenderPos(randomCoord(minPos, maxPos));
    attackBsm.setSenderPosConfidence(randomCoord(minPosC, maxPosC));
    attackBsm.setSenderSpeed(randomCoord(minSpd, maxSpd));
    attackBsm.setSenderSpeedConfidence(randomCoord(minSpdC, maxSpdC));
    attackBsm.setSenderAccel(randomCoord(minAcl, maxAcl));
    attackBsm.setSenderAccelConfidence(randomCoord(minAclC, maxAclC));
    attackBsm.setSenderHeading(randomCoord(minHed, maxHed));
    attackBsm.setSenderHeadingConfidence(randomCoord(minHedC, maxHedC));
    attackBsm.setSenderWidth(genLib.RandomDouble(minWidth, maxWidth));
    attackBsm.setSenderLength(genLib.RandomDouble(minLength, maxLength));

    attackBsm.setSenderPseudonym(pseudonyms[seq]);
    attackBsm.setPowerControlType(powerControlType);
    if (powerControl[seq])
        powerControl[seq]->apply(attackBsm);
    lastBSM[seq] = attackBsm;

    return attackBsm;
}
