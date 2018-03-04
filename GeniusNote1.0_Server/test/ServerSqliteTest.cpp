//
// Created by geniusrabbit on 18-3-1.
//


#include "../include/SqliteServer.h"

using namespace GeniusNote;
int callback(void *NotUsed,int argc,char **argv,char **azColName){
  for(int i=0;i<argc;i++){
    printf("%s\n",argv[i]);
  }
  return 0;
}
int testmain(){
  return 1;
}