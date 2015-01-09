#pragma once

#include <string>
#include <sstream>
#include <map>
#include <vector>

using std::map;
using std::vector;
using std::string;

class HLogger;
class TH1D;

/**
 * \class LogManager
 * \brief Container class for individual loggers
 */
class LogManager
{
public:
  static void    InitPath(string const& path) { GetLM()->fBasePath = path; }//!< Initialise the Manager's base path

  static void    AddLogger(string const&, string const&,int);                //!< Add a new log file to the manager
  static HLogger& GetLogger(string const&);                                  //!< Get a log from the manager

  static void    AddLogEntry(string const& log, string ent); //!< Add entry 'ent' to log ID 'log'
  static void    AddLogEntry(string const& log, double,double,double,bool); //!< Add entry 'ent' to log ID 'log'

  static void    ExportLogs();    //!< Export all active loggers

private:
  LogManager();
  ~LogManager();

  static LogManager* GetLM(){
    if (logInstance == 0)
      logInstance = new LogManager();

    return logInstance;
  }

  static LogManager* logInstance; //!< Singleton log instance

  typedef map<size_t, HLogger> LogMap;   //!< LogMap typedef
  LogMap fMap;                          //!< hash map of available loggers
  string fBasePath;                     //!< Base path of logger

};

/**
 * \class Logger
 * \brief Basic logging class. All methods private - can only be handled by logmanager
 */
class HLogger
{
  HLogger(string const&, string const&,int);

  void AddLogEntry(double,double,double,bool);
  void Export();

  const string fLogname;
  const string fFilename;
  int fVegas;

  vector<double> fEta;
  vector<TH1D*>  fHisto;
  vector<TH1D*>  fHistoITE;
  double **fW;

  friend class LogManager;
};
