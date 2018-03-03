//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_SQLITESERVER_H
#define GENIUSNOTE1_0_SQLITESERVER_H

#include <string>
#include <sqlite3.h>

typedef struct {
  char Time_Tag[128];
  char Type[3];
}NoteStruct;

using namespace std;

namespace GeniusNote{
class ServerSqlite{

};
class ServerSqlite {
 public:
  bool SqlTableOpen(char* UserName,char* paswd);
  int SqlTableIint(char* UserName);
  int ServerTableUpdate(char* UserName,char* terminal,NoteStruct* Note);
  int ServerTableReturn(char* UserName,NoteStruct* Note,char* terminal);
 private:
};
}

#endif //GENIUSNOTE1_0_SQLITESERVER_H
