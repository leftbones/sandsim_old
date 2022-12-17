#pragma once

#include "main.h"

#include <vector>
#include <functional>
#include <map>

using namespace std;

// Element IDs
constexpr unsigned int NONE = 0;
constexpr unsigned int SAND = 1;
constexpr unsigned int WATER = 2;
constexpr unsigned int OIL = 3;
constexpr unsigned int FIRE = 4;
constexpr unsigned int WOOD = 5;
constexpr unsigned int GUNPOWDER = 6;

// Element Colors
constexpr Color C_SAND = { 255, 255, 0, 255 };
constexpr Color C_WATER = { 0, 0, 255, 255 };
constexpr Color C_OIL = { 140, 40, 40, 255 };
constexpr Color C_FIRE = { 255, 0, 0, 255 };
constexpr Color C_WOOD = { 85, 65, 40, 255 };
constexpr Color C_GUNPOWDER = { 80, 60, 60, 255 };

// Element Data
struct ElementData {
    unsigned int id = NONE;
};

struct Vector2i {
    int x = 0;
    int y = 0;
};

constexpr Vector2i Vector2iZERO = { 0, 0 };
constexpr Vector2i Vector2iLEFT= { -1, 0 };
constexpr Vector2i Vector2iRIGHT = { 1, 0 };
constexpr Vector2i Vector2iDOWN = { 0, 1 };
constexpr Vector2i Vector2iDOWNLEFT = { -1, 1 };
constexpr Vector2i Vector2iDOWNRIGHT = { 1, 1 };
constexpr Vector2i Vector2iUP = { 0, -1 };

// Element Grid
class ElementGrid;

using ElementGridCallback = function<void(ElementData&, ElementGrid&, Vector2i&)>;

class ElementGrid {
    public:
        ElementGrid(int x, int y);

        vector<ElementData> Elements;

        ElementData& Get(const Vector2i& pos); // Return a reference to an element in the grid
        ElementData& Get(const Vector2i& pos, const Vector2i& offset);

        void Set(const Vector2i& pos, unsigned int id); // Override an element in the grid with another
        void Set(const Vector2i& pos, const Vector2i& offset, unsigned int id);

        bool InBounds(const Vector2i& pos); // Check if a grid position is valid
        bool InBounds(const Vector2i& pos, const Vector2i& offset);

        bool IsEmpty(const Vector2i& pos); // Check if a grid position is empty (contains 'NONE' element)
        bool IsEmpty(const Vector2i& pos, const Vector2i& offset);

        void Swap(const Vector2i& pos1, const Vector2i& offset); // Swap two elements in the grid regardless of type
        bool SwapIfEmpty(const Vector2i& pos, const Vector2i& offset); // Swap two elements in the grid if one of them is empty

        void DoForEach(ElementGridCallback callback); // Perform a callback for all elements in the grid not including NONE elements
};

// Base Element
class Element {
    public:
        string name;
        virtual ~Element() = default;
        virtual void Create(ElementData& data, ElementGrid& grid, Vector2i& pos) {}
        virtual void Update(ElementData& data, ElementGrid& grid, Vector2i& pos) {}
        virtual Color GetColor(ElementData& data, ElementGrid& grid, Vector2i& pos) { return MAGENTA; }
};

// Element Types
class Powder: public Element {
    public:
        virtual ~Powder() = default;
        void Update(ElementData& data, ElementGrid& grid, Vector2i& pos) override;
};

class Liquid: public Element {
    public:
        virtual ~Liquid() = default;
        void Update(ElementData& data, ElementGrid& grid, Vector2i& pos) override;
};

// Element Registry
static map<unsigned int, Element*> Elements;
void RegisterElements();

Element* GetElement(unsigned int id);