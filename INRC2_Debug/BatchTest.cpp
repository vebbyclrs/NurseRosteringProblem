#include "INRC2_Debug.h"


using namespace std;
using namespace INRC2;


char *fullArgv[ArgcVal::full] = {
    "INRC2.exe",
    "--sce", "",
    "--his", "",
    "--week", "",
    "--sol", "",
    "--timeout", "",
    "--rand", "",
    "--cusIn", "",
    "--cusOut", ""
};

const char *testOutputDir = "output/";
const std::string dir( "../INRC2/instance/" );
const std::vector<std::string> instance = {
    "n005w4", "n012w8", "n021w4", "n030w4", "n030w8",           // 5
    "n040w4", "n040w8", "n050w4", "n050w8", "n060w4", "n060w8", // 11
    "n080w4", "n080w8", "n100w4", "n100w8", "n120w4", "n120w8"  // 17
};

const std::vector<double> instTimeout = {
    60, 60, 60, 70.13, 70.13,
    122.73, 122.73, 175.34, 175.34, 227.94, 227.94,
    333.14, 333.14, 438.34, 438.34, 543.54, 543.54
};

const std::string scePrefix( "/Sc-" );
const std::string weekPrefix( "/WD-" );
const std::string initHisPrefix( "/H0-" );
const std::string hisPrefix( "history-week" );
const std::string solPrefix( "sol-week" );
const std::string fileSuffix( ".txt" );
const std::string cusPrefix( "custom-week" );


char genInitHisIndex()
{
    return ((rand() % INIT_HIS_NUM) + '0');
}

void genWeekdataSequence( int instIndex, char *weekdata )
{
    int weekNum = instance[instIndex][5] - '1';
    for (; weekNum >= 0; --weekNum) {
        weekdata[weekNum] = (rand() % WEEKDATA_NUM) + '0';
    }
}

void makeSureDirExist( const string &dir )
{
    static const std::string mkdir_win32cmd( "mkdir " );
    static const std::string mkdir_unixshell( "mkdir -p " );
    system( (mkdir_win32cmd + dir).c_str() );
    system( (mkdir_unixshell + dir).c_str() );
}

void test( const char *outputDir, int instIndex, char initHis, const char *weeks, double timeoutInSec )
{
    makeSureDirExist( outputDir );
    ostringstream t;
    t << timeoutInSec;

    char *argv[ArgcVal::full];
    char argvBuf[ArgcVal::full][MAX_ARGV_LEN];
    int argc = ArgcVal::noCusInCusOut;

    prepareArgv_FirstWeek( outputDir, argv, argvBuf, instIndex, initHis, weeks[0], t.str() );
    run( argc, argv );

    for (char w = '1'; w < instance[instIndex][5]; w++) {
        prepareArgv( outputDir, argv, argvBuf, instIndex, weeks, w, t.str() );
        run( argc, argv );
    }
}

void test( const char *outputDir, int instIndex, char initHis, const char *weeks, double timeoutInSec, int randSeed )
{
    makeSureDirExist( outputDir );
    ostringstream t, r;
    t << timeoutInSec;
    r << randSeed;

    char *argv[ArgcVal::full];
    char argvBuf[ArgcVal::full][MAX_ARGV_LEN];
    int argc = ArgcVal::noCusInCusOut;

    prepareArgv_FirstWeek( outputDir, argv, argvBuf, instIndex, initHis, weeks[0], t.str(), r.str() );
    run( argc, argv );

    for (char w = '1'; w < instance[instIndex][5]; w++) {
        prepareArgv( outputDir, argv, argvBuf, instIndex, weeks, w, t.str(), r.str() );
        run( argc, argv );
    }
}

void test_customIO( const char *outputDir, int instIndex, char initHis, const char *weeks, double timeoutInSec )
{
    makeSureDirExist( outputDir );
    ostringstream t;
    t << timeoutInSec;

    int argc;
    char *argv[ArgcVal::full];
    char argvBuf[ArgcVal::full][MAX_ARGV_LEN];

    argc = ArgcVal::noRandCusIn;
    prepareArgv_FirstWeek( outputDir, argv, argvBuf, instIndex, initHis, weeks[0],
        t.str(), "", (outputDir + cusPrefix + '0') );
    run( argc, argv );

    char w = '1';
    for (; w < (instance[instIndex][5] - 1); w++) {
        argc = ArgcVal::noRand;
        prepareArgv( outputDir, argv, argvBuf, instIndex, weeks, w, t.str(), "",
            outputDir + cusPrefix + static_cast<char>(w - 1), outputDir + cusPrefix + w );
        run( argc, argv );
    }

    argc = ArgcVal::noRandCusOut;
    prepareArgv( outputDir, argv, argvBuf, instIndex, weeks, w, t.str(), "",
        outputDir + cusPrefix + static_cast<char>(w - 1), "" );
    run( argc, argv );
}

