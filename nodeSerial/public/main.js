var socket = io.connect('http://localhost:8080');

window.onload=function(){
    document.getElementById("point-button").onclick = function() {submitPoint()};
    var rPotVal = document.getElementById("rPotVal");
    var lPotVal = document.getElementById("lPotVal");
    /*var toggleLEDBtn = document.getElementById("toggleLED");
    toggleLEDBtn.addEventListener("click", toggle);*/
    //var pointVal = document.getElementById("pointVal");
    //createPlot();
}

var LEDStatus = 0;
var dataset = [];
var unParsedData = "";
var rightReading = "";
var leftReading = "";
var prevx;
var prevy;
var qr; //q2_a in arduino code
var ql; //q1_a in arduino code
var x_a;
var y_a;
var point;
/*var canvas = document.getElementById("whiteboard");
var ctx = canvas.getContext('2d');
ctx.strokeStyle = 'rgba(63, 116, 191, 0.05)';
ctx.scale(3, 3);*/

//sockets
socket.on('updatePot', function(data){
    unParsedData += data;
    if(unParsedData.charAt(0) === "B"){
        console.log("in if x:" + unParsedData);
        rightReading = unParsedData.substring(unParsedData.indexOf('B')+1);
        //document.getElementById("rPotVal").textContent = rightReading;
        console.log("x: " + rightReading);
        dataset.push([rightReading,leftReading]); 
        unParsedData = ""; //reset string that receives data
        update(); //update d3 scatterplot
    }
    //left potentiometer reading
    if(unParsedData.charAt(0) === "C"){    
        console.log("in if y:" + unParsedData);
        leftReading = unParsedData.substring(unParsedData.indexOf('C')+1);
        console.log("y: " + leftReading);
        //document.getElementById("lPotVal").textContent = leftReading;
        unParsedData = "";
        dataset.push([rightReading,leftReading]);
        update(); //update d3 scatterplot
    }
});

//toggle led
function toggle(){
    LEDStatus ^= 1; //XOR with 1 to toggle bit 
    console.log(LEDStatus);
    socket.emit('update LED', LEDStatus);
}

//get desired point

function submitPoint(){
    console.log("submit point");
    var desPx =  document.getElementById('point-inputx').value;
    var desPy =  document.getElementById('point-inputy').value;
    console.log(desPx + " " + desPy);
}

//dropdowns
function changeMode(mode){
    var hashCode;
    if (mode == "elastic"){
        console.log("sending elastic");
        hashCode = '0';
    }
    if (mode == "muck"){
        console.log("sending muck");
        hashCode = '1';
    }
    if (mode == "circle"){
        console.log("sending circle");
        hashCode = '2';
    }
    if (mode == "wall"){
        console.log("sending wall");
        hashCode = '3';
    }
    socket.emit('selected mode', hashCode);
}

function changeShape(shape){
    var hashCode;
    if (shape == "flower"){
        console.log("sending flower");
        hashCode = '4';
    }
    /*if (shape == "circle"){
        console.log("sending circle");
        hashCode = '5';
    }*/
    if (shape == "fig-eight"){
        console.log("sending figure eight");
        hashCode = '5';
    }
    if (shape == "spiral"){
        console.log("sending spiral");
        hashCode = '6';
    }
    socket.emit('selected shape', hashCode);
}

//canvas drawing
function drawLine(fromx, tox, fromy, toy){
        console.log("fromx: " + fromx + "fromy: " + fromy + "tox: " + tox + "toy: " + toy);
        fromx = parseInt(fromx);
        tox = parseInt(tox);
        fromy = parseInt(fromy);
        toy = parseInt(toy);
        ctx.moveTo(fromx, fromy);
        ctx.lineTo(tox, toy);
        ctx.stroke();
}

//=============MATH FOR CALCULATING POSITION==========================================
var cosThetaL = 0.5735764; //cos(55)
var cosThetaR = 0.6427876; //cos(50)
var sinThetaL = 0.8191520; //sin(55)
var sinThetaR = 0.7660444; //sin(50)
var arm = 8;
var B = 24.5;


function getCoords(rightReading, leftReading){
    console.log("rightReading: " + rightReading + " leftReading: " + leftReading);
    var point = [];
    var ql = Math.abs(calcQ(leftReading));
    var qr = Math.abs(calcQ(rightReading));
    console.log("ql: " + ql);
    console.log("qr: " + qr);
    var pl = calcPl(ql);
    var pr = calcPr(qr);
    console.log("pl: " + pl);
    console.log("pr: " + pr);
    point = dKin(pl,pr);
    console.log("point: " + point);
    return point;
}

