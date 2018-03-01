//
// Created by geniusrabbit on 18-2-27.
//

#include <cstdio>
#include "../include/SqliteServer.h"
#include "../include/Log.h"


sqlite3 *db = nullptr;
char *zErrMsg= nullptr;
int ret=0;

const char* SqlInit="CREATE TABLE NOTE(NOTEMESSAGE  TEXT   NOT NULL );";
const char* SqlAdd="INSERT INTO NOTE (NOTEMESSAGE) VALUES(%Q);";
const char* SqlDel="DELETE from NOTE where NOTEMESSAGE=%Q ;SELECT * from NOTE";
const char* SqlRe="SELECT * from NOTE";

//char *s=sqlite3_mprintf(".db");
//char **bufout=(char**)malloc(200*sizeof(char*));

namespace GeniusNote{
int callback(void *NotUsed,int argc,char **argv,char **azColName){
  for(int i=0;i<argc;i++){
    printf("%s\n",argv[i]);
    //bufout[i]=(char*)malloc(10*sizeof(char));
    //strcpy(bufout[i],argv[i]);
  }
  return 0;
}

int Sqlite::SqlInit(char* name){
  LOG_INFO("Create Database...")

  ret = sqlite3_open((const char*)name,&db);

  //char* sql= nullptr;
  //sql = sqlite3_mprintf("CREATE TABLE NOTE(NOTEMESSAGE  TEXT   NOT NULL );");

  ret =sqlite3_exec(db,SqlInit,callback,0,&zErrMsg);

  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  return 1;
}

int Sqlite::OpenDB(char* name){
  this->name=name;

  ret = sqlite3_open((const char*)name,&db);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  return 1;
}
int Sqlite::AddNote(void* bufin){
  ret = sqlite3_open((const char*)this->name,&db);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  char *sql = sqlite3_mprintf(SqlAdd,bufin);
  ret =sqlite3_exec(db,SqlAdd, nullptr, nullptr,&zErrMsg);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  sqlite3_free(sql);
  sqlite3_close(db);

}
int Sqlite::DeleteNote(void* bufin){
  ret = sqlite3_open((const char*)this->name,&db);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  char *sql= sqlite3_mprintf(SqlDel,bufin);

  ret =sqlite3_exec(db,sql,callback, nullptr,&zErrMsg);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  sqlite3_free(sql);
  sqlite3_close(db);

  return 1;
}
int Sqlite::ReloadNote(){
  ret=sqlite3_open((const char*)this->name,&db);

  char* sql = sqlite3_mprintf(SqlRe);

  ret = sqlite3_exec(db, sql, callback, nullptr, &zErrMsg);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  sqlite3_close(db);

  return 1;
}


}