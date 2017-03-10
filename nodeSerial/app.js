
var express = require('express');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);
var SerialPort = require('serialport');
var port = new SerialPort("COM5", {   //*change this to COM port arduino is on
      baudrate: 9600,
      // defaults for Arduino serial communication
       dataBits: 8, 
       parity: 'none', 
       stopBits: 1, 
       flowControl: false 
  });

var mode;
var shape;
//set up server=====================================================================
var fs = require( 'fs' );

var data_file = 'data/data.txt';

server.listen(8080);
console.log('port started on localhost:8080');

app.use(express.static('public'));

//sockets========================================================================
io.on('connection', function (socket) {
    //socket.emit('news', { hello: 'world' });

        socket.on('update LED', function(data){
            console.log("LED status: " + data);
            port.write(data + "E", function(err){
                if (err) {
                  return console.log('Error on write: ', err.message);
                }
            });
        });
        socket.on('selected mode', function (data) {
            console.log("selected mode: " + data);
            mode = data;
        });
        socket.on('selected shape', function(data){
            console.log("selected shape: " + data);
            shape = data;
        });
});

//serial port communication ==========================================================
var receivedData = "";
// list serial ports:
port.on('open', function() {
    console.log('Serial Port Opened');

    port.write('main screen turn on', function(err) {
        if (err) {
            return console.log('Error on write: ', err.message);
        }
        console.log('message written');
    });

    /*port.on('data', function(data){
      console.log(data[0]);
      var text = data[0];
      fs.writeFile( data_file, text );
    });*/
});

// open errors will be emitted as an error event
port.on('error', function(err) {
  console.log('Error: ', err.message);
});


