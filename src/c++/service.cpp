// Use ZeroMQ and a ZMQ_REP socket to respond to requests to calculate
// a fibbonci number.

#include <zmq.hpp>
#include <iostream>
#include <sstream>

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
            std::stringstream ss(data);
            int serial = 0;
            ss >> serial;
            int n = 0;
            ss >> n;
            std::cout << "serial: " << serial << ", n: " << n << std::endl;

            int r = fibonacci(n);

            char buffer[255];
            int msgLen = snprintf(buffer, 255, "%d %d", serial, r);
            
            zmq::message_t outMessage(msgLen);
            void* outData = outMessage.data();
            memcpy(outData, buffer, msgLen); // not sending a null terminator
            
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
