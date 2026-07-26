# Returning JSON Responses

This example demonstrates how to return **JSON** data from the ESP32-S3 using the ESP-IDF HTTP server. While previous examples generated complete HTML pages, many modern applications separate the frontend from the backend. Instead of sending an entire web page, the ESP32 acts as a lightweight web service that returns structured data, allowing web applications, mobile apps, or desktop software to decide how that data should be displayed.

Before sending the response, the server sets the HTTP **Content-Type** to `application/json`. This informs the client that the response contains JSON data rather than plain text or an HTML document. Browsers, mobile applications, and API clients can then automatically recognize the format and parse the received data into native objects without requiring any additional processing.


In this example, the server returns a simple JSON object containing temperature and humidity values whenever the registered endpoint is accessed. Although the response is static, the same approach can be used to generate dynamic JSON documents containing live sensor measurements, GPIO states, network information, or any other data collected by the ESP32.


