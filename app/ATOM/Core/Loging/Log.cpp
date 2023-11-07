//
// Created by toor on 11/7/23.
//


#include "Log.h"

namespace Atom {

    Ref<spdlog::logger> Log::s_Logger;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_Logger = spdlog::stdout_color_mt("Bright");
        s_Logger->set_level(spdlog::level::trace);

    }

}