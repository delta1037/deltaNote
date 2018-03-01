//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_SQLITESERVER_H
#define GENIUSNOTE1_0_SQLITESERVER_H

#include <string>
#include <sqlite3.h>
using namespace std;

namespace GeniusNote{
class Sqlite {
 public:
  //int callback(void *NotUsed,int argc,char **argv,char **azColName);

  int SqlInit(char* name);
  int OpenDB(char* name);
  int AddNote(void* bufin);
  int DeleteNote(void* bufin);
  int ReloadNote();

  //char **bufout=(char**)malloc(sizeof(char*));

 private:
  const char* name;
};
}

#endif //GENIUSNOTE1_0_SQLITESERVER_H
