#include "RandomSybilRandomPCAttack.h"

#include <list>

void RandomSybilRandomPCAttack::init(attackTypes::Attacks myAttackType, double MaxRandomPosX, double MaxRandomPosY, F2MDParameters *params) {
    std::cout << "RandomSybilWithPCAttack::init START" << endl;

    this->params = params;
    DoSInitiated = false;

    this->MaxRandomPosX = MaxRandomPosX;
    this->MaxRandomPosY = MaxRandomPosY;

//    localDosMultipleFreq = (int) genLib.RandomDouble((1 - params->parVar), (1 + params->parVar)) * params->DosMultipleFreq;
//    if (localDosMultipleFreq < 2) {
//        localDosMultipleFreq = 2;
//    }

    localDosMultipleFreqSybil = (int) genLib.RandomDouble((1 - params->parVar), (1 + params->parVar)) * params->DosMultipleFreqSybil;
    if (localDosMultipleFreqSybil < 2) {
        localDosMultipleFreqSybil = 2;
    }

    std::cout << "RandomSybilWithPCAttack::init END" << endl;
}

BasicSafetyMessage RandomSybilRandomPCAttack::launchAttack(LinkControl *LinkC) {
    cout << "RandomSybilRandomPCAttack::launchAttack" << endl;
    BasicSafetyMessage attackBsm;

    // F2MDDoSRandomSybil START
    if (!DoSInitiated) {
        beaconInterval->setRaw(beaconInterval->raw() / localDosMultipleFreqSybil);
        DoSInitiated = true;
    }
    attackBsm = myBsm[0];

    attackBsm.setSenderPseudonym(pcPolicy->getNextPseudonym());

    double x = genLib.RandomDouble(0, MaxRandomPosX);
    double y = genLib.RandomDouble(0, MaxRandomPosY);

    double sx = genLib.RandomDouble(0, params->RandomSpeedX);
    double sy = genLib.RandomDouble(0, params->RandomSpeedY);

    double ax = genLib.RandomDouble(0, params->RandomAccelX);
    double ay = genLib.RandomDouble(0, params->RandomAccelY);

    double hx = genLib.RandomDouble(-1, 1);
    double hy = genLib.RandomDouble(-1, 1);

    attackBsm.setSenderPos(Coord(x, y, (*curPosition).z));
    attackBsm.setSenderPosConfidence(*curPositionConfidence);

    attackBsm.setSenderSpeed(Coord(sx, sy, (*curSpeed).z));
    attackBsm.setSenderSpeedConfidence(*curSpeedConfidence);

    attackBsm.setSenderAccel(Coord(ax, ay, (*curAccel).z));
    attackBsm.setSenderAccelConfidence(*curAccelConfidence);

    attackBsm.setSenderHeading(Coord(hx, hy, (*curHeading).z));
    attackBsm.setSenderHeadingConfidence(*curHeadingConfidence);

    attackBsm.setSenderWidth(*myWidth);
    attackBsm.setSenderLength(*myLength);
    // F2MDDoSRandomSybil END

    /*string s;
   for (int i = 0 ; i < myBsmNum ; i++) {
       s += std::to_string(myBsm->getPowerControl()) + " ";
   }
   cout << "myBsm " << s << endl;*/
    double txPower = (*myBsmNum > 0) ? myBsm[1].getPowerControlTxPower() + 1.0 : 17.5;
    if (txPower > 25.0)
        txPower = 15.0;
    attackBsm.setPowerControlTxPower(txPower);



    return attackBsm;
}
