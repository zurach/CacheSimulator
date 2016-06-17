#include "associative_cache.h"

associative_cache::associative_cache(Size size, Ways way, const oper & operations) : cache(size, operations) {
	this->set_no /= way;
	way_list.resize(this->set_no);
	for (auto it = way_list.begin(); it != way_list.end(); ++it) {
		(*it).resize(way);
	}
}

Result associative_cache::run() {

	v_long block_no;
	v_long set_no;
	v_long tag;

	for (auto iter = opers.begin(); iter != opers.end(); ++iter) {
		
		//Find necessary indexes
		block_no = ret_block(iter->addr, cache_line);
		set_no = ret_set(block_no, this->set_no);
		tag = ret_tag(block_no, this->set_no);
	
		setArray & ref = way_list[set_no];
		bool hit = false;

		//See if there is a hit
		for (auto it = ref.begin(); it != ref.end(); ++it) {
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
		//Handle replacement here
		if (!hit) {
			auto lru = ref.begin();
			for (auto it = ref.begin(); it != ref.end(); ++it) {
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

Result associative_cache::run_on_write_miss() {
	
	v_long block_no;
	v_long set_no;
	v_long tag;

	for (auto iter = opers.begin(); iter != opers.end(); ++iter) {

		//Find necessary indexes
		block_no = ret_block(iter->addr, cache_line);
		set_no = ret_set(block_no, this->set_no);
		tag = ret_tag(block_no, this->set_no);

		setArray & ref = way_list[set_no];
		bool hit = false;

		//See if there is a hit
		for (auto it = ref.begin(); it != ref.end(); ++it) {
			cache_slot & slot = *it;
			//If there is a hit happened, just increment time stamp of the rest
			if (hit) {
				slot.time++;
			}
			//If there is no hit yet,
			else {
				if (slot.valid != true && iter->inst == "L") {
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
		//Handle replacement here
		if (!hit && (iter->inst == "L")) {
			auto lru = ref.begin();
			for (auto it = ref.begin(); it != ref.end(); ++it) {
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

bool associative_cache::go_over_set(v_long addr, bool _count) {

	v_long block_no = ret_block(addr, cache_line);
	v_long set_no = ret_set(block_no, this->set_no);
	v_long tag = ret_tag(block_no, this->set_no);

	setArray & ref = way_list[set_no];

	bool hit = false;
	bool placed = false;

	for (auto it = ref.begin(); it != ref.end(); ++it) {
		cache_slot & slot = *it;
		//If there is a hit happened, just increment time stamp of the rest
		if (hit || placed) {
			slot.time++;
		}
		//If there is no hit yet,
		else {
			if (slot.valid != true) {
				slot.valid = true;
				slot.tag = tag;
				slot.time = 0;
				placed = true;
				continue;
			}

			if (slot.tag == tag) {
				slot.time = 0;
				if(_count) ++(this->count);
				hit = true;
				continue;
			}
			else {
				slot.time++;
			}
		}
	}

	if (!(hit || placed)) {
		auto lru = ref.begin();
		for (auto it = ref.begin(); it != ref.end(); ++it) {
			if (lru->time < it->time) {
				lru = it;
			}
		}
		lru->tag = tag;
		lru->time = 0;
	}

	return hit;
}

Result associative_cache::pre_fetch() {
	for (auto iter = opers.begin(); iter != opers.end(); ++iter) {
		go_over_set(iter->addr, true);
		go_over_set(iter->addr + 32, false);
	}
	return Result(this->count, this->access_no);
}

Result associative_cache::pre_fetch_on_miss() {
	for (auto iter = opers.begin(); iter != opers.end(); ++iter) {
		bool hit = go_over_set(iter->addr, true);
		if(!hit) //Meaning if miss
			go_over_set(iter->addr + 32, false);
	}
	return Result(this->count, this->access_no);
}

void associative_cache::extra_credit() {

	for (auto iter = opers.begin(); iter != opers.end(); ++iter) {
		
		v_long block_no = ret_block(iter->addr, cache_line);
		v_long set_no = ret_set(block_no, this->set_no);
		v_long tag = ret_tag(block_no, this->set_no);

		setArray & ref = way_list[set_no];

		bool hit = false;
		bool placed = false;

		for (auto it = ref.begin(); it != ref.end(); ++it) {
			cache_slot & slot = *it;
	
			if (slot.valid != true) {
				slot.valid = true;
				slot.access++;
				slot.tag = tag;
				placed = true;
				continue;
			}

			if (slot.tag == tag) {
				++(this->count);
				slot.access++;
				hit = true;
				continue;
			}
		}

		if (!(hit || placed)) {
			
			auto lru = ref.begin();
			for (auto it = ref.begin(); it != ref.end(); ++it) {
				if (lru->access < it->access) {
					lru = it;
				}
			}
			lru->tag = tag;
			lru->access++;
		}
	}
	cout << "Count : " << this->count << " Access : " << this->access_no << endl;
}