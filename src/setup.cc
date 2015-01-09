#include "hfill.h"
#include <cstdio>
#include <sstream>

string expe[] = { "DEFAULT", "CMS", "ATLAS7", "ATLAS2", "CDF", "D0" };
string file[] = { "hfill_default","hfill_cms","hfill_atlas7",
                  "hfill_atlas2","hfill_cdf","hfill_d0"};

extern "C" {

  void hfillsetup_(int *i,int *job,int *vegas)
  {
    LogManager::InitPath(".");
    
    if (*i < 7) {
      std::stringstream mf("");
      if (*job < 10)
	mf << file[*i] << "_00" << *job << ".dat";
      else if (*job < 100)
	mf << file[*i] << "_0" << *job << ".dat";
      else
	mf << file[*i] << "_" << *job << ".dat";

      LogManager::AddLogger(expe[*i],mf.str(),*vegas);
    }
  }
  
  void hfill_(int *i,double *eta, double *pT, double *w, bool *s)
  {
    LogManager::AddLogEntry(expe[*i],*eta,*pT,*w,*s);
  }
  
  void hfillexport_()
  {
    LogManager::ExportLogs();
  }
  
}
