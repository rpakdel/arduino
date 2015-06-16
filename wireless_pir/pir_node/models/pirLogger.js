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
};

module.exports.PirLogger = PirLogger;