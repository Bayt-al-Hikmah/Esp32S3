const ws = new WebSocket("ws://192.168.4.1/ws");
ws.binaryType = "blob";

let currentUrl = null;
let frameCount = 0;
let lastFpsTime = performance.now();
let ledState = false;

// DOM Elements
const cam = document.getElementById("cam");
const cameraOverlay = document.getElementById("cameraOverlay");
const statusDot = document.getElementById("statusDot");
const statusText = document.getElementById("statusText");
const fpsCounter = document.getElementById("fpsCounter");
const ledLight = document.getElementById("ledLight");
const ledLabel = document.getElementById("ledLabel");

ws.onopen = () => {
    updateConnectionStatus(true);
    cam.classList.add("hidden");
    requestFrame();
};

ws.onclose = () => {
    updateConnectionStatus(false);
    cameraOverlay.classList.remove("hidden");
    cameraOverlay.innerHTML = "<span>Connection lost. Refresh to reconnect.</span>";
};

ws.onerror = (err) => {
    console.error("WebSocket error:", err);
    updateConnectionStatus(false);
};

function updateConnectionStatus(connected) {
    if (connected) {
        statusDot.classList.add("connected");
        statusDot.classList.remove("disconnected");
        statusText.textContent = "Connected";
    } else {
        statusDot.classList.remove("connected");
        statusDot.classList.add("disconnected");
        statusText.textContent = "Disconnected";
    }
}

function requestFrame() {
    if (ws.readyState === WebSocket.OPEN) {
        ws.send("GET_FRAME");
    }
}

ws.onmessage = (event) => {
    if (event.data instanceof Blob) {
        // Clean up previous URL
        if (currentUrl) {
            URL.revokeObjectURL(currentUrl);
        }

        currentUrl = URL.createObjectURL(event.data);
        cam.src = currentUrl;
        cam.classList.remove("hidden");
        cameraOverlay.classList.add("hidden");

        // FPS counter
        frameCount++;
        const now = performance.now();
        if (now - lastFpsTime >= 1000) {
            fpsCounter.textContent = frameCount + " FPS";
            frameCount = 0;
            lastFpsTime = now;
        }

        requestFrame();
    }
};

function ledOn() {
    if (ws.readyState === WebSocket.OPEN) {
        ws.send("LED_ON");
        updateLedState(true);
    }
}

function ledOff() {
    if (ws.readyState === WebSocket.OPEN) {
        ws.send("LED_OFF");
        updateLedState(false);
    }
}

function updateLedState(isOn) {
    ledState = isOn;
    if (isOn) {
        ledLight.classList.add("on");
        ledLabel.textContent = "ON";
        ledLabel.style.color = "#ffeb3b";
    } else {
        ledLight.classList.remove("on");
        ledLabel.textContent = "OFF";
        ledLabel.style.color = "var(--text-secondary)";
    }
}

// Cleanup on page unload
window.addEventListener("beforeunload", () => {
    if (currentUrl) {
        URL.revokeObjectURL(currentUrl);
    }
    if (ws.readyState === WebSocket.OPEN) {
        ws.close();
    }
});