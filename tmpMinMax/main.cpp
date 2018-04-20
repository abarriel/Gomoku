#include <iostream>

int heurastic() {
	static int i = 0;

	i++;
	std::cout << i;
	return i;
}

int MinMax(int deapth, unsigned int &place) {
	int curentRating;
	int v = -1000;
	unsigned int OPplace;

	if (deapth == 0)
		return heurastic();
	for (size_t i = 0; i < 3; i++) {
		std::cout << '-';
		curentRating = -MinMax(deapth - 1, OPplace);
		if (curentRating > v)
		{
			v = curentRating;
			place = i;
		}
	}
	return v;
}

int	main() {
	unsigned int place;
	int res = MinMax(4, place);
	std::cout << "\nMinMax : " << res << " place : " << place << '\n';
	return 0;
}
