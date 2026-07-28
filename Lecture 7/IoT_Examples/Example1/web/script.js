

document.querySelectorAll("button").forEach((button, index) => {
    button.addEventListener("click", (e) => {
        fetch("/led", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
            },
            body: JSON.stringify({ "Led":index + 1})})
        .then(response => response.json())
        .then(data => {e.target.innerHTML = "Turn Led " + (index + 1) + " " + data.NextState; e.target.className= data.NextState=='ON'?'green':'red'});
    });
});
