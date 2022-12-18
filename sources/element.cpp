#include "element.h"


// Element Grid
//
ElementData& ElementGrid::Get(const Vector2i& pos) {
	return elements[(GRID_W * pos.y) + pos.x];
}

ElementData& ElementGrid::Get(const Vector2i& pos, const Vector2i& offset) {
    return elements[(GRID_W * (pos.y + offset.y)) + (pos.x + offset.x)];
}

void ElementGrid::Set(const Vector2i& pos, unsigned int id) {
    if (InBounds(pos))
        Get(pos).id = id;
}

void ElementGrid::Set(const Vector2i& pos, Vector2i& offset, unsigned int id) {
    if (InBounds(pos, offset))
        Get(pos, offset).id = id;
}

bool ElementGrid::IsEmpty(const Vector2i& pos) {
    return InBounds(pos) && Get(pos).id == AIR;
}

bool ElementGrid::IsEmpty(const Vector2i& pos, const Vector2i& offset) {
    return InBounds(pos, offset) && Get(pos, offset).id == AIR;
}

bool ElementGrid::InBounds(const Vector2i& pos) {
    return pos.x >= 0 && pos.x < GRID_W && pos.y >= 0 && pos.y < GRID_H;
}

bool ElementGrid::InBounds(const Vector2i& pos, const Vector2i& offset) {
    return pos.x + offset.x >= 0 && pos.x + offset.x < GRID_W && pos.y + offset.y >= 0 && pos.y + offset.y < GRID_H;
}

void ElementGrid::Swap(const Vector2i& pos, const Vector2i& offset) {
    if (!InBounds(pos, offset))
        return;

    ElementData &e1 = Get(pos);
    ElementData &e2 = Get(pos, offset);

    unsigned int id1 = e1.id;
    e1.id = e2.id;
    e2.id = id1;

	int life1 = e1.life;
	e1.life = e2.life;
	e2.life = life1;
}

bool ElementGrid::SwapIfEmpty(const Vector2i& pos, const Vector2i& offset) {
    if (!InBounds(pos, offset) || Get(pos, offset).id != AIR)
        return false;

    Swap(pos, offset);
    return true;
}


// Base Elements (Temp)
//
void Powder::Update(ElementGrid& grid, ElementData& data, Vector2i& pos) {
	if (!grid.IsEmpty(pos, V_BELOW) && !grid.IsEmpty(pos, V_LEFT) && !grid.IsEmpty(pos, V_RIGHT) && !grid.IsEmpty(pos, V_BELOW_LEFT) && !grid.IsEmpty(pos, V_BELOW_RIGHT))
	return;

    if (grid.SwapIfEmpty(pos, V_BELOW))
        return;

    if (grid.SwapIfEmpty(pos, V_BELOW_LEFT))
        return;

    if (grid.SwapIfEmpty(pos, V_BELOW_RIGHT))
        return;
}

void Solid::Update(ElementGrid& grid, ElementData& data, Vector2i& pos) {
	return;
}

void Liquid::Update(ElementGrid& grid, ElementData& data, Vector2i& pos) {
	if (!grid.IsEmpty(pos, V_BELOW) && !grid.IsEmpty(pos, V_LEFT) && !grid.IsEmpty(pos, V_RIGHT))
		return;

    if (grid.SwapIfEmpty(pos, V_BELOW))
        return;

	int dist = RandRange(1, 4);

	if (RandRoll(2)) {
		if (grid.SwapIfEmpty(pos, Vector2i{-dist, 0}))
			return;

		if (grid.SwapIfEmpty(pos, Vector2i{dist, 0}))
			return;
	} else {
		if (grid.SwapIfEmpty(pos, Vector2i{dist, 0}))
			return;

		if (grid.SwapIfEmpty(pos, Vector2i{-dist, 0}))
			return;
	}
}

void Gas::Update(ElementGrid& grid, ElementData& data, Vector2i& pos) {
	data.life--;

	if (data.life <= 0) {
		GetElement(AIR)->Create(grid, data, pos);
		return;
	}

	if (!grid.IsEmpty(pos, V_ABOVE) && !grid.IsEmpty(pos, V_LEFT) && !grid.IsEmpty(pos, V_RIGHT) && !grid.IsEmpty(pos, V_ABOVE_LEFT) && !grid.IsEmpty(pos, V_ABOVE_RIGHT))
		return;

	if (RandChance(0.60) && grid.SwapIfEmpty(pos, V_ABOVE))
		return;

	if (RandChance(0.10) && grid.SwapIfEmpty(pos, V_ABOVE_LEFT))
		return;

	if (RandChance(0.10) && grid.SwapIfEmpty(pos, V_ABOVE_RIGHT))
		return;

	if (RandChance(0.50) && grid.SwapIfEmpty(pos, V_LEFT))
		return;

	if (RandChance(0.50) && grid.SwapIfEmpty(pos, V_RIGHT))
		return;
}


// Element Types (Temp)
//
class Air : public Gas {
	void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
		data.id = AIR;
		name = "Air";
	}

	Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return BLACK; }
};

class Sand : public Powder {
	void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
		data.id = SAND;
		name = "Sand";
	}

	Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return YELLOW; }
};

class Water : public Liquid {
	void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
		data.id = WATER;
		name = "Water";
	}

	Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return BLUE; }
};

class Wood : public Solid {
	void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
		data.id = WOOD;
		name = "Wood";
	}

	Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return BROWN; }
};

class Fire : public Gas {
	public:
		static constexpr int burn_time = 120;

	void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
		data.id = FIRE;
		data.life = burn_time;
		name = "Fire";
	}

	Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return RED; }
};


// Element Registry
//
Element* GetElement(unsigned int id) {
	return Elements[id];
}

void RegisterElements() {
	Elements[0] = new Air();
	Elements[1] = new Sand();
	Elements[2] = new Water();
	Elements[3] = new Wood();
	Elements[4] = new Fire();
}