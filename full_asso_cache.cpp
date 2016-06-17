#include "full_asso_cache.h"

full_asso_cache::full_asso_cache(Size size, const oper & operation) : cache(size, operation){
	hot_cold.resize(sets.size() - 1, 0);
}

Result full_asso_cache::run_lru() {
	v_long tag;

	for (auto iter = opers.begin(); iter != opers.end(); ++iter) {
		
		tag = ret_block(iter->addr, cache_line);

		bool hit = false;
		for (auto it = sets.begin(); it != sets.end(); ++it) {
			cache_slot & slot = *it;

			//If there is a hit happened, just increment time stamp of the rest
			if (hit) {
				slot.time++;
			}

			//If there is no hit yet,
			else {
				if (slot.valid != true) {
					slot.valid = true;
					slot.tag = tag;
					slot.time = 0;
					hit = true;
					continue;
				}

				if (slot.tag == tag) {
					slot.time = 0;
					++count;
					hit = true;
					continue;
				}
				else {
					slot.time++;
				}
			}
		}
		if (!hit) {
			auto lru = sets.begin();
			for (auto it = sets.begin(); it != sets.end(); ++it) {
				if (lru->time < it->time) {
					lru = it;
				}
			}
			++miss;
			lru->tag = tag;
			lru->time = 0;
		}	
	}
	return Result(this->count, this->access_no);
}

Result full_asso_cache::cold_hot_approximation() {
	
	v_long tag;

	for (auto iter = opers.begin(); iter != opers.end(); ++iter) {
		
		tag = ret_block(iter->addr, cache_line);
	
		bool hit = false;
		ui index = -1;
		for (auto it = sets.begin(); it != sets.end(); ++it) {
			
			cache_slot & slot = *it;

			index = std::distance(sets.begin(), it);

			if (slot.tag == tag) {
				++count;
				hit = true;
				update_after_access(index);
				break;
			}
		}
		if (!hit) {
			ui index = replace();
			update_after_access(index);
			sets[index].valid = true;
			sets[index].tag = tag;
		}
	}

	return Result(this->count, this->access_no);
}

//Converts from cache index to hot cold bits
void full_asso_cache::update_after_access(ui index) {
	
	ui leaf_index = -1;
	bool is_left = false;

	if (index % 2 == 0) {
		is_left = true;
		leaf_index = (index + hot_cold.size()) / 2;
	}
	else {
		leaf_index = ((index + hot_cold.size()) / 2) - 1;
	}

	heap_reverse_traverse(leaf_index, is_left);
}

void full_asso_cache::heap_reverse_traverse(ui index, bool is_left) {
	ui upper_leaf = -1;
	bool new_is_left = false;

	if (index % 2 == 0) {
		upper_leaf = (index - 2) / 2;
	}
	else { 
		new_is_left = true;
		upper_leaf = (index - 1) / 2;
	}

	hot_cold[index] = 1;
	if (is_left)
		hot_cold[index] = 0;

	if (index == 0) {
		return;
	}

	heap_reverse_traverse(upper_leaf, new_is_left);
}

ui full_asso_cache::replace() {
	const ui START = 0;
	return convert(heap_traverse(START));
}

ui full_asso_cache::convert(ui index) {
	//2 x (index - (size / 2)) || +1 
	if (hot_cold[index] == 0) {
		//reverse(index);
		return 2 * (index - (hot_cold.size()) / 2) + 1;
	}
	else {
		//reverse(index);
		return 2 * (index - (hot_cold.size()) / 2);
	}
}

ui full_asso_cache::heap_traverse(ui index) {
	if (index >= hot_cold.size() / 2) {
		return index;
	}

	ui new_index = -1;
	if (hot_cold[index] == 0) {
		new_index = move_right(index);
		//reverse(index);
		return heap_traverse(new_index);
	}

	else {
		new_index = move_left(index);
		//reverse(index);
		return heap_traverse(new_index);
	}
}

ui full_asso_cache::move_left(ui index) {
	return 2 * index + 1;
}

ui full_asso_cache::move_right(ui index) {
	return 2 * index + 2;
}