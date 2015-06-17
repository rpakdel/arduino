var ViewModel = function () {
  var self = this;
  this.detected = ko.observable(-1);
  this.error = ko.observable();
  this.date = ko.observable();
  this.color = ko.computed(function () {
    var d = self.detected();
    switch (d) {
      case 0:
        return 'lightgreen';
      case 1:
        return 'red';
    }
    return 'orange';
  });
  this.duration = ko.observable(0);
  this.durationHuman = ko.computed(function () {
    var m = moment.duration(self.duration());
    return m.humanize();
  });
  this.durationSeconds = ko.computed(function () {
    return Math.ceil(self.duration() / 1000) + "s";
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
      self.duration(data.duration);
    } else {
      self.detected(-1);
      self.error('error querying pir status');
    }
  };
  
  request.onerror = function () {
  };
  
  request.send();
}

ViewModel.prototype.start = function () {
  setInterval(this.checkPir.bind(this), 1000);
};

