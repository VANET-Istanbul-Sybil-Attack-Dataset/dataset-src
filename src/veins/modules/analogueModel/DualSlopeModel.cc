//
// Copyright (C) 2007 Technische Universitaet Berlin (TUB), Germany, Telecommunication Networks Group
// Copyright (C) 2007 Technische Universiteit Delft (TUD), Netherlands
// Copyright (C) 2007 Universitaet Paderborn (UPB), Germany
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "veins/base/messages/AirFrame_m.h"

#include <math.h>
#include <veins/modules/analogueModel/DualSlopeModel.h>

using namespace veins;

using veins::AirFrame;

double Friis(double Pt, double Gt, double Gr, double lambda, double L, double d) {
    /*
     * Friis free space equation:
     *
     *       Pt * Gt * Gr * (lambda^2)
     *   P = --------------------------
     *       (4 * pi * d)^2 * L
     */
    if (d == 0.0) //XXX probably better check < MIN_DISTANCE or some such
        return Pt;
    double M = lambda / (4 * PI * d);
    return (Pt * Gt * Gr * (M * M)) / L;
}

// https://www.isi.edu/nsnam/ns/doc/node220.html
double DualSlopeModel::ns2_shadowing_with_dualslope(Signal* signal) {
    auto senderPos = signal->getSenderPoa().pos.getPositionAt();
    auto receiverPos = signal->getReceiverPoa().pos.getPositionAt();
    //double sqrDistance = useTorus ? receiverPos.sqrTorusDist(senderPos, playgroundSize) : receiverPos.sqrdist(senderPos);

    // loc of transmitter
    double Xt = senderPos.x;
    double Yt = senderPos.y;
    double Zt = senderPos.z;
    // loc of receiver
    double Xr = receiverPos.x;
    double Yr = receiverPos.y;
    double Zr = receiverPos.z;


    // double L = ifp->getL();         // system loss
    double L = 1;  // https://www.isi.edu/nsnam/ns/doc/node217.html

    std::string s = "wavelength: ";
    // double lambda = ifp->getLambda();   // wavelength
    double lambda;
    for (uint16_t i = 0; i < signal->getNumValues(); i++) {
        double wavelength = BaseWorldUtility::speedOfLight() / signal->getSpectrum().freqAt(i);
        lambda = wavelength;
        s += " " + std::to_string(wavelength);
    }
    // EV << s << endl;

    double dX = Xr - Xt;
    double dY = Yr - Yt;
    double dZ = Zr - Zt;
    double dist = sqrt(dX * dX + dY * dY + dZ * dZ);

    // get antenna gain
    // double Gt = t->getAntenna()->getTxGain(dX, dY, dZ, lambda);
    // double Gr = r->getAntenna()->getRxGain(dX, dY, dZ, lambda);
    double Gt = 1; // https://www.isi.edu/nsnam/ns/doc/node217.html
    double Gr = 1; // https://www.isi.edu/nsnam/ns/doc/node217.html

    double txPower_mW = signal->getMax();
    double txPower_dBm = FWMath::mW2dBm(txPower_mW);

    // calculate receiving power at reference distance
    // double Pr0 = Friis(t->getTxPr(), Gt, Gr, lambda, L, dist0_);
    double Pr0 = Friis(txPower_dBm, Gt, Gr, lambda, L, dist0);

    // calculate average power loss predicted by path loss model
    double powerLoss_db;

    if (dist < dist0) {
        powerLoss_db = 0.0;
    } else if (dist < dc) {
        // r = -10 * 2.56 * log10(distance / d0) + RNGCONTEXT normal(0, 3.9);
        double avg_db = -10.0 * pathlossExp1 * log10(dist / dist0);
        powerLoss_db = avg_db + RNGCONTEXT normal(0, std1_db);
    } else {
        // r = -10 * 2.56 * log10(dc / d0) + RNGCONTEXT normal(0, 3.9) -10 * 6.34 * log10(distance / dc) + RNGCONTEXT normal(0, 5.2);
        double avg_db = -10.0 * pathlossExp1 * log10(dc / dist0) - 10.0 * pathlossExp2 * log10(dist / dc);
        powerLoss_db = avg_db + RNGCONTEXT normal(0, std2_db);
    }

    // get power loss by adding a log-normal random variable (shadowing)
    // the power loss is relative to that at reference distance dist0_
    // double powerLoss_db = avg_db + ranVar->normal(0.0, std_db_);


    // calculate the receiving power at dist
    double Pr = Pr0 * pow(10.0, powerLoss_db / 10.0);

    return Pr;
}

