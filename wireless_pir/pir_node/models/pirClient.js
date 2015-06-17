var net = require('net');

var PirClient = function (port, ip, checkIntervalPeriod, logger) {
  this.port = port;
  this.ip = ip;
  this.checkIntervalPeriod = checkIntervalPeriod;
  this.connected = false;
  this.detected = 0;
  this.error = null;
  this.date = Date.now();
  this.duration = 0;
  this.prevDetected = false;
  this.logger = logger;
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
      if (value == "1") {
        self.detected = 1;
      }
      else if (value == "0") {
        self.detected = 0;
      }
    }
    if (self.prevDetected != self.detected) {
      self.date = Date.now();
      self.logger.log(self.date, self.detected);
    }
    self.error = null;
    self.duration = Date.now() - self.date;
  });
  
  client.on('close', function () {
    self.connected = false;
    console.log('Connection closed');
  });
  
  client.on('error', function (ex) {
    self.error = ex;
    self.detected = -1;
    console.log(ex);
  });
}

PirClient.prototype.getStatus = function () {
  return {
    pirIp: this.ip,
    pirPort: this.port,
    connected: this.connected,
    detected: this.detected,
    date: this.date,
    duration: this.duration,
    error: this.error
  };
}

PirClient.prototype.getLogs = function () {
  var data = this.logger.getLogs();
  var lines = data.split('\r\n');
  var len = lines.length;
  var result = [];
  
  var pushLine = function (l) {
    if (l.length == 0) {
      return;
    }
    result.push(l);
  }

  if (len > 0) {
    pushLine(lines[0]);
    for (var i = 1; i < len; i++) {
      var pl = lines[i - 1];
      var cl = lines[i];
      var plVal = this.logger.getLogEntryValuePart(pl);
      var clVal = this.logger.getLogEntryValuePart(cl);
      if (clVal != -1 && clVal != plVal) {
        pushLine(cl);
      }
    }
  }
  return result;
}

PirClient.prototype.getHistogram = function () {
  var lines = this.getLogs();
  // initialize
  var result = {};
  for (var i = 0; i < 24; i++) {
    result[i] = 0;
  }
  var len = lines.length;
  for (var j = 0; j < len; j++) {
    var v = this.logger.getLogEntryValuePart(lines[j]);
    if (v == 1) {
      var t = this.logger.getLogEntryTimePart(lines[j]);
      var d = new Date(t);
      var h = d.getHours();
      result[h] += 1;
    }
  }
  return result;
}

PirClient.prototype.start = function () {
  setInterval(this.connect.bind(this), this.checkIntervalPeriod);
}

module.exports = PirClient;