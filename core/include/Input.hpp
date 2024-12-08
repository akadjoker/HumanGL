
#pragma once
#include "Config.hpp"
class Input
{
private:
    static std::unordered_map<SDL_Keycode, bool> currentKeys;
    static std::unordered_map<SDL_Keycode, bool> previousKeys;
    static std::unordered_map<uint8_t, bool> currentMouseButtons;
    static std::unordered_map<uint8_t, bool> previousMouseButtons;
    static int mouseX;
    static int mouseY;
    static int mouseDeltaX;
    static int mouseDeltaY;
    static bool quit;
    static int quit_key;

public:
    static void Init();
    static void Update();
    static void ProcessEvent(const SDL_Event &event);

    // Keyboard functions
    static bool IsKeyPressed(SDL_Keycode key);  // Triggered once when key is pressed
    static bool IsKeyDown(SDL_Keycode key);     // True while key is held down
    static bool IsKeyReleased(SDL_Keycode key); // Triggered once when key is released
    static bool IsKeyUp(SDL_Keycode key);       // True while key is not pressed

    // Mouse functions
    static bool IsMouseButtonPressed(uint8_t button);
    static bool IsMouseButtonDown(uint8_t button);
    static bool IsMouseButtonReleased(uint8_t button);
    static bool IsMouseButtonUp(uint8_t button);

    // Mouse position
    static int GetMouseX();
    static int GetMouseY();
    static void GetMousePosition(int &x, int &y);
    static int GetMouseDeltaX();
    static int GetMouseDeltaY();
    static void GetMouseDelta(int &deltaX, int &deltaY);

    // Window state
    static bool ShouldQuit();
    static void SetQuit(bool shouldQuit);
    static void SetQuitKey(int key);
};