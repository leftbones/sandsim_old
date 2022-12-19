#pragma once

#include "main.h"
#include <vector>


// Element Data
//

struct ElementData {
	unsigned int id = AIR;
	int life = 0;
};


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

		bool InBounds(const Vector2i& pos);
		bool InBounds(const Vector2i& pos, const Vector2i& offset);

		bool IsEmpty(const Vector2i& pos);
		bool IsEmpty(const Vector2i& pos, const Vector2i& offset);

		void Swap(const Vector2i& pos, const Vector2i& offset);
		bool SwapIfEmpty(const Vector2i& pos, const Vector2i& offset);
};


// Base Element
//
class Element {
	public:
		std::string name;

		virtual ~Element() = default;
		virtual void Create(ElementGrid& grid, ElementData& data, Vector2i& pos) {}
		virtual void Update(ElementGrid& grid, ElementData& data, Vector2i& pos) {}
		virtual Color GetColor(ElementGrid& grid, ElementData& data, Vector2i& pos) { return MAGENTA; }
};


// Base Element Types
//
class Powder : public Element {
	public:
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