var ViewModel = function () {
  var self = this;
  this.detected = ko.observable(false);
  this.error = ko.observable();
  this.date = ko.observable();
  this.color = ko.computed(function () {
    if (self.detected()) {
      return 'lightgreen';
    }
    return 'red';
  });
  this.duration = ko.computed(function() {
      return ((Date.now() - self.date()) / 1000.0).toFixed(0);
  });
};

ViewModel.prototype.checkPir = function () {
  var self = this;
  var request = new XMLHttpRequest();
  request.open('GET', '/pir/api/status', true);
  
  request.onload = function () {
    if (request.status >= 200 && request.status < 400) {
      var data = JSON.parse(request.responseText);
      self.detected(data.detected);
      self.date(new Date(data.date));
    } else {
    // We reached our target server, but it returned an error
    }
  };
  
  request.onerror = function () {
  };
  
  request.send();
}

ViewModel.prototype.start = function () {
  setInterval(this.checkPir.bind(this), 1000);
};

