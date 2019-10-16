#pragma once
#include <random>

class RandomEngine
{
private:
	std::random_device rd;	//initialize random device
public:
	std::mt19937 mt{ rd() };	//seed mt engine

	RandomEngine() : rd(), mt(rd()) {}

	template<class T>
	std::uniform_int_distribution<T> uni_int_distr(T begin, T end)
	{
		std::uniform_int_distribution<T> dist(begin, end);
		return dist;
	}
};