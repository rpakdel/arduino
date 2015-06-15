var net = require('net');

var PirClient = function (port, ip, checkIntervalPeriod) {
  this.port = port;
  this.ip = ip;
  this.checkIntervalPeriod = checkIntervalPeriod;
  this.connected = false;
  this.detected = false;
  this.error = null;
  this.date = null;
  this.prevDetected = false;
}

PirClient.prototype.connect = function () {
  var self = this;
  console.log('Connecting to PIR');
  var client = net.connect(this.port, this.ip);

  client.on('connect', function () {
    self.connected = true;
    console.log('Connected to PIR');
    client.write('PIR');
  });
  
  client.on('data', function (data) {
    var buff = new Buffer(data);
    var value = buff.toString();
    console.log('Received: ' + value);
    self.prevDetected = self.detected;
    if (value) {
      self.detected = (value == "1");
    }
    if (self.prevDetected != self.detected) {
      self.date = Date.now();
    }
  });
  
  client.on('close', function () {
    self.connected = false;
    console.log('Connection closed');
  });
  
  client.on('error', function (ex) {
    self.error = ex;
    console.log(ex);
  });
}

PirClient.prototype.getStatus = function () {
  return {
    connected: this.connected,
    detected: this.detected,
    date: this.date,
    error: this.error
  };
}

PirClient.prototype.start = function () {
  setInterval(this.connect.bind(this), this.checkIntervalPeriod);
}

module.exports = PirClient;