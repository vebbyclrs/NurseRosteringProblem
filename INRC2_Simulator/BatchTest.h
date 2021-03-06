/**
*   usage : 1. simulator for calling solver on stages.
*
*   note :  1.
*/

#ifndef INRC2_DEBUG_H
#define INRC2_DEBUG_H


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <thread>
#include <chrono>
#include <algorithm>
#include <queue>
#include <cstring>
#include <cstdlib>

#include "DebugFlag.h"
#include "INRC2.h"


enum ArgcVal
{
    single = 2,
    full = 21,
    noRand = full - single,
    noCusIn = full - single,
    noCusOut = full - single,
    noCusInCusOut = full - 2 * single,
    noRandCusIn = full - 2 * single,
    noRandCusOut = full - 2 * single,
    noRandCusInCusOut = noCusInCusOut - single
};

enum ArgvIndex
{
    program = 0, __id, id, __config, config,
    __sce, sce, __his, his, __week, week, __sol, sol,
    __timout, timeout, __randSeed, randSeed, 
    __cusIn, cusIn, __cusOut, cusOut
};

enum InstIndex
{
    n005w4, n012w8, n021w4,
    n030w4, n030w8,
    n035w4, n035w8,
    n040w4, n040w8,
    n050w4, n050w8,
    n060w4, n060w8,
    n070w4, n070w8,
    n080w4, n080w8,
    n100w4, n100w8,
    n110w4, n110w8,
    n120w4, n120w8
};

struct TestCase
{
public:
    TestCase( const std::string &name, char his, std::string &seq )
        : instName( name ), initHis( his ), weekdataSeq( seq )
    {
    }

    std::string instName;
    char initHis;
    std::string weekdataSeq;
};

static const int MAX_ARGV_LEN = 256;
static const int INIT_HIS_NUM = 3;
static const int WEEKDATA_NUM = 10;
static const int MAX_WEEK_NUM = 8;
static const int WEEKDATA_SEQ_SIZE = (MAX_WEEK_NUM + 1);

extern char fullArgv[ArgcVal::full][MAX_ARGV_LEN];

extern const std::string outputDirPrefix;
extern const std::string instanceDir;
extern const std::vector<std::string> instance;
extern const std::map<std::string, int> instIndexMap;

extern const std::string timoutFileName;
extern std::map<int, double> instTimeout;
extern const std::string instSeqFileName;
extern std::vector<TestCase> testCases;

extern const std::string configFileName;
extern std::string configString;

extern const std::string scePrefix;
extern const std::string weekPrefix;
extern const std::string initHisPrefix;
extern const std::string hisPrefix;
extern const std::string solPrefix;
extern const std::string fileSuffix;
extern const std::string cusPrefix;

extern const char *FeasibleCheckerHost;


// load config from configFile
void loadConfig();
// load timeout from timoutFile
void loadInstTimeOut();
// load instance sequence from instSeqFile
void loadInstSeq( const std::string &filename = instSeqFileName );

int getNurseNum( int instIndex );
int getWeekNum( int instIndex );

// this functions do not guarantee the sequence is feasible
char genInitHisIndex();
void genWeekdataSequence( int instIndex, char weekdata[WEEKDATA_SEQ_SIZE] );
// generate initHis and weekdata randomly
// this function will make sure the sequence is feasible by asking feasible checker
void genInstanceSequence( int instIndex, char &initHis, char weekdata[WEEKDATA_SEQ_SIZE] );

void makeSureDirExist( const std::string &dir );

void testAllInstancesParallel( int threadNum, int round );
void testHeterogeneousInstancesWithPreloadedInstSeq( const std::string &id, int runCount );
void testAllInstancesWithPreloadedInstSeq( const std::string &id, int runCount );
void testAllInstances( const std::string &id, int runCount, int seedForInstSeq );
void test( const std::string &id, const std::string &outputDir, int instIndex, char initHis, const char *weeks, double timeoutInSec );
void test_r( const std::string &id, const std::string &outputDir, int instIndex, char initHis, const char *weeks, double timeoutInSec, int randSeed );
void test_customIO( const std::string &id, const std::string &outputDir, int instIndex, char initHis, const char *weeks, double timeoutInSec );
void test_customIO_r( const std::string &id, const std::string &outputDir, int instIndex, char initHis, const char *weeks, double timeoutInSec, int randSeed );
void prepareArgv_FirstWeek( const std::string &id, const std::string &outputDir, char *argv[], char argvBuf[][MAX_ARGV_LEN], int instIndex, char initHis,
    char week, const std::string &timeoutInSec, const std::string &randSeed = "", const std::string &cusOut = "" );
void prepareArgv( const std::string &id, const std::string &outputDir, char *argv[], char argvBuf[][MAX_ARGV_LEN], int instIndex, const char *weeks, char week,
    const std::string &timeoutInSec, const std::string &randSeed = "", const std::string &cusIn = "", const std::string &cusOut = "" );

#ifdef INRC2_CHECK_INSTANCE_FEASIBILITY_ONLINE
void httpget( std::ostream &os, const char *host, const char *file );
#endif


#endif