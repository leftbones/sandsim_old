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


const char *material_names[] = {
	[0] = "Empty",
	[1] = "Sand",
	[2] = "Water",
	[3] = "Salt",
};


typedef struct Particle {
	unsigned int id;
	int weight;
	int flow_rate;
	bool has_moved;
	bool is_awake;
	Color color;
} Particle;


const Particle materials[] = {
	// Empty
	[0] = {
		.id = 0,
		.weight = 0,
		.flow_rate = 0,
		.color = G_BLACK,
	},

	// Sand
	[1] = {
		.id = 1,
		.weight = 1,
		.flow_rate = 1,
		.color = G_YELLOW,
	},

	// Water
	[2] = {
		.id = 2,
		.weight = 1,
		.flow_rate = 5,
		.color = G_BLUE,
	},

	// Salt
	[3] = {
		.id = 3,
		.weight = 1,
		.flow_rate = 1,
		.color = G_WHITE,
	},

	// Saltwater
	[8] = {
		.id = 8,
		.weight = 1,
		.flow_rate = 1,
		.color = G_TEAL,
	}
};
