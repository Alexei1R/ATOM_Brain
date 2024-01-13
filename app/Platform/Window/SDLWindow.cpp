//
// Created by toor on 1/13/24.
//

#include "SDLWindow.h"



namespace Atom {


    Window* Window::Create(const WindowSpecs& specs)
    {
        return new SDLWindow(specs);
    }

    SDLWindow::SDLWindow(const WindowSpecs& spec)
    {
        Init(spec);
    }

    SDLWindow::~SDLWindow()
    {
        Shutdown();
    }


    // init OpenGl window stuff
    void SDLWindow::Init(const WindowSpecs& specs)
    {
        m_Data.Title = specs.Title;
        m_Data.Width = specs.Width;
        m_Data.Height = specs.Height;

        ATLOG_INFO("Creating Window ::  {0}, {1}:{2}", m_Data.Title, m_Data.Width, m_Data.Height)
        int success = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD);
        if(success != 0){ATLOG_CRITICAL("!Succes SDL Init") }


        // Enable native IME.
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

        Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
        m_Window = SDL_CreateWindow("Dear ImGui SDL3+SDL_Renderer example", 1280, 720, window_flags);
        if (m_Window == nullptr)
        {
            ATLOG_CRITICAL("Error: SDL_CreateWindow(): {0}\n", SDL_GetError());

        }
        m_Renderer = SDL_CreateRenderer(m_Window, nullptr, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
        if (m_Renderer == nullptr)
        {
            SDL_Log("Error: SDL_CreateRenderer(): {0}\n", SDL_GetError());
        }
        SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(m_Window);
        ATLOG_INFO("SDL Info:");
        ATLOG_INFO("  Version: {}", SDL_GetRevision());



        SetVSync(true);
        SetCallbacks();
    }
    //close window
    void SDLWindow::Shutdown()
    {
        SDL_DestroyRenderer(m_Renderer);
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
    }

    std::pair<int, int> SDLWindow::GetSize() const {
        int width, height;
        SDL_GetWindowSize(m_Window, &width, &height);
        return {width, height};
    }

    std::pair<int, int> SDLWindow::GetPosition() const {
        int xpos, ypos;
        SDL_GetWindowPosition(m_Window, &xpos, &ypos);
        return {xpos, ypos};
    }



    void SDLWindow::OnUpdate()
    {

        SDL_RenderPresent(m_Renderer);
    }

    void SDLWindow::ClearDisplay(glm::vec3 color)
    {
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(m_Renderer);
    }

    void SDLWindow::SetVSync(bool enabled)
    {
        if (enabled)
        m_Data.VSync = enabled;
    }

    bool SDLWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

    void SDLWindow::SetCallbacks()
    {

    }


}
