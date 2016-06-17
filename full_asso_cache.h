#ifndef FULL_ASSO_CACHE_H
#define FULL_ASSO_CACHE_H

#include "cache.h"

class full_asso_cache : public cache
{
public:
	full_asso_cache(Size, const oper &);

	//using least recently used
	Result run_lru();

	//using hot cold approximation 
	Result cold_hot_approximation();

private:
	
	std::vector<ui> hot_cold;

	void update_after_access(ui);
	void heap_reverse_traverse(ui, bool);
	
	ui heap_traverse(ui);
	ui move_left(ui);
	ui move_right(ui);
	ui replace();
	ui convert(ui);
};

#endif