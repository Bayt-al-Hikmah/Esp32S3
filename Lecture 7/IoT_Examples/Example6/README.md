# ESP32-S3 Camera WebSocket Streaming Server
This project demonstrates how to build a low-latency wireless camera streaming system using the ESP32-S3, the ESP32 Camera Driver, and WebSockets. The ESP32 operates as a Wi-Fi Access Point, hosts a custom web dashboard stored on LittleFS, and continuously exchanges data with the browser over a persistent WebSocket connection.

Unlike traditional MJPEG streaming, where the server continuously pushes images through an HTTP response, this project uses WebSockets to establish a two-way communication channel between the ESP32 and the browser. The browser requests a new frame whenever it is ready, while also sending commands such as turning an LED on or off without creating additional HTTP requests.

## Features

- **Standalone Access Point:** The ESP32-S3 creates its own Wi-Fi network (AP mode) with WPA2-PSK security, allowing up to four clients to connect directly.
    
- **WebSocket Communication:** A persistent full-duplex connection enables both the browser and the ESP32 to send data at any time without reconnecting.
    
- **On-Demand Camera Frames:** The browser explicitly requests each JPEG frame through the WebSocket, reducing unnecessary network traffic.
    
- **Binary Image Transmission:** Camera frames are transmitted as binary WebSocket packets without additional encoding.
    
- **LED Remote Control:** The browser sends simple text commands over the same WebSocket connection to control the onboard LED.
    
- **Embedded Web Server:** HTML, CSS, and JavaScript files are served directly from a LittleFS partition.
    
- **High-Speed PSRAM Utilization:** Uses Octal SPI PSRAM to store camera frame buffers efficiently.
    
- **Modular Architecture:** Camera initialization, networking, LittleFS mounting, HTTP request handling, and WebSocket communication are separated into dedicated C modules.
    
- **Chunked File Serving:** Static web assets are transmitted in small 255-byte chunks to reduce RAM usage.
    
- **Automatic Image Adjustments:** Camera output is horizontally mirrored and vertically flipped to match the standard camera orientation.
    

# Hardware Setup

The camera module is connected to the ESP32-S3 as shown below.

|ESP32-S3 Pin Label|Code Definition (`camera.h`)|GPIO Pin|
|---|---|---|
|CAM_XCLK|CAM_PIN_XCLK|GPIO 15|
|CAM_SIOD|CAM_PIN_SIOD|GPIO 4|
|CAM_SIOC|CAM_PIN_SIOC|GPIO 5|
|CAM_Y9|CAM_PIN_D7|GPIO 16|
|CAM_Y8|CAM_PIN_D6|GPIO 17|
|CAM_Y7|CAM_PIN_D5|GPIO 18|
|CAM_Y6|CAM_PIN_D4|GPIO 12|
|CAM_Y5|CAM_PIN_D3|GPIO 10|
|CAM_Y4|CAM_PIN_D2|GPIO 8|
|CAM_Y3|CAM_PIN_D1|GPIO 9|
|CAM_Y2|CAM_PIN_D0|GPIO 11|
|CAM_VSYNC|CAM_PIN_VSYNC|GPIO 6|
|CAM_HREF|CAM_PIN_HREF|GPIO 7|
|CAM_PCLK|CAM_PIN_PCLK|GPIO 13|

Notice that the data pins defined in the software (D0-D7) correspond to the camera module pins labeled Y2-Y9.

The GPIO definitions inside `camera.h` must exactly match the physical wiring between the ESP32-S3 and the camera module. Incorrect pin assignments will prevent the camera from initializing.

---

# Project Architecture

- **main/Example6.c** – Application entry point. Initializes LittleFS, the camera, the Wi-Fi Access Point, and the web server.
    
- **components/camera.c** – Configures the ESP32 Camera Driver, initializes the camera hardware, selects JPEG mode, and applies the image orientation settings.
    
- **components/wifi.c** – Configures the ESP32-S3 as a standalone Wi-Fi Access Point and initializes the networking stack.
    
- **components/ltfs.c** – Mounts the LittleFS partition at `/web` so the HTTP server can access the dashboard files.
    
- **components/server.c** – Starts the HTTP server and registers all HTTP and WebSocket endpoints.
    
- **components/static.c** – Serves the HTML, CSS, and JavaScript files from LittleFS using small memory-efficient chunks.
    
- **components/web_socket.c** – Implements the WebSocket endpoint, receives browser commands, captures camera frames, and transmits JPEG images as binary WebSocket packets.
    

---

# Project Configuration

To build this project successfully, several ESP-IDF features must be enabled before compiling.

## 1. Add Required Dependencies

Create or modify the `idf_component.yml` file inside the `main` directory.

```yaml
dependencies:
  espressif/esp32-camera: "*"
  joltwallet/littlefs: "~=1.21.0"
```

During the build process, the ESP-IDF Component Manager automatically downloads and integrates these components.
## 2. Enable PSRAM

Because camera frame buffers require considerably more memory than the ESP32-S3's internal RAM, external PSRAM must be enabled.
```bash
idf.py menuconfig
```

Navigate to:

