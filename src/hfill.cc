#include "hfill.h"
#include "md5.h"

#include "TH1D.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cstdlib>
#include <cmath>
using namespace std;

// Symbol for logmanager singleton
LogManager *LogManager::logInstance = 0;

/**
 * @brief LogManager::LogManager()
 * Private constructor for the Log file manager class
 */
LogManager::LogManager():
fMap(),
fBasePath("")
{

}

/**
 * @brief LogManager::~LogManager()
 * LogManager singleton destructor
 */
LogManager::~LogManager()
{
  fMap.clear();
}

/**
 * @brief LogManager::AddLogger
 * Adds a new logger class to the manager.
 * Logger classes are kept in a hash map, indexed by the hash of logname
 * @param logname The identifier for the log, e.g NNFIT or GAMinimizer
 * @param filename The target filename to which the log is written out. Relative to base path in LogManager
 */
void LogManager::AddLogger(string const& logname, string const& filename, int vegas)
{
  LogManager* LM = GetLM();
  size_t hashval = HHash::IntHash(logname.c_str());

  // Collision
  if (LM->fMap.find(hashval)!=LM->fMap.end())
  {
    cerr << "LogManager::AddLogger Error - hash collision for new log: \'" <<logname<<"\' with hash: "<< hashval<<". ";
    cerr << "Log already exists!"<<endl;
    exit(-1);
  }

  // Insert new logger
  string targetfile = LM->fBasePath + "/" + filename;
  LM->fMap.insert(std::pair<size_t,HLogger>(hashval,HLogger(logname, targetfile, vegas)));
}

/**
 * @brief LogManager::GetLogger
 * Returns a reference to specific logger, identified by name
 * @param logname The identifier for the log, e.g NNFIT or GAMinimizer
 */
HLogger& LogManager::GetLogger(string const& logname)
{
  LogManager* LM = GetLM();
  size_t hashval = HHash::IntHash(logname.c_str());

  // Collision
  LogMap::iterator iLog = LM->fMap.find(hashval);
  if ( iLog == LM->fMap.end())
  {
    cerr << "LogManager::GetLogger Error - log: \'" <<logname<<"\' does not exist!"<<endl;
    exit(-1);
  }

  return (*iLog).second;
}

void LogManager::AddLogEntry(string const& logname, double eta, double pT, double w, bool s)
{
  GetLogger(logname).AddLogEntry(eta,pT,w,s);
}

/**
 * @brief LogManager::ExportLogs
 * Loops through all managed loggers, and calls their export method.
 */
void LogManager::ExportLogs()
{
  cout << "-- LogManager:: Exporting Logs"<<endl;
  LogMap::iterator iLog = GetLM()->fMap.begin();
  for (; iLog != GetLM()->fMap.end(); iLog++)
    (*iLog).second.Export();
  cout << "-- LogManager:: Log Export Completed"<<endl;

}

/**
 * @brief Logger::Logger
 * Constructor for an individual log class.
 * Each instance of Logger is associated to a filename
 * @param logname The identifier for the new log, e.g NNFIT or GAMinimizer
 * @param filename The filename of the new log, given as an absolute or relative path to the running directory.
 */
