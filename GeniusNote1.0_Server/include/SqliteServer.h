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

namespace GeniusNote{
class ServerSqlite {
 public:
  bool SqlTableOpen(char* UserName,char* paswd);
  int SqlTableIint(char* UserName);
  int ServerTableUpdate(char* UserName,const char* terminal,NoteStruct* Note);
  int ServerTableReturn(char* UserName,NoteStruct* Note,char* terminal,int (*callback)(void *, int, char **, char **));
};
}
#endif //GENIUSNOTE1_0_SQLITESERVER_H
