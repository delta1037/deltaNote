//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_SQLITESERVER_H
#define GENIUSNOTE1_0_SQLITESERVER_H

#include  <sqlite3.h>
//#include <stdio>
#include <string>

//using namespace std;

namespace GeniusNote{
class Sqlite
{
 public:
  //int callback(void *NotUsed,int argc,char **argv,char **azColName);

  int sqinit(char* name);

  int open(char* name);

  int addNote(void* bufin);

  int deleteNote(void* bufin);

  int reNote();
  //char **bufout=(char**)malloc(sizeof(char*));

 private:

  const char* name;
};
}

#endif //GENIUSNOTE1_0_SQLITESERVER_H
