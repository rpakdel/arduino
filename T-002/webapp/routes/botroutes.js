var express = require('express');
var router = express.Router();
var Bot = require('../model/bot');

const STATION_IP = '192.168.1.27';
const STATION_PORT = 6969;

var mBot = new Bot(0, STATION_IP, STATION_PORT);

// GET bot location
router.get('/:id/l', function (req, res) {
    res.send(mBot.loc);
});

// PUT bot joy
router.put('/:id/j', function (req, res) {
    mBot.joy = req.body;
    res.send({ status: 200 });
});

module.exports = router;