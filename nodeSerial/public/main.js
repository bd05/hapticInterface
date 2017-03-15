var socket = io.connect('http://localhost:8080');

window.onload=function(){
    var toggleLEDBtn = document.getElementById("toggleLED");
    toggleLEDBtn.addEventListener("click", toggle);
    var potVal = document.getElementById("potVal");
    
    /*if(!canvas) 
        console.log("canvas not found");
    else
        var ctx = canvas.getContext('2d');*/
}

var LEDStatus = 0;
var dataArr = [];
var unParsedData = "";
var reading = "";
var prevReading;
var canvas = document.getElementById("whiteboard");
var ctx = canvas.getContext('2d');

var DataPoint = function(val){
    this.val = val;
    this.rotor = "left";
}

//sockets
socket.on('updatePot', function(data){
    unParsedData += data;
    if(unParsedData.charAt(0) === "B" && unParsedData.slice(-1) === "E"){
        //console.log(unParsedData);
        reading = unParsedData.substring(unParsedData.indexOf('B') + 1, unParsedData.indexOf('E'));
        document.getElementById("potVal").textContent = reading;
        dataArr.push(reading);
        drawLine(prevReading, reading, 50, 50); //hard coded in y-coordinates for canvas drawings for now
        prevReading = reading;
        unParsedData = "";
    }
    console.log(dataArr);
});

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

//canvas
function drawLine(fromx, tox, fromy, toy){
        fromx = parseInt(fromx);
        tox = parseInt(tox);
        ctx.moveTo(fromx, fromy);
        ctx.lineTo(tox, toy);
        ctx.stroke();
}