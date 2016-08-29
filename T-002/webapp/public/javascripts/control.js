var Control = class {
    constructor() {
        this.mIsTouching = false;
        this.mJoyData = { x: 0, y: 0 };
        this.mAlphaInit = 0;
        this.mGyro = { alpha: 0, beta: 0, gamma: 0 };
    }

    init() {
        // first try the gyroscope
        this.enableGyro();
        // enable touch joystick
        this.enableVJoy();

        // get whether bot is online every 1 second
        this.getIsOnline();
        setInterval(this.getIsOnline, 1000);

        // get gps coord every 3 seconds
        this.getGps();
        setInterval(this.getGps, 3000);
    }

    touchStart(self, evt) {
        var e = document.getElementById("touchcanvas");
        e.style.background = "blue";
        self.mIsTouching = true;
        self.mAlphaInit = self.mGyro.alpha;
        self.mJoyData = { x: 0, y: 0 };
    }

    touchEnd(self, evt) {
        var e = document.getElementById("touchcanvas");
        e.style.background = "yellow";
        self.mIsTouching = false;
        self.mAlphaInit = self.mGyro.alpha;
        self.mJoyData = { x: 0, y: 0 };
    }

    trackGyro(self, o) {
            //var t = document.getElementById("tracking");
            //t.innerText = "Tracking";
            var delta = { x: 0, y: 0 };
            if (o.alpha) {

                var alpha = Math.round(o.alpha);
                if (alpha == 360) {
                    alpha = 0;
                }

                // zero about north
                if (alpha > 180) {
                    alpha = 360 - alpha
                } else {
                    alpha = - alpha;
                }
                // clamp to -90, 90 with 0 in the centre
                if (alpha > 90) {
                    alpha = 90
                } else if (alpha < -90) {
                    alpha = -90;
                }

                self.mGyro.alpha = alpha;
                // take the difference from init
                var alphaDiff = alpha - self.mAlphaInit;
                // map to -255,255
                var alphaMapped = Math.round(self.mapRange([-90, 90], [-255, 255], alphaDiff));
                delta.x = alphaMapped;

                var e = document.getElementById("alpha");
                if (e) {

                    e.innerText = alpha + ", X: " + alphaMapped;
                }
            }

            if (o.beta) {
                var beta = Math.round(o.beta);
                if (beta == 360) {
                    beta = 0;
                }

                // reverse the direction
                beta = -beta;
                // clamp to 0, -90 with -30 in the centre
                if (beta > -5) {
                    beta = -5
                } else if (beta < -60) {
                    beta = -60;
                }

                self.mGyro.beta = beta;
                var betaMapped = Math.round(self.mapRange([-60, -5], [-255, 255], beta));
                delta.y = betaMapped;

                var e = document.getElementById("beta");
                if (e) {
                    e.innerText = beta + ", Y: " + betaMapped;
                }
            }

            if (o.gamma) {
                var e = document.getElementById("gamma");
                if (e) {
                    var gamma = Math.round(o.gamma);
                    if (gamma == 360) {
                        gamma = 0;
                    }

                    self.mGyro.gamma = gamma;
                    e.innerText = gamma;
                }
            }

            if (o.x) {
                var e = document.getElementById("x");
                if (e) {
                    e.innerText = o.x.toFixed(3);
                }
            }

            if (o.y) {
                var e = document.getElementById("y");
                if (e) {
                    e.innerText = o.y.toFixed(3);
                }
            }

            if (o.z) {
                var e = document.getElementById("z");
                if (e) {
                    e.innerText = o.z.toFixed(3);
                }
            }

            if (o.alpha && o.beta) {
                self.sendJoyData(self, delta);
            }
    }

    enableGyro() {

        var el = document.getElementById("touchcanvas");
        if (el) {
            var self = this;
            el.addEventListener("touchstart", function (evt) { self.touchStart(self, evt) }, false);
            el.addEventListener("touchend", function (evt) { self.touchEnd(self, evt) }, false);
        }

        gyro.frequency = 100;
        var self = this;
        gyro.startTracking(function (o) { self.trackGyro(self, o); });
    }

    mapRange(from, to, s) {
        return to[0] + (s - from[0]) * (to[1] - to[0]) / (from[1] - from[0]);
    }

    enableVJoy() {
        if (!VirtualJoystick.touchScreenAvailable()) {
            console.log("Touchscreen is not available.");
        } else {
            console.log("Touchscreen is available.");
        }

        var c = document.getElementById('vjoy');
        this.mJoystick = new VirtualJoystick({
            container: c,
            mouseSupport: true,
            stickRadius: 80,
            limitStickTravel: true
        });

        //this.mJoystick.addEventListener('touchStart', () => { this.mIsTouching = true; });
        //this.mJoystick.addEventListener('touchEnd', () => { this.mIsTouching = false; });

        // send joystick 
        setInterval(this.sendVJoyDelta, 1 / 30 * 1000);
    }

    sendVJoyDelta() {

        if (!this.mJoystick) {
            return;
        }

        var dx = this.mJoystick.deltaX();
        var dy = -this.mJoystick.deltaY();

        this.sendJoyData(self, { x: dx, y: dy });
    }

    sendJoyData(self, joyData) {


        if (!self.mIsTouching) {
            joyData.x = 0;
            joyData.y = 0;
        }

        console.log("Touch: " + self.mIsTouching + ", joy:" + JSON.stringify(joyData));

        var cjx = document.getElementById('jx');
        cjx.innerText = joyData.x;
        var cjy = document.getElementById('jy');
        cjy.innerText = joyData.y;

        //if (self.mJoyData.x != joyData.x || self.mJoyData.y != joyData.y) {

            self.mJoyData.x = joyData.x;
            self.mJoyData.y = joyData.y;

            var xhr = new XMLHttpRequest();
            xhr.open('PUT', 'api/v1/bot/0/joy');
            xhr.setRequestHeader('Content-Type', 'application/json');
            xhr.send(JSON.stringify(self.mJoyData));
        //}
    }

    getIsOnline() {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', 'api/v1/bot/0/isonline');
        xhr.onload = function () {
            var onlineE = document.getElementById("online");
            if (xhr.status === 200) {
                onlineE.innerText = xhr.response;
            } else {
                onlineE.innerText = false;
            }
        }
        xhr.send();
    }

    getGps() {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', 'api/v1/bot/0/loc');
        xhr.onload = function () {
            var lngE = document.getElementById("lng");
            var latE = document.getElementById("lat");

            if (xhr.status === 200) {

                if (xhr.responseText && xhr.responseText.length > 0) {
                    var gpsData = JSON.parse(xhr.response);

                    if (gpsData.valid) {
                        lngE.innerText = gpsData.lng;
                    } else {
                        lngE.innerText = "Inv";
                    }

                    if (gpsData.valid) {
                        latE.innerText = gpsData.lat;
                    } else {
                        latE.innerText = "Inv";
                    }
                }

            } else {
                console.log('Request failed.  Returned status of ' + xhr.status);
            }
        };
        xhr.send();
    }
}
