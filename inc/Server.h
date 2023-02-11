//
// Created by 沈逸潇 on 22/12/2022.
//

#ifndef SIMPLEGOMOKU_SERVER_H
#define SIMPLEGOMOKU_SERVER_H
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <iostream>
#include <netdb.h>
#include "Logging.h"
#include "../inc/Socket.h"
#define BACKLOG 120    // how many pending connections queue will hold
namespace gomoku {

    class Server {
    public:
        explicit Server(const std::string& port);
        void start();
    private:
        gomoku::RecvSocket recv_socket;
        gomoku::ClientSocket client_socket;
    };

} // Server

#endif //SIMPLEGOMOKU_SERVER_H
