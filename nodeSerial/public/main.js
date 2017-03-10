var socket = io.connect('http://localhost:8080');
socket.on('news', function (data) {
    console.log(data);
});
window.onload=function(){
    var toggleLEDBtn = document.getElementById("toggleLED");
    toggleLEDBtn.addEventListener("click", toggle);
}
var LEDStatus = 0;

//toggle led
function toggle(){
    LEDStatus ^= 1; //XOR with 1 to toggle bit 
    console.log(LEDStatus);
    socket.emit('update LED', LEDStatus);
}

//dropdowns
function changeMode(mode){
    //console.log(dirDiff);
    var hashCode;
    if (mode == "haptic"){
        console.log("sending haptic");
        hashCode = '0';
    }
    if (mode == "autonomous"){
        console.log("sending autonomous");
        hashCode = '1';
    }
    socket.emit('selected mode', hashCode);
}

function changeShape(shape){
    var hashCode;
    if (shape == "line"){
        console.log("sending line");
        hashCode = '2';
    }
    if (shape == "circle"){
        console.log("sending circle");
        hashCode = '3';
    }
    if (shape == "fig-eight"){
        console.log("sending figure eight");
        hashCode = '4';
    }
    socket.emit('selected shape', hashCode);
}
