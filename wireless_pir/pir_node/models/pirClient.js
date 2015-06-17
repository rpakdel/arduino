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
  var getVal = function (l) {
    if (l.length == 0) {
      return -1;
    }
    var split = l.split(',');
    if (split.length == 2) {
      return parseInt(split[1]);
    }
    return -1;
  };
  
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
      var plVal = getVal(pl);
      var clVal = getVal(cl);
      if (clVal != -1 && clVal != plVal) {
        pushLine(cl);
      }
    }
  }
  return result;
}

PirClient.prototype.start = function () {
  setInterval(this.connect.bind(this), this.checkIntervalPeriod);
}

module.exports = PirClient;