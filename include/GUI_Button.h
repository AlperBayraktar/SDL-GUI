#pragma once

#include <SDL.h>

#include <GUI_WidgetBase.h>
#include <GUI_Text.h>

struct GUI_Button : public GUI_WidgetBase
{
    int X_SPACING = 0, Y_SPACING = 0;
    const static int DEFAULT_X_SPACING = 20;
    const static int DEFAULT_Y_SPACING = 15;
    
    GUI_Text* text = nullptr;
    bool isHovered = false;

    GUI_Button() : GUI_WidgetBase() {}
    
    GUI_Button(GUI_Text* text_, int X_SPACING_ = DEFAULT_X_SPACING, int Y_SPACING_ = DEFAULT_Y_SPACING)
    : GUI_WidgetBase()
    {
        SetSpaces(X_SPACING_, Y_SPACING_);

        text = text_;
        text->SetLocation(text->rect.x + X_SPACING, text->rect.h + Y_SPACING);
        UpdateRectData();
    }

    void SetSpaces(int X_SPACING_ = DEFAULT_X_SPACING, int Y_SPACING_ = DEFAULT_Y_SPACING)
    {
        X_SPACING = X_SPACING_;
        Y_SPACING = Y_SPACING_;
    }

    void UpdateRectData()
    {
        rect.w = text->rect.w + X_SPACING * 2;
        rect.h = text->rect.h + Y_SPACING * 2;
        
        rect.x = text->rect.x - X_SPACING;
        rect.y = text->rect.y - Y_SPACING;
    }

    void SetValue(std::string newValue, bool renderAfterUpdate=true)
    {
        text->SetValue(newValue, false);
        UpdateRectData();

        if (renderAfterUpdate)
        {
            Render();
        }
    }

    void Render()
    {
        SDL_Color c = text->isHovered ? AQUA : WHITE;
        SDL_SetRenderDrawColor(RENDERER, c.r, c.g, c.b, c.a);
        SDL_RenderDrawRect(RENDERER, &rect);
        text->Render();
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
            text->isHovered = isHovered;
            text->UpdateTextureAndSurface();
            Render();
        }
    }

    void SetLocation(int x, int y)
    {
        text->rect.x = x + X_SPACING;
        text->rect.y = y + Y_SPACING;
        UpdateRectData();
    }
};