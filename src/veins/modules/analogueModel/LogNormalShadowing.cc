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

#include "veins/modules/analogueModel/LogNormalShadowing.h"

#include "veins/base/messages/AirFrame_m.h"

using namespace veins;

using veins::AirFrame;

LogNormalShadowing::LogNormalShadowing(cComponent* owner, bool useTorus, const Coord& playgroundSize, double mean, double stdDev)
    : AnalogueModel(owner),
    useTorus(useTorus),
    playgroundSize(playgroundSize),
    mean(mean),
    stdDev(stdDev)
{

//    EV << "[LogNormalShadowing] useTorus: " << std::to_string(useTorus) << endl;
//    EV << "[LogNormalShadowing] std_db: " << std::to_string(std_db) << endl;
//    EV << "[LogNormalShadowing] pathlossExp: " << std::to_string(pathlossExp) << endl;

//    if (useTorus)
//        EV_ERROR << "[LogNormalShadowing] Not implemented: useTorus" << endl;
}

void LogNormalShadowing::filterSignal(Signal* signal)
{
    auto senderPos = signal->getSenderPoa().pos.getPositionAt();
    auto receiverPos = signal->getReceiverPoa().pos.getPositionAt();

    /** Calculate the distance factor */
    double sqrDistance = useTorus ? receiverPos.sqrTorusDist(senderPos, playgroundSize) : receiverPos.sqrdist(senderPos);

    if (sqrDistance <= 1.0) {
        // attenuation is negligible
        return;
    }

    *signal *= FWMath::dBm2mW(-1.0 * (RNGCONTEXT normal(mean, stdDev)));
}
