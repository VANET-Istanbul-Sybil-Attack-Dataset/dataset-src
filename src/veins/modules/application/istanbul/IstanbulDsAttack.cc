#include <veins/modules/application/istanbul/IstanbulDsAttack.h>
#include "IstanbulDsParams.h"

IstanbulDsAttack::IstanbulDsAttack() {

}

void IstanbulDsAttack::init(attackTypes::Attacks myAttackType, double MaxRandomPosX, double MaxRandomPosY, F2MDParameters *params) {
    switch (myAttackType) {
//    case attackTypes::GridSybil:
//        return F2MDGridSybil::init(myAttackType, MaxRandomPosX, MaxRandomPosY, params);
//    case attackTypes::DoSRandomSybil:
//        return F2MDDoSRandomSybil::init(myAttackType, MaxRandomPosX, MaxRandomPosY, params);
//    case attackTypes::DoSDisruptiveSybil:
//        return F2MDDoSDisruptiveSybil::init(myAttackType, MaxRandomPosX, MaxRandomPosY, params);
//    case attackTypes::DataReplaySybil:
//        return F2MDDataReplaySybil::init(myAttackType, MaxRandomPosX, MaxRandomPosY, params);
//    case attackTypes::_RandomSybilRandomPC:
//        return RandomSybilRandomPCAttack::init(myAttackType, MaxRandomPosX, MaxRandomPosY, params);
    case attackTypes::_ReplaySybil:
        return ReplaySybil::init();
    case attackTypes::_RandomSybil:
        return RandomSybil::init(myAttackType, MaxRandomPosX, MaxRandomPosY, params);
    case attackTypes::_SimilarRandomSybil:
        return SimilarRandomSybil::init();
    case attackTypes::_GridSybil:
        return GridSybil::init();
    default:
        cout << myAttackType << endl;
        getSimulation()->getActiveEnvir()->alert("IstanbulDsAttack::init undefined type");
        exit(1);
    }
}


BasicSafetyMessage IstanbulDsAttack::launchAttack(attackTypes::Attacks myAttackType, LinkControl *LinkC) {
    // cout << "IstanbulDsAttack::launchAttack " << myAttackType << endl;

    switch (myAttackType) {
//    case attackTypes::GridSybil:
//        return F2MDGridSybil::launchAttack(LinkC);
//    case attackTypes::DoSRandomSybil:
//        return F2MDDoSRandomSybil::launchAttack(LinkC);
//    case attackTypes::DoSDisruptiveSybil:
//        return F2MDDoSDisruptiveSybil::launchAttack(LinkC);
//    case attackTypes::DataReplaySybil:
//        return F2MDDataReplaySybil::launchAttack(LinkC);
//    case attackTypes::_RandomSybilRandomPC:
//        return RandomSybilRandomPCAttack::launchAttack(LinkC);
    case attackTypes::_ReplaySybil:
        return ReplaySybil::launchAttack(LinkC);
    case attackTypes::_RandomSybil:
        return RandomSybil::launchAttack(LinkC);
    case attackTypes::_SimilarRandomSybil:
        return SimilarRandomSybil::launchAttack(LinkC);
    case attackTypes::_GridSybil:
        return GridSybil::launchAttack(LinkC);
    default:
        getSimulation()->getActiveEnvir()->alert("IstanbulDsAttack::launchAttack undefined type");
        exit(1);
    }
    return BasicSafetyMessage();
}

int IstanbulDsAttack::getPowerControlType(attackTypes::Attacks myAttackType) {
    switch (myAttackType) {
    case attackTypes::Genuine:
        return 0;
    case attackTypes::_ReplaySybil:
        return ReplaySybil::getPowerControlType();
    case attackTypes::_RandomSybil:
        return RandomSybil::getPowerControlType();
    case attackTypes::_SimilarRandomSybil:
        return SimilarRandomSybil::getPowerControlType();
    case attackTypes::_GridSybil:
        return GridSybil::getPowerControlType();
    default:
        getSimulation()->getActiveEnvir()->alert("IstanbulDsAttack::getPowerControlType undefined type");
        exit(1);
    }
}

