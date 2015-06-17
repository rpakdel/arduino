var express = require('express');
var router = express.Router();

var pirClient = {};

var setPirClient = function (p) {
  pirClient = p;
}

/* GET home page. */
router.get('/', function (req, res) {
  res.render('pir', { title: 'Wireless PIR' });
});

router.get('/api/status', function (req, res) {
  res.json(pirClient.getStatus());
});

router.get('/api/logs', function (req, res) {
  res.json(pirClient.getLogs());
});

router.get('/api/histogram', function (req, res) {
  res.json(pirClient.getHistogram());
});


module.exports.setPirClient = setPirClient;
module.exports.router = router;