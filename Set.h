#ifndef SET_H
#define SET_H
template <typename type>
struct Set {
	type base;
	type(*succ)(type, size_t);
	Set(type base_, type(*succ_)(type)) : base{ base_ }, succ{ succ_ } {}
	Set() : base{ type() }, succ{ nullptr } {}

	type get(size_t depth_) {
		type seed = base;
		for (size_t depth = 0; depth < depth_; depth++) { seed = succ(seed, depth); }
		return seed;
	}
};
#endif