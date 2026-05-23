#ifndef _WAVE_H
#define _WAVE_H
#include<vector>
#include"enemy_type.h"
struct Wave {
	struct SpawnEvent {
		double interval = 0;
		int point = 1;
		EnemyType enemy_type = EnemyType::ALienSoldier;
	};
	double reward = 0;
	double interval = 0;
	std::vector<SpawnEvent> spawn_event_list;
};
#endif
