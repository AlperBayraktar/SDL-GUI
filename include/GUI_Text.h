#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

#include <GUI_Colors.h>
#include <GUI_WidgetBase.h>
#include <GUI_Utilities.h>

struct GUI_Text : public GUI_WidgetBase {
    // The text that will be displayed
    std::string value;
    // Is text hovered
    bool isHovered;

    // SDL
    SDL_Texture* texture;
    SDL_Surface *surface;
    TTF_Font* font;


    GUI_Text() : GUI_WidgetBase() {}
    GUI_Text(
        std::string value_,
        int x=0, int y=0,
        // Do render in constructor
        bool renderForFirstTime=false
        ) :
        GUI_WidgetBase(),
        value(value_),
        surface(nullptr), texture(nullptr), font(nullptr), isHovered(false)
    {
        rect.w = 0;
        rect.h = 0;
        font = FONT;
        SetLocation(x, y);
        UpdateTextureAndSurface();
        UpdateRect();
        if (renderForFirstTime)
        {
            Render();
        }
    }

    // Updates texture and surface.
    // Called to display changes when value and color gets updated
    void UpdateTextureAndSurface()
    {
        // Free the old ones
        FreeMemory();

        // Update surface and texture
        surface = TTF_RenderText_Blended(font, value.c_str(), isHovered ? AQUA : WHITE);
        texture = SDL_CreateTextureFromSurface(RENDERER, surface);
    }

    // Update rect w and h
    void UpdateRect()
    {
        rect.w = surface->w;
        rect.h = surface->h;
    }

    // Renders the text onto the screen (without clearing the old render if old render exists)
    void Render()
    {
        SDL_RenderCopy(RENDERER, texture, NULL, &rect);
        SDL_RenderPresent(RENDERER);
    }

    // Updates is hovered state according to mouse postition
    // Called when SDL_MOUSEMOTION is triggered
    void MouseMotion()
    {
        bool oldState = isHovered;
        isHovered = IsMouseOnRect(rect);

        if (oldState != isHovered)
        {
            ClearWidgetRender(this, CLEAR_COLOR, RENDERER);
            UpdateTextureAndSurface();
            Render();
        }
    }

    void SetValue(std::string newValue, bool renderAfterUpdate=true)
    {
        value = newValue;
        UpdateTextureAndSurface();
        UpdateRect();
        
        if (renderAfterUpdate)
        {
            Render();
        }
    }


    // Free memory that text uses
    void FreeMemory()
    {
        SDL_FreeSurface(surface); surface = nullptr;
        SDL_DestroyTexture(texture); texture = nullptr;
    }


    ~GUI_Text()
    {
        FreeMemory();
    }
};