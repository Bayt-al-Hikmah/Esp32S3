#include "handlers.h"


const static char start_sse[] = "HTTP/1.1 200 OK\r\n"
                               "Cache-Control: no-store\r\n"
                               "Content-Type: text/event-stream\r\n"
                               "\r\n"
                               "data: No obstacle\r\n"
                               "\r\n";


esp_err_t event_handler(httpd_req_t *req){

    _data.argument.mySocketHD = req->handle;
    _data.argument.mySocketFD = httpd_req_to_sockfd(req);

    httpd_socket_send(_data.argument.mySocketHD, _data.argument.mySocketFD, start_sse, strlen(start_sse), 0);

    return ESP_OK;
}

