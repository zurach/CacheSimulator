#include "common_def.h"
#include "cache.h"
#include "associative_cache.h"
#include "full_asso_cache.h"

ofstream o_file;

void read_trace(const string);
oper operation;

void create_output(char *);
void close_output();

void direct_mapped_call();
void associative_cache_call();
void full_associative_call();
void associative_call_on_miss();
void prefetch_asso_cache_call();
void prefetch_asso_cache_on_miss_call();
void extra_credit();
void write_results(const results &);

int main(int argc, char * argv[]) {

	//Read files
	read_trace(argv[INPUT]);

	create_output(argv[OUTPUT]);

	direct_mapped_call();

	associative_cache_call();

	full_associative_call();

	associative_call_on_miss();

	prefetch_asso_cache_call();

	prefetch_asso_cache_on_miss_call();

	extra_credit();

	exit(EXIT_SUCCESS);
}

//Read traces
void read_trace(const string fn) {
	v_long addr_tmp;
	string instr;
	
	ifstream infile;
	infile.open(fn.c_str());
	if (!infile.is_open()) {
		cout << "There is a problem opening the trace file" << endl;
		cout << "Exiting..." << endl;
		exit(EXIT_FAILURE);
	}

	//Read inputs to corresponding data structures
	while (infile >> instr >> std::hex >> addr_tmp) {
		operation.push_back(Pair(addr_tmp, instr));
	}

	//Close the file when done with that
	infile.close();
}

void direct_mapped_call() {

	results set;

	cache one_kb(ONE_KB, operation);
	cache four_kb(FOUR_KB, operation);
	cache sixteen_kb(SIXTEEN_KB, operation);
	cache thirty_two_kb(THIRTYTWO_KB, operation);

	set.push_back(one_kb.run());
	set.push_back(four_kb.run());
	set.push_back(sixteen_kb.run());
	set.push_back(thirty_two_kb.run());

	write_results(set);
}

void associative_cache_call() {

	results set;

	associative_cache two_way(SIXTEEN_KB, TWO, operation);
	associative_cache four_way(SIXTEEN_KB, FOUR, operation);
	associative_cache eight_way(SIXTEEN_KB, EIGHT, operation);
	associative_cache sixteen_way(SIXTEEN_KB, SIXTEEN, operation);

	set.push_back(two_way.run());
	set.push_back(four_way.run());
	set.push_back(eight_way.run());
	set.push_back(sixteen_way.run());

	write_results(set);
}

void full_associative_call() {

	results set_one;
	results set_two;

	full_asso_cache full_asso_lru(SIXTEEN_KB, operation);
	full_asso_cache full_hot_cold(SIXTEEN_KB, operation);

	set_one.push_back(full_asso_lru.run_lru());
	set_two.push_back(full_hot_cold.cold_hot_approximation());

	write_results(set_one);
	write_results(set_two);
}

void associative_call_on_miss() {
	
	results set;

	associative_cache two_way(SIXTEEN_KB, TWO, operation);
	associative_cache four_way(SIXTEEN_KB, FOUR, operation);
	associative_cache eight_way(SIXTEEN_KB, EIGHT, operation);
	associative_cache sixteen_way(SIXTEEN_KB, SIXTEEN, operation);

	set.push_back(two_way.run_on_write_miss());
	set.push_back(four_way.run_on_write_miss());
	set.push_back(eight_way.run_on_write_miss());
	set.push_back(sixteen_way.run_on_write_miss());

	write_results(set);
}

void prefetch_asso_cache_call() {

	results set;

	associative_cache two_way(SIXTEEN_KB, TWO, operation);
	associative_cache four_way(SIXTEEN_KB, FOUR, operation);
	associative_cache eight_way(SIXTEEN_KB, EIGHT, operation);
	associative_cache sixteen_way(SIXTEEN_KB, SIXTEEN, operation);

	set.push_back(two_way.pre_fetch());
	set.push_back(four_way.pre_fetch());
	set.push_back(eight_way.pre_fetch());
	set.push_back(sixteen_way.pre_fetch());

	write_results(set);
}

void prefetch_asso_cache_on_miss_call() {

	results set;

	associative_cache two_way(SIXTEEN_KB, TWO, operation);
	associative_cache four_way(SIXTEEN_KB, FOUR, operation);
	associative_cache eight_way(SIXTEEN_KB, EIGHT, operation);
	associative_cache sixteen_way(SIXTEEN_KB, SIXTEEN, operation);

	set.push_back(two_way.pre_fetch_on_miss());
	set.push_back(four_way.pre_fetch_on_miss());
	set.push_back(eight_way.pre_fetch_on_miss());
	set.push_back(sixteen_way.pre_fetch_on_miss());

	write_results(set);
}

void extra_credit() {
	cout << "-----Extra Credit-----" << endl;
	associative_cache two_way(SIXTEEN_KB, TWO, operation);
	associative_cache four_way(SIXTEEN_KB, FOUR, operation);
	associative_cache eight_way(SIXTEEN_KB, EIGHT, operation);
	associative_cache sixteen_way(SIXTEEN_KB, SIXTEEN, operation);

	cout << "2-Way" << endl;
	two_way.extra_credit();
	cout << "4-Way" << endl;
	four_way.extra_credit();
	cout << "8-Way" << endl;
	eight_way.extra_credit();
	cout << "16-Way" << endl;
	sixteen_way.extra_credit();
}

void write_results(const results & vec) {
	for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
		o_file << (*iter).count << "," << (*iter).access << ";";
		if (iter + 1 != vec.end()) {
			o_file << " ";
		}
	}
	o_file << endl;
}

void create_output(char * file_name) {
	//Open the file
	o_file.open(file_name);
	if (!o_file.is_open()) {
		cout << "There was a problem opening the output file for writing." << endl;
		cout << "Exiting..." << endl;
		exit(1);
	}
}

void close_output() {
	//Close the file opened for output purpose
	o_file.close();
}