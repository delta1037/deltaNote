//
// Created by geniusrabbit on 18-2-27.
//

/**
 * 数据库设计：
 * 每个用户一张表，另写一张表保存用户名和密码，用来校验用户是否具有访问权限
 *
 * 函数实现：
 * １．初始化用户，新建表
 * ２．根据返回内容修改表中各条信息参数
 * time_tag  server  client android
 *    tag     true    true    true
 *
 *    建立状态码：已同步、未同步，已删除
 *
 * 各端分别维护一个表，更新时，由socket建立联系，并将表发送给服务器，服务器更新内容后给客户端更新内容
 * 客户端定时向服务器请求更新
 *
 *
 * １．初始化函数：输入用户名，存储用户名及其密码
 * ２．修改状态函数：输入客户端发送的数组，及其客户端种类，将客户端发来的
 * 表逐条与服务器的表对照，将客户端已同步的而服务器未同步的添加到表中
 * ，将服务器端已同步而客户端未同步的发送给客户端，完成同步
 */



#include <cstdio>
#include "../include/SqliteServer.h"
#include "../include/Log.h"

sqlite3 *db = nullptr;
char *zErrMsg= nullptr;
int ret=0;

namespace GeniusNote{
int callback(void *NotUsed,int argc,char **argv,char **azColName){
  for(int i=0;i<argc;i++){
    printf("%s\n",argv[i]);
    //bufout[i]=(char*)malloc(10*sizeof(char));
    //strcpy(bufout[i],argv[i]);
  }
  return 0;
}

bool ServerSqlite::SqlTableOpen(char* name,char* paswd){
  //创建用户密码表
  const char* Name_paswd="CRREATE TABLE IF NOT EXITS GeniusNote.Name_paswd("\
      "Name KEY NOTNULL,"\
      "Paswd CHAR(256) NOTNULL);";
  ret =sqlite3_exec(db,Name_paswd,callback,0,&zErrMsg);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})

  //查找用户名，对比密码
  LOG_INFO("Name and password selecting...")
  const char* Find_name="SELECT Name,Paswd From Geniusrabbit.Name_Paswd"\
      "WHERE Name=(%Q) AND Paswd=(%Q)";
  char* SqlFind=sqlite3_mprintf(Find_name,name,paswd);
  ret =sqlite3_exec(db,SqlFind,callback,0,&zErrMsg);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})
  return ret==SQLITE_OK;

}
int ServerSqlite::SqlTableIint(char* UserName){
  const char *CreateTable="CREATE TABLE GeniusNote.(%Q)("\
      "Time_Tag       KEY       NOTNULL,"\
      "ServerState    CHAR(1)   NOTNULL,"\
      "ClientState    CHAR(1)   NOTNULL,"\
      "AndroidState   CHAR(1)   NOTNULL);";
  char* SqlInit=sqlite3_mprintf(CreateTable,UserName);
  ret =sqlite3_exec(db,SqlInit,callback,0,&zErrMsg);
  CHECK(ret,SQLITE_OK,{LOG_ERROR(stderr,"SQL error:%s\n",zErrMsg)})
}
int ServerSqlite::ServerTableUpdate(char* name,char* type,char array[][]){
  const char* Update="";
}
int ServerSqlite::ServerTableReturn(char* name){

}
}





const char* SqlAdd="INSERT INTO NOTE (NOTEMESSAGE) VALUES(%Q);";
const char* SqlDel="DELETE from NOTE where NOTEMESSAGE=%Q ;SELECT * from NOTE";
const char* SqlRe="SELECT * from NOTE";

//char *s=sqlite3_mprintf(".db");
//char **bufout=(char**)malloc(200*sizeof(char*));


namespace GeniusNote{

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