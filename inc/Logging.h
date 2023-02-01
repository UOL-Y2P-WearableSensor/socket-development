//
// Created by 沈逸潇 on 24/12/2022.
//

#ifndef SIMPLEGOMOKU_LOGGING_H
#define SIMPLEGOMOKU_LOGGING_H
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
namespace gomoku {

    class Logging {
    public:

    private:
        static std::shared_ptr<spdlog::logger> server_logger;
    };

} // gomoku

#define INFO(...) spdlog::info(__VA_ARGS__)
#define WARN(...) spdlog::warn(__VA_ARGS__)
#define CRITICAL(...) spdlog::critical(__VA_ARGS__)
#define DEBUG(...) spdlog::debug(__VA_ARGS__)
#define ERROR(...) spdlog::error(__VA_ARGS__)
#endif //SIMPLEGOMOKU_LOGGING_H
