var socket = io.connect('http://localhost:8080');

window.onload=function(){
    var toggleLEDBtn = document.getElementById("toggleLED");
    toggleLEDBtn.addEventListener("click", toggle);
    var rPotVal = document.getElementById("rPotVal");
    var lPotVal = document.getElementById("lPotVal");
}

var LEDStatus = 0;
var dataset = [];//[[0,0]];
var unParsedData = "";
var rightReading = "";
var leftReading = "";
var prevRReading;
var canvas = document.getElementById("whiteboard");
var ctx = canvas.getContext('2d');
ctx.strokeStyle = 'rgba(63, 116, 191, 0.05)';

//sockets
socket.on('updatePot', function(data){
    unParsedData += data;
    if(unParsedData.charAt(0) === "B" && unParsedData.slice(-1) === "E"){
        rightReading = unParsedData.substring(unParsedData.indexOf('B') + 1, unParsedData.indexOf('E'));
        document.getElementById("rPotVal").textContent = rightReading;
        dataset.push([rightReading,leftReading]); 
        drawLine(prevRReading, rightReading, 50, 50); //hard coded in y-coordinates for canvas drawings for now
        prevRReading = rightReading;
        unParsedData = "";
        update(); //update d3 scatterplot
    }
    if(unParsedData.charAt(0) === "C" && unParsedData.slice(-1) === "F"){
        console.log("parsing left reading");
        leftReading = unParsedData.substring(unParsedData.indexOf('C') + 1, unParsedData.indexOf('F'));
        document.getElementById("lPotVal").textContent = leftReading;
        unParsedData = "";
    }
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

//=============MATH FOR CALCULATING POSITION==========================================
function calculate_q1(L_reading)
{
  var q1 =  -(((867 - L_reading) / 34) + 3);

  return q1;
}

function calculate_q2(R_reading)
{
  var q2 = ((875 - R_reading) / 32) + 3;

  return q2;
}

function direct_kin_x (q1, q2){
    var x = 0;
    x = (q1 + q2) / 2;
    return x;    
 }

function direct_kin_y (q1, q2){
    var y = 0, delta_q = 0;
    delta_q = (abs(q1 - q2) / 2);
    y = Math.sqrt(Math.pow(L,2) - Math.pow(delta_q,2));

    return y;
 }

//==========================D3 scatter plot========================================================
/*var dataset = [ [5, 10], [13, 19], [21, 25], [22, 18], [15, 13],
                [11, 12], [15, 20], [18, 17], [16, 18], [23, 25] ];*/

// Setup settings for graphic
var canvas_width = 500;
var canvas_height = 300;
var padding = 30;  // for chart edges

// Create scale functions
var xScale = d3.scale.linear()  // xScale is width of graphic
        .domain([0, 1023])
        .range([padding, canvas_width - padding * 2]); // output range

var yScale = d3.scale.linear()  // yScale is height of graphic
        .domain([0, d3.max(dataset, function(d) {
            return d[1];  // input domain
        })])
        .range([canvas_height - padding, padding]);  // remember y starts on top going down so we flip

// Define X axis
var xAxis = d3.svg.axis()
    .scale(xScale)
    .orient("bottom")
    .ticks(5);

            // Define Y axis
var yAxis = d3.svg.axis()
    .scale(yScale)
    .orient("left")
    .ticks(5);

// Create SVG element
var svg = d3.select("h3")  // This is where we put our vis
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
    .attr("r", 2);  // radius

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

function update() {
    // Update scale domains
    console.log("time to update");
    xScale.domain([0, 1023]);

    yScale.domain([0, d3.max(dataset, function(d) {
        return d[1]; 
    })]);

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
        .attr("r", 3);

// Update circles
    svg.selectAll("circle")
        .data(dataset)  // Update with new data
        .transition()  // Transition from old to new
        .duration(1000)  // Length of animation
        .each("start", function() {  // Start animation
                d3.select(this)  // 'this' means the current element
                    .attr("fill", "red")  // Change color
                    .attr("r", 5);  // Change size
        })
        .delay(function(d, i) {
            return i / dataset.length * 500;  // Dynamic delay (i.e. each item delays a little longer)
        })
        //.ease("linear")  // Transition easing - default 'variable' (i.e. has acceleration), also: 'circle', 'elastic', 'bounce', 'linear'
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
            .attr("fill", "black")  // Change color
            .attr("r", 2);  // Change radius
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

var testy = 0;

/*setInterval(function(){
    console.log("time to update");
    dataset.push([testy,12]);
    testy++;
    update();
}, 1000); */

//===========================D3 BAR GRAPH=======================================================

/*createBarChart(dataArr, dataset2);

function createBarChart(dataset1, dataset2){

var dataset = [dataset1[dataset1.length - 1], dataset2[dataset2.length - 1]];
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
}*/