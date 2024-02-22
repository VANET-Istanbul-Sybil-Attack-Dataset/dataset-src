#include <veins/modules/application/istanbul/attacks/GridSybil.h>
#include <veins/modules/application/istanbul/IstanbulDsParams.h>
#include <list>


GridSybil::GridSybil() {
    for (int i = 0 ; i < MAX_SYBIL_NUM ; i++)
        powerControl[i] = NULL;
}

GridSybil::~GridSybil() {
    for (int i = 0 ; i < MAX_SYBIL_NUM ; i++) {
        if (powerControl[i]) {
            delete powerControl[i];
            powerControl[i] = NULL;
        }
    }
}

void GridSybil::init() {
    std::cout << "GridSybil::init START" << endl;

    int sybilNodeCountSelect[] = { 10, 9, 6, 5, 3 };
    if (istanbulDsParams.selectedGridShape == -1)
      sybilNodeCount = sybilNodeCountSelect[genLib.RandomInt(0, 4)];
    else
      sybilNodeCount = sybilNodeCountSelect[istanbulDsParams.selectedGridShape];
    cout << "sybilNodeCount: " << sybilNodeCount << endl;

    seq = 0;
    for (int i = 0 ; i < sybilNodeCount ; i++) {
        pseudonyms[i] = pcPolicy->getNextPseudonym();
        cout << "pseudonyms " << pseudonyms[i] << endl;
    }
    PowerControlAttackBase::init(sybilNodeCount);
    cout << log << endl;
    std::cout << "GridSybil::init END" << endl;
}


static inline Coord randomize_coord(GeneralLib& genLib, const Coord& coord, double value) {
    return Coord(coord.x * (1.0 + genLib.RandomDouble(-value/2, value/2)),
            coord.y * (1.0 + genLib.RandomDouble(-value/2, value/2)),
            coord.z * (1.0 + genLib.RandomDouble(-value/2, value/2)));
}

BasicSafetyMessage GridSybil::launchAttack(LinkControl *LinkC) {
    if (!initCompleted) {
        beaconInterval->setRaw(beaconInterval->raw() / sybilNodeCount);
        initCompleted = true;
        // TODO: width & length randomization ?
    }

    if (++seq >= sybilNodeCount)
        seq = 0;

    double time = simTime().dbl();
    const Coord& hed = *curHeading;
    const Coord& pos = *curPosition;
    double speed = sqrt(pow(curSpeed->x, 2.0) + pow(curSpeed->y, 2.0) + pow(curSpeed->z, 2.0));

    const Coord prevAttackPos = lastBSM[seq].getSenderPos();
    // const Coord prevAttackSpd = lastBSM[seq].getSenderSpeed();
    double prevTime = times[seq];

    double followDistance = std::min(1.0, 2 * speed);
    const Coord vertical = hed * ((*myLength + followDistance) *  genLib.RandomDouble(1-0.025, 1+0.025));
    const Coord horizontal = hed.rotatedYaw(-90) * (*myWidth * genLib.RandomDouble(2-0.025, 2+0.025));

    Coord attackPos;
    if (sybilNodeCount == 10) // 5x2
        attackPos = pos + vertical * (2 - seq / 2) + horizontal * (1 - seq % 2);
    else if (sybilNodeCount == 9) // 3x3
        attackPos = pos + vertical * (1 - seq / 3) + horizontal * (1 - seq % 3);
    else if (sybilNodeCount == 6) // 3x2
        attackPos = pos + vertical * (1 - seq / 3) +  horizontal * (1 - seq % 2);
    else if (sybilNodeCount == 5) // 5x1
        attackPos = pos + vertical * (2 - seq % 5);
    else  // 3x1
        attackPos = pos + vertical * (1 - seq % 3);

    const Coord attackSpd = (prevTime > 0.00001) ? (attackPos - prevAttackPos) / (time - prevTime) : randomize_coord(genLib, *curSpeed, 0.02);

    BasicSafetyMessage attackBsm;
    attackBsm.setSenderPos(attackPos);
    attackBsm.setSenderPosConfidence(randomize_coord(genLib, *curPositionConfidence, 0.02));
    attackBsm.setSenderSpeed(attackSpd);
    attackBsm.setSenderSpeedConfidence(randomize_coord(genLib, *curSpeedConfidence, 0.02));
    attackBsm.setSenderAccel(randomize_coord(genLib, *curAccel, 0.02));
    attackBsm.setSenderAccelConfidence(randomize_coord(genLib, *curAccelConfidence, 0.02));
    attackBsm.setSenderHeading(randomize_coord(genLib, *curHeading, 0.01));
    attackBsm.setSenderHeadingConfidence(randomize_coord(genLib, *curHeadingConfidence, 0.02));
    attackBsm.setSenderWidth(*myWidth);
    attackBsm.setSenderLength(*myLength);

    times[seq] = time;
    attackBsm.setSenderPseudonym(pseudonyms[seq]);
    attackBsm.setPowerControlType(powerControlType);
    if (powerControl[seq])
        powerControl[seq]->apply(attackBsm);
    lastBSM[seq] = attackBsm;

//    cout << "attack pseudonym" << attackBsm.getSenderPseudonym() << endl;
//    cout << "speed: " << speed << " - " << "attackSpdVal: " << attackBsm.getSenderSpeed().length() << endl;
//    cout << "attackSpd: " << attackSpd.x << "," << attackSpd.y << endl;
//    cout << "spd: " << curSpeed->x << "," << curSpeed->y << endl;
//    cout << "accel: " << curAccel->length() << " - " << "attackAclVal: " << attackBsm.getSenderAccel().length() << endl;
//    cout << "distance: " << (*curPosition - attackBsm.getSenderPos()).length() << endl << endl;
    return attackBsm;
}
