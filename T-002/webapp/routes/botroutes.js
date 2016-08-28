var express = require('express');
var router = express.Router();
var Bot = require('../model/bot');

const STATION_IP = '192.168.1.27';
const STATION_PORT = 6969;

var mBot = new Bot(0, STATION_IP, STATION_PORT);

// GET bot location
router.get('/:id/loc', function (req, res) {
    res.setHeader('Content-Type', 'application/json');
    res.send(mBot.loc);
});

// GET bot location
router.get('/:id/isonline', function (req, res) {
    res.setHeader('Content-Type', 'application/json');
    res.send(mBot.isOnline);
});

// PUT bot joy
router.put('/:id/joy', function (req, res) {
    mBot.joy = req.body;
    res.send({ status: 200 });
});

module.exports = router;