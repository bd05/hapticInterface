var socket = io.connect('http://localhost:8080');

window.onload=function(){
    var toggleLEDBtn = document.getElementById("toggleLED");
    toggleLEDBtn.addEventListener("click", toggle);
    var potVal = document.getElementById("potVal");
}

var LEDStatus = 0;
var dataArr = [];
var unParsedData = "";
var reading = "";
var prevReading;
var canvas = document.getElementById("whiteboard");
var ctx = canvas.getContext('2d');
ctx.strokeStyle = 'rgba(63, 116, 191, 0.05)';
//ctx.fillStyle = 'rgba(0,0,0,0.7)';

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

//canvas drawing
function drawLine(fromx, tox, fromy, toy){
        fromx = parseInt(fromx);
        tox = parseInt(tox);
        ctx.moveTo(fromx, fromy);
        ctx.lineTo(tox, toy);
        ctx.stroke();
}

//D3======================================================
var dataset = [ 5, 10, 13, 19, 21, 25, 22, 18, 15, 13,
                11, 12, 15, 20, 18, 17, 16, 18, 23, 25 ];
//Width and height
var w = 500;
var h = 1050;
var barPadding = 1; 
//Create SVG element
var svg = d3.select("body")
            .append("svg")
            .attr("width", w)
            .attr("height", h);

//make bars for bar graph
svg.selectAll("rect")
   .data(dataset)
   .enter()
   .append("rect")
   .attr("x", function(d, i) {
    return i * (w/dataset.length - barPadding);  //Bar width of 20 plus 1 for padding
    })
   .attr("y", function(d) {
    return h - d;  //Height minus data value
    })
   .attr("width", 20)
   .attr("height", function(d) {
    return d;
    })
   .attr("fill", function(d) {
    return "rgb(0, 0, " + (d * 10) + ")";
    });
//label each bar with its value
svg.selectAll("text")
   .data(dataset)
   .enter()
   .append("text")
   .text(function(d) {
        return d;
   })
   .attr("x", function(d, i) {
        return i * (w / dataset.length) + (w / dataset.length - barPadding) / 2;
    })
   .attr("y", function(d) {
        return h - (d - 3);
   })
   .attr("text-anchor", "middle");