HLogger::HLogger(string const& logname, string const& filename, int vegas):
  fLogname(logname),
  fFilename(filename),
  fVegas(vegas),
  fW(NULL)
{
  cout << "** New Log File Generated. Log \'"<<logname<<"\' at "<<filename<<endl;

  TH1D::SetDefaultSumw2(true);
  if (logname == "DEFAULT")
    {
      fEta.push_back(0.0);
      
      double pTeta0[] = {
	114.0059,
	124.5553,
	136.0810,
	148.6731,
	162.4305,
	177.4608,
	193.9014,
	211.8440,
	231.4468,
	252.8635,
	276.2621,
	301.8560,
	329.7880,
	360.3047,
	393.6452,
	430.0709,
	469.8672,
	513.3973,
	560.9041,
	612.8070,
	669.5126,
	731.4655,
	799.1511,
	873.1872,
	953.9870,
	1042.263,
	1138.7085,
	1244.0780,
	1359.3338,
	1485.1187,
	1622.5431,
	1772.6839,
	1936.7179, 
	2115.9306 };
	
      fHisto.push_back(new TH1D("eta0","eta0",33,pTeta0));      
      fEta.push_back(0.5);

      double pTeta1[] = {
	113.9945, 
	124.9420, 
	136.9410, 
	150.0922, 
	164.5065, 
	180.2870, 
	197.6010, 
	216.5778, 
	237.3771, 
	260.1738, 
	285.1314, 
	312.5142, 
	342.5268, 
	375.4217, 
	411.4757, 
	450.9471, 
	494.2542, 
	541.7204, 
	593.7450, 
	650.7659, 
	713.1915, 
	781.6836, 
	856.7533, 
	939.0324, 
	1029.2133,
	1128.0548,
	1236.2650,
	1354.9909,
	1485.1187,
	1627.7435,
	1784.0655 };

      fHisto.push_back(new TH1D("eta1","eta1",30,pTeta1));
      fEta.push_back(1.0);

      double pTeta2[] = {
	114.0059,
	125.9582,
	139.1636,
	153.7688,
	169.8898,
	187.7009,
	207.3794,
	229.1439,
	253.1672,
	279.7091,
	309.0645,
	341.4666,
	377.2658,
	416.8598,
	460.5632, 
	508.8483,
	562.2519, 
	621.1981, 
	686.3241, 
	758.2779, 
	837.8591, 
	925.6998, 
	1022.7496,
	1130.0871,
	1248.5648,
	1379.4636,
	1524.2382, 
	1684.0384 
      };

      fHisto.push_back(new TH1D("eta2","eta2",27,pTeta2));
      fEta.push_back(1.5);

      double pTeta3[] = {
	114.0059,
	125.9582,  
	139.1636,  
	153.7534,  
	169.8728,  
	187.6822,  
	207.3587,  
	229.0980,  
	253.1418,  
	279.6811,  
	309.0027, 
	341.3984, 
	377.1904,  
	416.7348,  
	460.4250,  
	508.6957,  
	562.0833,  
	621.0117,  
	686.1183, 
	758.0505, 
	837.5241, 
	925.3296, 
	1022.3406,
	1129.5222, 
	1247.9407 };

      fHisto.push_back(new TH1D("eta3","eta3",24,pTeta3));
      fEta.push_back(2.0);

      double pTeta4[] = {
	113.9945, 
	127.1351,  
	141.7762, 
	158.1035,  
	176.3287,  
	196.6352,  
	219.3020,  
	244.5574,  
	272.7212,  
	304.1588,  
	339.1865,  
	378.2480,  
	421.8501,  
	470.4313,  
	524.6598,  
	585.0808,  
	652.4601,  
	727.6717,  
	811.4721,
	905.0136 };      
      
      fHisto.push_back(new TH1D("eta4","eta4",19,pTeta4));      
      fEta.push_back(2.5);
    }
  else if (logname == "CMS")
    {
      fEta.push_back(0.0);
      
      double pTeta0[] = {
	114,
	133,
	153,
	174,
	196,
	220,
	245,
	272, 
	300, 
	330, 
	362, 
	395, 
	430, 
	468, 
	507, 
	548, 
	592, 
	638, 
	686, 
	737, 
	790, 
	846, 
	905, 
	967, 
	1032, 
	1101, 
	1172,
	1248, 
	1327, 
	1410, 
	1497,
	1588, 
	1784, 
	2116 };

      double pTeta1[] = {
	114, 
	133, 
	153, 
	174, 
	196, 
	220,
	245,
	272,
	300,
	330,
	362,
	395,
	430,
	468,
	507,
	548,
	592,
	638,
	686,
	737,
	790,
	846,
	905,
	967,
	1032, 
	1101,
	1172, 
	1248, 
	1327, 
	1410, 
	1784 };

      double pTeta2[] = {
	114,
	133,
	153, 
	174, 
	196, 
	220, 
	245, 
	272, 
	300, 
	330, 
	362, 
	395, 
	430, 
	468, 
	507, 
	548, 
	592,
	638, 
	686, 
	737, 
	790, 
	846, 
	905, 
	967, 
	1032,
	1101,
	1172,
	1684 
      };

      double pTeta3[] = {
	114, 
	133, 
	153, 
	174, 
	196, 
	220, 
	245, 
	272, 
	300, 
	330, 
	362, 
	395, 
	430, 
	468, 
	507, 
	548, 
	592, 
	638, 
	686, 
	737, 
	790, 
	846, 
	905, 
	967, 
	1248
      };
	
      double pTeta4[] = {
	114, 
	133, 
	153, 
	174, 
	196, 
	220, 
	245, 
	272, 
	300, 
	330, 
	362, 
	395, 
	430, 
	468, 
	507, 
	548, 
	592, 
	638, 
	686, 
	905	
      };
	   
      fEta.push_back(0.5);
      fEta.push_back(1.0);
      fEta.push_back(1.5);
      fEta.push_back(2.0);
      fEta.push_back(2.5);

      fHisto.push_back(new TH1D("eta0","eta0",33,pTeta0));
      fHisto.push_back(new TH1D("eta1","eta1",30,pTeta1));
      fHisto.push_back(new TH1D("eta2","eta2",27,pTeta2));
      fHisto.push_back(new TH1D("eta3","eta3",24,pTeta3));
      fHisto.push_back(new TH1D("eta4","eta4",19,pTeta4));

    }
  else if (logname == "ATLAS7")
    {

      double pTeta0[] = {
	20.0,   
	30.0,   
	45.0,   
	60.0,   
	80.0,   
	110.0,  
	160.0,  
	210.0,  
	260.0,  
	310.0,  
	400.0,  
	500.0,  
	600.0, 
	800.0,  
	1000.0, 
	1200.0,
	1500.0
      };
      double pTeta1[] = {
	20.0,    
	30.0,    
	45.0,    
	60.0,    
	80.0,    
	110.0,   
	160.0,   
	210.0,   
	260.0,   
	310.0,   
	400.0,   
	500.0,  
	600.0,   
	800.0,  
	1000.0, 
	1200.0,  
	1500.0
      };
      double pTeta2[] = {
	20.0,  
	30.0,  
	45.0,  
	60.0, 
	80.0,  
	110.0, 
	160.0, 
	210.0, 
	260.0, 
	310.0, 
	400.0, 
	500.0, 
	600.0, 
	800.0, 
	1000.0,
	1200.0,  
	1500.0 
      };
      double pTeta3[] = {
	20.0,  
	30.0,  
	45.0,  
	60.0,  
	80.0,  
	110.0, 
	160.0, 
	210.0, 
	260.0, 
	310.0,  
	400.0,  
	500.0,  
	600.0,  
	800.0,  
	1000.0,  
	1200.0
      };
      double pTeta4[] = {
	20.0,
	30.0,  
	45.0,  
	60.0,  
	80.0,  
	110.0, 
	160.0, 
	210.0, 
	260.0, 
	310.0, 
	400.0, 
	500.0,   
	600.0
      };
      double pTeta5[] = {
	20.0,  
	30.0,  
	45.0,  
	60.0,  
	80.0,  
	110.0, 
	160.0, 
	210.0, 
	260.0,   
	310.0 
      };
      double pTeta6[] = {
	20.0,    
	30.0,    
	45.0,    
	60.0,    
	80.0,    
	110.0,   
	160.0
      };

      fEta.push_back(0.0);
      fEta.push_back(0.3);
      fEta.push_back(0.8);
      fEta.push_back(1.2);
      fEta.push_back(2.1);
      fEta.push_back(2.8);
      fEta.push_back(3.6);
      fEta.push_back(4.4);

      fHisto.push_back(new TH1D("eta0","eta0",16,pTeta0));
      fHisto.push_back(new TH1D("eta1","eta1",16,pTeta1));
      fHisto.push_back(new TH1D("eta2","eta2",16,pTeta2));
      fHisto.push_back(new TH1D("eta3","eta3",15,pTeta3));
      fHisto.push_back(new TH1D("eta4","eta4",12,pTeta4));
      fHisto.push_back(new TH1D("eta5","eta5",9,pTeta5));
      fHisto.push_back(new TH1D("eta6","eta6",6,pTeta6));
    }
  else if (logname == "ATLAS2")
    {
      double pTeta0[] = {
	20, 
	30, 
	45, 
	60, 
	80, 
	110,
	160,
	210,
	260,
	310,
	400,  
	500
      };
      double pTeta1[] = {
	20, 
	30, 
	45, 
	60, 
	80, 
	110,
	160,
	210,
	260,
	310,
	400,  
	500 
      };
      double pTeta2[] = {  
	20, 
	30, 
	45, 
	60, 
	80, 
	110,
	160,
	210,
	260,
	310,  
	400 
      };
      double pTeta3[] = {
	20, 
	30, 
	45, 
	60, 
	80, 
	110,
	160,
	210,
	260,
	310,  
	400  
      };
      double pTeta4[] = {
	20, 
	30, 
	45, 
	60, 
	80, 
	110,
	160,
	210,  
	260
      };
      double pTeta5[] = {
	20, 
	30, 
	45, 
	60, 
	80, 
	110,
	160
      };
      double pTeta6[] = {
	20, 
	30,
	45,
	60
      };

      fEta.push_back(0.0);
      fEta.push_back(0.3);
      fEta.push_back(0.8);
      fEta.push_back(1.2);
      fEta.push_back(2.1);
      fEta.push_back(2.8);
      fEta.push_back(3.6);
      fEta.push_back(4.4);

      fHisto.push_back(new TH1D("eta0","eta0",11,pTeta0));
      fHisto.push_back(new TH1D("eta1","eta1",11,pTeta1));
      fHisto.push_back(new TH1D("eta2","eta2",10,pTeta2));
      fHisto.push_back(new TH1D("eta3","eta3",10,pTeta3));
      fHisto.push_back(new TH1D("eta4","eta4",8,pTeta4));
      fHisto.push_back(new TH1D("eta5","eta5",6,pTeta5));
      fHisto.push_back(new TH1D("eta6","eta6",3,pTeta6));
    }
  else if (logname == "CDF")
    {
      double pTeta0[] = {
	54., 
	62., 
	72.,
	83., 
	96., 
	110.,
	127.,
	146.,
	169.,
	195.,
	224.,
	259.,
	298.,
	344.,
	396.,
	457.,
	527.,
	700.
      };
      double pTeta1[] = {
	54. ,
	62. ,
	72. ,
	83. ,
	96. ,
	110.,
	127.,
	146.,
	169.,
	195.,
	224.,
	259.,
	298.,
	344.,
	396.,
	457.,
	527.,
	700.
      };
      double pTeta2[] = {
	54. ,
	62. ,
	72. ,
	83. ,
	96. ,
	110., 
	127., 
	146., 
	169., 
	195., 
	224., 
	259., 
	298., 
	344., 
	396., 
	457., 
	527. 
      };
      double pTeta3[] = {
	54. ,
	62. ,
	72. ,
	83. ,
	96. ,
	110.,
	127.,
	146.,
	169.,
	195.,
	224.,
	259.,
	298.,
	344.,
	396.
      };
      double pTeta4[] = {
	54. ,
	62. ,
	72. ,
	83. ,
	96. ,
	110.,
	127.,
	146.,
	169.,
	195.,
	224.,
	259.,
	298.
      };

      fEta.push_back(0.0);
      fEta.push_back(0.1);
      fEta.push_back(0.7);
      fEta.push_back(1.1);
      fEta.push_back(1.6);
      fEta.push_back(2.1);

      fHisto.push_back(new TH1D("eta0","eta0",17,pTeta0));
      fHisto.push_back(new TH1D("eta1","eta1",17,pTeta1));
      fHisto.push_back(new TH1D("eta2","eta2",16,pTeta2));
      fHisto.push_back(new TH1D("eta3","eta3",14,pTeta3));
      fHisto.push_back(new TH1D("eta4","eta4",12,pTeta4));
    }
  else if (logname == "D0")
    {
      double pTeta0[] = {
	50 ,
	60 ,
	70 ,
	80 ,
	90 ,
	100,
	110,
	120,
	130,
	145,
	160,
	180,
	200,
	220,
	240,
	265,
	295,
	325,
	360,
	400,
	445,
	490,
	540,  
	665 
      };
      double pTeta1[] = {
	50 ,  
	60 ,
	70 ,
	80 ,  
	90 ,  
	100,  
	110, 
	120, 
	130, 
	145, 
	160, 
	180, 
	200, 
	220, 
	240, 
	265, 
	295, 
	325, 
	360, 
	400, 
	445, 
	495,   
	635 
      };
      double pTeta2[] = {
	50 , 
	60 , 
	70 , 
	80 , 
	90 , 
	100, 
	110, 
	125, 
	140, 
	155, 
	170, 
	190, 
	210, 
	230, 
	250, 
	270, 
	300, 
	335, 
	375, 
	415,
	520
      };
      double pTeta3[] = {
	50 , 
	60 ,
	70 , 
	80 , 
	90 , 
	100, 
	110, 
	125, 
	140, 
	155, 
	170, 
	190,
	215,
	240, 
	265, 
	290, 
	325, 
	415 
      };
      double pTeta4[] = {
	50 , 
	60 , 
	70 , 
	80 , 
	90 , 
	100, 
	110, 
	125, 
	140, 
	160, 
	175, 
	190, 
	210, 
	235, 
	260,   
	320 
      };
      double pTeta5[] = {
	50 , 
	60 , 
	70 , 
	80 , 
	90 , 
	100, 
	110, 
	120, 
	130, 
	145, 
	160, 
	175, 
	200,   
	230 
      };

      fEta.push_back(0.0);
      fEta.push_back(0.4);
      fEta.push_back(0.8);
      fEta.push_back(1.2);
      fEta.push_back(1.6);
      fEta.push_back(2.0);
      fEta.push_back(2.4);

      fHisto.push_back(new TH1D("eta0","eta0",23,pTeta0));
      fHisto.push_back(new TH1D("eta1","eta1",22,pTeta1));
      fHisto.push_back(new TH1D("eta2","eta2",20,pTeta2));
      fHisto.push_back(new TH1D("eta3","eta3",17,pTeta3));
      fHisto.push_back(new TH1D("eta4","eta4",15,pTeta4));
      fHisto.push_back(new TH1D("eta5","eta5",13,pTeta5));
    }

  // filling weight matrix
  fW = new double*[fEta.size()-1];
  for (int i = 0; i < (int) fEta.size()-1; i++)
    {
      cout << fHisto[i]->GetNbinsX() << endl;
      fW[i] = new double[fHisto[i]->GetNbinsX()];
      for (int  j = 0; j < (int) fHisto[i]->GetNbinsX(); j++)
        fW[i][j] = 0.0;

      fHistoITE.push_back((TH1D*) fHisto[i]->Clone());
    }
  
  return;
}

