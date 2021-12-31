#pragma once

#include <SDL.h>

struct GUI_WidgetBase {
    const char* a = "alper";
    // Holds x,y,w,h info
    SDL_Rect rect;

    // Called when SDL_MOUSEMOTION is triggered
    virtual void MouseMotion();
    virtual void Render();
    
    void SetLocation(int x, int y)
    {
        rect.x = x;
        rect.y = y;
    }
};