
const slider = document.querySelector("#angle");
const label = document.querySelector("#angleLabel");
const gaugeFill = document.querySelector("#gaugeFill");
const gaugeValue = document.querySelector("#gaugeValue");
const toast = document.querySelector("#toast");
const presetButtons = document.querySelectorAll(".btn");

let toastTimeout;

function updateUI(value) {

    label.innerHTML = "Angle: " + value + "°";
    
    gaugeValue.textContent = value;
    const percentage = value / 180;
    const angle = percentage * 180;
    gaugeFill.style.setProperty("--fill-angle", angle + "deg");
    
    const hue = 240 + (percentage * 60); 
    gaugeFill.style.filter = `hue-rotate(${percentage * 30}deg)`;
}

function showToast() {
    toast.classList.add("show");
    clearTimeout(toastTimeout);
    toastTimeout = setTimeout(() => {
        toast.classList.remove("show");
    }, 1500);
}


function sendAngle(value) {
    fetch("/servo", {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify({ "Angle": value })
    })
    .then(response => response.json())
    .then(data => {
        updateUI(value);
        showToast();
    })
    .catch(err => {
        console.error("Servo error:", err);
        updateUI(value);
    });
}

slider.addEventListener("input", (e) => {
    updateUI(e.target.value);
});

slider.addEventListener("change", (e) => {
    sendAngle(e.target.value);
});

presetButtons.forEach(btn => {
    btn.addEventListener("click", () => {
        const angle = btn.dataset.angle;
        slider.value = angle;
        sendAngle(angle);
    });
});


updateUI(slider.value);