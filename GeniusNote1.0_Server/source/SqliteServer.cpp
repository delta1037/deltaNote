//
// Created by geniusrabbit on 18-2-27.
//

#include <cstdio>
#include <string>
#include "../include/SqliteServer.h"
#include "../include/Log.h"

//#include <sqlite3.h>


sqlite3 *db = nullptr;
char *zErrMsg=NULL;
int ret=0;
char *s=sqlite3_mprintf(".db");


using namespace std;

//char **bufout=(char**)malloc(200*sizeof(char*));

namespace GeniusNote{
int callback(void *NotUsed,int argc,char **argv,char **azColName){
  int i;
  //using namespace GeniusNote;

  for(i=0;i<argc;i++){
    printf("%s\n",argv[i]);
    //bufout[i]=(char*)malloc(10*sizeof(char));
    //strcpy(bufout[i],argv[i]);
  }
  return 0;
}

int Sqlite::sqinit(char* name){
  LOG_INFO("Create Database...")

  *name << *s ;
  ret = sqlite3_open((const char*)name,&db);

  char* sql=NULL;
  sql = sqlite3_mprintf("CREATE TABLE NOTE(NOTEMESSAGE  TEXT   NOT NULL );");

  ret =sqlite3_exec(db,sql,callback,0,&zErrMsg);

  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  return 1;

}
int Sqlite::open(char* name){
  *name << *s ;
  this->name=name;

  ret = sqlite3_open((const char*)name,&db);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  return 1;
}
int Sqlite::addNote(void* bufin){
  ret = sqlite3_open((const char*)this->name,&db);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  char *sql = sqlite3_mprintf("INSERT INTO NOTE (NOTEMESSAGE) VALUES(%Q);",bufin);

  ret =sqlite3_exec(db,sql,0,0,&zErrMsg);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  sqlite3_free(sql);
  sqlite3_close(db);

}
int Sqlite::deleteNote(void* bufin){
  ret = sqlite3_open((const char*)this->name,&db);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  char *sql= sqlite3_mprintf("DELETE from NOTE where NOTEMESSAGE=%Q ;" \
            "SELECT * from NOTE",bufin);

  ret =sqlite3_exec(db,sql,callback,0,&zErrMsg);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  sqlite3_free(sql);
  sqlite3_close(db);

  return 1;
}
int Sqlite::reNote(){
  ret=sqlite3_open((const char*)this->name,&db);

  char* sql = sqlite3_mprintf("SELECT * from NOTE");

  ret = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  //this->bufout=bufout;
  //关闭数据库
  sqlite3_close(db);

  return 1;
}


}