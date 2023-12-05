#ifndef __VEINS_ISTANBULVEINSAPP_H_
#define __VEINS_ISTANBULVEINSAPP_H_

#include <omnetpp.h>
#include <veins/modules/application/istanbul/BsmLogger.h>
#include "veins/veins.h"
#include <veins/modules/messages/BasicSafetyMessage_m.h>
#include "veins/modules/application/f2md/F2MDVeinsApp.h"
#include "IstanbulDsAttack.h"

using namespace omnetpp;
using namespace veins;

class IstanbulVeinsApp : public JosephVeinsApp {
protected:
    IstanbulDsAttack attackData;

    BsmLogger logger;

    virtual void localAttackerInit();
    virtual void onBSM(BasicSafetyMessage* bsm);
    virtual void onWSM(BaseFrame1609_4* wsm);
    virtual void onWSA(DemoServiceAdvertisment* wsa);
    virtual void handleSelfMsg(cMessage* msg);
    virtual void populateWSM(BaseFrame1609_4* wsm, LAddress::L2Type rcvId = LAddress::L2BROADCAST(), int serial = 0);
    virtual void handlePositionUpdate(cObject* obj);

public:

    inline IstanbulVeinsApp() {
        mdAttack = &attackData;
    }

    virtual void initialize(int stage) override;
};

#endif
