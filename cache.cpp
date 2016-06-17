#include "cache.h"

cache::cache(Size size, const oper & operations) : opers(operations), count(0), miss(0) {
	//Find number of lines in a cache
	this->set_no = size / cache_line;
	//Resize the cache based on number of line it is supposed to have
	this->sets.resize(this->set_no, cache_slot());
	//Number of accesses
	this->access_no = opers.size();
}

//Put Application Logic here
Result cache::run() {
	//Iterate over all values from a trace file

	v_long block_no;
	v_long set_no;
	v_long tag;

	for (auto iter = opers.begin(); iter != opers.end(); ++iter) {
		//Find necessary indexes
		block_no = ret_block(iter->addr, cache_line);
		set_no = ret_set(block_no, this->set_no);
		tag = ret_tag(block_no, this->set_no);

		cache_slot & slot = sets[set_no];

		if (slot.valid != true) {
			slot.valid = true;
			slot.tag = tag;
			continue;
		}

		if (slot.tag == tag) {
			++count;
		}
		else {
			++miss;
			slot.tag = tag;
		}
	}

	//Returns number of accesses
	//cout << "count : " << count << " access: " << access_no << endl;
	return Result(this->count, this->access_no);
}

//Returns block number corresponding to a section in memory
v_long cache::ret_block(v_long addr, ui block_size) {
	return addr / block_size;
}

//Returns index to a set in the cache
v_long cache::ret_set(ui block_no, ui no_of_sets) {
	return block_no % no_of_sets;
}

//Returns the tag
v_long cache::ret_tag(ui block_number, ui no_of_sets) {
	return block_number / no_of_sets;
}