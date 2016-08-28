"use strict";
const dgram = require('dgram');
const SERVER_PORT = 6969;

var Bot = class {
    constructor(id, station_ip, station_port) {

        this.self = this;
        this.mId = id;
        this.mIsOnline = false;
        this.mStationIp = station_ip;
        this.mStationPort = station_port;
        this.mJoy = {
            "x": 0,
            "y": 0
        };

        this.mServer = dgram.createSocket('udp4');

        this.mServer.on('close', () => {
            this.mIsOnline = false;
        });

        this.mServer.on('message', (message, remote) => {
            this.mIsOnline = true;
            console.log(remote.address + ':' + remote.port + ' - ' + message);
            var str = message.toString();
            if (str.length > 0 && str[0] == 'l') {
                var l = str.substr(1);
                this.mLoc = JSON.parse(l);
                console.log(l);
            }
        });
        
        this.mServer.on('listening', () => {
            var address = this.mServer.address();
            console.log('UDP Server listening on ' + address.address + ":" + address.port);
        });

        this.mServer.bind(SERVER_PORT);
    }

    get loc() {
        return JSON.stringify(this.mLoc);
    }

    get joy() {
        return this.mJoy;
    }

    get isOnline() {
        return this.mIsOnline;
    }

    sendJoyToBase() {
        var message = "j" + JSON.stringify(this.mJoy);
        console.log(message);
        this.mServer.send(message, 0, message.length, this.mStationPort, this.mStationIp, (err) => {
            if (err) {
                console.log(err);
                this.mIsOnline = false;
            } else {
                this.mIsOnline = true;
            }
        });
    }

    set joy(j) {
        this.mJoy = j;
        this.sendJoyToBase();
    }
}



module.exports = Bot;