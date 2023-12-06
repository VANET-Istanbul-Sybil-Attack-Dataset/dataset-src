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
#include <veins/modules/analogueModel/ShadowingModel.h>

using namespace veins;

using veins::AirFrame;

static double Friis(double Pt, double Gt, double Gr, double lambda, double L, double d) {
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

// https://github.com/VeReMi-dataset/veins/blob/securecomm2018/src/veins/modules/analogueModel/LogNormalShadowing.cc

// https://www.isi.edu/nsnam/ns/doc/node220.html
double ShadowingModel::ns2_shadowing(Signal* signal) {

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
        // s += " " + std::to_string(wavelength);
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
    // double Pr0 = Friis(txPower_dBm, Gt, Gr, lambda, L, dist0);
    double Pr0 = Friis(txPower_mW, Gt, Gr, lambda, L, dist0);

    // calculate average power loss predicted by path loss model
    double avg_db;
    if (dist > dist0) {
        avg_db = -10.0 * pathlossExp * log10(dist / dist0);
    } else {
        avg_db = 0.0;
    }

    // get power loss by adding a log-normal random variable (shadowing)
    // the power loss is relative to that at reference distance dist0_
    // double powerLoss_db = avg_db + ranVar->normal(0.0, std_db_);
    double powerLoss_db = avg_db + RNGCONTEXT normal(0, std_db);

    // calculate the receiving power at dist
    // double Pr = Pr0 * pow(10.0, powerLoss_db / 10.0);
    double Pr = Pr0 * FWMath::dBm2mW(powerLoss_db);

    return Pr;
}

ShadowingModel::ShadowingModel(cComponent* owner, bool useTorus, const Coord& playgroundSize, double dist0, double std_db, double pathlossExp)
    : AnalogueModel(owner),
    useTorus(useTorus),
    playgroundSize(playgroundSize),
    dist0(dist0),
    std_db(std_db),
    pathlossExp(pathlossExp)
{

    EV << "[ShadowingModel] useTorus: " << std::to_string(useTorus) << endl;
    EV << "[ShadowingModel] std_db: " << std::to_string(std_db) << endl;
    EV << "[ShadowingModel] pathlossExp: " << std::to_string(pathlossExp) << endl;

    if (useTorus)
        EV_ERROR << "[ShadowingModel] Not implemented: useTorus" << endl;
}

void ShadowingModel::filterSignal(Signal* signal)
{
    double txPower_mW = signal->getMax();
    //double rxPower_db = ns2_shadowing(signal);
    //double rxPower_mW = FWMath::dBm2mW(rxPower_db);
    double rxPower_mW = ns2_shadowing(signal);
    double factor = rxPower_mW / txPower_mW;
    *signal *= factor;
}
