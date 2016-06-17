#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <iterator>
#include <ctime>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;

//This pair has a constructor
using v_long = unsigned long long;
using ui = unsigned int;

struct Pair {
	Pair(v_long addr, const string inst) {
		this->addr = addr;
		this->inst = inst;
	}
	v_long addr;
	string inst;
};

struct Result {
	Result(v_long count, v_long access) {
		this->count = count;
		this->access = access;
	}
	v_long count;
	v_long access;
};

//Initial State of a cache slot
//lru stands for least recently used
struct cache_slot {
	cache_slot():valid(false), tag(0), time(0), access(0) {

	}
	bool valid;
	ui tag;
	ui time;
	ui access; //To be used for extra credit
};

using oper = std::vector<Pair>;
using setArray = std::vector<cache_slot>;
using ways = std::vector<setArray>;
using results = std::vector<Result>;

enum io {
	INPUT = 1,
	OUTPUT= 2
};

enum Size {
	HALF_KB = 512,
	ONE_KB = 1024,
	FOUR_KB = 4096,
	SIXTEEN_KB = 16384,
	THIRTYTWO_KB = 32768
};

enum Ways {
	TWO = 2,
	FOUR = 4,
	EIGHT = 8,
	SIXTEEN = 16
};
#endif