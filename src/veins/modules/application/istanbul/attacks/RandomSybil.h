#ifndef __RANDOM_SYBIL_H__
#define __RANDOM_SYBIL_H__

#include <veins/modules/application/f2md/mdAttacks/MDAttackBase.h>
#include "PowerControlAttack.h"


class RandomSybil: virtual public MDAttackBase, public PowerControlAttackBase {
private:
    GeneralLib genLib = GeneralLib();

    F2MDParameters *params;
    double MaxRandomPosX;
    double MaxRandomPosY;

    bool initCompleted = false;
    int sybilNodeCount = 0;
    int seq = 0;
    unsigned long pseudonyms[MAX_SYBIL_NUM];
    BasicSafetyMessage lastBSM[MAX_SYBIL_NUM];
    double times[MAX_SYBIL_NUM];

public:

    RandomSybil();
    virtual ~RandomSybil();

    void init(attackTypes::Attacks myAttackType, double MaxRandomPosX,
            double MaxRandomPosY, F2MDParameters *params);
    BasicSafetyMessage launchAttack(LinkControl *LinkC);

};

#endif /* __RANDOM_SYBIL_H__ */
