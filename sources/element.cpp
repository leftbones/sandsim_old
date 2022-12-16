#include "element.h"

using namespace std;

// Element Grid
//
ElementGrid::ElementGrid(int x, int y) {
    Elements.resize(x * y);
}

// Get
ElementData& ElementGrid::Get(const Vector2i& pos) {
    return Elements[(GRID_W * pos.y) + pos.x];
}

ElementData& ElementGrid::Get(const Vector2i& pos, const Vector2i& offset) {
    return Elements[(GRID_W * (pos.y + offset.y)) + (pos.x + offset.x)];
}

// Set
void ElementGrid::Set(const Vector2i& pos, unsigned int id) {
    if (InBounds(pos))
        Get(pos).id = id;
}

void ElementGrid::Set(const Vector2i& pos, const Vector2i& offset, unsigned int id) {
    if (InBounds(pos, offset))
        Get(pos, offset).id = id;
}

// IsEmpty
bool ElementGrid::IsEmpty(const Vector2i& pos) {
    return InBounds(pos) && Get(pos).id == NONE;
}

bool ElementGrid::IsEmpty(const Vector2i& pos, const Vector2i& offset) {
    return InBounds(pos, offset) && Get(pos, offset).id == NONE;
}

// InBounds
bool ElementGrid::InBounds(const Vector2i& pos) {
    return pos.x >= 0 && pos.x < GRID_W && pos.y >= 0 && pos.y < GRID_H;
}

bool ElementGrid::InBounds(const Vector2i& pos, const Vector2i& offset) {
    return pos.x + offset.x >= 0 && pos.x + offset.x < GRID_W && pos.y + offset.y >= 0 && pos.y + offset.y < GRID_H;
}

// Swap
void ElementGrid::Swap(const Vector2i& pos, const Vector2i& offset) {
    if (!InBounds(pos, offset))
        return;

    ElementData &e1 = Get(pos);
    ElementData &e2 = Get(pos, offset);

    unsigned int id1 = e1.id;
    e1.id = e2.id;
    e2.id = id1;
}

// SwapIfEmpty
bool ElementGrid::SwapIfEmpty(const Vector2i& pos, const Vector2i& offset) {
    if (!InBounds(pos, offset) || Get(pos, offset).id != NONE)
        return false;

    Swap(pos, offset);
    return true;
}

// DoForEach
void ElementGrid::DoForEach(ElementGridCallback callback, bool valid_only) {
    if (!callback)
        return;

    static Vector2i pos;

    bool is_even_tick = ((tick_count % 2) == 0);
    int dir = is_even_tick ? 0 : 1;

    for (pos.y = GRID_H - 1; pos.y >= 0; pos.y--) {
        for (pos.x = dir ? 0 : GRID_W - 1; dir ? pos.x < GRID_W : pos.x > 0; dir ? pos.x++ : pos.x--) {
            ElementData& data = Get(pos);
            if (!valid_only || data.id != NONE)
                callback(data, *this, pos);
        }
    }
}


// Elements (Temp)
//

// Powder
//
void Powder::Update(ElementData& data, ElementGrid& grid, Vector2i& pos) {
    if (!grid.IsEmpty(pos, Vector2iDOWN) && !grid.IsEmpty(pos, Vector2iLEFT) && !grid.IsEmpty(pos, Vector2iRIGHT) && !grid.IsEmpty(pos, Vector2iDOWNLEFT) && !grid.IsEmpty(pos, Vector2iDOWNRIGHT))
        return;

    if (grid.SwapIfEmpty(pos, Vector2iDOWN))
        return;

    if (grid.SwapIfEmpty(pos, Vector2iDOWNLEFT))
        return;

    if (grid.SwapIfEmpty(pos, Vector2iDOWNRIGHT))
        return;
}

// Sand
class Sand: public Powder {
    void Create(ElementData& data, ElementGrid& grid, Vector2i& pos) override {
        data.id = SAND;
    }

    Color GetColor(ElementData& data, ElementGrid& grid, Vector2i& pos) override { return YELLOW; }
};


// Liquid
//
void Liquid::Update(ElementData& data, ElementGrid& grid, Vector2i& pos) {
    if (!grid.IsEmpty(pos, Vector2iDOWN) && !grid.IsEmpty(pos, Vector2iLEFT) && !grid.IsEmpty(pos, Vector2iRIGHT))
        return;

    if (grid.SwapIfEmpty(pos, Vector2iDOWN))
        return;

    if (RandChance(0.5)) {
        if (grid.SwapIfEmpty(pos, Vector2iLEFT))
            return;

        if (grid.SwapIfEmpty(pos, Vector2iRIGHT))
            return;
    } else {
        if (grid.SwapIfEmpty(pos, Vector2iRIGHT))
            return;

        if (grid.SwapIfEmpty(pos, Vector2iLEFT))
            return;
    }
}

// Water
class Water: public Liquid {
    void Create(ElementData& data, ElementGrid& grid, Vector2i& pos) override {
        data.id = WATER;
    }

    Color GetColor(ElementData& data, ElementGrid& grid, Vector2i& pos) override { return BLUE; }
};


// Element Registry
//
Element* GetElement(unsigned int id) {
    return Elements[id];
}

void RegisterElements() {
    Elements.insert_or_assign(SAND, new Sand());
    Elements.insert_or_assign(WATER, new Water());
}