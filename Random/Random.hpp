#pragma once

#include <random>

class Random
{
public:
	Random() : m_engine(std::random_device{}()) {}
	template <typename T> class Generator
	{
		using Distribution
			= std::conditional_t<std::is_integral_v<T>, std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>;

	public:
		Generator(std::mt19937& engine, T min, T max) : m_engine(engine), m_dist(min, max) {}
		auto Next() -> T { return m_dist(m_engine); }

	private:
		std::mt19937& m_engine;
		Distribution m_dist;
	};
	template <typename T> auto Range(T min, T max) const -> Generator<T> { return Generator<T>(m_engine, min, max); }

private:
	mutable std::mt19937 m_engine;
};

static const Random RANDOM_INSTANCE;
