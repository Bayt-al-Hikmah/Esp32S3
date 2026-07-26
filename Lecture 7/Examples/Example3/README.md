# Serving HTML Pages

This example demonstrates how to serve a complete HTML web page from the ESP32-S3 using the ESP-IDF HTTP server. While returning plain text is useful for verifying that the server is running correctly, most embedded web applications require HTML to create user interfaces that can be viewed and interacted with through a web browser.

Before sending the response, the server sets the HTTP **Content-Type** to `text/html`. This informs the browser that the received data should be interpreted as an HTML document rather than plain text. As a result, the browser renders the page normally, displaying headings, paragraphs, images, buttons, and any other HTML elements contained in the response.

In this example, the HTML document is embedded directly into the application as a string and transmitted to the client whenever the endpoint is requested. Although this approach is suitable for small demonstration pages, larger web applications typically store their HTML, CSS, JavaScript, and image files in a file system such as **LittleFS**, allowing the web content to be managed separately from the application source code.
