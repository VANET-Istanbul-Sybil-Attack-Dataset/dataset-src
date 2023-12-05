#include <veins/modules/application/f2md/mdAttacks/MDAttackBase.h>


void MDAttackBase::setBeaconInterval(simtime_t* beaconInterval) {
    this->beaconInterval = beaconInterval;
}

void MDAttackBase::setCurHeading(Coord* curHeading) {
    this->curHeading = curHeading;
}

void MDAttackBase::setCurHeadingConfidence(Coord* curHeadingConfidence) {
    this->curHeadingConfidence = curHeadingConfidence;
}

void MDAttackBase::setCurPosition(Coord* curPosition) {
    this->curPosition = curPosition;
}

void MDAttackBase::setCurPositionConfidence(Coord* curPositionConfidence) {
    this->curPositionConfidence = curPositionConfidence;
}

void MDAttackBase::setCurSpeed(Coord* curSpeed) {
    this->curSpeed = curSpeed;
}

void MDAttackBase::setCurSpeedConfidence(Coord* curSpeedConfidence) {
    this->curSpeedConfidence = curSpeedConfidence;
}

void MDAttackBase::setCurAccel(Coord* curAccel) {
    this->curAccel = curAccel;
}

void MDAttackBase::setCurAccelConfidence(Coord* curAccelConfidence) {
    this->curAccelConfidence = curAccelConfidence;
}

void MDAttackBase::setDetectedNodes(NodeTable* detectedNodes) {
    this->detectedNodes = detectedNodes;
}

void MDAttackBase::setMyBsm(BasicSafetyMessage* myBsm) {
    this->myBsm = myBsm;
}

void MDAttackBase::setMyBsmNum(int* myBsmNum) {
    this->myBsmNum = myBsmNum;
}

void MDAttackBase::setMyLength(double* myLength) {
    this->myLength = myLength;
}

void MDAttackBase::setMyPseudonym(unsigned long * myPseudonym) {
    this->myPseudonym = myPseudonym;
}

void MDAttackBase::setMyWidth(double* myWidth) {
    this->myWidth = myWidth;
}

void MDAttackBase::setPcPolicy(PCPolicy* pcPolicy) {
    this->pcPolicy = pcPolicy;
}

BasicSafetyMessage MDAttackBase::launchAttack(attackTypes::Attacks myAttackType, LinkControl* LinkC) {
    cout << "MDAttackBase::launchAttack" << endl;
    throw std::runtime_error("MDAttackBase::launchAttack");
}
