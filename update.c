// update.c

// Powders
// ------------------------------

void UpdateSand(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {
	// Down
	if (IsEmpty(grid, x, y+p.weight) && InBounds(grid, x, y+p.weight)) {
		SwapParticles(grid, x, y, x, y+p.weight);
	}

	// Displace water
	else if (grid[x][y+p.weight].id == P_WATER && DiceRoll(2)) {
		SwapParticles(grid, x, y, x, y+p.weight);
	}

	if (p.has_moved) {
		// Down + Left
		if (IsEmpty(grid, x-p.flow_rate, y+p.weight) && InBounds(grid, x-p.flow_rate, y+p.weight)) {
			SwapParticles(grid, x, y, x-p.flow_rate, y+p.weight);
		}

		// Down + Right
		else if (IsEmpty(grid, x+p.flow_rate, y+p.weight) && InBounds(grid, x+p.flow_rate, y+p.weight)) {
			SwapParticles(grid, x, y, x+p.flow_rate, y+p.weight);
		}

		else {
			grid[x][y].has_moved = false;
		}
	}
}

void UpdateSalt(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {
	// Down
	if (IsEmpty(grid, x, y+p.weight) && InBounds(grid, x, y+p.weight)) {
		SwapParticles(grid, x, y, x, y+p.weight);
	}

	// Displace water
	else if (grid[x][y+p.weight].id == P_WATER && DiceRoll(4)) {
		SwapParticles(grid, x, y, x, y+p.weight);
	}

	// Down + Left
	else if (IsEmpty(grid, x-p.flow_rate, y+p.weight) && InBounds(grid, x-p.flow_rate, y+p.weight)) {
		SwapParticles(grid, x, y, x-p.flow_rate, y+p.weight);
	}

	// Down + Right
	else if (IsEmpty(grid, x+p.flow_rate, y+p.weight) && InBounds(grid, x+p.flow_rate, y+p.weight)) {
		SwapParticles(grid, x, y, x+p.flow_rate, y+p.weight);
	}

	else {
		grid[x][y].has_moved = false;
	}
}


// Liquids
// ------------------------------

void UpdateWater(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {
	// Down
	if (IsEmpty(grid, x, y+p.weight) && InBounds(grid, x, y+p.weight)) {
		SwapParticles(grid, x, y, x, y+p.weight);
//		y = y+p.weight;
//		p = grid[x][y+p.weight];
	}

	// Left
	else if (IsEmpty(grid, x-1, y) && InBounds(grid, x-1, y)) {
		for (int i = p.flow_rate; i > 0; i--) {
			if (InBounds(grid, x-i, y) && IsEmpty(grid, x-i, y)) {
				SwapParticles(grid, x, y, x-i, y);
				break;
			}
		}
	}

	// Right
	else if (IsEmpty(grid, x+1, y) && InBounds(grid, x+1, y)) {
		for (int i = p.flow_rate; i > 0; i--) {
			if (InBounds(grid, x+i, y) && IsEmpty(grid, x+i, y)) {
				SwapParticles(grid, x, y, x+i, y);
				break;
			}
		}
	}

	else {
		grid[x][y].has_moved = false;
	}
}


// Main Update
// ------------------------------

void UpdateParticles(Particle grid[GRID_WIDTH][GRID_HEIGHT]) {
	bool is_even_tick = ((tick_count % 2) == 0);
	int dir = is_even_tick ? 0 : 1;

	for (int y = GRID_HEIGHT; y > 0; y--) {
		for (int x = dir ? 0 : GRID_WIDTH - 1; dir ? x < GRID_WIDTH : x > 0; dir ? x++ : x--) {
			if (!IsEmpty(grid, x, y)) {
				Particle p = grid[x][y];

				switch(p.id) {
					case 1: // Sand
						UpdateSand(grid, p, x, y);
						break;
					case 2: // Water
						UpdateWater(grid, p, x, y);
						break;
					case 3: // Salt
						UpdateSalt(grid, p, x, y);
						break;
				}
			}
		}
	}
}

// Main Draw
// ------------------------------

void DrawParticles(Particle grid[GRID_WIDTH][GRID_HEIGHT]) {
	for (int y = GRID_HEIGHT; y > 0; y--) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			Particle p = grid[x][y];

			if (p.id != 0) {
				DrawRectangle(SCALE_FACTOR * x, SCALE_FACTOR * y, SCALE_FACTOR, SCALE_FACTOR, p.color);
			}
		}
	}
}
