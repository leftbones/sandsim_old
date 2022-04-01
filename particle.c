// particle.c

#include "particle.h"

// Create a new particle of a specified type and add it to the grid
void NewParticle(Particle grid[GRID_WIDTH][GRID_HEIGHT], int id, int x, int y) {
	Particle np;

	np.id = materials[id].id;
	np.weight = materials[id].weight;
	np.flow_rate = materials[id].flow_rate;
	np.color = materials[id].color;
	np.flammability = materials[id].flammability;

	// Optional parameters, left as null or set to default if not defined
	if (materials[id].pref_density) { np.pref_density = materials[id].pref_density; }

	np.has_moved = false;

	grid[x][y] = np;
}

// Swaps two particles at position 1 and position 2, can also swap with empty cells to "move" a particle
void SwapParticles(Particle grid[GRID_WIDTH][GRID_HEIGHT], int x1, int y1, int x2, int y2) {
	Particle p1 = grid[x1][y1];
	Particle p2 = grid[x2][y2];

	NewParticle(grid, p1.id, x2, y2);
	NewParticle(grid, p2.id, x1, y1);

	grid[x1][y1].has_moved = true;
	grid[x2][y2].has_moved = true;
}

// Changes one particle into another
void ChangeParticle(Particle grid[GRID_WIDTH][GRID_HEIGHT], int id, int x, int y) {
	NewParticle(grid, id, x, y);
}

// Returns the number of cells of a specific element in the 8 surrounding cells of the specified cell
int NumberTouching(Particle grid[GRID_WIDTH][GRID_HEIGHT], int id, int x, int y) {
	int num = 0;

	if (grid[x-1][y-1].id == id) { num++; } // Top Left
	if (grid[x][y-1].id == id) { num++; }   // Top Middle
	if (grid[x+1][y-1].id == id) { num++; } // Top Right
	if (grid[x-1][y].id == id) { num++; }   // Middle Left
	if (grid[x+1][y].id == id) { num++; }   // Middle Right
	if (grid[x-1][y+1].id == id) { num++; } // Bottom Left
	if (grid[x][y+1].id == id) { num++; }   // Bottom Middle
	if (grid[x+1][y+1].id == id) { num++; } // Bottom Right

	return num;
}

// Check if a grid cell is empty (contains id 0)
bool IsEmpty(Particle grid[GRID_WIDTH][GRID_HEIGHT], int x, int y) {
	return (grid[x][y].id == 0);
}

// Check if a grid cell contains a specific type
bool IsType(Particle grid[GRID_WIDTH][GRID_HEIGHT], int id, int x, int y) {
	return (grid[x][y].id == id);
}

// Check if a grid cell is within the grid boundaries
bool InBounds(Particle grid[GRID_WIDTH][GRID_HEIGHT], int x, int y) {
	return x > 0 && x < GRID_WIDTH && y > 0 && y < GRID_HEIGHT;
}

// Check if a grid cell is any of the passed particle types
bool IsAny(Particle p, int types[]) {
	int len = sizeof(types) / sizeof(types[0]);

	for (int i = 0; i < len; i++) {
		if (types[i] == p.id) {
			return true;
		}
	}

	return false;
}

// Spread material to neighboring materials of a specified type
void InfectNeighbors(Particle grid[GRID_WIDTH][GRID_HEIGHT], int source, int target, int chance, bool stop_on_success, bool check_diag, int x, int y) {
	if (DiceRoll(chance) && grid[x][y-1].id == target) { ChangeParticle(grid, source, x, y-1); if (stop_on_success) { return; }}
	if (DiceRoll(chance) && grid[x][y+1].id == target) { ChangeParticle(grid, source, x, y+1); if (stop_on_success) { return; }}
	if (DiceRoll(chance) && grid[x-1][y].id == target) { ChangeParticle(grid, source, x-1, y); if (stop_on_success) { return; }}
	if (DiceRoll(chance) && grid[x+1][y].id == target) { ChangeParticle(grid, source, x+1, y); if (stop_on_success) { return; }}

	if (!check_diag) { return; }

	if (DiceRoll(chance) && grid[x-1][y-1].id == target) { ChangeParticle(grid, source, x-1, y-1); if (stop_on_success) { return; }}
	if (DiceRoll(chance) && grid[x+1][y-1].id == target) { ChangeParticle(grid, source, x+1, y-1); if (stop_on_success) { return; }}
	if (DiceRoll(chance) && grid[x-1][y+1].id == target) { ChangeParticle(grid, source, x-1, y+1); if (stop_on_success) { return; }}
	if (DiceRoll(chance) && grid[x+1][y+1].id == target) { ChangeParticle(grid, source, x+1, y+1); if (stop_on_success) { return; }}
}

