#ifndef _TILE_H_
#define _TILE_H_
#include<vector>

struct Tile
{
	enum Direction {
		NONE =0,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	int terrian = 0;
	int special_flag = -1;
	int decoration = -1;
	bool has_tower = false;
	Direction direction = Direction::NONE;
};

typedef std::vector<std::vector<Tile>> TileMap;

#endif // !_TILE_H_
