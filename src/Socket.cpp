//
// Created by 沈逸潇 on 25/12/2022.
//

#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include "Socket.h"

namespace WearableSensor {

    void RecvSocket::startListen(int connectionsQuantity) {
        listen(this->file_descriptor, connectionsQuantity);
    }

    void RecvSocket::setOpt(int level, int option_name) {
        int option_value = 1;
        setsockopt(this->file_descriptor, level, option_name, &option_value, sizeof(int));
    }

    void RecvSocket::startBind(const struct sockaddr *ai_addr, socklen_t ai_addrlen) {

        if (bind(this->file_descriptor, ai_addr, ai_addrlen) == FAIL) {
            perror("server: bind");
            exit(0);
        }
    }

    int Socket::getFileDescriptor() const {
        return file_descriptor;
    }

    void Socket::setFileDescriptor(int fileDescriptor) {
        if (fileDescriptor == FAIL) {
            perror("[accept] invalid fd");
            exit(1);
        }
        file_descriptor = fileDescriptor;
    }

    void Socket::closeFD() const {
        close(this->file_descriptor);
    }

    void RecvSocket::setAddress(int ai_family, int ai_socktype, int ai_protocol) {
        this->setFileDescriptor(socket(ai_family, ai_socktype, ai_protocol));
    }

    void ArduinoSocket::recvResponse(char *buffer, size_t nbyte) {
        int tmp;
        if ((tmp = recv(this->file_descriptor, buffer, nbyte, MSG_WAITALL)) == -1) {
            perror("Arduino recv done");
            return;
        }
        this->read_bytes_quantity = tmp;


    }


    void ClientSocket::readResponse(char *buf, size_t nbyte) {
        int tmp;
        if ((tmp = read(this->file_descriptor, buf, nbyte)) == -1) {
            perror("read response failed");
            return;
        }
        this->read_bytes_quantity = tmp;
    }

    long ClientSocket::getReadBytesQuantity() const {
        return read_bytes_quantity;
    }

    void ArduinoSocket::send_ping_pong() {
        const char ping_msg[20] = "keep alive, yixiao\n";
        if (send(this->file_descriptor, ping_msg, sizeof ping_msg / sizeof(char), MSG_OOB) == -1) {
            perror("arduino sending ping-pong msg");
        } else {
            INFO("sent ping-pong msg to Arduino");
        }
    }

    void ClientSocket::sendFile(std::string file_path) {
        if (file_path == "/") {        //todo: string equaling may fall
            file_path = "/index.html";
        }
        std::string image_path = "../fileForServer" + file_path;
        INFO("ready to send \"{}\"", image_path);

        std::string head = "HTTP/1.1 200 Ok\r\n", mime_type = "Content-Type: ";

        if (file_path.substr(file_path.length() - 3, 3) == ".js") {
            mime_type += "text/javascript";
        } else if (file_path.substr(file_path.length() - 5, 5) == ".html") {
            mime_type += "text/html";
        } else if (file_path.substr(file_path.length() - 5, 5) == ".json") {
            mime_type += "application/json";
        }

        head += mime_type + "\r\n\r\n";

        if (write(this->file_descriptor, head.c_str(), head.size()) == FAIL) {
            perror("write dead");
            return;
        }

        int img_fd;
        if ((img_fd = open(image_path.c_str(), O_RDONLY)) == -1) {
            perror("file not found");
            return;
        }
        INFO("img_fd {} created\n\n", img_fd);

        off_t off = 0;
        off_t *len_ptr = &off;
        struct sf_hdtr *hdtr = nullptr;

        //todo: maybe unstable if the file is too large
        //sendfile(int fd, int s, off_t offset, off_t *len, struct sf_hdtr *hdtr, int flags);
        int tmp = sendfile(img_fd, this->file_descriptor, off, len_ptr, hdtr, 0);

        if (tmp == -1) {
            INFO("send failed");
        }

        close(img_fd);
    }


    void ArduinoSocket::loop() {
        INFO("ArduinoSocket FD is {}", this->getFileDescriptor());
        char buffer[1024];

        std::string image_path = "../fileForServer/IMU_data.txt";
        while (true) {
            INFO("ArduinoSocket::reading...");
            memset(buffer, 0, sizeof buffer / sizeof(char));

            this->recvResponse(buffer, 1024);
//            this->readResponse(buffer, 1024);

            int img_fd;
            if ((img_fd = open(image_path.c_str(), O_WRONLY | O_APPEND)) == -1) {
                perror("file not found");
                return;
            }

            if (write(img_fd, buffer, this->read_bytes_quantity) != this->read_bytes_quantity) {
                perror("something wrong in the Arduino Writing");
                exit(1);
            }
            close(img_fd);
            if (this->read_bytes_quantity > 0) {
//                INFO("\t\tdata from Arduino ({} bytes):\n{}", this->read_bytes_quantity, buffer);
            } else {
                break;
            }

        }
        INFO("receiving failed, ArduinoSocket's loop out & kill the child process & close the FD");
        this->closeFD();
        exit(0);
    }

    long long int ArduinoSocket::getCurrentTimeIdx() const {
        return current_time_idx;
    }

} // gomoku