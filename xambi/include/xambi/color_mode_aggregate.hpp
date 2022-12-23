#pragma once
#include <vector>
#include <cassert>
#include "color_aggregate.hpp"

namespace xambi {

template <int RBITS, int GBITS, int BBITS>
class color_mode_aggregate : public color_aggregate
{
	static_assert(RBITS + GBITS + BBITS <= 24, "color depth must be less than 25");

public:
	using bin_type = uint32_t;

	color_mode_aggregate() :
		m_bins(1 << (RBITS + GBITS + BBITS))
	{
	}

	rgb_color process(const rgb_color *data, std::size_t size) override
	{
		for (auto &bin : m_bins)
			bin = 0;

		std::size_t best_bin_index = 0;
		bin_type best_bin_value = 0;

		for (std::size_t i = 0; i < size; i++)
		{
			const auto &color = data[i];
			const auto bin_index = 
				(color.r >> (8 - RBITS)) |
				((color.g >> (8 - GBITS)) << RBITS) |
				((color.b >> (8 - BBITS)) << (RBITS + GBITS));
			
			assert(bin_index < m_bins.size());
			m_bins[bin_index]++;

			if (m_bins[bin_index] > best_bin_value)
			{
				best_bin_value = m_bins[bin_index];
				best_bin_index = bin_index;
			}
		}

		rgb_color mode_color;
		mode_color.r = (best_bin_index & ((1 << RBITS) - 1));
		mode_color.g = ((best_bin_index >> RBITS) & ((1 << GBITS) - 1));
		mode_color.b = ((best_bin_index >> (RBITS + GBITS)) & ((1 << BBITS) - 1));

		mode_color.r *= 255.f / ((1 << RBITS) - 1);
		mode_color.g *= 255.f / ((1 << GBITS) - 1);
		mode_color.b *= 255.f / ((1 << BBITS) - 1);

		return mode_color;
	}

private:
	std::vector<bin_type> m_bins;
};

}
