# Handling  POST Requests

This example demonstrates how to handle **HTTP POST requests** on the ESP32-S3 using the ESP-IDF HTTP server. While GET requests are typically used to retrieve web pages or other resources from the server, **POST** requests allow a client to send data to the ESP32. This mechanism is commonly used for submitting forms, updating device settings, controlling hardware, and exchanging information between a web application and the microcontroller.

When a user opens the web page, the browser downloads the HTML form from the ESP32. After the user enters information and presses the **Submit** button, the browser automatically sends an HTTP **POST** request to the endpoint specified by the form's `action` attribute. The submitted form data becomes the body of the HTTP request, where it can be received and processed by the ESP32.

The HTTP server receives the request body using `httpd_req_recv()`, which copies the transmitted data into a buffer for processing. The received data is typically formatted as key-value pairs, allowing the firmware to identify which values were submitted by the client. Once the request has been processed, the server sends an HTTP response back to the browser to acknowledge that the data was successfully received.

