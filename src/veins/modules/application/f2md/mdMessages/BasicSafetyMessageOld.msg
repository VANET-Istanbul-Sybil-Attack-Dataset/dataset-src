//
// Copyright (C) 2011 David Eckhoff <eckhoff@cs.fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
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

import veins.modules.messages.BaseFrame1609_4;
import veins.base.utils.Coord;
// import veins.base.utils.SimpleAddress;

namespace veins;

packet BasicSafetyMessageOld extends BaseFrame1609_4 {
    Coord senderPos;
    Coord senderPosConfidence;
    Coord senderSpeed;
    Coord senderSpeedConfidence;
    Coord senderHeading;
    Coord senderHeadingConfidence;
    Coord senderAccel;
    Coord senderAccelConfidence;
    double senderWidth;
    double senderLength;
    int senderMbType;
    int senderAttackType;
    Coord senderGpsCoordinates;
    LAddress::L2Type senderRealId;
    unsigned long senderPseudonym;
}

