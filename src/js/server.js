// partially stolen from  http://teddziuba.com/2011/10/node-js-is-cancer.html

var http = require("http");
var zmq = require("zmq");
var events = require("events");
var url = require('url');

function fibonacci(n) {
    if (n < 2)
        return 1;
    else
        return fibonacci(n-2) + fibonacci(n-1);
}

function MessageDispatcher() 
{
    events.EventEmitter.call(this);
    this._nextSerial = 0;
    this._serialCallbacks = {};

    return this;
}

MessageDispatcher.prototype = Object.create( events.EventEmitter.prototype );

MessageDispatcher.prototype.execFib = function(n, callback) 
{
    var serial = this._nextSerial;
    this._nextSerial++;

    var msg = serial.toString() + " " + n.toString();
    this._serialCallbacks[serial] = callback;
    socket.send(msg);
}

MessageDispatcher.prototype.dispatchMessage = function(data)
{
    console.log(data.toString());
    var dataList = data.toString().split(" ");
    if (dataList.length != 2)
        return;

    var serial = dataList[0];
    var body = dataList[1];

    if (serial in this._serialCallbacks)
    {
        var callback = this._serialCallbacks[serial];
        callback(body);
    }
}

var socket = zmq.createSocket('req');
socket.connect("tcp://127.0.0.1:5555");

socket.on('error', function(error) {
    console.error(error.toString());
});

function blockingServer()
{
    http.createServer(function (req, res) {
        res.writeHead(200, {'Content-Type': 'text/plain'});
        res.end(fibonacci(40).toString());
    }).listen(1337, "127.0.0.1");    
}

function zmqServer()
{
    var dispatcher = new MessageDispatcher;
    socket.on('message', function(data) {
        dispatcher.dispatchMessage(data)
    });

    http.createServer(function (req, res) {
        var params = url.parse(req.url, true);
        var queryParams = params['query'];
        if ('n' in queryParams)
        {
            var n = params['query']['n'];
            dispatcher.execFib(n, function(data) {
                res.writeHead(200, {'Content-Type': 'text/plain'});
                res.end(data.toString());
            })
        }
        else
        {
            res.writeHead(400);
            res.end();
        }
    }).listen(1337, "127.0.0.1");
}

//blockingServer();
zmqServer();
