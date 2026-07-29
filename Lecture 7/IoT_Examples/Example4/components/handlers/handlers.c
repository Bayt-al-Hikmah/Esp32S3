#include "handlers.h"


const static char start_sse[] = "HTTP/1.1 200 OK\r\n"
                               "Cache-Control: no-store\r\n"
                               "Content-Type: text/event-stream\r\n"
                               "\r\n"
                               "Event: Start session\r\n"
                               "data: Session Started\r\n"
                               "\r\n";

esp_err_t event_handler(httpd_req_t *req){

    sse_session arg;
    arg.mySocketHD = req->handle;
    arg.mySocketFD = httpd_req_to_sockfd(req);

    httpd_socket_send(arg.mySocketHD, arg.mySocketFD, start_sse, strlen(start_sse), 0);

   
    add_sse(arg);
    
    set_obstacle_state(NULL);
    set_fire_state(NULL);
    return ESP_OK;
}

