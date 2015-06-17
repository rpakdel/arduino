var fs = require('fs');

var PirLogger = function (logFile) {
  this.logFile = logFile;
};

PirLogger.prototype.log = function (date, status) {
  var data = date + ", " + status + "\r\n";
  fs.appendFileSync(this.logFile, data);
};

PirLogger.prototype.getLogs = function () {
  return fs.readFileSync(this.logFile, { encoding: 'ascii' });
}

PirLogger.prototype.getLogEntryValuePart = function (l) {
  if (l.length == 0) {
    return -1;
  }
  var split = l.split(',');
  if (split.length == 2) {
    return parseInt(split[1]);
  }
  return -1;
}

PirLogger.prototype.getLogEntryTimePart = function (l) {
  if (l.length == 0) {
    return 0;
  }
  var split = l.split(',');
  if (split.length == 2) {
    return parseInt(split[0]);
  }
  return 0;
}

module.exports.PirLogger = PirLogger;