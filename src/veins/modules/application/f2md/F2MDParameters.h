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

#ifndef __VEINS_MDParameters_H_
#define __VEINS_MDParameters_H_
#include <veins/modules/application/f2md/mdEnumTypes/AttackTypes.h>
#include <veins/modules/application/f2md/mdEnumTypes/MbTypes.h>
#include <veins/modules/application/f2md/mdEnumTypes/MdAppTypes.h>
#include <veins/modules/application/f2md/mdEnumTypes/MdChecksVersionTypes.h>
#include <veins/modules/application/f2md/mdEnumTypes/PseudoChangeTypes.h>
#include <veins/modules/application/f2md/mdEnumTypes/ReportTypes.h>

#define MAX_TARGET_LENGTH 1000
#define MAX_ACCUSED_LENGTH 1000
#define MYBSM_SIZE 20
#define MAX_INTER_NUM 10
#define MAX_BSM_LENGTH 20
#define MAX_MDM_LENGTH 20
#define MAX_REP_PSEUDOS 1000
#define MAX_NODES_LENGTH 200
#define MAX_DETECTED_NODES 5000
#define MAX_DETECTED_NODES_COOP 100000
#define MAX_PSEUDO_LIST 100
#define MAX_SYBIL_NUM 20
#define MAX_STALE_NUM 120

class F2MDParameters {
public:
    //Simulation Parameters
    std::string serialNumber = "IRT-DEMO";
    std::string savePath = "../../../../mdmSave/";

    bool veremiConf = true;
    bool randomConf = false;
    bool variableConf = false;
    double confPos = 10.0;
    double confSpd = 0.05;
    double confHea = 20.0;
    double confAcc = 0.01;
    double confPrec = 100.0;
    double minConf = 0.20; //20%

    double SAVE_PERIOD = 10; //60 seconds
    double PRINT_PERIOD = 60; //60 seconds

    double START_SAVE = 0; //60 seconds
    double START_ATTACK = 10; //60 seconds

    reportTypes::Report REPORT_VERSION = reportTypes::ProtocolReport;

    bool UseAttacksServer = false;
    bool MixLocalAttacks = true;
    bool RandomLocalMix = false;

    double LOCAL_ATTACKER_PROB = 0.05;

    attackTypes::Attacks LOCAL_ATTACK_TYPE = attackTypes::RandomSpeed;
    attackTypes::Attacks MixLocalAttacksList[19] = {attackTypes::ConstPos,
        attackTypes::Disruptive, attackTypes::RandomPos,
        attackTypes::StaleMessages, attackTypes::DoSRandomSybil,
        attackTypes::ConstPosOffset, attackTypes::ConstSpeed,
        attackTypes::DoS, attackTypes::RandomPosOffset,
        attackTypes::DataReplaySybil, attackTypes::DoSDisruptive,
        attackTypes::ConstSpeedOffset, attackTypes::RandomSpeedOffset,
        attackTypes::EventualStop, attackTypes::DoSDisruptiveSybil,
        attackTypes::DataReplay, attackTypes::DoSRandom,
        attackTypes::GridSybil, attackTypes::RandomSpeed};

    //ConstPos, ConstPosOffset, RandomPos, RandomPosOffset,
    //ConstSpeed, ConstSpeedOffset, RandomSpeed, RandomSpeedOffset,
    //EventualStop, Disruptive, DataReplay, StaleMessages,
    //DoS, DoSRandom, DoSDisruptive,
    //GridSybil, DataReplaySybil, DoSRandomSybil, DoSDisruptiveSybil,

    double GLOBAL_ATTACKER_PROB = 0.0;
    attackTypes::Attacks GLOBAL_ATTACK_TYPE = attackTypes::MAStress;
    // 1 MAStress

    bool EnablePC = false;
    pseudoChangeTypes::PseudoChange PC_TYPE = pseudoChangeTypes::Car2car;
    // Periodical, Disposable, DistanceBased, Random, Car2car
    //Detection Application

    bool EnableV1 = true;
    bool EnableV2 = true;
    bool SaveStatsV1 = true;
    bool SaveStatsV2 = true;

    mdChecksVersionTypes::ChecksVersion checksVersionV1 =
        mdChecksVersionTypes::ExperiChecks;
    mdChecksVersionTypes::ChecksVersion checksVersionV2 =
        mdChecksVersionTypes::ExperiChecks;

