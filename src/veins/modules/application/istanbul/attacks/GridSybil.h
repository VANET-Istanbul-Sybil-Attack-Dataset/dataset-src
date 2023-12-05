#ifndef __GRID_SYBIL_H__
#define __GRID_SYBIL_H__

#include <veins/modules/application/f2md/mdAttacks/MDAttackBase.h>
#include "PowerControlAttack.h"


class GridSybil: virtual public MDAttackBase, virtual public PowerControlAttackBase {
private:
    GeneralLib genLib = GeneralLib();

    bool initCompleted = false;
    int sybilNodeCount = 0;
    int seq = 0;
    unsigned long pseudonyms[MAX_SYBIL_NUM];
    BasicSafetyMessage lastBSM[MAX_SYBIL_NUM];
    double times[MAX_SYBIL_NUM];

public:

    GridSybil();
    virtual ~GridSybil();

    void init();
    BasicSafetyMessage launchAttack(LinkControl *LinkC);

};

#endif /* __GRID_SYBIL_H__ */
