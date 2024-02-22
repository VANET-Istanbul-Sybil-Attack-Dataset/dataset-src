#ifndef __POWER_CONTROL_ATTACK_H__
#define __POWER_CONTROL_ATTACK_H__

#include <veins/modules/messages/BasicSafetyMessage_m.h>
#include <veins/modules/application/f2md/mdSupport/GeneralLib.h>
#include <veins/modules/application/f2md/mdAttacks/MDAttackBase.h>

// #include "F2MDAttacks.h"

using namespace veins;

enum PowerControlType {
    PowerControlType_Disabled = 0,
    PowerControlType_Random = 1,
    PowerControlType_Rectangular = 2,
    PowerControlType_Stairs = 3,
    PowerControlType_SawtoothDiscrete = 4,
    PowerControlType_SawtoothContinuous = 5,
};

class IPowerControlAttack {
public:
    virtual void apply(BasicSafetyMessage& bsm) = 0;
};

class PowerControlAttackBase {
private:
    GeneralLib genLib = GeneralLib();

protected:
    IPowerControlAttack* powerControl[MAX_SYBIL_NUM];
    int powerControlType;
    char log[128];
public:
    void init(int sybilNodeCount);
    virtual int getPowerControlType() {
        return powerControlType;
    }
};


class PowerControlAttackRandom: public IPowerControlAttack {
    GeneralLib genLib = GeneralLib();
    double min;
    double max;
    double step;
public:
    void init(double min, double max, double step);
    void apply(BasicSafetyMessage& bsm);
};

class PowerControlAttackRepeatedPattern: public IPowerControlAttack {
protected:
    int stateCount;
    const double* pattern;
    double low;
    double high;

    int pulseWidth;
    int counter;

protected:
    void init(const double* pattern, int stateCount, double low, double high, int pulseWidth, int shift);
public:
    virtual void apply(BasicSafetyMessage& bsm);
};


class PowerControlAttackRectangular: public PowerControlAttackRepeatedPattern {
public:
    void init(double low, double high, int pulseWidth, int shift);
};


class PowerControlAttackStairs: public PowerControlAttackRepeatedPattern {
public:
    void init(int type, double low, double high, int pulseWidth, int shift);
};

class PowerControlAttackSawtoothDiscrete: public PowerControlAttackRepeatedPattern {
public:
    void init(int type, double low, double high, int pulseWidth, int shift);
};

class PowerControlAttackSawtoothContinuous: public IPowerControlAttack {
protected:
    bool reverse;
    int stepCount;
    double low;
    double high;
    int counter;
public:
    void init(bool reverse, int stepCount, double low, double high, int shift);
    virtual void apply(BasicSafetyMessage& bsm);
};

#endif
