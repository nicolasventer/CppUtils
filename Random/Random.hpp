#pragma once

#include <random>
#include <type_traits>

class Random
{
public:
	Random() noexcept : engine(std::random_device{}()) {}
	template <typename T> class Generator
	{
		using Distribution
			= std::conditional_t<std::is_integral_v<T>, std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>;

	public:
		Generator(std::mt19937& engine, T min, T max) : engine(&engine), dist(min, max) {}
		T next() { return dist(*engine); }

	private:
		std::mt19937* engine;
		Distribution dist;
	};
	template <typename T> Generator<T> range(T min, T max) const { return Generator<T>(engine, min, max); }

private:
	mutable std::mt19937 engine;
};

inline const Random RANDOM_INSTANCE;
