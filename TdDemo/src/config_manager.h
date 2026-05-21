#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include"manager.h"
#include<cJSON.h>
#include"map.h"
#include"wave.h"

class ConfigManager : public Manager<ConfigManager>
{
	friend class Manager<ConfigManager>;
public:
	struct BasicTemplate {
		std::string title = u8"能源保卫战";
		int window_width = 1280;
		int window_height = 720;
	};
	struct PlayerTemplate
	{
		double speed = 5.0;
		double attack_interval = 0.8;
		double attack_damage[10] = { 5 };
		double skill1_interval[10] = { 5 };
		double skill1_damage[10] = { 20 };
		double skill2_interval[10] = { 0 };
		double skill2_damage[10] = { 0 };
		double upgrade_cost[9] = { 30 };
	};
	struct TowerTemplate
	{
		double interval[10] = { 1.0 };
		double damage[10] = { 10 };
		double view_range[10] = { 5 };
		double cost[10] = {50};
		double upgrade_cost[9] = { 50 };
	};
	struct EnemyTemplate{
		double speed = 0.8;
		double hp = 80;
		double damage = 1;
		double reward_ratio = 0.5;
		double reward_number = 10;
		double recover_range = -1;
		double recover_intensity = 25;
	};
public:
	Map map;
	std::vector<Wave> wave_list;
	int level_Laser_gun = 0;
	int level_Stun_gun = 0;
	int level_Missle = 0;
	SDL_Rect rect_tile_map = { 0 };
	
	BasicTemplate basic_template;
	PlayerTemplate player_template;
	TowerTemplate Laser_gun_template, Stun_gun_template, Missile_template;
	EnemyTemplate AlienSoldier_template, AlienSoldierLeader_template, SpaceMonster_template, AlienWizard_template, SpaceBeast_template;

	bool is_game_win = false;
	bool is_game_over = false;

