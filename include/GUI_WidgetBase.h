#pragma once

#include <SDL.h>

#include <GUI_Colors.h>

struct GUI_WidgetBase {
    inline static TTF_Font* FONT = nullptr;
    inline static SDL_Renderer* RENDERER = nullptr;
    inline static SDL_Color CLEAR_COLOR = BLACK;

    // Holds x,y,w,h info
    SDL_Rect rect;

    // Called when SDL_MOUSEMOTION is triggered
    virtual void MouseMotion() {};
    // Draws the widget
    virtual void Render() {};
    
    // Sets x and y location
    virtual void SetLocation(int x, int y)
    {
        rect.x = x;
        rect.y = y;
    }
};