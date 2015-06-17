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
  this.chart = null;
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

ViewModel.prototype.refreshChart = function () {
  var self = this;
  var request = new XMLHttpRequest();
  request.open('GET', '/pir/api/histogram', true);
  
  request.onload = function () {
    if (request.status >= 200 && request.status < 400) {
      var data = JSON.parse(request.responseText);
      for (var i = 0; i < 23; i++) {
        var v = data[i];
        self.chart.datasets[0].points[i].value = v;
      }
      self.chart.update();
    }
  };
  
  request.onerror = function () {
  };
  
  request.send();  
}

ViewModel.prototype.createChart = function () {
  // Get the context of the canvas element we want to select
  var ctx = document.getElementById("myChart").getContext("2d");
  var data = {
    labels: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23],
    datasets: [
      {
        scaleShowGridLines: false,
        scaleShowHorizontalLines: false,
        scaleShowVerticalLines: false,
        bezierCurve: false,
        label: "My First dataset",
        fillColor: "rgba(220,220,220,0.2)",
        strokeColor: "rgba(220,220,220,1)",
        pointColor: "rgba(0,0,0,1)",
        pointStrokeColor: "#fff",
        pointHighlightFill: "#fff",
        pointHighlightStroke: "rgba(220,220,220,1)",
        data: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
      }
    ]
  };
  Chart.defaults.global.scaleFontColor = "rgba(0,0,0,1)";
  Chart.defaults.global.scaleShowGridLines = false;
  Chart.defaults.global.scaleShowHorizontalLines = false;
  Chart.defaults.global.scaleShowVerticalLines = false;
  Chart.defaults.global.bezierCurve = false;
  this.chart = new Chart(ctx).Line(data);
}

ViewModel.prototype.start = function () {
  this.checkPir();
  this.createChart();
  this.refreshChart();
  setInterval(this.checkPir.bind(this), 1000);
  setInterval(this.refreshChart.bind(this), 5000);
};

