#include <spdlog/spdlog.h>
#include "ATOM/Application.h"
#include "ATOM/Core/Loging/Log.h"

int main(){
    Atom::Log::Init();
    ATLOG_INFO("Initialized Log!");

    Atom::Application* app = new Atom::Application();
    app->Run();
    delete app;
}