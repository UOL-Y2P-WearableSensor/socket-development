#include <iostream>
#include "Server.h"
#include "Logging.h"
#include "Socket.h"


auto main() -> int{

    gomoku::Server server("3490");
    server.start();


}
