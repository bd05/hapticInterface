
var express = require('express');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);
var SerialPort = require("serialport").SerialPort;
var port = new SerialPort("/dev/tty.usbmodem1421"); //*change this to COM port arduino is on

server.listen(8080);
console.log('port started on localhost:8080');

app.use("/public", express.static(__dirname + '/public'));

app.get('/', function (req, res) {
  res.sendFile(__dirname + '/index.html');
});

var settings;

//serial port
port.on('open', function() {
    console.log('Serial Port Opened');

    port.write('main screen turn on', function(err) {
        if (err) {
            return console.log('Error on write: ', err.message);
            }
        console.log('message written');
    });

    port.on('data', function(data){
      console.log(data[0]);
    });
});

// open errors will be emitted as an error event
port.on('error', function(err) {
  console.log('Error: ', err.message);
});

//bluetooth
/*var BluetoothSerialPort = require("bluetooth-serial-port").BluetoothSerialPort;
var myBtPort = new BluetoothSerialPort();
var dataBuffer = "";

myBtPort.on('found', function (address, name) {
    console.log('Found: ' + address + ' with name ' + name);

    myBtPort.findSerialPortChannel(address, function(channel) {
        console.log('Found RFCOMM channel for serial port on ' + name + ': ' + channel);
        console.log('Attempting to connect...');

        myBtPort.connect(address, channel, function() {
            console.log('Connected. Receiving data...');

            myBtPort.write(new Buffer('1', 'utf-8'), function(err, count) {
                if (err) console.log('Cannot conect');
            });

            myBtPort.on('data', function(data) {
                console.log('Received: ' + data);
            });

        });
    });
});

myBtPort.on('finished', function() {
    console.log('scan did finish');
});

myBtPort.inquire();*/


io.on('connection', function (socket) {
    socket.emit('news', { hello: 'world' });
        socket.on('selected mode', function (data) {
            console.log("selected mode: " + data);
            settings = data;
            
           /* myBtPort.write(new Buffer(diffSetting, 'utf-8'), function(err, count) {
                if (err) console.log('Cannot conect');
            });*/
        });
});