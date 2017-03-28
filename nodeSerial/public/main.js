var socket = io.connect('http://localhost:8080');

window.onload=function(){
    var toggleLEDBtn = document.getElementById("toggleLED");
    toggleLEDBtn.addEventListener("click", toggle);
    var rPotVal = document.getElementById("rPotVal");
    var lPotVal = document.getElementById("lPotVal");
}

var LEDStatus = 0;
var dataset = [];
var unParsedData = "";
var rightReading = "";
var leftReading = "";
var prevRReading;
var qr; //q2_a in arduino code
var ql; //q1_a in arduino code
var x_a;
var y_a;
var point;
var canvas = document.getElementById("whiteboard");
var ctx = canvas.getContext('2d');
ctx.strokeStyle = 'rgba(63, 116, 191, 0.05)';

//sockets
socket.on('updatePot', function(data){
    unParsedData += data;
    //right potentiometer reading
    if(unParsedData.charAt(0) === "B" && unParsedData.slice(-1) === "E"){
        rightReading = unParsedData.substring(unParsedData.indexOf('B') + 1, unParsedData.indexOf('E'));
        document.getElementById("rPotVal").textContent = rightReading;
        //dataset.push([rightReading,leftReading]); 
        drawLine(prevRReading, rightReading, 50, 50); //hard coded in y-coordinates for canvas drawings for now
        prevRReading = rightReading;
        unParsedData = ""; //reset string that receives data
        //math transforms:
        point = getCoords(rightReading,leftReading);
        console.log(point);
        dataset.push(point);
        update(); //update d3 scatterplot
    }
    //left potentiometer reading
    if(unParsedData.charAt(0) === "C" && unParsedData.slice(-1) === "F"){
        leftReading = unParsedData.substring(unParsedData.indexOf('C') + 1, unParsedData.indexOf('F'));
        document.getElementById("lPotVal").textContent = leftReading;
        unParsedData = "";
        point = getCoords(rightReading,leftReading);
        //console.log(point);
        dataset.push(point);
        update(); //update d3 scatterplot
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
function getCoords(rightReading, leftReading){
    console.log("rightReading: " + rightReading + " leftReading: " + leftReading);
    var point = [];
    var qr = calculate_qr(rightReading);
    var ql = calculate_ql(leftReading);
    var x = direct_kin_x(ql,qr);
    var y = direct_kin_y(ql,qr);
    console.log(y);
    point = [x,y];
    return point;
}

function calculate_ql(L_reading)
{
  var q1 =  -(((867 - L_reading) / 34) + 3);
  console.log("q1: " + q1);
  return q1;
}

function calculate_qr(R_reading)
{
  var q2 = ((875 - R_reading) / 32) + 3;
  console.log("q2: " + q2);
  return q2;
}

function direct_kin_x (q1, q2){
    var x = 0;
    x = (q1 + q2) / 2;
    return x;    
 }

function direct_kin_y (q1, q2){
    var L = 8; //length of arm
    var y = 0, delta_q = 0;
    delta_q = (Math.abs(q1 - q2) / 2);
    var a = Math.pow(L,2);
    var b = Math.pow(delta_q,2);
    var diff = a - b; //comes out neg when it shouldn't
    console.log("diff= " + diff);
    y = Math.sqrt(Math.pow(L,2) - Math.pow(delta_q,2)); //this is messing up
    console.log("y: " + y);
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
        .domain([-5, 5])
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
    .attr("r", 2)
    .on("mouseover", function(d) {      
            tooltip.transition()        
                .duration(200)      
                .style("opacity", .9);      
            div .html(xScale(d[0]) + "<br/>"  + yScale(d[1]))  
                .style("left", (d3.event.pageX) + "px")     
                .style("top", (d3.event.pageY - 28) + "px");    
            })                  
        .on("mouseout", function(d) {       
            tooltip.transition()        
                .duration(500)      
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

function update() {
    // Update scale domains
    xScale.domain([-5, 5]);

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
        .duration(500)  // Length of animation
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
            .attr("fill", "#66ff66")  // Change color
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




