#pragma warning(push)
#pragma warning(disable: 26451)

#include "judge.h"

#include <array>
#include <iostream>
#include <cassert>

using std::cout; using std::endl; using std::boolalpha;

using map_t = std::array<std::array<char, 10>, 10>;

map_t g_Map;

#define gobang_judge(...) Judge<5, map_t>(__VA_ARGS__)

int main() {
	// place chesses
	for (auto i = 0u; i < 5u; i++) {
		g_Map[i + 4][i + 4] = 1;
		g_Map[i][0] = 2;
		g_Map[1][i] = 3;
		g_Map[8 - i][3 + i] = 4;
	}
	// pass
	assert(gobang_judge(g_Map, 0, 0));
	assert(gobang_judge(g_Map, 4, 4));
	assert(gobang_judge(g_Map, 1, 0));
	assert(gobang_judge(g_Map, 3, 3));
	return 0;
}