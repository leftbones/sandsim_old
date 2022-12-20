#pragma once

#include "main.h"
#include <vector>


// Element Data
//

struct ElementData {
	unsigned int id = AIR;
	Color color = MAGENTA;
	int life = 0;
	bool settled = false;
};

enum ElementType { TypePowder, TypeSolid, TypeLiquid, TypeGas };

// Element Grid
//
class ElementGrid {
	public:
		Vector2i size;
		std::vector<ElementData> elements;

		ElementGrid(int w, int h) {
			size = {w, h};
			elements.resize(w * h);
		}

		ElementData& Get(const Vector2i& pos);
		ElementData& Get(const Vector2i& pos, const Vector2i& offset);

		void Set(const Vector2i& pos, unsigned int id);
		void Set(const Vector2i& pos, Vector2i& offset, unsigned int id);

		bool IsEmpty(const Vector2i& pos);
		bool IsEmpty(const Vector2i& pos, const Vector2i& offset);

		bool InBounds(const Vector2i& pos);
		bool InBounds(const Vector2i& pos, const Vector2i& offset);

		void WakeNeighbors(const Vector2i& pos);

		void Swap(const Vector2i& pos, const Vector2i& offset);
		bool SwapIfEmpty(const Vector2i& pos, const Vector2i& offset);
};


// Base Element
//
class Element {
	public:
		std::string name;
		ElementType type;

		virtual ~Element() = default;
		virtual void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {}
		virtual void Update(ElementGrid& grid, ElementData& data, Vector2i& pos) {}
		virtual Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return MAGENTA; }
};


// Base Element Types
//
class Powder : public Element {
	public:
		float friction = 0.05;

		virtual ~Powder() = default;
		void Update(ElementGrid& grid, ElementData& data, Vector2i& pos) override;
};

class Solid : public Element {
	public:
		virtual ~Solid() = default;
		void Update(ElementGrid& grid, ElementData& data, Vector2i& pos) override;
};

class Liquid : public Element {
	public:
		virtual ~Liquid() = default;
		void Update(ElementGrid& grid, ElementData& data, Vector2i& pos) override;
};

class Gas : public Element {
	public:
		virtual ~Gas() = default;
		void Update(ElementGrid& grid, ElementData& data, Vector2i& pos) override;
};


// Element Registry
//

static Element* Elements[4];
void RegisterElements();

Element* GetElement(unsigned int id);