#include "camera.h"



static camera_config_t camera_config = {
    .pin_pwdn  = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SIOD,
    .pin_sccb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_VGA,
    .jpeg_quality =  20, 
    .fb_count = 1, 
    .grab_mode = CAMERA_GRAB_LATEST
};

esp_err_t camera_init(){
    if(CAM_PIN_PWDN != -1){
        gpio_set_direction(CAM_PIN_PWDN, GPIO_MODE_OUTPUT);

        gpio_set_level(CAM_PIN_PWDN, 0);

        
    }

    esp_camera_init(&camera_config);

    sensor_t * s = esp_camera_sensor_get();
    s->set_vflip(s, 1);   
    s->set_hmirror(s, 1); 
    return ESP_OK;
}


struct async_resp_arg {
    httpd_handle_t hd;
    int fd;
};


struct async_resp_arg {
    httpd_handle_t hd;
    int fd;
};

static void ws_async_send(void *arg){
    struct async_resp_arg *resp_arg = (struct async_resp_arg *)arg;

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        free(resp_arg);
        return;
    }


    httpd_ws_frame_t ws_pkt = {
        .final = true,
        .fragmented = false,
        .type = HTTPD_WS_TYPE_BINARY,
        .payload = fb->buf,
        .len = fb->len
    };

    esp_err_t ret =
        httpd_ws_send_frame_async(
            resp_arg->hd,
            resp_arg->fd,
            &ws_pkt);

    esp_camera_fb_return(fb);

    free(resp_arg);
}


static esp_err_t trigger_async_send(httpd_handle_t handle, httpd_req_t *req) {
    struct async_resp_arg *resp_arg =
        malloc(sizeof(struct async_resp_arg));

    if (!resp_arg) {
        return ESP_ERR_NO_MEM;
    }

    resp_arg->hd = req->handle;
    resp_arg->fd = httpd_req_to_sockfd(req);

    esp_err_t ret =
        httpd_queue_work(
            handle,
            ws_async_send,
            resp_arg);

    if (ret != ESP_OK) {
        free(resp_arg);
    }

    return ret;
}

 esp_err_t ws_handler(httpd_req_t *req){

    if (req->method == HTTP_GET) {
        return ESP_OK;
    }

    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(ws_pkt));

    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    esp_err_t ret =
        httpd_ws_recv_frame(req, &ws_pkt, 0);

    if (ret != ESP_OK) {
        return ret;
    }

    if (ws_pkt.len == 0) {
        return ESP_OK;
    }

    uint8_t *buf = calloc(1, ws_pkt.len + 1);

    if (!buf) {
        return ESP_ERR_NO_MEM;
    }

    ws_pkt.payload = buf;

    ret =
        httpd_ws_recv_frame(
            req,
            &ws_pkt,
            ws_pkt.len);

    if (ret != ESP_OK) {
        free(buf);
        return ret;
    }

    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT) {
        if (strcmp((char *)buf, "GET_FRAME") == 0) {

            free(buf);

            return trigger_async_send(
                req->handle,
                req);
        }

        if (strcmp((char *)buf, "LED_ON") == 0) {

            gpio_set_level(LED_PIN, 1);
        }

        if (strcmp((char *)buf, "LED_OFF") == 0) {

            gpio_set_level(LED_PIN, 0);
        }

        if (strcmp((char *)buf, "PING") == 0) {

            httpd_ws_frame_t reply = {
                .type = HTTPD_WS_TYPE_TEXT,
                .payload = (uint8_t *)"PONG",
                .len = 4
            };

            httpd_ws_send_frame(req, &reply);
        }
    }

    free(buf);

    return ESP_OK;
}
