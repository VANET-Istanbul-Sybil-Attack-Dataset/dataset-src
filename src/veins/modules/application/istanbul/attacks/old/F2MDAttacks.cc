#include "F2MDAttacks.h"

void F2MDGridSybil::init(attackTypes::Attacks myAttackType, double MaxRandomPosX,double MaxRandomPosY,F2MDParameters *params) {
    std::cout << "F2MDGridSybil::init START" << endl;
    this->params = params;
    SybilMyOldPseudo = (*myPseudonym);
    SybilVehSeq = 0;

    localSybilVehNumber = (int) genLib.RandomDouble((1-params->parVar), (1+params->parVar)) * params->SybilVehNumber;
    if(localSybilVehNumber < 1){
        localSybilVehNumber = 1;
    }

    localSybilDistanceX = genLib.RandomDouble((1-params->parVar), (1+params->parVar)) * params->SybilDistanceX;
    localSybilDistanceY = genLib.RandomDouble((1-params->parVar), (1+params->parVar)) * params->SybilDistanceY;

    if (myAttackType == attackTypes::GridSybil) {
        for (int var = 0; var < localSybilVehNumber; ++var) {
            SybilPseudonyms[var] = pcPolicy->getNextPseudonym();
        }
    }
    std::cout << "F2MDGridSybil::init END" << endl;
}

void F2MDDoSDisruptiveSybil::init(attackTypes::Attacks myAttackType, double MaxRandomPosX,double MaxRandomPosY,F2MDParameters *params) {
    std::cout << "F2MDDoSDisruptiveSybil::init START" << endl;
    DoSInitiated = false;

    localDosMultipleFreq = (int) genLib.RandomDouble((1-params->parVar), (1+params->parVar)) * params->DosMultipleFreq;
    if(localDosMultipleFreq <2){
        localDosMultipleFreq = 2;
    }

    localDosMultipleFreqSybil = (int) genLib.RandomDouble((1-params->parVar), (1+params->parVar)) * params->DosMultipleFreqSybil;
    if(localDosMultipleFreqSybil <2){
        localDosMultipleFreqSybil = 2;
    }

    std::cout << "F2MDDoSDisruptiveSybil::init END" << endl;
}

void F2MDDataReplaySybil::init(attackTypes::Attacks myAttackType, double MaxRandomPosX,double MaxRandomPosY,F2MDParameters *params) {
    std::cout << "F2MDDataReplaySybil::init START" << endl;
    localReplaySeqNum = (int) genLib.RandomDouble((1-params->parVar), (1+params->parVar)) * params->ReplaySeqNum;
    if(localReplaySeqNum <2){
        localReplaySeqNum = 2;
    }

    ReplaySeq = 0;

    std::cout << "F2MDDataReplaySybil::init END" << endl;
}

void F2MDDoSRandomSybil::init(attackTypes::Attacks myAttackType, double MaxRandomPosX,double MaxRandomPosY,F2MDParameters *params) {
    std::cout << "F2MDDoSRandomSybil::init START" << endl;
    this->params = params;
    DoSInitiated = false;

    this->MaxRandomPosX = MaxRandomPosX;
    this->MaxRandomPosY = MaxRandomPosY;

    localDosMultipleFreq = (int) genLib.RandomDouble((1-params->parVar), (1+params->parVar)) * params->DosMultipleFreq;
    if(localDosMultipleFreq <2){
        localDosMultipleFreq = 2;
    }

    localDosMultipleFreqSybil = (int) genLib.RandomDouble((1-params->parVar), (1+params->parVar)) * params->DosMultipleFreqSybil;
    if(localDosMultipleFreqSybil <2){
        localDosMultipleFreqSybil = 2;
    }

    std::cout << "F2MDDoSRandomSybil::init END" << endl;
}


