#ifndef CACHE_CPP
#define CACHE_CPP

#include "common_def.h"

class cache
{
public:
	cache(Size, const oper &);

	Result run();

protected:
	ui set_no;
	const oper & opers;

	//Return block number
	v_long ret_block(v_long, ui);
	//Return Set Number
	v_long ret_set(ui, ui);
	//Return tag 
	v_long ret_tag(ui, ui);

	setArray sets;
	ui count;
	ui miss;
	ui access_no;

	const static int cache_line = 32;
};


#endif