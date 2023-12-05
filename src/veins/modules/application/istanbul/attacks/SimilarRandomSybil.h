#ifndef __SIMILAR_RANDOM_SYBIL_H__
#define __SIMILAR_RANDOM_SYBIL_H__

#include <veins/modules/application/f2md/mdAttacks/MDAttackBase.h>
#include "PowerControlAttack.h"


class SimilarRandomSybil: virtual public MDAttackBase, public PowerControlAttackBase {
private:
    GeneralLib genLib = GeneralLib();

    bool initCompleted = false;
    int sybilNodeCount = 0;
    int seq = 0;
    unsigned long pseudonyms[MAX_SYBIL_NUM];
    BasicSafetyMessage lastBSM[MAX_SYBIL_NUM];
    IPowerControlAttack* powerControl[MAX_SYBIL_NUM];

    Coord randomCoord(const Coord &min, const Coord &max);

public:

    SimilarRandomSybil();
    virtual ~SimilarRandomSybil();

    void init();
    BasicSafetyMessage launchAttack(LinkControl *LinkC);

};

#endif /* __SIMILAR_RANDOM_SYBIL_H__ */
