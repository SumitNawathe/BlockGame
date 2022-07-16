#pragma once

/* floor function from float to int
* ex: +2.4 -> 2, -2.4 ->3, -4.0 -> -4
*/
int ffloor(float input) {
	if (input >= 0.0f) return static_cast<int>(input);
	int truncated = static_cast<int>(input);
	if ((float)truncated == input) return truncated;
	return truncated - 1;
}


/* ceiling function from float to int
* ex: +2.4 -> 3, -2.4 -> -2, -4.0 -> -4
*/
int fceil(float input) {
	if (input <= 0.0f) return static_cast<int>(input);
	int truncated = static_cast<int>(input);
	if ((float)truncated == input) return truncated;
	return truncated + 1;
}

