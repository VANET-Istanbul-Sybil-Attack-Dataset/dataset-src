#ifndef __VEINS_IstanbulDsAttack_H_
#define __VEINS_IstanbulDsAttack_H_

#include <veins/modules/application/f2md/mdAttacks/MDAttackBase.h>
// #include "attacks/F2MDAttacks.h"
// #include "attacks/RandomSybilRandomPCAttack.h"
#include "attacks/GridSybil.h"
#include "attacks/ReplaySybil.h"
#include "attacks/RandomSybil.h"
#include "attacks/SimilarRandomSybil.h"

class IstanbulDsAttack:
//        public F2MDGridSybil,
//        public F2MDDoSRandomSybil,
//        public F2MDDoSDisruptiveSybil,
//        public F2MDDataReplaySybil,
//        public RandomSybilRandomPCAttack,
        public ReplaySybil,
        public RandomSybil,
        public SimilarRandomSybil,
        public GridSybil
{

public:
    IstanbulDsAttack();
    void init(attackTypes::Attacks myAttackType, double MaxRandomPosX,double MaxRandomPosY,F2MDParameters *params);
    BasicSafetyMessage launchAttack(attackTypes::Attacks myAttackType, LinkControl* LinkC);
    int getPowerControlType(attackTypes::Attacks myAttackType);
};

#endif
