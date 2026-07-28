// ===== ESP32 Obstacle Detection - Enhanced SSE Client =====

const DOM = {
    obstacle: document.getElementById('obstacle'),
    connectionDot: document.getElementById('connectionDot'),
    connectionText: document.getElementById('connectionText'),
    radarCenter: document.querySelector('.radar-center'),
    radarIcon: document.getElementById('radarIcon'),
    radarPing: document.getElementById('radarPing'),
    infoCard: document.getElementById('infoCard'),
    infoBarFill: document.getElementById('infoBarFill'),
    logList: document.getElementById('logList'),
    logCount: document.getElementById('logCount')
};

let eventCount = 0;
let lastState = null;
let reconnectAttempts = 0;
let source = null;

// ===== CONNECTION =====
function connect() {
    updateConnectionStatus('connecting');

    source = new EventSource('/events');

    source.onopen = () => {
        reconnectAttempts = 0;
        updateConnectionStatus('connected');
        addLog('System', 'Connected to ESP32', 'clear');
    };

    source.onmessage = (event) => {
        handleData(event.data);
    };

    source.onerror = () => {
        updateConnectionStatus('disconnected');
        source.close();

        // Exponential backoff retry
        const delay = Math.min(1000 * Math.pow(2, reconnectAttempts), 30000);
        reconnectAttempts++;

        addLog('System', `Connection lost. Retrying in ${delay/1000}s...`, 'detected');

        setTimeout(() => {
            if (source.readyState === EventSource.CLOSED) {
                connect();
            }
        }, delay);
    };
}

function updateConnectionStatus(status) {
    DOM.connectionDot.className = 'status-dot';

    switch(status) {
        case 'connected':
            DOM.connectionDot.classList.add('connected');
            DOM.connectionText.textContent = 'Live Connected';
            break;
        case 'disconnected':
            DOM.connectionDot.classList.add('disconnected');
            DOM.connectionText.textContent = 'Disconnected';
            break;
        default:
            DOM.connectionText.textContent = 'Connecting...';
    }
}

// ===== DATA HANDLING =====
function handleData(data) {
    const trimmed = data.trim();
    if (!trimmed) return;

    // Detect state from data string
    const isDetected = detectObstacle(trimmed);
    const state = isDetected ? 'detected' : 'clear';

    // Update main display
    DOM.obstacle.textContent = trimmed;
    DOM.obstacle.className = 'info-value ' + state;

    // Update card styling
    DOM.infoCard.className = 'info-card ' + state;

    // Update bar
    DOM.infoBarFill.className = 'info-bar-fill ' + state;

    // Update radar
    DOM.radarCenter.className = 'radar-center ' + state;
    DOM.radarIcon.textContent = isDetected ? '⚠️' : '✅';

    // Trigger ping animation on state change
    if (lastState !== state) {
        triggerPing();
        lastState = state;
    }

    // Add to log
    addLog('Sensor', trimmed, state);
}

function detectObstacle(data) {
    const lower = data.toLowerCase();

    
    if(lower == "obstacle") {
        return true;
    }else if(lower == "no obstacle"){
        return false;
    }
  

}

function triggerPing() {
    DOM.radarPing.classList.remove('active');
    void DOM.radarPing.offsetWidth; // force reflow
    DOM.radarPing.classList.add('active');

    setTimeout(() => {
        DOM.radarPing.classList.remove('active');
    }, 1000);
}

// ===== LOGGING =====
function addLog(source, message, type) {
    eventCount++;
    DOM.logCount.textContent = `${eventCount} event${eventCount !== 1 ? 's' : ''}`;

    const now = new Date();
    const timeStr = now.toLocaleTimeString('en-US', { 
        hour12: false, 
        hour: '2-digit', 
        minute: '2-digit', 
        second: '2-digit' 
    });

    const item = document.createElement('div');
    item.className = 'log-item ' + type;
    item.innerHTML = `
        <span class="log-time">${timeStr}</span>
        <span class="log-badge ${type}">${type}</span>
        <span class="log-message">${escapeHtml(message)}</span>
    `;

    DOM.logList.insertBefore(item, DOM.logList.firstChild);

    // Keep max 50 entries
    while (DOM.logList.children.length > 50) {
        DOM.logList.removeChild(DOM.logList.lastChild);
    }
}

function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

// ===== INIT =====
connect();

// Handle page visibility - reconnect when tab becomes active
document.addEventListener('visibilitychange', () => {
    if (!document.hidden && source && source.readyState === EventSource.CLOSED) {
        connect();
    }
});
