/*******************************************************************************
 * @author  Joseph Kamel
 * @email   josephekamel@gmail.com
 * @date    28/11/2018
 * @version 2.0
 *
 * SCA (Secure Cooperative Autonomous systems)
 * Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
 * All rights reserved.
 *******************************************************************************/

#ifndef __VEINS_MDReport_H_
#define __VEINS_MDReport_H_

#include <tuple>
#include <omnetpp.h>
#include "veins/veins.h"
#include <time.h>
#include "veins/base/utils/SimpleAddress.h"
#include "veins/base/utils/Coord.h"

using namespace veins;
using namespace omnetpp;

#include "../mdBase/BsmCheck.h"
#include <sys/stat.h>
#include <veins/modules/application/f2md/mdSupport/JsonWriter.h>
#include <veins/modules/application/f2md/mdSupport/XmlWriter.h>



class MDReport {

protected:
    double generationTime;
    unsigned long senderPseudonym;
    unsigned long reportedPseudo;

    LAddress::L2Type senderRealId;
    LAddress::L2Type reportedRealId;

    std::string mbType;
    std::string attackType;

    veins::Coord senderGps;
    veins::Coord reportedGps;


public:
    MDReport();

    void setBaseReport(MDReport baseReport);

    void setGenerationTime(double time);
    void setSenderPseudo(unsigned long pseudo);
    void setReportedPseudo(unsigned long pseudo);
    void setMbType(std::string type);
    void setAttackType(std::string type);

    void setSenderGps(veins::Coord senderGps);
    void setReportedGps(veins::Coord reportedGps);

    void setSenderRealId(LAddress::L2Type senderRealId);
    void setReportedRealId(LAddress::L2Type reportedRealId);

    double getGenerationTime();
    unsigned long getSenderPseudo();
    unsigned long getReportedPseudo();
    std::string getMbType();
    std::string getAttackType();

    veins::Coord getSenderGps();
    veins::Coord getReportedGps();

    LAddress::L2Type getSenderRealId();
    LAddress::L2Type getReportedRealId();

    std::string getBaseReportXml();
    std::string getBaseReportJson(std::string reportType);
    bool writeStrToFile(const std::string strFileCnst, const std::string serial,
            const std::string version, const std::string outStr,const std::string curDate);
    bool writeStrToFileList(const std::string strFileCnst, const std::string serial,
            const std::string version, const std::string outStr,const std::string curDate);

};

#endif
