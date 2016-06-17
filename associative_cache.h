#ifndef ASSOCIATIVE_CACHE_H
#define ASSOCIATIVE_CACHE_H

#include "cache.h"

class associative_cache : public cache
{
public:
	associative_cache(Size, Ways, const oper &);
	Result run();
	Result run_on_write_miss();
	Result pre_fetch();
	Result pre_fetch_on_miss();

	//Extra Credit
	void extra_credit();

private:
	bool go_over_set(v_long, bool);
	ways way_list;
};

#endif