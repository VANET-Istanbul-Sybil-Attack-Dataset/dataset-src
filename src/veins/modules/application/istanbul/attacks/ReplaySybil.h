#ifndef __REPLAY_SYBIL_H__
#define __REPLAY_SYBIL_H__

#include <veins/modules/application/f2md/mdAttacks/MDAttackBase.h>
#include "PowerControlAttack.h"


class ReplaySybil: virtual public MDAttackBase, public PowerControlAttackBase {
private:
    GeneralLib genLib = GeneralLib();

    bool initCompleted = false;
    int sybilNodeCount = 0;
    int seq = 0;
    unsigned long pseudonyms[MAX_SYBIL_NUM];
    unsigned long targets[MAX_SYBIL_NUM];
    BasicSafetyMessage lastBSM[MAX_SYBIL_NUM];
    double times[MAX_SYBIL_NUM];
    bool targetsIncludes(unsigned long x);

public:

    ReplaySybil();
    virtual ~ReplaySybil();

    void init();
    BasicSafetyMessage launchAttack(LinkControl *LinkC);

};

#endif /* __REPLAY_SYBIL_H__ */
