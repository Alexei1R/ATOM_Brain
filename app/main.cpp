#include <spdlog/spdlog.h>
#include "ATOM/Application.h"
#include "ATOM/Core/Loging/Log.h"
#include <filesystem>

int main(){
    Atom::Log::Init();
    ATLOG_INFO("Initialized Log!");
    ATLOG_INFO("Curent path: {0}", std::filesystem::current_path().string());
    std::filesystem::path current_path = std::filesystem::current_path();
    try {
        std::string path;
        current_path = current_path.parent_path().parent_path();
        path = current_path.string() + "/app";
        std::filesystem::current_path(path);
        ATLOG_INFO("Curent path  set to: {0}", path);
    } catch (const std::filesystem::filesystem_error& ex) {
        ATLOG_ERROR("Error setting current path:  {0}", ex.what());
    }

    Atom::Application* app = new Atom::Application();
    app->Run();
    delete app;
}