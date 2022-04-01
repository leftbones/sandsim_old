// update.c

// Powders
// ------------------------------

void UpdateSand(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {
    // Down
    if (IsEmpty(grid, x, y+p.weight) && InBounds(grid, x, y+p.weight)) {
        SwapParticles(grid, x, y, x, y+p.weight);
    }

    // Displace water + saltwater
    else if (DiceRoll(2) && (grid[x][y+p.weight].id == P_WATER /*|| grid[x][y+p.weight].id == P_SALTWATER*/)) {
        SwapParticles(grid, x, y, x, y+p.weight);
    }

    if (p.has_moved) {
        // Down + Left
        if (InBounds(grid, x-p.flow_rate, y+p.weight) && IsEmpty(grid, x-p.flow_rate, y+p.weight)) {
            SwapParticles(grid, x, y, x-p.flow_rate, y+p.weight);
        }

        // Down + Right
        else if (InBounds(grid, x+p.flow_rate, y+p.weight) && IsEmpty(grid, x+p.flow_rate, y+p.weight))  {
            SwapParticles(grid, x, y, x+p.flow_rate, y+p.weight);
        }

        // Down + Left (Submerged)
        else if (InBounds(grid, x-p.flow_rate, y+p.weight) && grid[x-p.flow_rate][y+p.weight].id == P_WATER && grid[x][y+p.weight].id == P_SAND) {
            SwapParticles(grid, x, y, x-p.flow_rate, y+p.weight);
        }

        // Down + Right (Submerged)
        else if (InBounds(grid, x+p.flow_rate, y+p.weight) && grid[x+p.flow_rate][y+p.weight].id == P_WATER && grid[x][y+p.weight].id == P_SAND) {
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

    // Disolve in water (only checks cardinal directions)
    int dissolve_chance = 1000;

    if (DiceRoll(dissolve_chance) && (grid[x][y-1].id == P_WATER || grid[x][y-1].id == P_SALTWATER)) { // Upper
        NewParticle(grid, P_SALTWATER, x, y);
        NewParticle(grid, P_EMPTY, x, y-1);
    }

    else if (DiceRoll(dissolve_chance) && (grid[x-1][y].id == P_WATER || grid[x-1][y].id == P_SALTWATER)) { // Left
        NewParticle(grid, P_SALTWATER, x, y);
        NewParticle(grid, P_EMPTY, x-1, y);
    }

    else if (DiceRoll(dissolve_chance) && (grid[x+1][y].id == P_WATER || grid[x+1][y].id == P_SALTWATER)) { // Right
        NewParticle(grid, P_SALTWATER, x, y);
        NewParticle(grid, P_EMPTY, x+1, y);
    }

    else if (DiceRoll(dissolve_chance) && (grid[x][y+1].id == P_WATER || grid[x][y+1].id == P_SALTWATER)) { // Lower
        NewParticle(grid, P_SALTWATER, x, y);
        NewParticle(grid, P_EMPTY, x, y+1);
    }

    // Displace water + saltwater
    else if (DiceRoll(4) && (grid[x][y+p.weight].id == P_WATER || grid[x][y+p.weight].id == P_SALTWATER)) {
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

    // Down + Left (Submerged)
    else if (InBounds(grid, x-p.flow_rate, y+p.weight) && grid[x-p.flow_rate][y+p.weight].id == P_WATER && grid[x][y+p.weight].id == P_SALT) {
        SwapParticles(grid, x, y, x-p.flow_rate, y+p.weight);
    }

    // Down + Right (Submerged)
    else if (InBounds(grid, x+p.flow_rate, y+p.weight) && grid[x+p.flow_rate][y+p.weight].id == P_WATER && grid[x][y+p.weight].id == P_SALT) {
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
//      y = y+p.weight;
//      p = grid[x][y+p.weight];
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


void UpdateSaltwater(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {
    // Down
    if (IsEmpty(grid, x, y+p.weight) && InBounds(grid, x, y+p.weight)) {
        SwapParticles(grid, x, y, x, y+p.weight);
//      y = y+p.weight;
//      p = grid[x][y+p.weight];
    }

    else if (grid[x][y-1].id == P_WATER) {
        SwapParticles(grid, x, y, x, y-p.weight);
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

    // Disperse Left
    else if (grid[x-1][y].id == P_WATER) {
        for (int i = p.flow_rate; i > 0; i--) {
            if (InBounds(grid, x-i, y) && grid[x-i][y].id == P_WATER) {
                SwapParticles(grid, x, y, x-i, y);
                break;
            }
        }
    }

    // Disperse Right
    else if (grid[x+1][y].id == P_WATER) {
        for (int i = p.flow_rate; i > 0; i--) {
            if (InBounds(grid, x+i, y) && grid[x+i][y].id == P_WATER) {
                SwapParticles(grid, x, y, x+i, y);
                break;
            }
        }
    }

    else {
        grid[x][y].has_moved = false;
    }
}


void UpdateOil(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {
    // Down
    if (IsEmpty(grid, x, y+p.weight) && InBounds(grid, x, y+p.weight)) {
        SwapParticles(grid, x, y, x, y+p.weight);
//      y = y+p.weight;
//      p = grid[x][y+p.weight];
    }

    else if (grid[x][y-1].id == P_WATER) {
        SwapParticles(grid, x, y, x, y-p.weight);
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

    // Disperse Left
    else if (grid[x-1][y].id == P_WATER) {
        for (int i = p.flow_rate; i > 0; i--) {
            if (InBounds(grid, x-i, y) && grid[x-i][y].id == P_WATER) {
                SwapParticles(grid, x, y, x-i, y);
                break;
            }
        }
    }

    // Disperse Right
    else if (grid[x+1][y].id == P_WATER) {
        for (int i = p.flow_rate; i > 0; i--) {
            if (InBounds(grid, x+i, y) && grid[x+i][y].id == P_WATER) {
                SwapParticles(grid, x, y, x+i, y);
                break;
            }
        }
    }

    else {
        grid[x][y].has_moved = false;
    }
}


void UpdateSlime(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {
    int types = {P_WATER, P_SALTWATER};

    // Down
    if (IsEmpty(grid, x, y+p.weight) && InBounds(grid, x, y+p.weight)) {
        SwapParticles(grid, x, y, x, y+p.weight);
//      y = y+p.weight;
//      p = grid[x][y+p.weight];
    }

    else if (IsAny(p, &types)) {
        SwapParticles(grid, x, y, x, y-p.weight);
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

    // Disperse Left
    else if (IsAny(grid[x-1][y], &types)) {
        for (int i = p.flow_rate; i > 0; i--) {
            if (InBounds(grid, x-i, y) && IsAny(p, &types)) {
                SwapParticles(grid, x, y, x-i, y);
                break;
            }
        }
    }

    // Disperse Right
    else if (IsAny(grid[x+1][y], &types)) {
        for (int i = p.flow_rate; i > 0; i--) {
            if (InBounds(grid, x+i, y) && IsAny(p, &types)) {
                SwapParticles(grid, x, y, x+i, y);
                break;
            }
        }
    }

    else {
        grid[x][y].has_moved = false;
    }
}


// Gases
// ------------------------------

void UpdateFire(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {
    // Try to burn flammable things
    BurnNeighbors(grid, p.id, x, y);

    // Random chance to turn into steam when touching water or saltwater
    if (NumberTouching(grid, P_WATER, x, y) > 0 || NumberTouching(grid, P_SALTWATER, x, y) > 0) {
        int steam_chance = 50;
        if (DiceRoll(steam_chance)) {
            NewParticle(grid, P_STEAM, x, y);
            return;
        }
    }

    // Random chance to die each update
    int die_chance = 50;

    if (DiceRoll(die_chance)) {
        NewParticle(grid, P_EMPTY, x, y);
        return;
    }

    // Random chance to create smoke each update
    int smoke_chance = 500;

    if (DiceRoll(smoke_chance) && IsEmpty(grid, x-RandRange(-p.weight, p.weight), y-p.weight)) {
        NewParticle(grid, P_SMOKE, x, y-p.weight);
    }

    // Random chance to rise each update
    // If it succeeds a second roll, it will leave a fire particle behind, as if spreading through the air
    int rise_chance = 6;

    if (DiceRoll(rise_chance)) {
        // Up
        if (IsEmpty(grid, x, y-p.weight) && InBounds(grid, x, y-p.weight)) {
            if (DiceRoll(rise_chance*2)) { NewParticle(grid, P_FIRE, x, y-p.weight); }
            else { SwapParticles(grid, x, y, x, y-p.weight); }
        }

        // Up + Left
        else if (DiceRoll(rise_chance * 4) && IsEmpty(grid, x-p.flow_rate, y-p.weight) && InBounds(grid, x-p.flow_rate, y-p.weight)) {
            if (DiceRoll(rise_chance*2)) { NewParticle(grid, P_FIRE, x-p.flow_rate, y-p.weight); }
            else { SwapParticles(grid, x, y, x-p.flow_rate, y-p.weight); }
        }

        // Up + Right
        else if (DiceRoll(rise_chance * 4) && IsEmpty(grid, x+p.flow_rate, y-p.weight) && InBounds(grid, x+p.flow_rate, y-p.weight)) {
            if (DiceRoll(rise_chance*2)) { NewParticle(grid, P_FIRE, x+p.flow_rate, y-p.weight); }
            else { SwapParticles(grid, x, y, x+p.flow_rate, y-p.weight); }
        }
    }

    else {
        grid[x][y].has_moved = false;
    }
}

void UpdateSmoke(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {
    // Rise through certain materials
    int types = { P_WATER, P_SALTWATER };
    if (IsAny(grid[x][y-p.weight], &types)) {
        SwapParticles(grid, x, y, x, y-p.weight);
    }

    // Random chance to die each update
    int die_chance = 700;

    if (DiceRoll(die_chance)) {
        NewParticle(grid, P_EMPTY, x, y);
        return;
    }

    // Random chance to rise each update
    int rise_chance = 6;

    // Up
    if (DiceRoll(rise_chance)) {
        if (IsEmpty(grid, x, y-p.weight) && InBounds(grid, x, y-p.weight)) {
            SwapParticles(grid, x, y, x, y-p.weight);
        }

        // Up + Left
        else if (DiceRoll(rise_chance / 2) && IsEmpty(grid, x-p.flow_rate, y-p.weight) && InBounds(grid, x-p.flow_rate, y-p.weight)) {
            SwapParticles(grid, x, y, x-p.flow_rate, y-p.weight);
        }

        // Up + Right
        else if (DiceRoll(rise_chance / 2) && IsEmpty(grid, x+p.flow_rate, y-p.weight) && InBounds(grid, x+p.flow_rate, y-p.weight)) {
            SwapParticles(grid, x, y, x+p.flow_rate, y-p.weight);
        }
    }

    else {
        grid[x][y].has_moved = false;
    }
}

void UpdateSteam(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {
    // Rise through certain materials
    int types = { P_WATER, P_SALTWATER };
    if (IsAny(grid[x][y-p.weight], &types)) {
        SwapParticles(grid, x, y, x, y-p.weight);
    }

    // Random chance to die each update
    int die_chance = 700;

    if (DiceRoll(die_chance)) {
        NewParticle(grid, P_EMPTY, x, y);
        return;
    }

    // Random chance to rise each update
    int rise_chance = 4;

    // Up
    if (DiceRoll(rise_chance)) {
        if (IsEmpty(grid, x, y-p.weight) && InBounds(grid, x, y-p.weight)) {
            SwapParticles(grid, x, y, x, y-p.weight);
        }

        // Up + Left
        else if (DiceRoll(rise_chance / 2) && IsEmpty(grid, x-p.flow_rate, y-p.weight) && InBounds(grid, x-p.flow_rate, y-p.weight)) {
            SwapParticles(grid, x, y, x-p.flow_rate, y-p.weight);
        }

        // Up + Right
        else if (DiceRoll(rise_chance / 2) && IsEmpty(grid, x+p.flow_rate, y-p.weight) && InBounds(grid, x+p.flow_rate, y-p.weight)) {
            SwapParticles(grid, x, y, x+p.flow_rate, y-p.weight);
        }
    }

    else {
        grid[x][y].has_moved = false;
    }
}


// Solids
// ------------------------------

void UpdatePlant(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {
    int touching = NumberTouching(grid, P_WATER, x, y);
    if ((touching >= 4 && touching <= 6) || DiceRoll(200)) { InfectNeighbors(grid, p.id, P_WATER, 250, true, true, x, y); }
}

void UpdateEmber(Particle grid[GRID_WIDTH][GRID_HEIGHT], Particle p, int x, int y) {
    // Die when touching water
    if (NumberTouching(grid, P_WATER, x, y) > 0 || NumberTouching(grid, P_SALTWATER, x, y) > 0) { NewParticle(grid, P_EMPTY, x, y); }

    // Try to burn flammable things
    BurnNeighbors(grid, p.id, x, y);

    // Random chance to create smoke
    int smoke_chance = 800;
    if (DiceRoll(smoke_chance) && InBounds(grid, x, y-1) && IsEmpty(grid, x, y-1)) {
        NewParticle(grid, P_SMOKE, x, y-1);
    }

    if (InBounds(grid, x, y+p.weight) && IsEmpty(grid, x, y+p.weight)) {
        // Chance to burn out when falling
        int die_chance = 100;
        if (DiceRoll(die_chance)) {
            NewParticle(grid, P_EMPTY, x, y);
        } else {
            SwapParticles(grid, x, y, x, y+p.weight);   
        }
    }

    // Random chance to burn out, turns into fire
    int die_chance = 400;
    if (DiceRoll(die_chance)) {
        NewParticle(grid, P_FIRE, x, y);
        if (InBounds(grid, x, y-1) && IsEmpty(grid, x, y-1)) {
            NewParticle(grid, P_SMOKE, x, y);
        }
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
                    case 4: // Oil
                        UpdateOil(grid, p, x, y);
                        break;
                    case 5: // Slime
                        UpdateSlime(grid, p, x, y);
                        break;
                    case 6: // Fire
                        UpdateFire(grid, p, x, y);
                        break;
                    case 7: // Plant
                        UpdatePlant(grid, p, x, y);
                        break;
                    case 8: // Saltwater
                        UpdateSaltwater(grid, p, x, y);
                        break;
                    case 9: // Smoke
                        UpdateSmoke(grid, p, x, y);
                        break;
                    case 10: // Steam
                        UpdateSteam(grid, p, x, y);
                        break;
                    case 11: // Ember
                        UpdateEmber(grid, p, x, y);
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