// UNFINISHED - Spread material to cells that are not immediately touching the material's type
void InfectNeighborsAntisocial(Particle grid[GRID_WIDTH][GRID_HEIGHT], int source, int target, int chance, int x, int y) {
	if (DiceRoll(chance)) {
		if (InBounds(grid, x-1, y) && IsType(grid, target, x-1, y)) {}
		if (InBounds(grid, x+1, y) && IsType(grid, target, x-1, y)) {}
		if (InBounds(grid, x, y-1) && IsType(grid, target, x-1, y)) {}
		if (InBounds(grid, x, y+1) && IsType(grid, target, x-1, y)) {}
	}
}

// Spread flames to nearby flammable materials
void BurnNeighbors(Particle grid[GRID_WIDTH][GRID_HEIGHT], int source, int x, int y) {
	bool burned = false;

	if (InBounds(grid, x, y-1) && grid[x][y-1].flammability != 0 && DiceRoll(BURN_RATE - materials[grid[x][y-1].id].flammability)) { ChangeParticle(grid, source, x, y-1); burned = true; }
	if (InBounds(grid, x, y+1) && grid[x][y+1].flammability != 0 && DiceRoll(BURN_RATE - materials[grid[x][y+1].id].flammability)) { ChangeParticle(grid, source, x, y+1); burned = true; }
	if (InBounds(grid, x-1, y) && grid[x-1][y].flammability != 0 && DiceRoll(BURN_RATE - materials[grid[x-1][y].id].flammability)) { ChangeParticle(grid, source, x-1, y); burned = true; }
	if (InBounds(grid, x+1, y) && grid[x+1][y].flammability != 0 && DiceRoll(BURN_RATE - materials[grid[x+1][y].id].flammability)) { ChangeParticle(grid, source, x+1, y); burned = true; }
	if (InBounds(grid, x-1, y-1) && grid[x-1][y-1].flammability != 0 && DiceRoll(BURN_RATE - materials[grid[x-1][y-1].id].flammability)) { ChangeParticle(grid, source, x-1, y-1); burned = true; }
	if (InBounds(grid, x+1, y-1) && grid[x+1][y-1].flammability != 0 && DiceRoll(BURN_RATE - materials[grid[x+1][y-1].id].flammability)) { ChangeParticle(grid, source, x+1, y-1); burned = true; }
	if (InBounds(grid, x-1, y+1) && grid[x-1][y+1].flammability != 0 && DiceRoll(BURN_RATE - materials[grid[x-1][y+1].id].flammability)) { ChangeParticle(grid, source, x-1, y+1); burned = true; }
	if (InBounds(grid, x+1, y+1) && grid[x+1][y+1].flammability != 0 && DiceRoll(BURN_RATE - materials[grid[x+1][y+1].id].flammability)) { ChangeParticle(grid, source, x+1, y+1); burned = true; }

	if (burned && InBounds(grid, x, y-1) && IsEmpty(grid, x, y-1)) {
		NewParticle(grid, P_EMBER, x, y);
		NewParticle(grid, P_SMOKE, x, y-1);
	}
}

// UNFINISHED - Wake neighboring cells only when necessary, for optimization
void WakeNeighbors(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {

}

// Creates particles in a random radius around the mouse position
void PaintParticles(Particle grid[GRID_WIDTH][GRID_HEIGHT], int brush_size, int brush_density, int id, int x, int y) {
	int n = 0;
	int tries = 0;
	int limit = 10;
	int goal = brush_size / brush_density;

	while (n < goal) {
		float r = brush_size * sqrt((float)rand() / RAND_MAX);
		float t = (float)rand() / RAND_MAX * 2 * M_PI;

		int px = round((x) + r * cos(t)) / 2;
		int py = round((y) + r * sin(t)) / 2;

		if (InBounds(grid, px, py) && IsEmpty(grid, px, py)) {
			NewParticle(grid, id, px, py);
			n += 1;
			tries = 0;
		} else {
			if (tries >= limit) {
				n += 1;
				tries = 0;
			} else {
				tries += 1;
			}
		}
	}
}

// Paint solid particles (square brush only)
void PaintSolidParticles(Particle grid[GRID_WIDTH][GRID_HEIGHT], int brush_size, int id, int x, int y) {
	for (int c = y-brush_size; c < y+brush_size; c++) {
		for (int r = x-brush_size; r < x+brush_size; r++) {
			if (InBounds(grid, r / 2, c / 2)) {
				NewParticle(grid, id, r / 2, c / 2);
			}
		}
	}
}

// Returns the number of non-empty cells in the grid
int GetParticleCount(Particle grid[GRID_WIDTH][GRID_HEIGHT]) {
	int count = 0;

	for (int y = GRID_HEIGHT-1; y > 0; y--) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (grid[x][y].id != 0) { count++; }
		}
	}

	return count;
}
