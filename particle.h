// particle.h

#define    P_EMPTY        0 // Literal empty space
#define    P_SAND         1 // Basic powder, hardens into glass when heated
#define    P_WATER        2 // Basic liquid, freezes when chilled, evaporates when heated
#define    P_SALT         3 // Disolves in water, turns water to saltwater
#define    P_OIL          4 // Highly flammable liquid, burns somewhat slowly
#define    P_SLIME        5 // Sits on top of water, powders slowly move through it, has other strange behaviors
#define    P_FIRE         6 // Burns flammable things, heats other things
#define    P_PLANT        7 // Spreads when exposed to water, flammable
#define    P_SALTWATER    8 // Harder to freeze than freshwater, leaves behind salt when evaporated
#define    P_SMOKE        9 // Byproduct of fire contacting anything flammable, rises and dissipates
#define    P_STEAM       10 // Byproduct of fire contacting water or saltwater, rises and dissipates
#define    P_EMBER       11 // Byproduct of fire and flammable solids, burns for awhile then disappears


const char *material_names[] = {
    [0] = "Empty",
    [1] = "Sand",
    [2] = "Water",
    [3] = "Salt",
    [4] = "Oil",
    [5] = "Slime",
    [6] = "Fire",
    [7] = "Plant",
    [8] = "Saltwater",
    [9] = "Smoke",
    [10] = "Steam",
    [11] = "Ember",
};


typedef struct Particle {
    unsigned int id; // id of the particle type
    int weight; // how quickly it rises or falls
    int flow_rate; // how quickly it disperses horizontally
    bool has_moved; // if it has moved since the last update
    bool is_awake; // if it is awake and able to be updated
    int pref_density; // if it has a preferred brush density
    int flammability; // how flammable it is (0 is not flammable)
    Color color; // color of the particle
} Particle;


const Particle materials[] = {
    // Empty
    [0] = {
        .id = 0,
        .weight = 0,
        .flow_rate = 0,
        .flammability = 0,
        .color = G_BLACK,
    },

    // Sand
    [1] = {
        .id = 1,
        .weight = 1,
        .flow_rate = 1,
        .flammability = 0,
        .color = G_YELLOW,
    },

    // Water
    [2] = {
        .id = 2,
        .weight = 1,
        .flow_rate = 5,
        .flammability = 0,
        .color = G_BLUE,
    },

    // Salt
    [3] = {
        .id = 3,
        .weight = 1,
        .flow_rate = 1,
        .flammability = 0,
        .color = G_WHITE,
    },

    // Oil
    [4] = {
        .id = 4,
        .weight = 1,
        .flow_rate = 4,
        .flammability = 30,
        .color = G_RED,
    },

    // Slime
    [5] = {
        .id = 5,
        .weight = 1,
        .flow_rate = 3,
        .flammability = 0,
        .color = G_TEAL,
    },

    // Fire
    [6] = {
        .id = 6,
        .weight = 1,
        .flow_rate = 3,
        .pref_density = 1,
        .flammability = 0,
        .color = G_ORANGE,
    },

    // Plant
    [7] = {
        .id = 7,
        .weight = 1,
        .flow_rate = 1,
        .flammability = 20,
        .color = G_GREEN,
    },

    // Saltwater
    [8] = {
        .id = 8,
        .weight = 1,
        .flow_rate = 5,
        .flammability = 0,
        .color = G_DARKBLUE,
    },

    // Smoke
    [9] = {
        .id = 9,
        .weight = 1,
        .flow_rate = 3,
        .flammability = 0,
        .color = G_ADARKGRAY,
    },

    // Steam
    [10] = {
        .id = 10,
        .weight = 1,
        .flow_rate = 6,
        .flammability = 0,
        .color = G_AGRAY,
    },

    // Ember
    [11] = {
        .id = 11,
        .weight = 1,
        .flow_rate = 0,
        .flammability = 10,
        .color = G_ORANGE,
    },

    // Methane
    [12] = {
        .id = 12,
        .weight = 2,
        .flow_rate = 10,
        .flammability = 15,
        .color = G_ATEAL,
    },

    // Hydrogen
    [13] = {
        .id = 13,
        .weight = 2,
        .flow_rate = 10,
        .flammability = 15,
        .color = G_AWHITE,
    }
};
