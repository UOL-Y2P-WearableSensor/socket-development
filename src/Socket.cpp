//
// Created by 沈逸潇 on 25/12/2022.
//

#include <unistd.h>
#include <sys/stat.h>
#include "../inc/Socket.h"

namespace gomoku {

    void RecvSocket::startListen(int connectionsQuantity) {
        listen(this->file_descriptor, connectionsQuantity);
    }

    void RecvSocket::setOpt(int level, int option_name) {
        int option_value=1;
        setsockopt(this->file_descriptor, level, option_name, &option_value, sizeof( int));
    }

    void RecvSocket::startBind(const struct sockaddr * ai_addr, socklen_t ai_addrlen) {

        if (bind(this->file_descriptor, ai_addr, ai_addrlen)==FAIL){
            perror("server: bind");
            exit(0);
        }
    }

    int Socket::getFileDescriptor() const {
        return file_descriptor;
    }

    void Socket::setFileDescriptor(int fileDescriptor) {
        if(fileDescriptor==FAIL){
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

    void ClientSocket::readResponse(char *buf, size_t nbyte) {
        int tmp;
        if ( (tmp = read(this->file_descriptor , buf, nbyte))== -1 ){
            perror("read response failed");
            return;
        }
        this->read_bytes_quantity=tmp;
    }

    long ClientSocket::getReadBytesQuantity() const {
        return read_bytes_quantity;
    }

    void ClientSocket::writeFile(std::string key){
        if (key!="123123") return;
        return;


    }

    void ClientSocket::sendFile(std::string file_path) {
        if (file_path=="/"){        //todo: string equaling may fall
            file_path="/index.html";
        }
        std::string image_path="../fileForServer"+file_path;
        INFO("ready to send \"{}\"", image_path);

        std::string head="HTTP/1.1 200 Ok\r\n", mime_type="Content-Type: ";

        if (file_path.substr( file_path.length()-3, 3)==".js"){
            mime_type+="text/javascript";
        }else if (file_path.substr( file_path.length()-5, 5)==".html"){
            mime_type+="text/html";
        }else if (file_path.substr( file_path.length()-5, 5)==".json"){
            mime_type+="application/json";
        }

        head+=mime_type+"\r\n\r\n";

        if(write(this->file_descriptor, head.c_str(), head.size()) == FAIL){
            perror("write dead");
            return;
        }

        int img_fd;
        if (( img_fd = open(image_path.c_str(), O_RDONLY))==-1){
            perror("file not found");
            return;
        }
        INFO("img_fd {} created\n\n", img_fd);

        off_t off=0;
        off_t* len_ptr=&off;
        struct sf_hdtr *hdtr = nullptr;

        //todo: maybe unstable if the file is too large
        //sendfile(int fd, int s, off_t offset, off_t *len, struct sf_hdtr *hdtr, int flags);
        int tmp=sendfile(img_fd, this->file_descriptor, off, len_ptr, hdtr, 0);

        if (tmp==-1){
            INFO("send failed");
        }

        close(img_fd);
    }


} // gomoku