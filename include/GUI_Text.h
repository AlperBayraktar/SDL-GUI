#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <GUI_Colors.h>


struct GUI_TextStyle{
    SDL_Color color = WHITE;
    SDL_Color hoverColor = AQUA;
};

GUI_TextStyle GUI_TextDefaultStyle;


struct GUI_Text {
    // What is the text
    std::string value;
    // Is text hovered
    bool isHovered;
    // Holds x,y,w,h info
    SDL_Rect rect;

    // Style
    GUI_TextStyle* style;

    // SDL
    TTF_Font* font;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Surface *surface;


    GUI_Text(){}
    GUI_Text(
        std::string value_,
        int x, int y,
        TTF_Font* font_,
        SDL_Renderer* renderer_,
        bool addDefaultStyle=false,
        bool renderForFirstTime=true
        ) :
        value(value_), font(font_), renderer(renderer_),
        surface(nullptr), texture(nullptr), isHovered(false), style(nullptr)
    {
        SetLocation(x, y);
        if (addDefaultStyle)
        {
            SetStyle();
        }
    }

    // Style must be set for rendering
    // If you didn't set addDefaultStyle to true in constructor, you have to call this function manually
    void SetStyle(GUI_TextStyle* newStyle=&GUI_TextDefaultStyle, bool renderWithNewStyle=true)
    {
        style = newStyle;
        UpdateTextureAndSurface();
        if (renderWithNewStyle)
        {
            Render();
        }
    }

    void SetLocation(int x, int y)
    {
        rect.x = x;
        rect.y = y;
    }

    void UpdateTextureAndSurface()
    {
        // Free the old ones
        FreeMemory();

        // Update surface and texture
        surface = TTF_RenderText_Blended(font, value.c_str(), isHovered ? style->hoverColor : style->color);
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
    void UpdateIsHoveredState()
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