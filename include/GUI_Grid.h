#pragma once

#include <GUI_Text.h>
#include <GUI_WidgetBase.h>
#include <vector>


struct GUI_Grid : public GUI_WidgetBase
{
    std::vector< std::vector<GUI_WidgetBase*>* > rows;
    int Y_SPACE, X_SPACE;
    const static int DEFAULT_SPACE = 20;

    GUI_Grid(int x, int y, int Y_SPACE_=DEFAULT_SPACE, int X_SPACE_=DEFAULT_SPACE)
    : GUI_WidgetBase()
    {
        AddEmptyRow();
        SetLocation(x, y);
        SetSpaces(Y_SPACE_, X_SPACE_);
    }

    void ApplyForEveryWidget(auto functionToRun)
    {
        for(int rowIndex = 0; rowIndex < rows.size(); rowIndex++)
        {
            std::vector<GUI_WidgetBase*>* row_ = rows[rowIndex];
            for(int col = 0; col < row_->size(); col++)
            {
                functionToRun((*row_)[col]);
            }
        }
    }

    void AddEmptyRow()
    {
        rows.push_back(new std::vector<GUI_WidgetBase*>);
    }


    void SetSpaces(int Y_SPACE_=DEFAULT_SPACE, int X_SPACE_=DEFAULT_SPACE )
    {
        Y_SPACE = Y_SPACE_;
        X_SPACE = X_SPACE_;
    }

    void Add(GUI_WidgetBase* newElement, int row=-1, int col=-1)
    {
        if (row == -1)
        {
            row = rows.size() - 1;
        }
        else if (row > rows.size() - 1)
        {
            for (int i = 0; i < row - ( rows.size() - 1 ); i++)
            { AddEmptyRow(); }
        }

        std::vector<GUI_WidgetBase*>* rowToAddElement = rows[row];

        if (col == -1)
        { col = rowToAddElement->size(); }

        rowToAddElement->insert(rowToAddElement->begin() + col, newElement);
    }

    int HowManyElementsAtRow(int nthRow)
    {
        std::vector<GUI_WidgetBase*>* rowToCountElements = rows[nthRow];
        return rowToCountElements->size();
    }

    template<typename WidgetType>
    WidgetType* GetElementAtLocation(int row, int col)
    {
        std::vector<GUI_WidgetBase*>* rowToGetElementFrom= rows[row];
        WidgetType* widget = static_cast<WidgetType*>((*rowToGetElementFrom)[col]);
        return widget;
    }

    void MouseMotion()
    {   
        ApplyForEveryWidget( [](GUI_WidgetBase* widget)  { widget->MouseMotion(); } );
    }

    void UpdateWidgetLocations()
    {
        int y = rect.y;
        for(int row = 0; row < rows.size(); row++)
        {
            std::vector<GUI_WidgetBase*>* rowToRender = rows[row];
            int x = rect.x;

            GUI_WidgetBase* widgetWithMaxHeight = nullptr;
            for(int col = 0; col < rowToRender->size(); col++)
            {
                GUI_WidgetBase* widget = (*rowToRender)[col];
                widget->SetLocation(x, y);

                x += X_SPACE + widget->rect.w;
                if (col != rowToRender->size() - 1)
                {
                    rect.w += X_SPACE + widget->rect.w;
                }
                else
                {
                    rect.w += widget->rect.w;
                }
                
                if (widget->rect.h > ( widgetWithMaxHeight == nullptr ? 0 : widgetWithMaxHeight->rect.h) )
                {
                    widgetWithMaxHeight = widget;
                }
            }

            if (row != rows.size() - 1)
            {
                y += widgetWithMaxHeight->rect.h + Y_SPACE;
                rect.h += widgetWithMaxHeight->rect.h + Y_SPACE;
            }
            else {
                y += widgetWithMaxHeight->rect.h;
                rect.h += widgetWithMaxHeight->rect.h;
            }
        }
    }

    void Render()
    {
        UpdateWidgetLocations();
        ApplyForEveryWidget( [](GUI_WidgetBase* widget)  { widget->Render(); } );
    }
};