#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <GUI_Colors.h>

struct GUI_Text {
    std::string value;
    bool isHovered = false;

    SDL_Color color;
    SDL_Color hoverColor;
    
    SDL_Rect rect;
    TTF_Font* font;
    SDL_Texture* texture;
    SDL_Surface *surface;
    SDL_Renderer* renderer;
    
    inline static TTF_Font* FONT;
    inline static SDL_Renderer* RENDERER;

    GUI_Text(){}
    
    GUI_Text(std::string value_,
    int x = 0, int y = 0,
    TTF_Font* font_ = nullptr,
    SDL_Renderer* renderer_ = nullptr,
    bool renderOnConstructor = 1,
    SDL_Color color_ = WHITE, SDL_Color hoverColor_ = AQUA
    ) : 
    color(color_), hoverColor(hoverColor_), surface(nullptr), texture(nullptr)
    {
        // Set renderer
        if (renderer_ == nullptr) { renderer = RENDERER; }
        else { renderer = renderer_ ; }

        // Set font
        if (font_ == nullptr) { font = FONT; }
        else { font = font_; }

        // Setup
        UpdatePosition(x, y);
        UpdateValue(value_, renderOnConstructor);
    }

    void UpdateTextureAndSurface()
    {
        // Free the old ones
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Update surface
        surface = TTF_RenderText_Blended(font, value.c_str(), isHovered ? hoverColor : color);
        // Update texture
        texture = SDL_CreateTextureFromSurface(renderer, surface);
    }

    void UpdateRectData()
    {
        // Update rect
        rect.w = surface->w;
        rect.h = surface->h;
    }


    void UpdateIsHoveredState()
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        bool oldState = isHovered;

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

        if (oldState != isHovered)
        {
            UpdateTextureAndSurface();
            Render();
        }
    }


    // Renders the text onto the screen
    void Render()
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b,color.a);
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);
    }

    // Frees values created for object
    void Free()
    {
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    // --- Settings ---
    
    // Updates value and texture, rect data according to new value.
    // Renders the text again with new values if renderAfterUpdate is true

    void UpdateValue(std::string newValue, bool renderAfterUpdate=true)
    {
        value = newValue;
        UpdateTextureAndSurface();
        UpdateRectData();
        if (renderAfterUpdate)
        {
            Render();
        }
    }

    void UpdateColor(SDL_Color newColor, bool renderAfterUpdateIfUnhovered=true)
    {
        color = newColor;
        if (renderAfterUpdateIfUnhovered && !isHovered) { UpdateTextureAndSurface(); Render(); }
    }

    void UpdateHoverColor(SDL_Color newHoverColor, bool renderAfterUpdateIfHovered=true)
    {
        hoverColor = newHoverColor;
        if (renderAfterUpdateIfHovered && isHovered) { UpdateTextureAndSurface(); Render(); }
    }

    void UpdatePosition(int xNew, int yNew)
    {
        rect.x = xNew;
        rect.y = yNew;
    }
};