#ifndef SET_H
#define SET_H
#include <iostream>
template <typename type>
struct Set {
	type base;
	type(*succ)(type, size_t);
	Set(type base_, type(*succ_)(type, size_t)) : base{ base_ }, succ{ succ_ } {}
	Set() : base{ type() }, succ{ nullptr } {}

	type get(size_t depth_) {
		type seed = base;
		for (size_t depth = 0; depth < depth_; depth++) { seed = succ(seed, depth); }
		return seed;
	}

	friend std::ostream& operator<<(std::ostream& os, Set<type>& set_) {
		os << "{" << set_.get(0) << ",";
		for (size_t i = 1; i < 4; i++) {
			os << " " << set_.get(i) << ",";
		}
		os << "...}";

		return os;
	}
};
#endif