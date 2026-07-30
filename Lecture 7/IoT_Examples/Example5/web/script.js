        const streamImg = document.getElementById('streamImg');
        const loadingOverlay = document.getElementById('loadingOverlay');
        const errorOverlay = document.getElementById('errorOverlay');
        const statusDot = document.getElementById('statusDot');
        const statusText = document.getElementById('statusText');
        const fpsCounter = document.getElementById('fpsCounter');
        const resBadge = document.getElementById('resBadge');
        const uptimeEl = document.getElementById('uptime');
        const streamContainer = document.getElementById('streamContainer');

        let frameCount = 0;
        let lastFpsTime = performance.now();
        let streamStartTime = Date.now();
        let isMirror = false;
        let isFlip = false;
        let connectionLost = false;

        // FPS Counter
        function updateFPS() {
            frameCount++;
            const now = performance.now();
            if (now - lastFpsTime >= 1000) {
                fpsCounter.textContent = frameCount;
                frameCount = 0;
                lastFpsTime = now;
            }
            requestAnimationFrame(updateFPS);
        }
        requestAnimationFrame(updateFPS);

        // Uptime counter
        setInterval(() => {
            const elapsed = Math.floor((Date.now() - streamStartTime) / 1000);
            const h = String(Math.floor(elapsed / 3600)).padStart(2, '0');
            const m = String(Math.floor((elapsed % 3600) / 60)).padStart(2, '0');
            const s = String(elapsed % 60).padStart(2, '0');
            uptimeEl.textContent = `${h}:${m}:${s}`;
        }, 1000);

        // Detect resolution once loaded
        streamImg.onload = function() {
            loadingOverlay.classList.add('hidden');
            streamImg.classList.remove('loading');
            streamImg.classList.add('loaded');
            statusDot.classList.add('active');
            statusDot.classList.remove('error');
            statusText.textContent = 'Live';
            resBadge.textContent = `${this.naturalWidth} x ${this.naturalHeight}`;
            connectionLost = false;
            errorOverlay.classList.remove('visible');
        };

        // Error handling
        streamImg.onerror = function() {
            statusDot.classList.remove('active');
            statusDot.classList.add('error');
            statusText.textContent = 'Disconnected';
            loadingOverlay.classList.add('hidden');
            if (!connectionLost) {
                errorOverlay.classList.add('visible');
                connectionLost = true;
            }
        };

        // Retry connection
        function retryStream() {
            errorOverlay.classList.remove('visible');
            loadingOverlay.classList.remove('hidden');
            streamImg.classList.remove('loaded');
            streamImg.classList.add('loading');
            statusText.textContent = 'Reconnecting...';
            statusDot.classList.remove('error');

            // Force reload by appending timestamp
            const src = '/stream';
            streamImg.src = src + '?t=' + Date.now();
            streamStartTime = Date.now();
            connectionLost = false;
        }

        // Snapshot (client-side canvas capture)
        function takeSnapshot() {
            if (!streamImg.naturalWidth) return;

            const canvas = document.createElement('canvas');
            canvas.width = streamImg.naturalWidth;
            canvas.height = streamImg.naturalHeight;
            const ctx = canvas.getContext('2d');

            // Apply mirror/flip transforms
            ctx.save();
            ctx.translate(canvas.width / 2, canvas.height / 2);
            ctx.scale(isMirror ? -1 : 1, isFlip ? -1 : 1);
            ctx.drawImage(streamImg, -canvas.width / 2, -canvas.height / 2);
            ctx.restore();

            const link = document.createElement('a');
            link.download = `esp32-s3-snapshot-${new Date().toISOString().slice(0,19).replace(/:/g,'-')}.jpg`;
            link.href = canvas.toDataURL('image/jpeg', 0.92);
            link.click();
        }

        // Fullscreen
        function toggleFullscreen() {
            if (!document.fullscreenElement) {
                streamContainer.requestFullscreen().catch(err => {
                    console.log('Fullscreen error:', err);
                });
            } else {
                document.exitFullscreen();
            }
        }

        // Mirror
        function toggleMirror() {
            isMirror = !isMirror;
            updateTransform();
        }

        // Flip
        function toggleFlip() {
            isFlip = !isFlip;
            updateTransform();
        }

        function updateTransform() {
            streamImg.style.transform = `scale(${isMirror ? -1 : 1}, ${isFlip ? -1 : 1})`;
        }

        // Auto-retry on disconnect every 5 seconds
        setInterval(() => {
            if (connectionLost) {
                retryStream();
            }
        }, 5000);