#include "hfill.h"
#include <iostream>

int main() {

  LogManager::InitPath(".");
  LogManager::AddLogger("MyTestLog","test.dat",10);
  LogManager::AddLogEntry("MyTestLog",0.4,50,0.1,false);
  LogManager::ExportLogs();

  return 0;
}