BasicSafetyMessage F2MDGridSybil::launchAttack(LinkControl *LinkC) {
    BasicSafetyMessage attackBsm = BasicSafetyMessage();

    // cout << "F2MDGridSybil::launchAttack" << endl;

    if (!DoSInitiated) {
        beaconInterval->setRaw(
                beaconInterval->raw() / (localSybilVehNumber + 1));
        DoSInitiated = true;
    }

    if (SybilMyOldPseudo != *myPseudonym) {
        SybilMyOldPseudo = *myPseudonym;
        for (int var = 0; var < localSybilVehNumber; ++var) {
            SybilPseudonyms[var] = pcPolicy->getNextPseudonym();
        }
    }

    int SquareX = SybilVehSeq / 2;
    int SquareY = SybilVehSeq % 2;

    if (!params->SelfSybil) {
        if (detectedNodes->getNodesNum() > 0) {
            attackBsm = *detectedNodes->getNextAttackedBsm(*curPosition,
                    saveAttackBsm.getSenderPseudonym(),
                    saveAttackBsm.getArrivalTime().dbl());
            saveAttackBsm = attackBsm;
            targetNode = attackBsm.getSenderPseudonym();

            SquareX = (SybilVehSeq + 1) / 2;
            SquareY = (SybilVehSeq + 1) % 2;

            double sybDistXrand = genLib.RandomDouble(
                    -(localSybilDistanceX / 10.0),
                    (localSybilDistanceX / 10.0));
            double sybDistYrand = genLib.RandomDouble(
                    -(localSybilDistanceY / 10.0),
                    (localSybilDistanceY / 10.0));

            double XOffset = -SquareX
                    * (attackBsm.getSenderLength() + localSybilDistanceX)
                    + sybDistXrand;
            double YOffset = -SquareY
                    * (attackBsm.getSenderWidth() + localSybilDistanceY)
                    + sybDistYrand;
            MDMLib mdmLib = MDMLib();
            double curHeadingAngle = mdmLib.calculateHeadingAnglePtr(
                    &attackBsm.getSenderHeading());

            Coord relativePos = Coord(XOffset, YOffset, 0);
            double DeltaAngle = mdmLib.calculateHeadingAnglePtr(&relativePos);

            double newAngle = curHeadingAngle + DeltaAngle;
            newAngle = std::fmod(newAngle, 360);

            newAngle = 360 - newAngle;

            double DOffset = sqrt((pow(XOffset, 2)) + (pow(YOffset, 2)));

            double relativeX = DOffset * cos(newAngle * PI / 180);
            double relativeY = DOffset * sin(newAngle * PI / 180);

            double stearingAngle = std::fmod((saveHeading - curHeadingAngle),
                    360);
            if (stearingAngle > 180) {
                stearingAngle = 360 - stearingAngle;
            }
            if (stearingAngle > 5) {
                attackBsm.setSenderPseudonym(1);
            }

            saveHeading = curHeadingAngle;

            Coord newSenderPos = Coord(attackBsm.getSenderPos().x + relativeX,
                    attackBsm.getSenderPos().y + relativeY,
                    attackBsm.getSenderPos().z);

            double mapdistance = LinkC->calculateDistance(newSenderPos, 50, 50);
            if (mapdistance > params->MAX_DISTANCE_FROM_ROUTE) {
                attackBsm.setSenderPseudonym(1);
            }

            attackBsm.setSenderPos(newSenderPos);

            double PosConfDelta = genLib.RandomDouble(
                    -0.1 * attackBsm.getSenderPosConfidence().x,
                    +0.1 * attackBsm.getSenderPosConfidence().x);
            if (PosConfDelta < 0) {
                PosConfDelta = 0;
            }
            Coord newPosConf = Coord(
                    attackBsm.getSenderPosConfidence().x + PosConfDelta,
                    attackBsm.getSenderPosConfidence().y + PosConfDelta,
                    attackBsm.getSenderPosConfidence().z);

            attackBsm.setSenderPos(newSenderPos);
            attackBsm.setSenderPosConfidence(newPosConf);

            double randSpeedX = genLib.RandomDouble(
                    -0.05 * attackBsm.getSenderSpeed().x,
                    +0.05 * attackBsm.getSenderSpeed().x);
            double randSpeedY = genLib.RandomDouble(
                    -0.05 * attackBsm.getSenderSpeed().x,
                    +0.05 * attackBsm.getSenderSpeed().x);
            Coord newSpeed = Coord(attackBsm.getSenderSpeed().x + randSpeedX,
                    attackBsm.getSenderSpeed().y + randSpeedY,
                    attackBsm.getSenderSpeed().z);

            double SpeedConfDelta = genLib.RandomDouble(
                    -0.1 * attackBsm.getSenderSpeedConfidence().x,
                    +0.1 * attackBsm.getSenderSpeedConfidence().x);
            if (SpeedConfDelta < 0) {
                SpeedConfDelta = 0;
            }
            Coord newSpeedConf = Coord(
                    attackBsm.getSenderSpeedConfidence().x + SpeedConfDelta,
                    attackBsm.getSenderSpeedConfidence().y + SpeedConfDelta,
                    attackBsm.getSenderSpeedConfidence().z);

            attackBsm.setSenderSpeed(newSpeed);
            attackBsm.setSenderSpeedConfidence(newSpeedConf);

            double randAccelx = genLib.RandomDouble(
                    -0.05 * attackBsm.getSenderAccel().x,
                    +0.05 * attackBsm.getSenderAccel().x);
            double randAccely = genLib.RandomDouble(
                    -0.05 * attackBsm.getSenderAccel().x,
                    +0.05 * attackBsm.getSenderAccel().x);
            Coord newAccel = Coord(attackBsm.getSenderAccel().x + randAccelx,
                    attackBsm.getSenderAccel().y + randAccely,
                    attackBsm.getSenderAccel().z);

            double AccelConfDelta = genLib.RandomDouble(
                    -0.1 * attackBsm.getSenderAccelConfidence().x,
                    +0.1 * attackBsm.getSenderAccelConfidence().x);
            if (AccelConfDelta < 0) {
                AccelConfDelta = 0;
            }
            Coord newAccelConf = Coord(
                    attackBsm.getSenderAccelConfidence().x + AccelConfDelta,
                    attackBsm.getSenderAccelConfidence().y + AccelConfDelta,
                    attackBsm.getSenderAccelConfidence().z);

            attackBsm.setSenderSpeed(newAccel);
            attackBsm.setSenderSpeedConfidence(newAccelConf);

            double deltaAngle = curHeadingAngle
                    + genLib.RandomDouble(-curHeadingAngle / 360,
                            +curHeadingAngle / 360);
            deltaAngle = std::fmod(deltaAngle, 360);
            deltaAngle = 360 - deltaAngle;
            Coord newHeading = Coord(cos(deltaAngle * PI / 180),
                    sin(deltaAngle * PI / 180), 0);

            double HeadingConfDelta = genLib.RandomDouble(
                    -0.1 * attackBsm.getSenderHeadingConfidence().x,
                    +0.1 * attackBsm.getSenderHeadingConfidence().x);
            if (HeadingConfDelta < 0) {
                HeadingConfDelta = 0;
            }
            Coord newHeadingConf = Coord(
                    attackBsm.getSenderHeadingConfidence().x + HeadingConfDelta,
                    attackBsm.getSenderHeadingConfidence().y + HeadingConfDelta,
                    attackBsm.getSenderHeadingConfidence().z);

            attackBsm.setSenderHeading(newHeading);
            attackBsm.setSenderHeadingConfidence(newHeadingConf);
        }
    } else {

        double sybDistXrand = genLib.RandomDouble(-(localSybilDistanceX / 10.0),
                (localSybilDistanceX / 10.0));
        double sybDistYrand = genLib.RandomDouble(-(localSybilDistanceY / 10.0),
                (localSybilDistanceY / 10.0));

        double XOffset = -SquareX * (*myLength + localSybilDistanceX)
                + sybDistXrand;
        double YOffset = -SquareY * (*myWidth + localSybilDistanceY)
                + sybDistYrand;
        MDMLib mdmLib = MDMLib();
        double curHeadingAngle = mdmLib.calculateHeadingAnglePtr(curHeading);
        Coord relativePos = Coord(XOffset, YOffset, 0);
        double DeltaAngle = mdmLib.calculateHeadingAnglePtr(&relativePos);
        double newAngle = curHeadingAngle + DeltaAngle;

        newAngle = std::fmod(newAngle, 360);

        newAngle = 360 - newAngle;

        double DOffset = sqrt((pow(XOffset, 2)) + (pow(YOffset, 2)));

        double relativeX = DOffset * cos(newAngle * PI / 180);
        double relativeY = DOffset * sin(newAngle * PI / 180);

        attackBsm = myBsm[0];

        Coord newSenderPos = Coord((*curPosition).x + relativeX,
                (*curPosition).y + relativeY, (*curPosition).z);

        double PosConfDelta = genLib.RandomDouble(
                -0.1 * curPositionConfidence->x,
                +0.1 * curPositionConfidence->x);
        if (PosConfDelta < 0) {
            PosConfDelta = 0;
        }
        Coord newPosConf = Coord(curPositionConfidence->x + PosConfDelta,
                curPositionConfidence->y + PosConfDelta,
                curPositionConfidence->z);

        attackBsm.setSenderPos(newSenderPos);
        attackBsm.setSenderPosConfidence(newPosConf);

        double randSpeedX = genLib.RandomDouble(-0.05 * curSpeed->x,
                +0.05 * curSpeed->x);
        double randSpeedY = genLib.RandomDouble(-0.05 * curSpeed->x,
                +0.05 * curSpeed->x);
        Coord newSpeed = Coord(curSpeed->x + randSpeedX,
                curSpeed->y + randSpeedY, curSpeed->z);

        double SpeedConfDelta = genLib.RandomDouble(
                -0.1 * curSpeedConfidence->x, +0.1 * curSpeedConfidence->x);
        if (SpeedConfDelta < 0) {
            SpeedConfDelta = 0;
        }
        Coord newSpeedConf = Coord(curSpeedConfidence->x + SpeedConfDelta,
                curSpeedConfidence->y + SpeedConfDelta, curSpeedConfidence->z);

        attackBsm.setSenderSpeed(newSpeed);
        attackBsm.setSenderSpeedConfidence(newSpeedConf);

        double randAccelx = genLib.RandomDouble(-0.05 * curAccel->x,
                +0.05 * curAccel->x);
        double randAccely = genLib.RandomDouble(-0.05 * curAccel->x,
                +0.05 * curAccel->x);
        Coord newAccel = Coord(curAccel->x + randAccelx,
                curAccel->y + randAccely, curAccel->z);

        double AccelConfDelta = genLib.RandomDouble(
                -0.1 * curAccelConfidence->x, +0.1 * curAccelConfidence->x);
        if (AccelConfDelta < 0) {
            AccelConfDelta = 0;
        }
        Coord newAccelConf = Coord(curAccelConfidence->x + AccelConfDelta,
                curAccelConfidence->y + AccelConfDelta, curAccelConfidence->z);

        attackBsm.setSenderAccel(newAccel);
        attackBsm.setSenderAccelConfidence(newAccelConf);

        double deltaAngle = curHeadingAngle
                + genLib.RandomDouble(-curHeadingAngle / 360,
                        +curHeadingAngle / 360);
        deltaAngle = std::fmod(deltaAngle, 360);
        deltaAngle = 360 - deltaAngle;
        Coord newHeading = Coord(cos(deltaAngle * PI / 180),
                sin(deltaAngle * PI / 180), 0);

        double HeadingConfDelta = genLib.RandomDouble(
                -0.1 * curHeadingConfidence->x, +0.1 * curHeadingConfidence->x);
        if (HeadingConfDelta < 0) {
            HeadingConfDelta = 0;
        }
        Coord newHeadingConf = Coord(curHeadingConfidence->x + HeadingConfDelta,
                curHeadingConfidence->y + HeadingConfDelta,
                curHeadingConfidence->z);

        attackBsm.setSenderHeading(newHeading);
        attackBsm.setSenderHeadingConfidence(newHeadingConf);

        attackBsm.setSenderWidth(*myWidth);
        attackBsm.setSenderLength(*myLength);

        double stearingAngle = std::fmod((saveHeading - curHeadingAngle), 360);
        if (stearingAngle > 180) {
            stearingAngle = 360 - stearingAngle;
        }
        if (stearingAngle > 5 && SybilVehSeq > 0) {
            attackBsm.setSenderPseudonym(1);
        }
        saveHeading = curHeadingAngle;

        double mapdistance = LinkC->calculateDistance(newSenderPos, 50, 50);
        if (mapdistance > params->MAX_DISTANCE_FROM_ROUTE) {
            attackBsm.setSenderPseudonym(1);
        }
        //             std::cout<<"SybilVehSeq:"<<SybilVehSeq<<"\n";
        //             std::cout<<"SquareX:"<<SquareX<<"\n";
        //             std::cout<<"SquareY:"<<SquareY<<"\n";
        //             std::cout<<"XOffset:"<<XOffset<<"\n";
        //             std::cout<<"YOffset:"<<YOffset<<"\n";
    }

    if (attackBsm.getSenderPseudonym() > 1) {
        if (SybilVehSeq > 0) {
            attackBsm.setSenderPseudonym(SybilPseudonyms[SybilVehSeq - 1]);
        } else {
            attackBsm.setSenderPseudonym(0);
        }
    }

    if (SybilVehSeq < localSybilVehNumber) {
        SybilVehSeq++;
    } else {
        SybilVehSeq = 0;
    }

    return attackBsm;
}


