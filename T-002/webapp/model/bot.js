"use strict";
const dgram = require('dgram');
const SERVER_PORT = 6969;

var Bot = class {
    constructor(id, station_ip, station_port) {
        this.mThis = this;
        this.mId = id;
        this.mStationIp = station_ip;
        this.mStationPort = station_port;
        this.mJoy = {
            "x": 0,
            "y": 0
        };

        this.mServer = dgram.createSocket('udp4');

        var t = this;
        this.mServer.on('message', function (message, remote) {
            console.log(remote.address + ':' + remote.port + ' - ' + message);
            var str = message.toString();
            if (str.length > 0 && str[0] == 'l') {
                var l = str.substr(1);
                t.mLoc = l;
            }
        });
        
        this.mServer.on('listening', function () {
            var address = t.mServer.address();
            console.log('UDP Server listening on ' + address.address + ":" + address.port);
        });

        this.mServer.bind(SERVER_PORT);
    }

    get loc() {
        return this.mLoc;
    }

    get joy() {
        return this.mJoy;
    }

    set joy(j) {
        this.mJoy = j;
        var message = "j" + JSON.stringify(j);
        console.log(message);
        this.mServer.send(message, 0, message.length, this.mStationPort, this.mStationIp, (err) => {
            if (err) {
                console.log(err);
            }
        });
    }
}



module.exports = Bot;