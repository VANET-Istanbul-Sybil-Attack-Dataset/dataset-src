#include <list>

#include <veins/modules/application/istanbul/attacks/ReplaySybil.h>
#include <veins/modules/application/istanbul/IstanbulDsParams.h>


ReplaySybil::ReplaySybil() {
    for (int i = 0 ; i < MAX_SYBIL_NUM ; i++)
        powerControl[i] = NULL;
}

ReplaySybil::~ReplaySybil() {
    for (int i = 0 ; i < MAX_SYBIL_NUM ; i++) {
        if (powerControl[i]) {
            delete powerControl[i];
            powerControl[i] = NULL;
        }
    }
}

void ReplaySybil::init() {
    std::cout << "ReplaySybil::init START" << endl;

    sybilNodeCount = genLib.RandomInt(istanbulDsParams.minSybilNodeCount, istanbulDsParams.maxSybilNodeCount);
    if (sybilNodeCount < 2)
        sybilNodeCount = 2;
    seq = 0;
    for (int i = 0 ; i < sybilNodeCount ; i++) {
        pseudonyms[i] = pcPolicy->getNextPseudonym();
        targets[i] = 0;
        cout << "pseudonyms " << pseudonyms[i] << endl;
    }
    PowerControlAttackBase::init(sybilNodeCount);
    cout << log << endl;
    std::cout << "ReplaySybil::init END" << endl;
}

bool ReplaySybil::targetsIncludes(unsigned long x) {
    for (int i = 0; i < sybilNodeCount; i++) {
        if (targets[i] == x)
            return true;
    }
    return false;
}


static inline BasicSafetyMessage emptyBsm() {
    BasicSafetyMessage bsm;
    bsm.setSenderPos(Coord(0, 0, 0));
    bsm.setSenderPseudonym(0);
    return bsm;
}


BasicSafetyMessage ReplaySybil::launchAttack(LinkControl *LinkC) {
    if (!initCompleted) {
        beaconInterval->setRaw(beaconInterval->raw() / sybilNodeCount);
        initCompleted = true;
    }

    if (++seq >= sybilNodeCount)
        seq = 0;

    // check old target and find new posible target
    double targetDistance = 0;
    unsigned long nextTarget = 0;
    auto x = detectedNodes->getNearVehcilesSorted(*curPosition);
    for (int i = 0 ; i < x.size() ; i++) {
        double distance = get<0>(x.at(i));
        unsigned int node = get<1>(x.at(i));
        // cout << distance << " " << node << endl;
        if (targets[seq] == node)
            targetDistance = distance;
        if (nextTarget == 0 && !targetsIncludes(node))
            nextTarget = node;
    }

    if (targets[seq] == 0)
        targets[seq] = nextTarget;

    BasicSafetyMessage attackBsm = emptyBsm();
    if (targets[seq] != 0) {
        BasicSafetyMessage *_attackBsm = detectedNodes->getLastBSM(targets[seq]);
        if (_attackBsm)
            attackBsm = *_attackBsm;
    }

    times[seq] = attackBsm.getArrivalTime().dbl();
    attackBsm.setSenderPseudonym(pseudonyms[seq]);
    attackBsm.setPowerControlType(powerControlType);
    if (powerControl[seq])
        powerControl[seq]->apply(attackBsm);
    lastBSM[seq] = attackBsm;

    return attackBsm;
}
