// Use ZeroMQ and a ZMQ_REP socket to respond to requests to calculate
// a fibbonci number.

#include <zmq.hpp>
#include <iostream>

int fibonacci(int n) {
    if (n < 2)
        return 1;
    else
        return fibonacci(n-2) + fibonacci(n-1);
}

int main(int argc, char** argv)
{
    zmq::context_t context(1);

    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");
    //socket.bind("icp:///tmp/fib/0");

    zmq::message_t message(255);

    while (1)
    {
        if (socket.recv(&message, 0))
        {
            char* data = reinterpret_cast<char*>(message.data());
            int n = atoi(data);

            //int r = fibonacci(n);
            int r = 42;

            zmq::message_t outMessage(255);
            char* outData = reinterpret_cast<char*>(outMessage.data());
            snprintf(outData, 255, "%d\0", r);

            if (!socket.send(outMessage, 0))
            {
                // Failed to send the message.
                break;
            }
        }
        else
        {
            // Failed to recv a message.
            break;
        }
    }

    return 0;
}
