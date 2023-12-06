#ifndef __SIMPLE_POWER_CONTROL_ATTACK__
#define __SIMPLE_POWER_CONTROL_ATTACK__

#include "F2MDAttacks.h"

class RandomSybilRandomPCAttack: virtual public MDAttackBase {
private:
    F2MDParameters *params;
    unsigned long targetNode = 0;
    double MaxRandomPosX;
    double MaxRandomPosY;
    bool DoSInitiated;
    // int localDosMultipleFreq = 0;
    int localDosMultipleFreqSybil = 0;

    // std::map<int, std::list<BasicSafetyMessage>> msgHistory;

public:
    void init(attackTypes::Attacks myAttackType, double MaxRandomPosX,double MaxRandomPosY,F2MDParameters *params);
    BasicSafetyMessage launchAttack(LinkControl* LinkC);
};

#endif /* __SIMPLE_POWER_CONTROL_ATTACK__ */
