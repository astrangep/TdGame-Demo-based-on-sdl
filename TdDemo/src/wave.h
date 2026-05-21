#ifndef _WAVE_H
#define _WAVE_H
#include<vector>
#include"enemy_type.h"
struct Wave {
	struct SpawnEvent {
		int interval = 0;
		int generate_point = 1;
		EnemyType enemy_type = EnemyType::ALienSoldier;
	};
	int reward = 0;
	int interval = 0;
	std::vector<SpawnEvent> spawn_event_list;
};
#endif
