var socket = io.connect('http://localhost:8080');
socket.on('news', function (data) {
    console.log(data);
});

//dropdowns
function changeMode(dirDiff){
    //console.log(dirDiff);
    var hashCode;
    if (dirDiff == "haptic"){
        console.log("sending haptic");
        hashCode = '0';
    }
    if (dirDiff == "autonomous"){
        console.log("sending autonomous");
        hashCode = '1';
    }
    if (dirDiff == "line"){
        console.log("sending line");
        hashCode = '2';
    }
    if (dirDiff == "circle"){
        console.log("sending circle");
        hashCode = '3';
    }
    if (dirDiff == "fig-eight"){
        console.log("sending figure eight");
        hashCode = '4';
    }
    socket.emit('selected mode', hashCode);
}