/**
 * @brief Logger::Export
 * Write the contents of the Logger class to it's target filename.
 */
void HLogger::Export()
{
  ofstream targetFile;
  targetFile.open(fFilename.c_str());

  if (!targetFile.good())
  {
    cerr << "HLogger::Export Error - target filename "<<fFilename<<" cannot be accessed!"<<endl;
    cerr << "Log not exported"<<endl;
    return;
  }

  targetFile << "Identifier: "<<fLogname << endl;
  targetFile << "# ID, pTmin, pTmax, ds/dpT/d|y|, error "<< endl;

  int index = 1;
  for (int i = 0; i < (int) fEta.size()-1; i++)
    {
      for (int j = 1; j <= fHisto[i]->GetNbinsX(); j++) {
	double Sw = fHisto[i]->GetBinContent(j)/fVegas;
        targetFile << fixed << index << "\t" 
		   << fHisto[i]->GetBinLowEdge(j) << "\t" 
		   << fHisto[i]->GetBinLowEdge(j+1) << "\t" << scientific 
		   << fHisto[i]->GetBinContent(j)/fVegas/fHisto[i]->GetBinWidth(j) << "\t" 
		   << sqrt( (fW[i][j-1]-Sw*Sw) / (fVegas-1) ) / fHisto[i]->GetBinWidth(j) << endl;
        index++;
        }
    }

  cout << "** Log "<<fLogname<<" successfully exported to "<<fFilename<<endl;
}

void HLogger::AddLogEntry(double eta,double pT,double w,bool s)
{
  if (s == true) {

    for (int i = 0; i < (int) fEta.size()-1; i++) {
      for (int j = 1; j <= fHisto[i]->GetNbinsX(); j++)
	fW[i][j-1] += pow(fHistoITE[i]->GetBinContent(j),2);
      fHistoITE[i]->Reset();
    }
    

    } else {

      for (int i = 0; i < (int) fEta.size()-1; i++)
        if ( fabs(eta) < fEta[i+1] && fabs(eta) >= fEta[i]) {
          fHisto[i]->Fill(pT,w);
	  fHistoITE[i]->Fill(pT,w);
	}

    }
}


