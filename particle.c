// particle.c

#include "particle.h"


void NewParticle(Particle grid[GRID_WIDTH][GRID_HEIGHT], int id, int x, int y) {
	Particle np;

	np.id = materials[id].id;
	np.weight = materials[id].weight;
	np.flow_rate = materials[id].flow_rate;
	np.color = materials[id].color;
	np.has_moved = false;

	grid[x][y] = np;
}

void SwapParticles(Particle grid[GRID_WIDTH][GRID_HEIGHT], int x1, int y1, int x2, int y2) {
	Particle p1 = grid[x1][y1];
	Particle p2 = grid[x2][y2];

	NewParticle(grid, p1.id, x2, y2);
	NewParticle(grid, p2.id, x1, y1);

	grid[x1][y1].has_moved = true;
	grid[x2][y2].has_moved = true;
}

bool IsEmpty(Particle grid[GRID_WIDTH][GRID_HEIGHT], int x, int y) {
	return (grid[x][y].id == 0);
}

bool InBounds(Particle grid[GRID_WIDTH][GRID_HEIGHT], int x, int y) {
	return x > 0 && x < GRID_WIDTH && y > 0 && y < GRID_HEIGHT;
}

void WakeNeighbors(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {

}

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

int GetParticleCount(Particle grid[GRID_WIDTH][GRID_HEIGHT]) {
	int count = 0;

	for (int y = GRID_HEIGHT-1; y > 0; y--) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (grid[x][y].id != 0) { count++; }
		}
	}

	return count;
}
