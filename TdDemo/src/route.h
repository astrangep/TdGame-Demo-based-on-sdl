#ifndef _ROUTE_H

#define _ROUTE_H
#include <vector>
#include"map.h"

class Route
{
	typedef std::vector<SDL_Point> IdxList;
public:
	Route() = default;
	Route(TileMap& tile_map, const SDL_Point& idx_origin) {
		size_t weight = tile_map[0].size();
		size_t height = tile_map.size();
		SDL_Point idx_next = idx_origin;
		while (true) {
			if (idx_next.x < 0 || idx_next.x >= weight || idx_next.y < 0 || idx_next.y >= height) break;
			if (check_idx_duplicate(idx_next)) break;
			idx_list.push_back(idx_next);
			Tile tile = tile_map[idx_next.y][idx_next.x];
			bool idx_next_valid = true;
			if (tile.special_flag <= 0) break;
			else
				switch (tile.direction) {
				case Tile::Direction::UP: 
					idx_next.y -= 1;
					break;
				case Tile::Direction::DOWN: 
					idx_next.y += 1;
					break;
				case Tile::Direction::LEFT: 
					idx_next.x -= 1;
					break;
				case Tile::Direction::RIGHT:
					idx_next.x += 1;
					break;
				default:
					idx_next_valid = false;
					break;
				}
			if (idx_next_valid == false) break;
		}
	}
	const IdxList& get_idx_list() const { return idx_list; }	
private:
	IdxList idx_list;
	bool check_idx_duplicate(SDL_Point target_idx) {
		for (auto& i : idx_list) {
			if (i.x == target_idx.x && i.y == target_idx.y) return true;
		}
		return false;
	}
};



#endif // !_ROUTE_H
