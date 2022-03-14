// ------------------------------
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

// Move between points on the grid, checking for an open space
void MoveBetweenPoints(Particle grid[SCREEN_W][SCREEN_H], int x1, int y1, int x2, int y2) {

}


// ------------------------------
// Material Update Functions
// ------------------------------

//
// Sand
//
void UpdateSand(Particle grid[SCREEN_W][SCREEN_H], int x, int y) {
	Particle p = grid[x][y];

	if (InBounds(grid, x, y+1) && (IsEmpty(grid, x, y+1))) // Move down
	{
		p.falling = true;

		int nY = FallParticle(grid, x, y);

		SetParticle(grid, P_SAND, x, y+nY);
		SetParticle(grid, P_EMPTY, x, y);
	}
	else if (InBounds(grid, x, y+1) && grid[x][y+1].id == P_WATER && 1 + (rand() % 3) == 1) // Chance to displace water
	{
		p.falling = true;
		grid[x][y+1].falling = true;
		SetParticle(grid, P_SAND, x, y+1);
		SetParticle(grid, P_WATER, x, y);
	}
	else if (IsFalling(grid, x, y))
	{
		if (InBounds(grid, x-1, y+1) && IsEmpty(grid, x-1, y+1)) // Move down + left
		{
			MoveBetweenPoints(grid, x, y, x-p.flow, y+p.fall);
		}
		else if (InBounds(grid, x+1, y+1) && IsEmpty(grid, x+1, y+1)) // Move down + right
		{
			MoveBetweenPoints(grid, x, y, x+p.flow, y+p.fall);
		}
		else
		{
			p.falling = false;
		}
	}
}


// Water
void UpdateWater(Particle grid[SCREEN_W][SCREEN_H], int x, int y) {
	Particle p = grid[x][y];

	if (InBounds(grid, x, y+1) && IsEmpty(grid, x, y+1)) // Move down
	{
		int nY = FallParticle(grid, x, y);

		SetParticle(grid, P_WATER, x, y+nY);
		SetParticle(grid, P_EMPTY, x, y);

		p.falling = true;
	}
	else if (IsFalling(grid, x, y))
	{
		if (InBounds(grid, x-1, y) && IsEmpty(grid, x-1, y)) // Flow left
		{
			int nX = FlowParticle(grid, -1, x, y);

			SetParticle(grid, P_WATER, x-nX, y);
			SetParticle(grid, P_EMPTY, x, y);
		}
		else if (InBounds(grid, x+1, y) && IsEmpty(grid, x+1, y)) // Flow right
		{
			int nX = FlowParticle(grid, 1, x, y);

			SetParticle(grid, P_WATER, x+nX, y);
			SetParticle(grid, P_EMPTY, x, y);
		}
		else
		{
			p.falling = false;
		}
	}
}
