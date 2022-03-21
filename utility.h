// Utility Functions for C
// ------------------------------

// Roll a die with specified number of sides, returns the result
int DiceRoll(int sides) {
	return (1 + rand() % sides) == sides;
}

// Returns random int from min to max, supports negative numbers
int RandRange(int min, int max) {
	return rand() % (max + 1 - min) + min;
}

// Returns either a or b, selected at random
int RandChoice(int a, int b) {
	return (rand() % 2)? a: b;
}

