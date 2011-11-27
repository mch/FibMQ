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

    while (1)
    {
        zmq::message_t message;
            
        if (socket.recv(&message, 0))
        {
            char* data = reinterpret_cast<char*>(message.data());
            data[message.size()] = 0;

            // Copy the data out and null terminate it.
            char* buffer = new char[message.size()];
            memcpy(buffer, message.data(), message.size() - 1);
            buffer[message.size()] = 0;
            
            int serial = 0;
            int n = 0;

            {
                std::stringstream ss(buffer);
                ss >> serial;
                ss >> n;
            }
            delete buffer;
            buffer = 0;
            
            std::cout << "serial: " << serial << ", n: " << n << std::endl;

            int r = fibonacci(n);

            int msgLen = snprintf(buffer, 0, "%d %d", serial, r);
            buffer = new char[msgLen + 1];
            snprintf(buffer, msgLen + 1, "%d %d", serial, r);
            
            zmq::message_t outMessage(msgLen);
            void* outData = outMessage.data();
            memcpy(outData, buffer, msgLen); // not sending a null terminator
            delete buffer;
            buffer = 0;
            
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
