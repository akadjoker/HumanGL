#pragma once
#include "Config.hpp"
#include "Gui.hpp"

void LogError(const char *msg, ...);
void LogWarning(const char *msg, ...);
void LogInfo(const char *msg, ...);
unsigned char *LoadDataFile(const char *fileName, unsigned int *bytesRead);
char *LoadTextFile(const char *fileName);
bool FileExists(const char *fileName);
bool DirectoryExists(const char *dirPath);              
bool IsFileExtension(const char *fileName, const char *ext); // Check file extension (including point: .png, .wav)
const char *TextFormat(const char *text, ...);
const char *GetFileExtension(const char *fileName);         // Get pointer to extension for a filename string (includes dot: '.png')
const char *GetFileName(const char *filePath);              // Get pointer to filename for a path string
const char *GetFileNameWithoutExt(const char *filePath);    // Get filename string without extension (uses static string)
const char *GetDirectoryPath(const char *filePath);         // Get full path for a given fileName with path (uses static string)




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