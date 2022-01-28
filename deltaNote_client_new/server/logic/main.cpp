#include "log.h"
#include "inter_var.h"
#include "http.h"
#include "s_sync_data.h"


int register_process(HTTP &server){
    ErrorCode error_code;
    if(RET_FAILED == server.s_register_callback(SYNC_SIGN_IN, SSyncData::sync_sign_in, error_code)){
        d_logic_error("server %s process register error, code:%d", SYNC_SIGN_IN, error_code);
    }
    return RET_SUCCESS;
}

int main(){
    d_logic_info("%s", "main_start")

    HTTP server(HTTP_server, "0.0.0.0", 1234);

    server.s_start_listen();
    return 0;
}