#include "../../include/sqliteTools/cmdProcess.h"

bool isExit(std::string str){
    return str == "exit" || str == "q" || str == "quit";
}

bool isShow(std::string str){
    return str == "SHOW" || str == "show";
}

bool isUsers(std::string str){
    return str == "USERS" || str == "users";
}

bool isTable(std::string str){
    return str == "TABLE" || str == "table";
}