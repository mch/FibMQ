// partially stolen from  http://teddziuba.com/2011/10/node-js-is-cancer.html

function fibonacci(n) {
    if (n < 2)
        return 1;
    else
        return fibonacci(n-2) + fibonacci(n-1);
}

var http = require("http");
var zmq = require("zmq");

var socket = zmq.createSocket('req');
socket.connect("tcp://127.0.0.1:5555");

function blockingServer()
{
    http.createServer(function (req, res) {
        res.writeHead(200, {'Content-Type': 'text/plain'});
        res.end(fibonacci(40).toString());
    }).listen(1337, "127.0.0.1");    
}

function zmqServer()
{
    http.createServer(function (req, res) {
        socket.send("40");
        socket.on('message', function(data) {
            res.writeHead(200, {'Content-Type': 'text/plain'});
            res.end(data.toString());
        })
    }).listen(1337, "127.0.0.1");
}

//blockingServer();
zmqServer();
