//
// Created by 沈逸潇 on 25/12/2022.
//

#ifndef SIMPLEGOMOKU_SOCKET_H
#define SIMPLEGOMOKU_SOCKET_H

#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netdb.h>
#include "../inc/Logging.h"
#define FAIL (-1)
namespace gomoku {

    class Socket {
    protected:
        int file_descriptor{};

    public:
        int getFileDescriptor() const;
        void setFileDescriptor(int fileDescriptor);
        void closeFD() const;
    };

    class RecvSocket : public Socket {
    public:
        void setAddress(int, int, int);
        void setOpt(int, int);
        void startBind(const struct sockaddr *, socklen_t);
        void startListen(int);
    };

    class ClientSocket : public Socket {
    public:
        void readResponse(char *buf, size_t nbyte);
        long getReadBytesQuantity() const;
        void sendFile(std::string file_path);
        long read_bytes_quantity{};

    };

    static void *get_in_addr(struct sockaddr *sa){
        if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
        }
        return &(((struct sockaddr_in6*)sa)->sin6_addr);
    }

    typedef std::uint64_t hash_t;
    constexpr hash_t prime = 0x100000001B3ull;
    constexpr hash_t basis = 0xCBF29CE484222325ull;

    constexpr hash_t hash(char const* str, hash_t last_value = basis){
        return *str ? hash(str + 1, (*str ^ last_value) * prime) : last_value;
    }




} // gomoku

#endif //SIMPLEGOMOKU_SOCKET_H
