var Control = class {
    constructor() {
        this.mIsTouching = false;
        this.mTouchDelta = { x: 0, y: 0 };
    }

    init() {
        // first try the gyroscope
        this.enableGyro();
        // enable touch joystick
        //this.enableTouch();

        // get gps coord every 3 seconds
        setInterval(this.getGps, 3000);
    }

    enableGyro() {
        gyro.frequency = 100;
        var self = this;
        gyro.startTracking(function (o) {
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
                // map to -255,255
                var alphaMapped = Math.round(self.mapRange([-90, 90], [-255, 255], alpha));
                delta.x = alphaMapped;

                var e = document.getElementById("alpha");
                if (e) {
                    
                    e.innerText = alpha + ", " + alphaMapped;
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

                var betaMapped = Math.round(self.mapRange([-60, -5], [-255, 255], beta));
                delta.y = betaMapped;

                var e = document.getElementById("beta");
                if (e) {
                    e.innerText = beta + ", " + betaMapped;
                }
            }

            if (o.gamma) {
                var e = document.getElementById("gamma");
                if (e) {
                    var gamma = Math.round(o.gamma);
                    if (gamma == 360) {
                        gamma = 0;
                    }
                    e.innerText = gamma;
                }
            }

            if (o.x) {
                var e = document.getElementById("x");
                if (e) {
                    e.innerText = o.x;
                }
            }

            if (o.y) {
                var e = document.getElementById("y");
                if (e) {
                    e.innerText = o.y;
                }
            }

            if (o.z) {
                var e = document.getElementById("z");
                if (e) {
                    e.innerText = o.z;
                }
            }

            if (o.alpha && o.beta) {
                self.sendJoyDelta(self, delta);
            }
        });
    }

    mapRange(from, to, s) {
        return to[0] + (s - from[0]) * (to[1] - to[0]) / (from[1] - from[0]);
    }

    enableTouch() {
        if (!VirtualJoystick.touchScreenAvailable()) {
            console.log("Touchscreen is not available.");
        } else {
            console.log("Touchscreen is available.");
        }

        var c = document.getElementById('container');
        this.mJoystick = new VirtualJoystick({
            container: c,
            mouseSupport: true,
        });

        var self = this;
        this.mJoystick.addEventListener('touchStart', function () {
            self.mIsTouching = true;
        });

        this.mJoystick.addEventListener('touchEnd', function () {
            self.mIsTouching = false;
        });

        // send joystick 
        setInterval(function () { self.sendTouchDelta(self); }, 1 / 30 * 1000);
    }

    sendTouchDelta(self) {

        if (!self.mJoystick) {
            return;
        }

        var dx = self.mJoystick.deltaX();
        var dy = -self.mJoystick.deltaY();

        self.sendJoyDelta(self, { x: dx, y: dy });
    }

    sendJoyDelta(self, newDelta) {

        if (self.mTouchDelta.x != newDelta.x || self.mTouchDelta.y != newDelta.y) {

            self.mTouchDelta.x = newDelta.x;
            self.mTouchDelta.y = newDelta.y;

            var xhr = new XMLHttpRequest();
            xhr.open('PUT', 'api/v1/bot/0/j');
            xhr.setRequestHeader('Content-Type', 'application/json');
            xhr.send(JSON.stringify(self.mTouchDelta));
        }
    }

    getGps() {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', 'api/v1/bot/0/l');
        xhr.onload = function () {
            if (xhr.status === 200) {
                console.log(xhr.responseText);
            } else {
                console.log('Request failed.  Returned status of ' + xhr.status);
            }
        };
        xhr.send();
    }
}
