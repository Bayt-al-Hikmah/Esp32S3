(function() {
    'use strict';

    // Config
    const SSE_URL = '/events';
    const RECONNECT_BASE = 1000;
    const RECONNECT_MAX = 30000;

    // State
    let source = null;
    let reconnectDelay = RECONNECT_BASE;
    let reconnectTimer = null;
    let startTime = Date.now();

    // DOM refs
    const connStatus = document.getElementById('conn-status');
    const uptimeEl = document.getElementById('uptime');

    // Sensor registry — maps event name to config
    const sensors = {
        'Obstacle Sensor': {
            id: 'obstacle',
            isBinary: true,
            okValue: 'no obstacle',      // adjust to match your ESP32 output
            alertValue: 'obstacle',      // adjust to match your ESP32 output
            unit: ''
        },
        'Flame Sensor': {
            id: 'flame',
            isBinary: true,
            okValue: 'no flame',
            alertValue: 'flame detected',
            unit: ''
        },
        'Light Sensor': {
            id: 'light',
            isBinary: false,
            unit: ''                      // add 'lux' or '%' if your firmware sends raw numbers
        },
        'Moisture Sensor': {
            id: 'moisture',
            isBinary: false,
            unit: ''                      // add '%' if you map it
        }
    };

    // Helpers
    function fmtTime(date) {
        return date.toLocaleTimeString('en-US', { 
            hour12: false, 
            hour: '2-digit', 
            minute: '2-digit', 
            second: '2-digit' 
        });
    }

    function timeAgo(ts) {
        const sec = Math.floor((Date.now() - ts) / 1000);
        if (sec < 5) return 'just now';
        if (sec < 60) return `${sec}s ago`;
        return fmtTime(new Date(ts));
    }

    function setConnectionState(state) {
        connStatus.classList.remove('online', 'offline');
        const dot = connStatus.querySelector('.conn-dot');
        const text = connStatus.querySelector('.conn-text');

        if (state === 'online') {
            connStatus.classList.add('online');
            text.textContent = 'Live';
            reconnectDelay = RECONNECT_BASE;
        } else {
            connStatus.classList.add('offline');
            text.textContent = 'Disconnected';
        }
    }

    function flashValue(el) {
        el.classList.remove('flash');
        void el.offsetWidth; // force reflow
        el.classList.add('flash');
    }

    function updateBadge(el, type, text) {
        el.className = 'status-badge';
        if (type) el.classList.add(type);
        el.textContent = text;
    }

    function handleSensorUpdate(eventName, data) {
        const cfg = sensors[eventName];
        if (!cfg) return;

        const valueEl = document.getElementById(cfg.id);
        const badgeEl = document.getElementById(cfg.id + '-badge');
        const timeEl = document.getElementById(cfg.id + '-time');
        const unitEl = document.getElementById(cfg.id + '-unit');

        if (!valueEl) return;

        const raw = data.trim();
        const now = Date.now();

        // Update value with flash effect
        valueEl.textContent = raw;
        flashValue(valueEl);

        // Update unit if present
        if (unitEl && cfg.unit) {
            unitEl.textContent = cfg.unit;
        }

        // Determine status
        let badgeType = 'ok';
        let badgeText = 'Normal';

        if (cfg.isBinary) {
            const lower = raw.toLowerCase();
            if (lower.includes(cfg.alertValue?.toLowerCase()) || lower === '1' || lower === 'high') {
                badgeType = 'alert';
                badgeText = 'Alert';
            } else if (lower.includes(cfg.okValue?.toLowerCase()) || lower === '0' || lower === 'low') {
                badgeType = 'ok';
                badgeText = 'Clear';
            } else {
                badgeType = 'active';
                badgeText = raw;
            }
        } else {
            // Analog sensors — you can add threshold logic here
            badgeType = 'active';
            badgeText = 'Active';
        }

        updateBadge(badgeEl, badgeType, badgeText);
        timeEl.dataset.ts = now;
        timeEl.textContent = timeAgo(now);
    }

    function connect() {
        if (source) {
            try { source.close(); } catch(e) {}
        }

        setConnectionState('offline');

        source = new EventSource(SSE_URL);

        source.onopen = () => {
            setConnectionState('online');
            console.log('[SSE] Connected');
        };

        source.onerror = (err) => {
            setConnectionState('offline');
            console.warn('[SSE] Error, reconnecting in', reconnectDelay, 'ms');
            source.close();

            clearTimeout(reconnectTimer);
            reconnectTimer = setTimeout(() => {
                reconnectDelay = Math.min(reconnectDelay * 2, RECONNECT_MAX);
                connect();
            }, reconnectDelay);
        };

        // Wire up your exact event names
        source.addEventListener('Obstacle Sensor', (e) => {
            handleSensorUpdate('Obstacle Sensor', e.data);
        });

        source.addEventListener('Flame Sensor', (e) => {
            handleSensorUpdate('Flame Sensor', e.data);
        });

        source.addEventListener('Light Sensor', (e) => {
            handleSensorUpdate('Light Sensor', e.data);
        });

        source.addEventListener('Moisture Sensor', (e) => {
            handleSensorUpdate('Moisture Sensor', e.data);
        });
    }

    // Update "last seen" labels every second
    setInterval(() => {
        document.querySelectorAll('.last-seen[data-ts]').forEach(el => {
            const ts = parseInt(el.dataset.ts, 10);
            if (!isNaN(ts)) el.textContent = timeAgo(ts);
        });
    }, 1000);

    // Uptime counter
    setInterval(() => {
        const sec = Math.floor((Date.now() - startTime) / 1000);
        const m = Math.floor(sec / 60);
        const s = sec % 60;
        uptimeEl.textContent = `Up ${m}m ${s}s`;
    }, 1000);

    // Start
    connect();

})();