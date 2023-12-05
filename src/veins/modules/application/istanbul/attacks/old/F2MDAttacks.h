#ifndef __VEINS_F2MDAttacks_H_
#define __VEINS_F2MDAttacks_H_

#include <veins/modules/application/f2md/mdAttacks/MDAttackBase.h>

class F2MDGridSybil: virtual public MDAttackBase {
private:
    F2MDParameters *params;
    unsigned long targetNode = 0;
    bool DoSInitiated;
    BasicSafetyMessage saveAttackBsm = BasicSafetyMessage();
    unsigned long SybilPseudonyms[MAX_SYBIL_NUM];
    int SybilVehSeq = 0;
    unsigned long SybilMyOldPseudo = 0;
    double saveHeading = 0;
    int localSybilVehNumber = 0;
    double localSybilDistanceX = 0;
    double localSybilDistanceY = 0;
public:
    void init(attackTypes::Attacks myAttackType, double MaxRandomPosX,double MaxRandomPosY,F2MDParameters *params);
    BasicSafetyMessage launchAttack(LinkControl* LinkC);
};



class F2MDDoSDisruptiveSybil: virtual public MDAttackBase {
private:
    unsigned long targetNode = 0;
    bool DoSInitiated;
    int localDosMultipleFreq = 0;
    int localDosMultipleFreqSybil = 0;
public:
    void init(attackTypes::Attacks myAttackType, double MaxRandomPosX,double MaxRandomPosY,F2MDParameters *params);
    BasicSafetyMessage launchAttack(LinkControl* LinkC);
};


class F2MDDataReplaySybil: virtual public MDAttackBase {
private:
    unsigned long targetNode = 0;
    BasicSafetyMessage saveAttackBsm = BasicSafetyMessage();
    unsigned long SybilPseudonyms[MAX_SYBIL_NUM];
    int localReplaySeqNum = 0;
    int ReplaySeq = 0;
public:
    void init(attackTypes::Attacks myAttackType, double MaxRandomPosX,double MaxRandomPosY,F2MDParameters *params);
    BasicSafetyMessage launchAttack(LinkControl* LinkC);
};

class F2MDDoSRandomSybil: virtual public MDAttackBase {
private:
    F2MDParameters *params;
    unsigned long targetNode = 0;
    double MaxRandomPosX;
    double MaxRandomPosY;
    bool DoSInitiated;
    int localDosMultipleFreq = 0;
    int localDosMultipleFreqSybil = 0;
public:
    void init(attackTypes::Attacks myAttackType, double MaxRandomPosX,double MaxRandomPosY,F2MDParameters *params);
    BasicSafetyMessage launchAttack(LinkControl* LinkC);
};

#endif
