#include "element.h"


// Element Grid
//
ElementData& ElementGrid::Get(const Vector2i& pos) {
	return elements[(size.x * pos.y) + pos.x];
}

ElementData& ElementGrid::Get(const Vector2i& pos, const Vector2i& offset) {
	return elements[(size.x * (pos.y + offset.y)) + (pos.x + offset.x)];
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
    return pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y;
}

bool ElementGrid::InBounds(const Vector2i& pos, const Vector2i& offset) {
    return pos.x + offset.x >= 0 && pos.x + offset.x < size.x && pos.y + offset.y >= 0 && pos.y + offset.y < size.y;
}

void ElementGrid::WakeNeighbors(const Vector2i& pos) {
	if (InBounds(Vector2i{pos.x - 1, pos.y})) Get(Vector2i{pos.x - 1, pos.y}).settled = false;
	if (InBounds(Vector2i{pos.x + 1, pos.y})) Get(Vector2i{pos.x + 1, pos.y}).settled = false;
	if (InBounds(Vector2i{pos.x, pos.y - 1})) Get(Vector2i{pos.x, pos.y - 1}).settled = false;
	if (InBounds(Vector2i{pos.x, pos.y + 1})) Get(Vector2i{pos.x, pos.y + 1}).settled = false;
	if (InBounds(Vector2i{pos.x - 1, pos.y - 1})) Get(Vector2i{pos.x - 1, pos.y - 1}).settled = false;
	if (InBounds(Vector2i{pos.x + 1, pos.y - 1})) Get(Vector2i{pos.x + 1, pos.y - 1}).settled = false;
	if (InBounds(Vector2i{pos.x - 1, pos.y + 1})) Get(Vector2i{pos.x - 1, pos.y + 1}).settled = false;
	if (InBounds(Vector2i{pos.x + 1, pos.y + 1})) Get(Vector2i{pos.x + 1, pos.y + 1}).settled = false;
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

	bool settled1 = e1.settled;
	e1.settled = e2.settled;
	e2.settled = settled1;

	Color color1 = e1.color;
	e1.color = e2.color;
	e2.color = color1;	
}

bool ElementGrid::SwapIfEmpty(const Vector2i& pos, const Vector2i& offset) {
    if (!InBounds(pos, offset) || Get(pos, offset).id != AIR)
        return false;

	Get(pos, offset).settled = false;

	WakeNeighbors(pos);
	WakeNeighbors(Vector2i{pos.x + offset.x, pos.y + offset.y});

    Swap(pos, offset);
    return true;
}


// Base Elements (Temp)
//
void Powder::Update(ElementGrid& grid, ElementData& data, Vector2i& pos) {
	if (grid.InBounds(pos, V_BELOW) && RandChance(0.5) && GetElement(grid.Get(pos, V_BELOW).id)->type == TypeLiquid) {
		grid.Swap(pos, V_BELOW);
		return;
	}

	if (!grid.IsEmpty(pos, V_BELOW) && !grid.IsEmpty(pos, V_LEFT) && !grid.IsEmpty(pos, V_RIGHT) && !grid.IsEmpty(pos, V_BELOW_LEFT) && !grid.IsEmpty(pos, V_BELOW_RIGHT)) {
		data.settled = true;
		return;
	}

    if (grid.SwapIfEmpty(pos, V_BELOW)) {
		data.settled = false;
        return;
	}

	if (RandChance(friction)) {
		data.settled = true;
		return;
	}

	if (!data.settled) {
		int dir = 1;
		if (RandChance(0.5))
			dir = -1;

		if (grid.SwapIfEmpty(pos, Vector2i{dir, 1}))
			return;

		if (grid.SwapIfEmpty(pos, Vector2i{-dir, 1}))
			return;

		if (RandChance(friction)) {
			if (grid.SwapIfEmpty(pos, Vector2i{dir, 0}))
				return;

			if (grid.SwapIfEmpty(pos, Vector2i{-dir, 0}))
				return;
		}
	}
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
	public:
		std::string name = "Air";

		void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
			data.id = AIR;
			name = "Air";
		}

		Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return BLACK; }
};

class Sand : public Powder {
	public:
		Sand() {
			name = "Sand";
			type = TypePowder;
		}

		void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
			data.id = SAND;
			data.color = OffsetColor(YELLOW, -50, 50);
			name = "Sand";
		}

		Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return YELLOW; }
};

class Water : public Liquid {
	public:
		Water() {
			name = "Water";
			type = TypeLiquid;
		}

		void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
			data.id = WATER;
			data.color = BLUE;
			name = "Water";
		}

		Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return BLUE; }
};

class Wood : public Solid {
	public:
		Wood() {
			name = "Wood";
			type = TypeSolid;
		}

		void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
			data.id = WOOD;
			data.color = OffsetColor(DARKBROWN, -25, 25);
		}

		Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return BROWN; }
};

class Fire : public Gas {
	public:
		static constexpr int burn_time = 120;

		Fire() {
			name = "Fire";
			type = TypeGas;
		}

		void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
			data.id = FIRE;
			data.life = burn_time;
			data.color = OffsetColor(ORANGE, -50, 50);
		}

		Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return RED; }
};

class Salt : public Powder {
	public:
		Salt() {
			name = "Salt";
			type = TypePowder;
		}

		void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
			data.id = SALT;
			data.color = OffsetColor(WHITE, -50, 50);
		}

	Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return WHITE; }
};

class Concrete_Powder : public Powder {
	public:
		Concrete_Powder() {
			name = "Concrete Powder";
			type = TypePowder;
		}

		void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
			data.id = CONCRETE_POWDER;
			data.color = OffsetColor(LIGHTGRAY, -50, 50);
		}

		Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return LIGHTGRAY; }
};

class Concrete : public Solid {
	public:
		Concrete() {
			name = "Concrete";
			type = TypeSolid;
		}

		void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
			data.id = CONCRETE;
			data.color = OffsetColor(GRAY, -50, 50);
		}

		Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return GRAY; }
};

class Oil : public Liquid {
	public:
		Oil() {
			name = "Oil";
			type = TypeLiquid;
		}

		void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {
			data.id = OIL;
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
	Elements[5] = new Salt();
	Elements[6] = new Concrete_Powder();
	Elements[7] = new Concrete();
	Elements[8] = new Oil();
}