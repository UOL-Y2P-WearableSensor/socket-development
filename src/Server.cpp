//
// Created by 沈逸潇 on 22/12/2022.
//

#include "../inc/Server.h"


#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <csignal>
#include <utility>
#include <sys/wait.h>

enum {
    BUFFER_SIZE = 10000
};

namespace gomoku {

    void sigchld_handler(int s)
    {
        (void)s; // quiet unused variable warning

        // waitpid() might overwrite errno, so we save and restore it:
        int saved_errno = errno;

        while(waitpid(-1, NULL, WNOHANG) > 0);

        errno = saved_errno;
    }

    //todo: lambda function to a function pointer





    Server::Server(const std::string& port) {
        struct addrinfo hints, *local_address;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;          //ipv4
        hints.ai_socktype = SOCK_STREAM;    //TCP Socket
        hints.ai_flags = AI_PASSIVE;        //local, 127.0.0.1

        ////bind external port&IP, resolve domain
        int rv;
        if ((rv = getaddrinfo(NULL, port.c_str(), &hints, &local_address)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        }
        INFO("本地游戏已在端口 {} 开启\n", port);

        //create file descriptor for server socket
        this->recv_socket.setAddress(local_address->ai_family,
                                     local_address->ai_socktype,
                                     local_address->ai_protocol);

        //set socket option
        this->recv_socket.setOpt(SOL_SOCKET, SO_REUSEADDR);

        //bind internal port&local
        this->recv_socket.startBind(local_address->ai_addr,
                                    local_address->ai_addrlen);

        // all done with this structure, after "getaddrinfo()"
        freeaddrinfo(local_address);
    }




    void Server::start() {
        struct sockaddr_storage connector_addr;
        socklen_t sin_size;
        struct sigaction sa;
        char s[INET6_ADDRSTRLEN];

        //begin listening
        this->recv_socket.startListen(BACKLOG);

        // reap all dead processes
        sa.sa_handler = sigchld_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        if (sigaction(SIGCHLD, &sa, NULL) == FAIL) {
            perror("sigaction");
            exit(1);
        }
        char* buffer = new char[BUFFER_SIZE];

        // main accept() loop
        while(true) {
            //create client socket
            sin_size = sizeof connector_addr;
            client_socket.setFileDescriptor(accept(recv_socket.getFileDescriptor(),
                                                   (struct sockaddr *) &connector_addr,
                                                   &sin_size));

            //read response from client

            client_socket.readResponse(buffer, BUFFER_SIZE);

            INFO("request from  ({} bytes):\n{}",  client_socket.read_bytes_quantity, buffer);

/**
 *  char buffer=
 *      'GET /fileForServer/pic_trulli.jpg HTTP/1.1...'
 *      'GET / HTTP/1.1...'
 *      std::string method=findTextNo(1);
 *      std::string file_path=findTextNo(2);
 */

            auto findTextNo=[buffer](int num){
                const char *ptr_right=buffer;
                const char *ptr_left;
                for (int i = 0; i <= num;++i) {
                    if (i==num){
                        ptr_left=ptr_right;
                    }
                    ptr_right = strchr(ptr_right,' ');
                    ++ptr_right;
                }
                return std::string( ptr_left, --ptr_right);
            };

            INFO("Request: {} {}", findTextNo(0), findTextNo(1));



            switch (hash(findTextNo(0).c_str(),basis)) {
                case hash("GET", basis):
                    INFO("GET method detected");
                    break;
                case hash("POST", basis):
                    INFO("POST method detected");
                    break;
            }

//            if(request_method="GET") {
//                if (!fork()) {
//                    close(main_socket_fd);
//                    switch (request_path(read_bytes_quantity)) {
//                        case JPG:
//                            send(JPGPATH, JPG_Head);
//                            break;
//                        case HTML:
//                            send(HTMLPATH, HTML_Head);
//                            break;
//                        case ICO:
//                            send(ICOPATH, ICO_Head);
//                            break;
//                        case CSS:
//                            send(CSSPATH, CSS_Head);
//                            break;
//                        case JS:
//                            send(JSPATH, JS_Head);
//                            break;
//                        case CHAT:
//                            send(CHATPATH, UPgrade_Head);
//                            break;
//                        default:
//                    }
//                    close(additional_socket_fd);
//                    exit(0);
//                }
//            }
                

            //send response to client's request...
            if (!fork()) { // this is the child process
                this->recv_socket.closeFD(); // child doesn't need the listener
                this->client_socket.sendFile(findTextNo(1));
                this->client_socket.closeFD();
                exit(0);    //kill child process
            }

            this->client_socket.closeFD();  // parent doesn't need this
        }
    }

} // Server

