#pragma once

#include <SDL.h>
#include <GUI_WidgetBase.h>


// Set renderer draw color according given colors r,g,b,a values
void SetRendererColor(SDL_Color color, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

// Covers the rect with clearColor that makes it look like widget render is undone
void ClearWidgetRender(GUI_WidgetBase* widget, SDL_Color clearColor, SDL_Renderer* renderer)
{
    SetRendererColor(clearColor, renderer);
    SDL_RenderFillRect(renderer, &widget->rect);
}

// Returns bool according to mouse position on rect
bool IsMouseOnRect(SDL_Rect rect)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (
        mouseX > rect.x && mouseX < ( rect.x + rect.w )
        &&
        mouseY > rect.y && mouseY < ( rect.y + rect.h )
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Centers widget using given width and height
void CenterWidget(GUI_WidgetBase* widget, int width, int height)
{
    int newX = (width - widget->rect.w) / 2;
    int newY = (height - widget->rect.h) / 2;

    widget->SetLocation(newX, newY);
}