BasicSafetyMessage F2MDDoSDisruptiveSybil::launchAttack(LinkControl *LinkC) {

    BasicSafetyMessage attackBsm = BasicSafetyMessage();

    if (!DoSInitiated) {
        beaconInterval->setRaw(
                beaconInterval->raw() / localDosMultipleFreqSybil);
        DoSInitiated = true;
    }

    if (detectedNodes->getNodesNum() > 0) {
        targetNode = attackBsm.getSenderPseudonym();
        attackBsm = *detectedNodes->getRandomBSM();
        attackBsm.setSenderPseudonym(pcPolicy->getNextPseudonym());
    }

    return attackBsm;
}


BasicSafetyMessage F2MDDataReplaySybil::launchAttack(LinkControl *LinkC) {

    BasicSafetyMessage attackBsm = BasicSafetyMessage();

    if (detectedNodes->getNodesNum() > 0) {
        if (ReplaySeq < localReplaySeqNum) {
            attackBsm = *detectedNodes->getNextAttackedBsm(*curPosition,
                    saveAttackBsm.getSenderPseudonym(),
                    saveAttackBsm.getArrivalTime().dbl());
            ReplaySeq++;
        } else {
            attackBsm = *detectedNodes->getRandomBSM();
            ReplaySeq = 0;
        }

        if (saveAttackBsm.getSenderPseudonym()
                != attackBsm.getSenderPseudonym()) {
            SybilPseudonyms[0] = pcPolicy->getNextPseudonym();
        }

        saveAttackBsm = attackBsm;
        targetNode = attackBsm.getSenderPseudonym();
        attackBsm.setSenderPseudonym(SybilPseudonyms[0]);
    } else {
        attackBsm.setSenderPseudonym(0);
    }

    return attackBsm;
}


BasicSafetyMessage F2MDDoSRandomSybil::launchAttack(LinkControl *LinkC) {

    BasicSafetyMessage attackBsm = BasicSafetyMessage();

    if (!DoSInitiated) {
        beaconInterval->setRaw(
                beaconInterval->raw() / localDosMultipleFreqSybil);
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

    return attackBsm;
}