	double home_initial_hp = 10;
	double initial_money = 100;
	double money_per_coin = 10;
public:
	bool load_level_config(const std::string& path) {
		std::ifstream file(path);

		if (!file.good()) return false;

		std::stringstream str_stream;
		str_stream << file.rdbuf();	
		file.close();

		cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
		if (!json_root) return false;
		if (json_root->type != cJSON_Array) {
			cJSON_Delete(json_root);
			return false;
		}

		cJSON* json_wave = nullptr;
		cJSON_ArrayForEach(json_wave, json_root) {
			if (json_wave->type != cJSON_Object) continue;

			wave_list.emplace_back();
			Wave& wave = wave_list.back();
			cJSON* json_wave_reward = cJSON_GetObjectItem(json_wave, "reward");	
			if (json_wave_reward && json_wave_reward->type == cJSON_Number) {
				wave.reward = json_wave_reward->valuedouble;
			}
			cJSON* json_wave_interval = cJSON_GetObjectItem(json_wave, "interval");
			if (json_wave_interval && json_wave_interval->type == cJSON_Number) {
				wave.interval = json_wave_interval->valuedouble;
			}
			cJSON* json_wave_spawn_list = cJSON_GetObjectItem(json_wave, "spawn_list");
			if (json_wave_spawn_list && json_wave_spawn_list->type == cJSON_Array) {
				cJSON* json_spawn_event = nullptr;
				cJSON_ArrayForEach(json_spawn_event, json_wave_spawn_list) {
					if (json_spawn_event->type != cJSON_Object) continue;
					wave.spawn_event_list.emplace_back();
					Wave::SpawnEvent& spawn_event = wave.spawn_event_list.back();
					cJSON* json_spawn_event_interval = cJSON_GetObjectItem(json_spawn_event, "interval");
					if (json_spawn_event_interval && json_spawn_event_interval->type == cJSON_Number) {
						spawn_event.interval = json_spawn_event_interval->valuedouble;
					}
					cJSON* json_spawn_event_point = cJSON_GetObjectItem(json_spawn_event, "point");
					if(json_spawn_event_point && json_spawn_event_point ->type == cJSON_Number){
						spawn_event.point = json_spawn_event_point->valueint;
					}
					cJSON* json_spawn_event_enemy_type = cJSON_GetObjectItem(json_spawn_event, "enemy");
					if(json_spawn_event_enemy_type && json_spawn_event_enemy_type->type == cJSON_String) {
						const std::string enemy_type_str = json_spawn_event_enemy_type->valuestring;
						if(enemy_type_str == "AlienSoldier") spawn_event.enemy_type = EnemyType::AlienSoldier;
						else if(enemy_type_str == "AlienSoldierLeader") spawn_event.enemy_type = EnemyType::AlienSoldierLeader;
						else if(enemy_type_str == "SpaceMonster") spawn_event.enemy_type = EnemyType::SpaceMonster;
						else if(enemy_type_str == "AlienWizard") spawn_event.enemy_type = EnemyType::AlienWizard;
						else if(enemy_type_str == "SpaceBeast") spawn_event.enemy_type = EnemyType::SpaceBeast;
					}	
				}	
			}
			if (wave.spawn_event_list.empty()) wave_list.pop_back();

		}
		cJSON_Delete(json_root);
		if (wave_list.empty()) return false;
		return true;
	};
	bool load_game_config(const std::string& path) {
		std::ifstream file(path);
		std::stringstream str_stream;
		str_stream << file.rdbuf();
		file.close();

		cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
		if (!json_root || json_root->type!=cJSON_Object) return false;

		cJSON* json_basic = cJSON_GetObjectItem(json_root, "basic");
		cJSON* json_player = cJSON_GetObjectItem(json_root, "player");
		cJSON* json_tower = cJSON_GetObjectItem(json_root, "tower");
		cJSON* json_enemy = cJSON_GetObjectItem(json_root, "enemy");

		if (!json_basic || json_basic->type != cJSON_Object || !json_player || json_player->type != cJSON_Object ||
			!json_tower || json_tower->type != cJSON_Object || !json_enemy || json_enemy->type != cJSON_Object) {
			cJSON_Delete(json_root);
			return false;
		}

		parse_basic_template(basic_template, json_basic);
		parse_player_template(player_template, json_player);
		parse_tower_template(Laser_gun_template, cJSON_GetObjectItem(json_tower, "Laser_gun"));
		parse_tower_template(Stun_gun_template, cJSON_GetObjectItem(json_tower, "Stun_gun"));
		parse_tower_template(Missile_template, cJSON_GetObjectItem(json_tower, "Missile"));
		parse_enemy_template(AlienSoldier_template, cJSON_GetObjectItem(json_enemy, "AlienSoldier"));
		parse_enemy_template(AlienSoldierLeader_template, cJSON_GetObjectItem(json_enemy, "AlienSoldierLeader"));
		parse_enemy_template(SpaceMonster_template, cJSON_GetObjectItem(json_enemy, "SpaceMonster"));
		parse_enemy_template(AlienWizard_template, cJSON_GetObjectItem(json_enemy, "AlienWizard"));
		parse_enemy_template(SpaceBeast_template, cJSON_GetObjectItem(json_enemy, "SpaceBeast"));

		cJSON_Delete(json_root);
		return true;

	};	
private:
	void parse_array_element(double* array, int length, cJSON* json_root) {
		if (!json_root && json_root->type != cJSON_Array) return;
		int idx = -1;
		cJSON* json_element = nullptr;
		cJSON_ArrayForEach(json_element, json_root) {
			idx++;
			if (json_element->type != cJSON_Number || idx >= length) continue;
			array[idx] = json_element->valuedouble;
		}
	}
	void parse_basic_template(BasicTemplate& tpl, cJSON* json_root) {
		if (!json_root && json_root->type != cJSON_Object) return;	
		cJSON* json_title = cJSON_GetObjectItem(json_root, "title");
		cJSON* json_window_width = cJSON_GetObjectItem(json_root, "window_width");
		cJSON* json_window_height = cJSON_GetObjectItem(json_root, "window_height");	
		if (json_title && json_title->type == cJSON_String) {
			tpl.title = json_title->valuestring;
		}
		if (json_window_width && json_window_width->type == cJSON_Number) {
			tpl.window_width = json_window_width->valueint;
		}
		if (json_window_height && json_window_height->type == cJSON_Number) {
			tpl.window_height = json_window_height->valueint;
		}
	}
	void parse_player_template(PlayerTemplate& tpl, cJSON* json_root) {
		if (!json_root && json_root->type != cJSON_Object) return;
		cJSON* json_speed = cJSON_GetObjectItem(json_root, "speed");
		cJSON* json_attack_interval = cJSON_GetObjectItem(json_root, "attack_interval");
		cJSON* json_attack_damage = cJSON_GetObjectItem(json_root, "attack_damage");
		cJSON* json_skill1_interval = cJSON_GetObjectItem(json_root, "skill1_interval");
		cJSON* json_skill1_damage = cJSON_GetObjectItem(json_root, "skill1_damage");
		cJSON* json_skill2_interval = cJSON_GetObjectItem(json_root, "skill2_interval");
		cJSON* json_skill2_damage = cJSON_GetObjectItem(json_root, "skill2_damage");
		cJSON* json_upgrade_cost = cJSON_GetObjectItem(json_root, "upgrade_cost");

		if(json_speed && json_speed->type == cJSON_Number) {
			tpl.speed = json_speed->valuedouble;
		}
		if(json_attack_interval && json_attack_interval->type == cJSON_Number){
			tpl.attack_interval = json_attack_interval->valuedouble;
		}
		parse_array_element(tpl.attack_damage, 10, json_attack_damage);
		parse_array_element(tpl.skill1_interval, 10, json_skill1_interval);
		parse_array_element(tpl.skill1_damage, 10, json_skill1_damage);
		parse_array_element(tpl.skill2_interval, 10, json_skill2_interval);
		parse_array_element(tpl.skill2_damage, 10, json_skill2_damage);
		parse_array_element(tpl.upgrade_cost, 9, json_upgrade_cost);
	}
	void parse_tower_template(TowerTemplate& tpl, cJSON* json_root) {
		if (!json_root && json_root->type != cJSON_Object) return;
		cJSON* json_interval = cJSON_GetObjectItem(json_root, "interval");
		cJSON* json_damage = cJSON_GetObjectItem(json_root, "damage");
		cJSON* json_view_range = cJSON_GetObjectItem(json_root, "view_range");	
		cJSON* json_cost = cJSON_GetObjectItem(json_root, "cost");	
		cJSON* json_upgrade_cost = cJSON_GetObjectItem(json_root, "upgrade_cost");

		parse_array_element(tpl.interval, 10, json_interval);
		parse_array_element(tpl.damage, 10, json_damage);
		parse_array_element(tpl.view_range, 10, json_view_range);
		parse_array_element(tpl.cost, 10, json_cost);
		parse_array_element(tpl.upgrade_cost, 9, json_upgrade_cost);
	}
	void parse_enemy_template(EnemyTemplate& tpl, cJSON* json_root) {
		if(!json_root && json_root->type != cJSON_Object) return;
		cJSON* json_hp = cJSON_GetObjectItem(json_root, "hp");
		cJSON* json_speed = cJSON_GetObjectItem(json_root, "speed");
		cJSON* json_damage = cJSON_GetObjectItem(json_root, "damage");
		cJSON* json_reward_ratio = cJSON_GetObjectItem(json_root, "reward_ratio");
		cJSON* json_reward_number = cJSON_GetObjectItem(json_root, "reward_number");
		cJSON* json_recover_range = cJSON_GetObjectItem(json_root, "recover_range");
		cJSON* json_recover_intensity = cJSON_GetObjectItem(json_root, "recover_intensity");

		if (json_hp && json_hp->type != cJSON_Number) {
			tpl.hp = json_hp->valuedouble;
		}
		if (json_speed && json_speed->type != cJSON_Number) {
			tpl.speed = json_speed->valuedouble;
		}
		if (json_damage && json_damage->type != cJSON_Number) {
			tpl.damage = json_damage->valuedouble;
		}
		if (json_reward_ratio && json_reward_ratio->type != cJSON_Number) {
			tpl.reward_ratio = json_reward_ratio->valuedouble;
		}
		if (json_reward_number && json_reward_number->type != cJSON_Number) {
			tpl.reward_number = json_reward_number->valuedouble;
		}
		if (json_recover_range && json_recover_range->type != cJSON_Number) {
			tpl.recover_range = json_recover_range->valuedouble;
		}
		if (json_recover_intensity && json_recover_intensity->type != cJSON_Number) {
			tpl.recover_intensity = json_recover_intensity->valuedouble;
		}
	}

	
	
protected:
	ConfigManager() = default;
	~ConfigManager() = default;	
};
#endif