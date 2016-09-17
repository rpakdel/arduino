R"***(
"use string";
var checkTime = function() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'api/v1/time');
    xhr.onload = function () {
        var te = document.getElementById("time");
        
        if (xhr.status === 200) {
            var r = JSON.parse(xhr.response);
            var s = r.time;
            var d = new Date(s*1000); // milliseconds
            te.innerText = d;
        } else {
            te.innerText = "Error";
        }
    }
    xhr.send();
}

checkTime();
setInterval(checkTime, 3000);


var ctx = document.getElementById("bpmChart");

var bpmData1 = [];

var bpmDataSet1 = {
    label: "BPM1",
    data: bpmData1
};

var bpmData = {
    datasets: [ bpmDataSet1 ]
};

var myChart = new Chart(ctx, {
    type: 'line',
    data: bpmData,
    options: {
        scales: {
            xAxes: [{
                type: 'time'
            }]
        }
    }
});



var checkBmp = function(self) {
    var q = 'api/v1/bpm';
    if (self.lastTime) {
        q += '?gt=' + self.lastTime;
    }
        
    var xhr = new XMLHttpRequest();
    xhr.open('GET', q);
    xhr.onload = function () {
        if (xhr.status === 200) {
            var r = JSON.parse(xhr.response);
            for(var i = 0; i < r.length; i++) {
                var h = r[i];
                var dataPoint = {
                    x : new Date(h.time * 1000),
                    y : h.bpm
                };

                self.myChart.data.datasets[0].data.push(dataPoint);
                self.myChart.update();
                if (i == (r.length - 1)) {
                    self.lastTime = h.time;
                }
            }
        } else {
            console.log("Error getting bmp");
        }
    }
    xhr.send();
}

var lastTime = 0;
var self = this;
checkBmp(self);
setInterval(function () { checkBmp(self); }, 5000);



)***"


