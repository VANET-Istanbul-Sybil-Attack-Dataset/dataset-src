#include <list>

#include <veins/modules/application/istanbul/attacks/RandomSybil.h>
#include <veins/modules/application/istanbul/IstanbulDsParams.h>

RandomSybil::RandomSybil() {
    for (int i = 0 ; i < MAX_SYBIL_NUM ; i++)
        powerControl[i] = NULL;
}

RandomSybil::~RandomSybil() {
    for (int i = 0 ; i < MAX_SYBIL_NUM ; i++) {
        if (powerControl[i]) {
            delete powerControl[i];
            powerControl[i] = NULL;
        }
    }
}

void RandomSybil::init(attackTypes::Attacks myAttackType, double MaxRandomPosX, double MaxRandomPosY, F2MDParameters *params) {
    std::cout << "RandomSybil::init START" << endl;

    this->params = params;
    this->MaxRandomPosX = MaxRandomPosX;
    this->MaxRandomPosY = MaxRandomPosY;

    sybilNodeCount = genLib.RandomInt(istanbulDsParams.minSybilNodeCount, istanbulDsParams.maxSybilNodeCount);
    if (sybilNodeCount < 2)
        sybilNodeCount = 2;

    for (int i = 0 ; i < sybilNodeCount ; i++) {
        pseudonyms[i] = pcPolicy->getNextPseudonym();
        cout << "pseudonyms " << pseudonyms[i] << endl;
    }

    seq = 0;
    PowerControlAttackBase::init(sybilNodeCount);
    cout << log << endl;
    std::cout << "RandomSybil::init END" << endl;
}


BasicSafetyMessage RandomSybil::launchAttack(LinkControl *LinkC) {
    // cout << "RandomSybil::launchAttack" << endl;
    if (!initCompleted) {
        beaconInterval->setRaw(beaconInterval->raw() / sybilNodeCount);
        initCompleted = true;
    }

    if (++seq >= sybilNodeCount)
        seq = 0;

    BasicSafetyMessage attackBsm;

    // f2md random
    double x = genLib.RandomDouble(0, MaxRandomPosX);
    double y = genLib.RandomDouble(0, MaxRandomPosY);
    double sx = genLib.RandomDouble(0, params->RandomSpeedX);
    double sy = genLib.RandomDouble(0, params->RandomSpeedY);
    double ax = genLib.RandomDouble(0, params->RandomAccelX);
    double ay = genLib.RandomDouble(0, params->RandomAccelY);
    double hx = genLib.RandomDouble(-1, 1);
    double hy = genLib.RandomDouble(-1, 1);
    attackBsm.setSenderPos(Coord(x, y, (*curPosition).z));
    attackBsm.setSenderPosConfidence(Coord(0, 0, 0)); // FIXME
    attackBsm.setSenderSpeed(Coord(sx, sy, (*curSpeed).z));
    attackBsm.setSenderSpeedConfidence(Coord(0, 0, 0)); // FIXME
    attackBsm.setSenderAccel(Coord(ax, ay, (*curAccel).z));
    attackBsm.setSenderAccelConfidence(Coord(0, 0, 0)); // FIXME
    attackBsm.setSenderHeading(Coord(hx, hy, (*curHeading).z));
    attackBsm.setSenderHeadingConfidence(*curHeadingConfidence);
    attackBsm.setSenderWidth(*myWidth);
    attackBsm.setSenderLength(*myLength);

    times[seq] = attackBsm.getArrivalTime().dbl();
    attackBsm.setSenderPseudonym(pseudonyms[seq]);
    attackBsm.setPowerControlType(powerControlType);
    if (powerControl[seq])
        powerControl[seq]->apply(attackBsm);
    lastBSM[seq] = attackBsm;

    return attackBsm;
}
