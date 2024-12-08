#pragma once
#include "Config.hpp"
#include "Gui.hpp"

class Device
{
private:
    static Device *instance;
    SDL_Window *window;
    SDL_GLContext glContext;
    int width;
    int height;
    std::string title;
    bool isRunning;
    SDL_Event event;
    double m_current;
    double m_previous;
    double m_update;
    double m_draw;
    double m_frame;
    double m_target;

    // Private constructor (Singleton)
    Device();
    ~Device();

    // Prevent copying
    Device(const Device &) = delete;
    Device &operator=(const Device &) = delete;

    bool initSDL();
    bool initGL(bool vzync);

public:
    static Device *GetInstance();
    static void DestroyInstance();

    bool Init(const char *windowTitle = "OpenGL Device",
              int windowWidth = 800,
              int windowHeight = 600, bool vzync = true);
    void Cleanup();

    // Getters
    SDL_Window *GetSDLWindow() const { return window; }
    SDL_GLContext GetGLContext() const { return glContext; }
    int GetWidth() const ;
    int GetHeight() const ;
   
    bool Running();

    void Update();

    // Device operations
    void Swap();
    void SetTitle(const char *newTitle);
    void SetSize(int newWidth, int newHeight);
    void Quit() { isRunning = false; }

    // Timer
    void Wait(float ms);
    void SetTargetFPS(int fps);
    int GetFPS(void);
    float GetFrameTime(void);
    double GetTime(void);
    u32 GetTicks(void);
};