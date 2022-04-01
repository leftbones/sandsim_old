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

// Draw text with font (easier than DrawTextEx)
void DrawTextF(Font font, const char *text, int x, int y, float size, Color color) {
	Vector2 pos = {x, y};
	DrawTextEx(font, text, pos, size, 1, color);
}

// Prepend character to the beginning of a string
void Prepend(char* str, const char* ch) {
	size_t len = strlen(str);
	memmove(str + len, str, strlen(str) + 1);
	memcpy(str, ch, len);
}

char FormatNumber(char* number) {
	char *fNumber;
	int size = strlen(number);
	int count = 0;
	
	for (int i = size; i > 0; i--) {
		Prepend(fNumber, &number[i]);
		count += 1;

		if (count == 3) {
			Prepend(fNumber, ",");
			Prepend(fNumber, &number[i]);
			count = 0;
		}
	}

	return *fNumber;
}
