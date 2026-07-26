# Creating and Starting an HTTP Server

This example demonstrates how to create and start an **HTTP server** on the ESP32-S3 using the ESP-IDF `esp_http_server` library. Once the ESP32 has been configured as a Wi-Fi Access Point, connected devices need a way to communicate with the firmware. An HTTP server provides this communication layer by accepting incoming HTTP requests and sending responses back to clients such as web browsers, smartphones, or desktop applications.

Communication between clients and the server is organized using **URI handlers**. Each handler is associated with a specific URL path and an HTTP method, such as **GET** or **POST**. When a client sends a request to a registered endpoint, the corresponding handler function is executed to process the request and generate the appropriate response. 

In this example, a single endpoint is registered that responds to **GET** requests. When a client accesses the corresponding URL, the server returns a simple text response, demonstrating the complete request-and-response workflow between the browser and the ESP32-S3.