function calcQ(potReading)
{
  if(potReading <= 786){
    q = 0.8 - ((786-potReading)/240);
  }
  else if(potReading <= 827){
    q = 1.5 - ((827-potReading)/82);
  }
  else{
    q = 5.5 - ((939-potReading)/38);
  }
  return q;
}

function calcPl(ql){
    var pl = [ql*cosThetaL, ql*sinThetaL]; 
    return pl;
}

function calcPr(qr){
    var pr = [B - qr*cosThetaR, qr*sinThetaR];
    return pr;
}

function dKin(pl,pr){ 
    var p3 = [pr[0] - pl[0], pr[1] - pl[1]];
    var mag = Math.sqrt(Math.pow(p3[0],2) + Math.pow(p3[1],2));
    var u = [p3[0]/mag, p3[1]/mag];
    console.log("unit vec: " + u);
    var uTran = [-u[1], u[0]];
    var halfBase = [(u[0]*(mag/2)+ pl[0]),(u[1]*(mag/2)+pl[1])];
    var l = Math.sqrt(Math.pow(arm,2) + Math.pow((mag/2), 2));
    var p = [l*uTran[0] + halfBase[0], l*uTran[1] + halfBase[1]];
    return p;
}


//===========================================D3 scatter plot========================================================

// Setup settings for graphic
var canvas_width = 500;
var canvas_height = 300;
var padding = 30;  // for chart edges
var xmin = 9;
var ymin = 8;
var xmax = 16;
var ymax = 15;

// Create scale functions
var xScale = d3.scale.linear()  // xScale is width of graphic
        .domain([xmin, xmax])
        .range([padding, canvas_width - padding * 2]); // output range

var yScale = d3.scale.linear()  // yScale is height of graphic
        .domain([ymin,ymax])
        .range([canvas_height - padding, padding]);  // remember y starts on top going down so we flip

//function createPlot(){

// Define X axis
var xAxis = d3.svg.axis()
    .scale(xScale)
    .orient("bottom")
    .ticks(10);

// Define Y axis
var yAxis = d3.svg.axis()
    .scale(yScale)
    .orient("left")
    .ticks(10);

// Create SVG element
var svg = d3.select("#scatter-plot")  // This is where we put our vis
    .append("svg")
    .attr("width", canvas_width)
    .attr("height", canvas_height)

// Create Circles
svg.selectAll("circle")
    .data(dataset)
    .enter()
    .append("circle")  // Add circle svg
    .attr("cx", function(d) {
        return xScale(d[0]);  // Circle's X
    })
    .attr("cy", function(d) {  // Circle's Y
        return yScale(d[1]);
    })
    .attr("r", 2)
    .on("mouseover", function(d) {      
            tooltip.transition()        
                .duration(50)      
                .style("opacity", .9);      
            div .html(xScale(d[0]) + "<br/>"  + yScale(d[1]))  
                .style("left", (d3.event.pageX) + "px")     
                .style("top", (d3.event.pageY - 28) + "px");    
            })                  
        .on("mouseout", function(d) {       
            tooltip.transition()        
                .duration(50)      
                .style("opacity", 0);   
        });  // radius

// Add to X axis
svg.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + (canvas_height - padding) +")")
    .call(xAxis);

// Add to Y axis
svg.append("g")
    .attr("class", "y axis")
    .attr("transform", "translate(" + padding +",0)")
    .call(yAxis);
//}

function update() {
    // Update scale domains
    xScale.domain([xmin, xmax]);

    yScale.domain([ymin,ymax]);

    //render newly added elements of array
    var dataSelection = svg.selectAll("circle")
                            .data(dataset);

    dataSelection.enter()
        .append("circle")  // Add circle svg
        .attr("cx", function(d) {
            return xScale(d[0]);  // Circle's X
        })
        .attr("cy", function(d) {  // Circle's Y
            return yScale(d[1]);
        })
        .attr("fill", "blue")
        .attr("r", 6.5);

// Update circles
    svg.selectAll("circle")
        .data(dataset)  // Update with new data
        .transition()  // Transition from old to new
        .attr("cx", function(d) {
            return xScale(d[0]);  // Circle's X
        })
        .attr("cy", function(d) {
            return yScale(d[1]);  // Circle's Y
        })
        .each("end", function() {  // End animation
            d3.select(this)  // 'this' means the current element
            .transition()
            .duration(500)
            .attr("fill", "#ff3399")  // Change color
            .attr("r", 5);  // Change radius
        });

    // Update X Axis
    svg.select(".x.axis")
        .transition()
        .duration(1000)
        .call(xAxis);

    // Update Y Axis
    svg.select(".y.axis")
       .transition()
       .duration(100)
       .call(yAxis);
}

