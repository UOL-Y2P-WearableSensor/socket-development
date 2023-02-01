#include <iostream>
#include "inc/Server.h"
#include "inc/Logging.h"
#include "inc/Socket.h"


auto main() -> int{

    gomoku::Server server("3490");
    server.start();


}