```
Component config
    → ESP PSRAM
        → Support for external, SPI connected RAM

SPI RAM config
    → Mode of SPI RAM chip in use
        → Octal Mode PSRAM
```
## 3. Enable WebSocket Support
The ESP-IDF HTTP server does not enable WebSocket functionality by default.

Open:
```bash
idf.py menuconfig
```
Navigate to:
```
Component config
    → HTTP Server
        → Enable WebSocket Server
```
After enabling this option, the HTTP server can register URI handlers that support WebSocket connections by setting:
```c
.is_websocket = true
```
This allows the `/ws` endpoint to perform the WebSocket handshake and maintain a persistent connection with the browser.
## 4. Configure the Partition Table
Configure ESP-IDF to use the custom partition table.
```bash
idf.py menuconfig
```
Navigate to:
```
Partition Table
    → Partition Table
        → Custom partition table CSV
```
## 5. Automatically Build the LittleFS Image
Add the following line to the project's top-level `CMakeLists.txt`:
```cmake
littlefs_create_partition_image(storage web FLASH_IN_PROJECT)
```
ESP-IDF automatically packages the contents of the `web` directory into a LittleFS image and flashes it together with the firmware.
# Camera Configuration
The camera is configured using the `camera_config_t` structure before calling `esp_camera_init()`.

This project uses:
- **Pixel Format:** JPEG
- **Frame Size:** VGA (640×480)
- **JPEG Quality:** 12
- **Frame Buffers:** 1
- **Grab Mode:** Latest Frame

After initialization, the image is vertically flipped and horizontally mirrored to match the physical mounting orientation of the camera.
# How WebSockets Work

Normally, HTTP follows a request-response model. The browser sends a request, the server sends one response, and the connection is closed.

WebSockets work differently.

The browser first sends a normal HTTP request asking to upgrade the connection to the WebSocket protocol. If the server accepts the request, the HTTP connection is upgraded and remains open.

From that point onward, both the browser and the ESP32 can send messages whenever they want without creating new HTTP requests.

This persistent connection significantly reduces communication overhead and provides much lower latency than repeatedly opening and closing HTTP connections.

Unlike MJPEG streaming, where the ESP32 continuously pushes frames to the browser, this project gives control to the browser. The browser requests a frame only when it is ready to display the next one.
# WebSocket Communication Workflow
The WebSocket endpoint is registered as `/ws`.
When the browser loads the dashboard, the JavaScript creates a WebSocket connection to this endpoint.

After the connection is established, communication occurs through simple text commands and binary image packets.

The ESP32 listens on the `/ws` endpoint. When a message arrives, the `ws_handler` reads it. If it is a `TEXT` frame, it uses standard C string comparison (`strcmp`) to determine the action:

- **Hardware Control:** If the message is `"LED_ON"` or `"LED_OFF"`, the ESP32 immediately fires `gpio_set_level(LED_PIN, x)` to change the hardware state.
- **Status Check:** If the message is `"PING"`, the ESP32 immediately replies with `"PONG"`
- **Video Streaming (The Async Magic):** If the message is `"GET_FRAME"`, the ESP32 does **not** fetch the camera frame immediately in the handler. Instead, it calls `trigger_async_send()`.
    - Fetching a large JPEG buffer takes time. If we did it in the main WebSocket thread, the server would freeze and miss other commands (like turning off the LED).    
    - By using `httpd_queue_work()`, we hand the job of capturing the frame (`esp_camera_fb_get`) and sending it (`httpd_ws_send_frame_async`) to a background worker thread.

# JavaScript WebSocket Management
Though the UI files are stored in LittleFS, the underlying JavaScript architecture required to talk to this ESP32 backend looks like this:

**1. Connecting:** The browser creates a WebSocket object connecting to the ESP32's IP.
```js
const ws = new WebSocket('ws://' + location.hostname + '/ws');
ws.binaryType = "blob"; // Tell JS to expect binary image data
```
**2. Sending Commands (Browser -> ESP32):** To turn on the LED, the browser sends a simple text string. To ask for a video frame, it sends the trigger command.
```js
function turnOnLED() { ws.send("LED_ON"); }
function requestFrame() { ws.send("GET_FRAME"); }
```
**3. Receiving Data (ESP32 -> Browser):** The browser listens for the `onmessage` event. It checks if the incoming data is text (like "PONG") or a binary Blob (a JPEG frame).
```js
ws.onmessage = function(event) {
    if (typeof event.data === "string") {
        console.log("Server says: ", event.data); // Handles "PONG"
    } else {
        // We received binary data (The JPEG frame)
        const imageElement = document.getElementById("camera-stream");
        imageElement.src = URL.createObjectURL(event.data);
        
        // Immediately ask for the next frame to create smooth video
        requestFrame(); 
    }
};
```
# Web Server Endpoints

|Endpoint|Method|Description|
|---|---|---|
|/|GET|Serves the dashboard.|
|/style.css|GET|Serves the stylesheet.|
|/script.js|GET|Serves the JavaScript file.|
|/ws|GET (WebSocket Upgrade)|Upgrades the HTTP connection to a persistent WebSocket connection.|
