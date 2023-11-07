//
// Created by toor on 11/7/23.
//

#ifndef ATOM_LOG_H
#define ATOM_LOG_H


#include "ATOM/atompch.h"
#include "ATOM/Core/macros.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Atom {

    class Log
    {
    public:
        static void Init();

        inline static Ref<spdlog::logger>& GetLogger() { return s_Logger; }
    private:
        static Ref<spdlog::logger> s_Logger;
    };
}


// Core log macros
#define ATLOG_TRACE(...)    ::Atom::Log::GetLogger()->trace(__VA_ARGS__);
#define ATLOG_INFO(...)     ::Atom::Log::GetLogger()->info(__VA_ARGS__);
#define ATLOG_WARN(...)     ::Atom::Log::GetLogger()->warn(__VA_ARGS__);
#define ATLOG_ERROR(...)    ::Atom::Log::GetLogger()->error(__VA_ARGS__);
#define ATLOG_CRITICAL(...) ::Atom::Log::GetLogger()->critical(__VA_ARGS__);



#endif //ATOM_LOG_H
