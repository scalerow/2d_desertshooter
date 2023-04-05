#include "components.h"

using namespace Components;

struct ListObject;

void components::ListBox(std::vector<ListObject> inputObject,Vector2 position, int width, int fontSize, bool includeIndex, bool transparent,  Color outlineColor, Color fillColor, Color textColor) 
{
    ListBoxDraw(inputObject, position, width, fontSize, includeIndex, transparent, outlineColor, fillColor, textColor);
    ListBoxActions();
}

void components::ListBoxActions(Rectangle entries[4], void (*func)()) 
{
    mousePoint = GetMousePosition();

    for(int i = 0; 0 <= sizeof(entries); i++ )
    {
        if(CheckCollisionPointRec(mousePoint, entries[i]))
        {
            func();
        }    
    }    
}


Rectangle[] components::ListBoxDraw(std::vector<ListObject> inputObject,Vector2 position, int width, int fontSize, bool includeIndex, bool transparent,  Color outlineColor, Color fillColor, Color textColor) 
{
    int rectHeight = (inputObject.size() * fontSize) + (5 * inputObject.size()); 
    Rectangle listRect = {position.x, position.y, width, rectHeight};
    
    if(!transparent)
    {
        DrawRectangleRec(listRect, fillColor);
    }

    DrawRectangleLinesEx(listRect, 5, outlineColor);
    

    for(int i = 0; i < inputObject.size(); i++)
    {
        if(includeIndex) 
        {
             char indexString[5 + sizeof(char)] = "";
            sprintf(indexString, "%d", inputObject[i].key);
            DrawText(indexString, position.x + 10, position.y + ((fontSize + 5) * i), fontSize, textColor);
        }
        int valueWidth = MeasureText(inputObject[i].value.c_str(), fontSize);
        DrawText(inputObject[i].value.c_str(), (position.x - 10 ) + (listRect.width - valueWidth), position.y + ((fontSize + 5) * i), fontSize, textColor);
    }
}