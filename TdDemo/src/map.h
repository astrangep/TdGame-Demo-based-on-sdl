#ifndef _MAP_H
#define _MAP_H

#include<SDL.h>
#include "tile.h"
#include<sstream>
#include<fstream>

class Map {
public:
	Map() = default;
	~Map() = default;

	bool load(const std::string& path) {
		std::ifstream file(path);
		if (!file.good()) return false;

		std::string str_line;
		int idx_x = -1; int idx_y = -1;
		TileMap tile_map_temp;
		while (std::getline(file, str_line)) {
			str_line = trim_str(str_line);
			if (str_line == "") continue;
			idx_y++;
			tile_map_temp.emplace_back();
			std::string str_tile;
			std::stringstream str_stream(str_line);
			while (std::getline(str_stream, str_tile, ',')) {
				idx_x++;
				tile_map_temp[idx_y].emplace_back();
				Tile& tile = tile_map_temp[idx_y].back();
				load_tile_from_string(tile, str_tile);
			}
		}
		file.close();
		
		if (tile_map_temp.empty() || tile_map_temp[0].empty()) return false;
		tile_map = tile_map_temp;
		return true;
	}

	size_t get_width() const { return tile_map.empty() ? 0 : tile_map[0].size(); }

	size_t get_height() const { return tile_map.size(); }
private:
	TileMap tile_map;
	SDL_Point idx_home = { 0.0 };
private:
	std::string trim_str(const std::string& str) {
		size_t begin = str.find_first_not_of(" \t");
		if (begin == std::string::npos) return "";
		size_t end = str.find_last_not_of(" \t");
		size_t range = end - begin + 1;
		return str.substr(begin, range);
	}
	void load_tile_from_string(Tile& tile, const std::string str_tile) {
		std::vector<int> values;
		std::string str_value;
		std::string str_tile_tidy = trim_str(str_tile);
		std::stringstream str_stream(str_tile_tidy);

		while (std::getline(str_stream, str_value, '\\')) {
			int value;
			try {
				value = std::stoi(str_value);
			}
			catch (const std::invalid_argument&) {
				value = -1;
			}
			values.push_back(value);
		}

		tile.terrian = (values.size() < 1 || values[0] < 0) ? 0 : values[0];
		tile.decoration = (values.size() < 2) ? -1 : values[1];
		tile.direction = (Tile::Direction)((values.size() < 3 || values[2] < 0) ? 0 : values[2]);
		tile.special_flag = (values.size() < 4) ? -1 : values[3];

	}

	void generate_map() {
		for (int y = 0; y < get_height(); y++) {
			for (int x = 0; x < get_width(); x++) {
				const Tile& tile = tile_map[y][x];
				if (tile.special_flag < 0) continue;
				if (tile.special_flag == 0) {
					idx_home = { x, y };
				}
			}
		}
	}
};
#endif // !_MAP_H
