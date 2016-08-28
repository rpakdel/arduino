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

        // get whether bot is online every 1 second
        this.getIsOnline();
        setInterval(this.getIsOnline, 1000);

        // get gps coord every 3 seconds
        this.getGps();
        setInterval(this.getGps, 3000);
    }

    touchStart(evt) {
        var e = document.getElementById("touch");
        e.innerHTML = "Touching";
        this.mIsTouching = true;
        this.mTouchDelta = { x: 0, y: 0 };
    }

    touchEnd(evt) {
        var e = document.getElementById("touch");
        e.innerHTML = "Not touching";
        this.mIsTouching = false;
        this.mTouchDelta = { x: 0, y: 0 };
    }

    enableGyro() {

        var el = document.getElementById("canvas");
        if (el) {
            el.addEventListener("touchstart", this.touchStart, false);
            el.addEventListener("touchend", this.touchEnd, false);
        }

        gyro.frequency = 100;
        var self = this;
        gyro.startTracking(function (o) {
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
                // map to -255,255
                var alphaMapped = Math.round(self.mapRange([-90, 90], [-255, 255], alpha));
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

        //this.mJoystick.addEventListener('touchStart', () => { this.mIsTouching = true; });
        //this.mJoystick.addEventListener('touchEnd', () => { this.mIsTouching = false; });

        // send joystick 
        setInterval(this.sendTouchDelta, 1 / 30 * 1000);
    }

    sendTouchDelta() {

        if (!this.mJoystick) {
            return;
        }

        var dx = this.mJoystick.deltaX();
        var dy = -this.mJoystick.deltaY();

        this.sendJoyDelta(self, { x: dx, y: dy });
    }

    sendJoyDelta(self, newDelta) {


        //if (!self.mIsTouching) {
        //    newDelta.x = 0;
        //    newDelta.y = 0;
        //}

        var cjx = document.getElementById('jx');
        cjx.innerText = newDelta.x;
        var cjy = document.getElementById('jy');
        cjy.innerText = newDelta.y;

        if (self.mTouchDelta.x != newDelta.x || self.mTouchDelta.y != newDelta.y) {

            self.mTouchDelta.x = newDelta.x;
            self.mTouchDelta.y = newDelta.y;

            var xhr = new XMLHttpRequest();
            xhr.open('PUT', 'api/v1/bot/0/joy');
            xhr.setRequestHeader('Content-Type', 'application/json');
            xhr.send(JSON.stringify(self.mTouchDelta));
        }
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
            if (xhr.status === 200) {
                var gpsData = JSON.parse(xhr.response);
                var lngE = document.getElementById("lng");
                if (gpsData.valid) {
                    lngE.innerText = gpsData.lng;
                } else {
                    lngE.innerText = "Inv";
                }

                var latE = document.getElementById("lat");
                if (gpsData.valid) {
                    latE.innerText = gpsData.lat;
                } else {
                    latE.innerText = "Inv";
                }

            } else {
                console.log('Request failed.  Returned status of ' + xhr.status);
            }
        };
        xhr.send();
    }
}
