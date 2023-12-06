#include "PowerControlAttack.h"
#include <veins/modules/application/istanbul/IstanbulDsParams.h>

static const double pattern_rectangular[] = { 0.0, 1.0 };
static const double pattern_stair_4[] = { 0.5, 1.0, 0.5, 0.0 };
static const double pattern_stair_8[] = { 0.5, 0.75, 1.0, 0.75, 0.5, 0.25, 0.0, 0.25 };
static const double pattern_sawtooth_discrete1_9[] = { 0.0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1.0 };
static const double pattern_sawtooth_discrete2_9[] = { 1.0, 0.875, 0.75, 0.625, 0.5, 0.375, 0.25, 0.125, 0.0 };


void PowerControlAttackBase::init(int sybilNodeCount) {

    if (genLib.RandomDouble(0, 1.0) < istanbulDsParams.probPowerControl)
        powerControlType = genLib.RandomInt(1, 5);
    else
        powerControlType = 0;

    cout << "powerControlType: " << powerControlType << endl;

    if (powerControlType == PowerControlType_Disabled) {
        for (int i = 0; i < sybilNodeCount; i++)
            powerControl[i] = NULL;

    } else if (powerControlType == PowerControlType_Random) {
        for (int i = 0; i < sybilNodeCount; i++) {
            auto *pc = new PowerControlAttackRandom();
            pc->init(17, 23, 0.5);
            powerControl[i] = pc;
        }

    } else if (powerControlType == PowerControlType_Rectangular) {
        int pulseWidth = 10 + 10 * genLib.RandomInt(0, 4);  // [10, 20, 30, 40, 50]
        double low = 17.0;
        double high = 23.0;
        for (int i = 0; i < sybilNodeCount; i++) {
            auto *pc = new PowerControlAttackRectangular();
            int shift = i * pulseWidth * 2 / sybilNodeCount;
            pc->init(low, high, pulseWidth, shift);
            powerControl[i] = pc;
        }

    } else if (powerControlType == PowerControlType_Stairs) {
        int pulseWidth = 10 + 10 * genLib.RandomInt(0, 2); // [10, 20, 30]
        int type = genLib.RandomInt(0, 1);
        double low = 15.0;
        double high = 25.0;
        for (int i = 0; i < sybilNodeCount; i++) {
            auto *pc = new PowerControlAttackStairs();
            int shift = i * pulseWidth * 2 / sybilNodeCount;
            pc->init(type, low, high, pulseWidth, shift);
            powerControl[i] = pc;
        }

    } else if (powerControlType == PowerControlType_SawtoothDiscrete) {
        int pulseWidth = 10 + 10 * genLib.RandomInt(0, 2); // [10, 20, 30]
        int type = genLib.RandomInt(0, 1);
        double low = 15.0;
        double high = 25.0;
        for (int i = 0; i < sybilNodeCount; i++) {
            auto *pc = new PowerControlAttackSawtoothDiscrete();
            int shift = i * pulseWidth * 2 / sybilNodeCount;
            pc->init(type, low, high, pulseWidth, shift);
            powerControl[i] = pc;
        }

    } else if (powerControlType == PowerControlType_SawtoothContinuous) {
        int stepCount = 10 + genLib.RandomDouble(0, 40);
        bool reverse = (genLib.RandomDouble(0, 1) > 0.5);
        double low = 15.0;
        double high = 25.0;
        for (int i = 0; i < sybilNodeCount; i++) {
            auto *pc = new PowerControlAttackSawtoothContinuous();
            int shift = i * stepCount / sybilNodeCount;
            pc->init(reverse, stepCount, low, high, shift);
            powerControl[i] = pc;
        }

    } else {
        getSimulation()->getActiveEnvir()->alert("ReplaySybil::init unknown power control method");
        exit(1);
    }

}

void PowerControlAttackRandom::init(double min, double max, double step) {
    this->min = min;
    this->max = max;
    this->step = step;
}

void PowerControlAttackRandom::apply(BasicSafetyMessage &bsm) {
    double value = min + step * genLib.RandomInt(0, round((max-min)/step));
    bsm.setPowerControlTxPower(value);
}


void PowerControlAttackRepeatedPattern::init(const double* pattern, int stateCount, double low, double high, int pulseWidth, int shift) {
    this->pattern = pattern;
    this->stateCount = stateCount;
    this->low = low;
    this->high = high;
    this->pulseWidth = pulseWidth;
    this->counter = shift % (pulseWidth * 2);
}

void PowerControlAttackRepeatedPattern::apply(BasicSafetyMessage &bsm) {
    if (++counter > pulseWidth * stateCount)
        counter = 0;
    double value = low + (high - low) * pattern[counter / pulseWidth];
    bsm.setPowerControlTxPower(value);
}

void PowerControlAttackRectangular::init(double low, double high, int pulseWidth, int shift) {
    PowerControlAttackRepeatedPattern::init(pattern_rectangular, 2, low, high, pulseWidth, shift);
}


void PowerControlAttackStairs::init(int type, double low, double high, int pulseWidth, int shift) {
    if (type == 0)
        PowerControlAttackRepeatedPattern::init(pattern_stair_4, 4, low, high, pulseWidth, shift);
    else // if (type == 1)
        PowerControlAttackRepeatedPattern::init(pattern_stair_8, 8, low, high, pulseWidth, shift);
}

void PowerControlAttackSawtoothDiscrete::init(int type, double low, double high, int pulseWidth, int shift) {
    if (type == 0)
        PowerControlAttackRepeatedPattern::init(pattern_sawtooth_discrete1_9, 9, low, high, pulseWidth, shift);
    else // if (type == 1)
        PowerControlAttackRepeatedPattern::init(pattern_sawtooth_discrete2_9, 9, low, high, pulseWidth, shift);
}


void PowerControlAttackSawtoothContinuous::init(bool reverse, int stepCount, double low, double high, int shift) {
    this->reverse = reverse;
    this->stepCount = stepCount;
    this->low = low;
    this->high = high;
    this->counter = shift % stepCount;
}

void PowerControlAttackSawtoothContinuous::apply(BasicSafetyMessage &bsm) {
    if (++counter > stepCount)
        counter = 0;
    double x = (double)counter / stepCount;
    if (reverse)
        x = 1 - x;
    double value = low + (high - low) * x;
    bsm.setPowerControlTxPower(value);
}
