#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

#include <GUI_Colors.h>
#include <GUI_WidgetBase.h>


struct GUI_Text : public GUI_WidgetBase {
    // What is the text
    std::string value;
    // Is text hovered
    bool isHovered;

    // SDL
    TTF_Font* font;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Surface *surface;


    GUI_Text() : GUI_WidgetBase() {}
    GUI_Text(
        std::string value_,
        int x, int y,
        TTF_Font* font_,
        SDL_Renderer* renderer_,
        bool renderForFirstTime=true
        ) :
        GUI_WidgetBase(),
        value(value_), font(font_), renderer(renderer_),
        surface(nullptr), texture(nullptr), isHovered(false)
    {
        SetLocation(x, y);
        UpdateTextureAndSurface();
        if (renderForFirstTime)
        {
            Render();
        }
    }

    void UpdateTextureAndSurface()
    {
        // Free the old ones
        FreeMemory();

        // Update surface and texture
        surface = TTF_RenderText_Blended(font, value.c_str(), isHovered ? AQUA : WHITE);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        
        // Update rect w and h
        rect.w = surface->w;
        rect.h = surface->h;
    }

    // Renders the text onto the screen
    void Render()
    {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);
    }

    // Updates is hovered state according to mouse postition
    // Called when SDL_MOUSEMOTION is triggered
    void MouseMotion()
    {
        // Get pos
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        bool oldState = isHovered;

        // Update
        if (
            mouseX > rect.x && mouseX < ( rect.x + rect.w )
            &&
            mouseY > rect.y && mouseY < ( rect.y + rect.h )
        )
        {
            isHovered = true;
        }
        else
        {
            isHovered = false;
        }

        // If state has changed, render again to display color change
        if (oldState != isHovered)
        {
            UpdateTextureAndSurface();
            Render();
        }
    }

    void SetValue(std::string newValue, bool renderAfterUpdate=true)
    {
        value = newValue;
        UpdateTextureAndSurface();
        
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