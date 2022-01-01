#pragma once

#include <GUI_Text.h>
#include <GUI_WidgetBase.h>
#include <vector>

// Used to store multipyle widgets in one container
struct GUI_Grid : public GUI_WidgetBase
{
    // Rows
    std::vector< std::vector<GUI_WidgetBase*>* > rows;

    // Do center widgets in UpdateWidgetLocationsAndRectData
    bool centerWidgets = true;

    // Spaces between elements
    int X_SPACE = 0, Y_SPACE = 0;

    // Default space
    const static int DEFAULT_SPACE = 20;


    GUI_Grid(int x=0, int y=0, int X_SPACE_=DEFAULT_SPACE, int Y_SPACE_=DEFAULT_SPACE, bool centerWidgets_=true)
    : GUI_WidgetBase(), centerWidgets(centerWidgets_)
    {
        // Add first empty row
        AddEmptyRow();
        // Set grid location
        SetLocation(x, y);
        // Set space values
        SetSpaces(Y_SPACE_, X_SPACE_);
    }

    // Calls functionToRun(widget) for every widget in grid
    // This can be used to apply something to all widgets in grid
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

    // Adds a new row
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
        // Handle rows
        if (row == -1)
        {
            // If row provided is not provided, select last row
            row = rows.size() - 1;
        }
        // If row parameter is an index that doesn't exist on grid, add new rows until row index exists.
        else if (row > rows.size() - 1)
        {
            for (int i = 0; i < row - ( rows.size() - 1 ); i++)
            { AddEmptyRow(); }
        }

        std::vector<GUI_WidgetBase*>* rowToAddElement = rows[row];

        // If row provided is not provided, select last column
        if (col == -1)
        { col = rowToAddElement->size(); }

        // Add the widget
        rowToAddElement->insert(rowToAddElement->begin() + col, newElement);
    }

    // Returns the widget that is at given row and column
    template<typename WidgetType>
    WidgetType* GetElementAtLocation(int row, int col)
    {
        std::vector<GUI_WidgetBase*>* rowToGetElementFrom= rows[row];
        WidgetType* widget = static_cast<WidgetType*>((*rowToGetElementFrom)[col]);
        return widget;
    }

    // Called when mouse moves
    void MouseMotion()
    { 
        ApplyForEveryWidget([](GUI_WidgetBase* widget) { widget->MouseMotion(); });
    }

    // Updates all of w,h,x,y info of grid's rect and widgets' rect
    void UpdateWidgetLocationsAndRectData()
    {
        int y = rect.y;
        std::vector<int> rowWidths;
        for(int row = 0; row < rows.size(); row++)
        {
            std::vector<GUI_WidgetBase*>* row_ = rows[row];
            int x = rect.x;

            int rowWidth = 0;

            GUI_WidgetBase* widgetWithMaxHeight = nullptr;
            for(int col = 0; col < row_->size(); col++)
            {
                GUI_WidgetBase* widget = (*row_)[col];
                widget->SetLocation(x, y);

                x += X_SPACE + widget->rect.w;
                if (col != row_->size() - 1)
                {
                    rowWidth += widget->rect.w + X_SPACE;
                }
                else
                {
                    rowWidth += widget->rect.w;
                }
                
                if (widget->rect.h > ( widgetWithMaxHeight == nullptr ? 0 : widgetWithMaxHeight->rect.h) )
                {
                    widgetWithMaxHeight = widget;
                }
            }

            rowWidths.push_back(rowWidth);

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
        int maxRowWidth = 0;

        for (int i=0; i < rowWidths.size(); i++)
        {
            if (rowWidths[i] > maxRowWidth)
            {
                maxRowWidth = rowWidths[i];
            }
        }
        rect.w = maxRowWidth;
        
        if ( !centerWidgets ) { return; }
        
        for(int row = 0; row < rows.size(); row++)
        {
            std::vector<GUI_WidgetBase*>* rowToRender = rows[row];

            int x = (rect.w - rowWidths[row]) / 2 + rect.x;

            for(int col = 0; col < rowToRender->size(); col++)
            {
                GUI_WidgetBase* widget = (*rowToRender)[col];
                widget->SetLocation(x, widget->rect.y);
                x += X_SPACE + widget->rect.w;
            }
        }
    }

    // Updates locations and rect data and renders every widget
    void Render()
    {
        UpdateWidgetLocationsAndRectData();
        ApplyForEveryWidget( [](GUI_WidgetBase* widget)  { widget->Render(); } );
    }
};