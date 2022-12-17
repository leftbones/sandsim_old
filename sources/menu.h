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
        Vector2 item_size = { 40, 24 };
        float list_gap = 32;
        int scroll = 0;

        Menu() {
            items_powder.push_back(new MenuItem("Sand", C_SAND));
            items_powder.push_back(new MenuItem("Gunpowder", C_GUNPOWDER));

            items_solid.push_back(new MenuItem("Wood", C_WOOD));

            items_liquid.push_back(new MenuItem("Water", C_WATER));
            items_liquid.push_back(new MenuItem("Oil", C_OIL));

            items_gas.push_back(new MenuItem("Fire", C_FIRE));
        }

        void Toggle();
        void ListPowder();
        void ListSolid();
        void ListLiquid();
        void ListGas();
        void Draw();
};