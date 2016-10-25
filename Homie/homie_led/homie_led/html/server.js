
// most basic dependencies
var express = require('express')
  , http = require('http')
  , os = require('os')
  , path = require('path')
  , url = require('url')
  , mqtt = require('mqtt')
  , bodyParser = require('body-parser');

// create app
var app = express();

app.set('port', process.env.PORT || 3000);
app.set('views', __dirname + '/views');
app.set('view engine', 'html');
app.use(express.static(path.join(__dirname, 'public')));
app.use(bodyParser.urlencoded({
    extended: true
}));

app.use(bodyParser.json());

app.get('/', function(req, res){
  res.render('index');
});

app.post('/publish', function(req, res) {
	
  var client = mqtt.connect('mqtt://192.168.1.174:1883');
  client.on('connect', function() {
  	// todo: change message here. topic, message
  	console.log(req.body.color);
    client.publish('t1', new Date().toString());
  });
});

http.createServer(app).listen(app.get('port'), function(){
  console.log("Express server listening on port " + app.get('port'));
});