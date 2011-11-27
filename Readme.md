# Fibonacci via message queues

[Ted Dziuba](http://teddziuba.com/2011/10/node-js-is-cancer.html)
makes a good point that doing any kind of computation on Node.JS
effectivly blocks the server and makes it unable to serve other
clients.  

This is a toy app that uses ZeroMQ to decouple the computation from
the servicing of client requests, allowing Node.js to do what it is
good at and relaying on a set of dumb processes to do what they are
good at.

