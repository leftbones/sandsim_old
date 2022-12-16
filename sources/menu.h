#pragma once

#include "main.h"
#include "element.h"

#include <vector>
#include <algorithm>


class MenuItem {
    public:
        string name;
        Color color;

        bool selected = false;

        MenuItem(string _name, Color _color) {
            name = _name;
            color = _color;
        }
};


class Menu {
    public:
        vector<MenuItem*> items_powder;
        vector<MenuItem*> items_solid;
        vector<MenuItem*> items_liquid;
        vector<MenuItem*> items_gas;

        int width = 250;
        bool is_open = false;
        Color color = {255, 255, 255, 50};

        Vector2 origin = { SCREEN_W - (float)(width - 10), 10 };
        Vector2 size = { 40, 24 };
        float gap = 32;
        int scroll = 0;

        Menu() {

        }

        void Toggle();
        void ListPowder();
        void ListSolid();
        void ListLiquid();
        void ListGas();
        void Draw();
};