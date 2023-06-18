#ifndef HEADER_H
#define HEADER_H
#include <set>
#include <functional>

enum class dir8 : int8_t {
	HORIZONTAL,
	VERTICAL,
	LEFT_TOP_TO_RIGHT_BOTTOM,
	LEFT_BOTTOM_TO_RIGHT_TOP
};

const std::set<dir8> dir4 = {
	dir8::HORIZONTAL,
	dir8::VERTICAL
};

const std::set<dir8> full_dir8 = {
	dir8::HORIZONTAL,
	dir8::VERTICAL,
	dir8::LEFT_BOTTOM_TO_RIGHT_TOP,
	dir8::LEFT_TOP_TO_RIGHT_BOTTOM
};

template<typename T>
using chess_type = typename T::value_type::value_type;
template<typename T>
using chess_getter_type = std::function<chess_type<T>(T, size_t, size_t)>;

template<size_t win_count, typename T>
bool Judge(T board,
	size_t srcX,
	size_t srcY,
	// chess_type<T> nullChess = chess_type<T>(0),
	std::set<dir8> directions = full_dir8,
	chess_getter_type<T> chess_getter = [](T b, size_t x, size_t y) -> chess_type<T> { return b[x][y]; }) {
	if (directions.empty())
		return false;

	size_t count = 0;
	chess_type<T> chess = board[srcX][srcY];

	// if (chess == nullChess)
	//		return false;

	size_t x, y;

	std::function<void(size_t&, size_t&)> Next;
	std::function<void(size_t&, size_t&, const T& b)> Initialize;
	std::function<bool(size_t&, size_t&, const T& b)> LoopCondition;

	for (auto& direction : directions) {
		switch (direction) {
		case dir8::HORIZONTAL: {
			Next = [](size_t& x, size_t& y) {x++; };
			Initialize = [](size_t& x, size_t& y, const T& b) {
				x -= (win_count - 1);
				if (int(x) < 0)
					x = 0;
			};
			LoopCondition = [](size_t& x, size_t& y, const T& b) -> bool {
				return x < b.size();
			};
			break;
		}
		case dir8::VERTICAL: {
			Next = [](size_t& x, size_t& y) {y++; };
			Initialize = [](size_t& x, size_t& y, const T& b) {
				y -= (win_count - 1);
				if (int(y) < 0)
					y = 0;
			};
			LoopCondition = [](size_t& x, size_t& y, const T& b) ->bool {
				return y < b.at(x).size();
			};
			break;
		}
		case dir8::LEFT_TOP_TO_RIGHT_BOTTOM: {
			Next = [](size_t& x, size_t& y) {
				x++;
				y++;
			};
			Initialize = [](size_t& x, size_t& y, const T& _b) {
				x = x - win_count + 1;
				y = x;

				if (int(x) < 0) {
					y -= x;
					x = 0;
				}
				else if (int(y) < 0) {
					y = 0;
					x = y - x;
				}
			};
			LoopCondition = [](size_t& x, size_t& y, const T& b) -> bool {
				return x < b.size() && y < b.at(x).size();
			};
			break;
		}
		case dir8::LEFT_BOTTOM_TO_RIGHT_TOP:
			Next = [](size_t& x, size_t& y) {
				x++;
				y--;
			};
			Initialize = [](size_t& x, size_t& y, const T& _b) {
				const size_t b = y - x;
				x -= win_count - 1;
				y += win_count + 1;
				if (int(x) < 0) {
					x = 0;
					y = b;
				}
				else if (y > _b.at(x).size()) {
					y = _b.at(x).size();
				}
			};
			LoopCondition = [](size_t& x, size_t& y, const T& b) -> bool {
				return x >= 0 && y < b.at(x).size();
			};
			break;
		default:
			return false;
		}
		x = srcX, y = srcY;
		count = 0;
		for (Initialize(x, y, board);
			LoopCondition(x, y, board);
			Next(x, y)
			) {
			if (chess_getter(board, x, y) == chess)
				count++;
			else {
				count = 0;
				continue;
			}
			if (count == win_count)
				return true;
		}

	}
	return false;
}

#endif // !HEADER_H
