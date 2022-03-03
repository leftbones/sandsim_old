// Particle Movement Functions
// ------------------------------

// Determine particle fall distance
int FallParticle(Particle grid[SCREEN_W][SCREEN_H], int x, int y) {
	Particle p = grid[x][y];

	for (int i = p.fall; i > 0; i--) {
		if (InBounds(grid, x, y+i) && IsEmpty(grid, x, y+i)) {
			return i;
		}
	}

	return 0;
}

// Determine particle flow distance
int FlowParticle(Particle grid[SCREEN_W][SCREEN_H], int dir, int x, int y) {
	Particle p = grid[x][y];

	if (dir < 0 ) {
		for (int i = p.flow; i > 0; i--) {
			if (InBounds(grid, x-i, y) && IsEmpty(grid, x-i, y)) {
				return i;
			}
		}
	} else {
		for (int i = p.flow; i > 0; i--) {
			if (InBounds(grid, x+i, y) && IsEmpty(grid, x+i, y)) {
				return i;
			}
		}
	}

	return 0;
}

// Material-specific Update Functions
// ------------------------------
// Sand
void UpdateSand(Particle grid[SCREEN_W][SCREEN_H], int x, int y) {
	if (InBounds(grid, x, y+1) && (IsEmpty(grid, x, y+1))) // Move down
	{
		grid[x][y].falling = true;

		int nY = FallParticle(grid, x, y);

		SetParticle(grid, 1, x, y+nY);
		SetParticle(grid, 0, x, y);
	}
	else if (InBounds(grid, x, y+1) && grid[x][y+1].id == 2 && 1 + (rand() % 4) == 1) // Chance to displace water
	{
		grid[x][y].falling = true;

		// Does not use fall speed
		SetParticle(grid, 1, x, y+1);
		SetParticle(grid, 2, x, y);
	}
	else if (IsFalling(grid, x, y))
	{
		if (InBounds(grid, x-1, y+1) && IsEmpty(grid, x-1, y+1)) // Move down + left
		{

			grid[x][y].falling = true;

			int nX = FlowParticle(grid, -1, x, y);
			int nY = FallParticle(grid, x-nX, y);

			SetParticle(grid, 1, x-nX, y+nY);
			SetParticle(grid, 0, x, y);
		}
		else if (InBounds(grid, x+1, y+1) && IsEmpty(grid, x+1, y+1)) // Move down + right
		{
			grid[x][y].falling = true;

			int nX = FlowParticle(grid, 1, x, y);
			int nY = FallParticle(grid, x+nX, y);

			SetParticle(grid, 1, x+nX, y+nY);
			SetParticle(grid, 0, x, y);
		}
		else
		{
			grid[x][y].falling = false;
		}
	}
}


// Water
void UpdateWater(Particle grid[SCREEN_W][SCREEN_H], int x, int y) {
	if (InBounds(grid, x, y+1) && IsEmpty(grid, x, y+1)) // Move down
	{
		int nY = FallParticle(grid, x, y);

		SetParticle(grid, 2, x, y+nY);
		SetParticle(grid, 0, x, y);
	}
	else if (InBounds(grid, x-1, y+1) && IsEmpty(grid, x-1, y+1)) // Move down + left
	{
		int nX = FlowParticle(grid, -1, x, y);
		int nY = FallParticle(grid, x-nX, y);

		SetParticle(grid, 2, x-nX, y+nY);
		SetParticle(grid, 0, x, y);
	}
	else if (InBounds(grid, x+1, y+1) && IsEmpty(grid, x+1, y+1)) // Move down + right
	{
		int nX = FlowParticle(grid, 1, x, y);
		int nY = FallParticle(grid, x+nX, y);

		SetParticle(grid, 2, x+nX, y+nY);
		SetParticle(grid, 0, x, y);
	}
	else if (InBounds(grid, x-1, y) && IsEmpty(grid, x-1, y)) // Flow left
	{
		int nX = FlowParticle(grid, -1, x, y);

		SetParticle(grid, 2, x-nX, y);
		SetParticle(grid, 0, x, y);
	}
	else if (InBounds(grid, x+1, y) && IsEmpty(grid, x+1, y)) // Flow right
	{
		int nX = FlowParticle(grid, 1, x, y);

		SetParticle(grid, 2, x+nX, y);
		SetParticle(grid, 0, x, y);
	}
}