void test_customIO( const char *outputDir, int instIndex, char initHis, const char *weeks, double timeoutInSec, int randSeed )
{
    makeSureDirExist( outputDir );
    ostringstream t, r;
    t << timeoutInSec;
    r << randSeed;

    int argc;
    char *argv[ArgcVal::full];
    char argvBuf[ArgcVal::full][MAX_ARGV_LEN];

    argc = ArgcVal::noCusIn;
    prepareArgv_FirstWeek( outputDir, argv, argvBuf, instIndex, initHis, weeks[0],
        t.str(), r.str(), (outputDir + cusPrefix + '0') );
    run( argc, argv );

    char w = '1';
    for (; w < (instance[instIndex][5] - 1); w++) {
        argc = ArgcVal::full;
        prepareArgv( outputDir, argv, argvBuf, instIndex, weeks, w, t.str(), r.str(),
            outputDir + cusPrefix + static_cast<char>(w - 1), outputDir + cusPrefix + w );
        run( argc, argv );
    }

    argc = ArgcVal::noCusOut;
    prepareArgv( outputDir, argv, argvBuf, instIndex, weeks, w, t.str(), r.str(),
        outputDir + cusPrefix + static_cast<char>(w - 1), "" );
    run( argc, argv );
}

void prepareArgv_FirstWeek( const char *outputDir, char *argv[], char argvBuf[][MAX_ARGV_LEN], int i, char h, char w, string t, string r, string co )
{
    string sce = dir + instance[i] + scePrefix + instance[i] + fileSuffix;
    string his = dir + instance[i] + initHisPrefix + instance[i] + '-' + h + fileSuffix;
    string week = dir + instance[i] + weekPrefix + instance[i] + '-' + w + fileSuffix;
    string sol = outputDir + solPrefix + "0" + fileSuffix;
    i = 0;
    argv[i] = fullArgv[ArgvIndex::program];
    argv[++i] = fullArgv[ArgvIndex::__sce];
    strcpy( argvBuf[++i], sce.c_str() );
    argv[i] = argvBuf[i];
    argv[++i] = fullArgv[ArgvIndex::__his];
    strcpy( argvBuf[++i], his.c_str() );
    argv[i] = argvBuf[i];
    argv[++i] = fullArgv[ArgvIndex::__week];
    strcpy( argvBuf[++i], week.c_str() );
    argv[i] = argvBuf[i];
    argv[++i] = fullArgv[ArgvIndex::__sol];
    strcpy( argvBuf[++i], sol.c_str() );
    argv[i] = argvBuf[i];
    argv[++i] = fullArgv[ArgvIndex::__timout];
    strcpy( argvBuf[++i], t.c_str() );
    argv[i] = argvBuf[i];
    if (!r.empty()) {
        argv[++i] = fullArgv[ArgvIndex::__randSeed];
        strcpy( argvBuf[++i], r.c_str() );
        argv[i] = argvBuf[i];
    }
    if (!co.empty()) {
        argv[++i] = fullArgv[ArgvIndex::__cusOut];
        strcpy( argvBuf[++i], co.c_str() );
        argv[i] = argvBuf[i];
    }
}

void prepareArgv( const char *outputDir, char *argv[], char argvBuf[][MAX_ARGV_LEN], int i, const char *weeks, char w, string t, string r, string ci, string co )
{
    string sce = dir + instance[i] + scePrefix + instance[i] + fileSuffix;
    string week = dir + instance[i] + weekPrefix + instance[i] + '-' + weeks[w - '0'] + fileSuffix;
    string sol = outputDir + solPrefix + w + fileSuffix;
    string his = outputDir + hisPrefix + (--w) + fileSuffix;
    i = 0;
    argv[i] = fullArgv[ArgvIndex::program];
    argv[++i] = fullArgv[ArgvIndex::__sce];
    strcpy( argvBuf[++i], sce.c_str() );
    argv[i] = argvBuf[i];
    argv[++i] = fullArgv[ArgvIndex::__his];
    strcpy( argvBuf[++i], his.c_str() );
    argv[i] = argvBuf[i];
    argv[++i] = fullArgv[ArgvIndex::__week];
    strcpy( argvBuf[++i], week.c_str() );
    argv[i] = argvBuf[i];
    argv[++i] = fullArgv[ArgvIndex::__sol];
    strcpy( argvBuf[++i], sol.c_str() );
    argv[i] = argvBuf[i];
    argv[++i] = fullArgv[ArgvIndex::__timout];
    strcpy( argvBuf[++i], t.c_str() );
    argv[i] = argvBuf[i];
    if (!r.empty()) {
        argv[++i] = fullArgv[ArgvIndex::__randSeed];
        strcpy( argvBuf[++i], r.c_str() );
        argv[i] = argvBuf[i];
    }
    if (!ci.empty()) {
        argv[++i] = fullArgv[ArgvIndex::__cusIn];
        strcpy( argvBuf[++i], ci.c_str() );
        argv[i] = argvBuf[i];
    }
    if (!co.empty()) {
        argv[++i] = fullArgv[ArgvIndex::__cusOut];
        strcpy( argvBuf[++i], co.c_str() );
        argv[i] = argvBuf[i];
    }
}