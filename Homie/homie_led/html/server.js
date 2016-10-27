var express = require('express')
	, http = require('http')
	, path = require('path')
	, mqtt = require('mqtt')
	, bodyParser = require('body-parser');

// create app
var app = express();

var client = mqtt.connect('mqtt://192.168.1.174:1883');

app.set('port', process.env.PORT || 3000);
app.use(express.static(path.join(__dirname, 'public')));
app.set('view engine', 'html');
app.use(bodyParser.urlencoded({
	extended: true
}));

app.use(bodyParser.json());

app.get('/', function(req, res){
	res.render('index');
});

app.post('/publish', function(req, res) {
	var color = req.body.color;
	client.publish('devices/24859-cef/ledstrip/color/set', color);
	res.end();
});

http.createServer(app).listen(app.get('port'), function(){
	console.log("Express server listening on port " + app.get('port'));
});

