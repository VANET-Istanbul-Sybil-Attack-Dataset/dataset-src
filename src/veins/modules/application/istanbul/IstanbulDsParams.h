#ifndef __ISTANBUL_DS_PARAMS_H_
#define __ISTANBUL_DS_PARAMS_H_

class IstanbulDsParams {
public:
    double probReplaySybil;
    double probRandomSybil;
    double probSimilarRandomSybil;
    double probGridSybil;

    double probPowerControl;
    int powerControlMinPulseWidth;
    int powerControlMaxPulseWidth;
    int selectedPowerControlType;
  
    int minSybilNodeCount;
    int maxSybilNodeCount;
    int selectedGridShape;
};

extern IstanbulDsParams istanbulDsParams;

#endif /* __ISTANBUL_DS_PARAMS_H_ */