    mdAppTypes::App appTypeV1 = mdAppTypes::ThresholdApp;
    mdAppTypes::App appTypeV2 = mdAppTypes::BehavioralApp;

    bool writeSelfMsg = false;
    bool writeListSelfMsg = false;

    //writeBsms
    bool writeBsmsV1 = false;
    bool writeBsmsV2 = false;
    bool writeListBsmsV1 = false;
    bool writeListBsmsV2 = false;

    //writeReport
    bool writeReportsV1 = false;
    bool writeReportsV2 = false;
    bool writeListReportsV1 = false;
    bool writeListReportsV2 = false;

    //writeReport
    bool writeVeReMi = false;
    double VeReMiSliceTime = 3600;

    bool sendReportsV1 = false;
    bool sendReportsV2 = false;
    std::string maHostV1 = "localhost";
    std::string maHostV2 = "localhost";
    int maPortV1 = 9980;
    int maPortV2 = 9981;
    //static int maPortV2 = 32790;

    bool enableVarThreV1 = false;
    bool enableVarThreV2 = false;
    //Simulation Parameters

    // ------ Detection Parameters -- Start
    double MAX_PROXIMITY_RANGE_L = 30;
    double MAX_PROXIMITY_RANGE_W = 3;
    double MAX_PROXIMITY_DISTANCE = 2;

    double MAX_CONFIDENCE_RANGE = 10;
    double MAX_PLAUSIBLE_RANGE = 420;
    double MAX_TIME_DELTA = 3.1;
    double MAX_DELTA_INTER = 2.0;
    double MAX_SA_RANGE = 420;
    double MAX_SA_TIME = 2.1;
    double MAX_KALMAN_TIME = 3.1;
    double KALMAN_POS_RANGE = 1.0;
    double KALMAN_SPEED_RANGE = 4.0;
    double KALMAN_MIN_POS_RANGE = 4.0;
    double KALMAN_MIN_SPEED_RANGE = 1.0;
    double MIN_MAX_SPEED = 40;
    double MIN_MAX_ACCEL = 3;
    double MIN_MAX_DECEL = 4.5;
    double MAX_MGT_RNG = 4;
    double MAX_MGT_RNG_DOWN = 6.2;
    double MAX_MGT_RNG_UP = 2.1;
    double MAX_BEACON_FREQUENCY = 0.9;
    double MAX_DISTANCE_FROM_ROUTE = 2;
    double MAX_NON_ROUTE_SPEED = -1;
    double MAX_HEADING_CHANGE = 90;
    double DELTA_BSM_TIME = 5;
    double DELTA_REPORT_TIME = 5;
    double POS_HEADING_TIME = 1.1;
    // ------ Detection Parameters -- End

    // ------ Storage Parameters -- Start
    double MAX_TARGET_TIME = 2;
    double MAX_ACCUSED_TIME = 2;
    // ------ Storage Parameters -- End

    // ------ Attacks Parameters -- Start
    double parVar = 0.55;
    double RandomPosOffsetX = 70.0;
    double RandomPosOffsetY = 70.0;
    double RandomSpeedX = 40.0;
    double RandomSpeedY = 40.0;
    double RandomSpeedOffsetX = 7.0;
    double RandomSpeedOffsetY = 7.0;
    double RandomAccelX = 2.0;
    double RandomAccelY = 2.0;
    double StopProb = 0.05;
    int StaleMessages_Buffer = 60;
    int DosMultipleFreq = 4;
    int DosMultipleFreqSybil = 2;
    int ReplaySeqNum = 6;
    int SybilVehNumber = 5;
    bool SelfSybil = false;
    double SybilDistanceX = 5;
    double SybilDistanceY = 2;
    // ------ Attacks Parameters -- End

    // ------ Pseudonym Parameters -- Start
    double Period_Change_Time = 240;
    int Tolerance_Buffer = 10;
    double Period_Change_Distance = 80;
    double Random_Change_Chance = 0.1;
    // ------ Pseudonym Parameters -- End

    //------ Report Parameters -- Start
    int InitialHistory = 5;
    double CollectionPeriod = 10;
    double UntolerancePeriod = 5;
    //------ Report Parameters -- End
};

#endif