DualSlopeModel::DualSlopeModel(cComponent* owner, bool useTorus, const Coord& playgroundSize,
        double dist0, double std1_db, double std2_db, double pathlossExp1, double pathlossExp2, double dc)
    : AnalogueModel(owner),
      useTorus(useTorus),
      playgroundSize(playgroundSize),
      dist0(dist0),
      std1_db(std1_db),
      std2_db(std2_db),
      pathlossExp1(pathlossExp1),
      pathlossExp2(pathlossExp2),
      dc(dc)
{
    EV << "[DualSlopeModel] useTorus: " << std::to_string(useTorus) << endl;
    EV << "[DualSlopeModel] std1_db: " << std::to_string(std1_db) << endl;
    EV << "[DualSlopeModel] std2_db: " << std::to_string(std2_db) << endl;
    EV << "[DualSlopeModel] pathlossExp1: " << std::to_string(pathlossExp1) << endl;
    EV << "[DualSlopeModel] pathlossExp2: " << std::to_string(pathlossExp2) << endl;
    EV << "[DualSlopeModel] dc: " << std::to_string(dc) << endl;

    if (useTorus)
        EV_ERROR << "[DualSlopeModel] Not implemented: useTorus" << endl;
}


void DualSlopeModel::filterSignal(Signal* signal)
{
    double txPower_mW = signal->getMax();
    double rxPower_mW = ns2_shadowing_with_dualslope(signal);
    double factor = rxPower_mW / txPower_mW;
    *signal *= factor;
    return;

#if 0
    auto senderPos = signal->getSenderPoa().pos.getPositionAt();
    auto receiverPos = signal->getReceiverPoa().pos.getPositionAt();
    //double sqrDistance = useTorus ? receiverPos.sqrTorusDist(senderPos, playgroundSize) : receiverPos.sqrdist(senderPos);
    double distance = receiverPos.distance(senderPos);
    double sendPower_mW = signal->getMax();

    //double PL0_dBm = 47;   // FIXME: ???  https://www.pasternack.com/t-calculator-fspl.aspx
    double PL0_dBm = -27;   // FIXME: ???  https://www.pasternack.com/t-calculator-fspl.aspx

    double PL0 = FWMath::dBm2mW(PL0_dBm);

    double d0 = 1;
#if 0
    double recvPower_dBm;
    if (distance <= 1)
        recvPower_dBm = PL0_dBm;
    else
        recvPower_dBm = PL0_dBm - 10 * 2 * log10(distance / d0);
    recvPower_dBm += RNGCONTEXT normal(0, 3.5);
#else
    double dc = 102;
    double r;
    if (distance <= 1)
        r = RNGCONTEXT normal(0, 3.5);
    else if (distance <= dc)
        r = -10 * 2.56 * log10(distance / d0) + RNGCONTEXT normal(0, 3.9);
    else
        r = -10 * 2.56 * log10(dc / d0) + RNGCONTEXT normal(0, 3.9) -10 * 6.34 * log10(distance / dc) + RNGCONTEXT normal(0, 5.2);

    double recvPower_mW = PL0 * FWMath::dBm2mW(r);
#endif


//    EV << "TX power is " << FWMath::mW2dBm(sendPower_mW) << " dBm  " << sendPower_mW << endl;
//    EV << "RX power is " << FWMath::mW2dBm(recvPower_mW) << " dBm  " << recvPower_mW << endl;
//    EV << "factor is: " << factor << " (i.e. " << FWMath::mW2dBm(factor) << " dB)" << endl;

    double factor = recvPower_mW / sendPower_mW;

    *signal *= factor;
#endif
}
