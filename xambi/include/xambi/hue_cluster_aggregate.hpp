#pragma once
#include <vector>
#include <cassert>
#include "color_aggregate.hpp"
#include "hsv.hpp"

namespace xambi {

class hue_cluster_aggregate : public color_aggregate
{
public:
	hue_cluster_aggregate() :
		m_bins(32)
	{
	}

	rgb_color process(const rgb_color *data, std::size_t size) override
	{
		int best_bin_index = 0;
		int best_bin_count = 0;

		for (auto &bin : m_bins)
			bin = {};

		for (std::size_t i = 0; i < size; i++)
		{
			const rgb_color color = data[i];

			if (color.r == 0 && color.g == 0 && color.b == 0) continue;

			const float r = color.r / 255.f;
			const float g = color.g / 255.f;
			const float b = color.b / 255.f;
			
			float h, s, v;
			xambi::rgb_to_hsv(r, g, b, h, s, v);
			
			const int bin_index = h * (m_bins.size() - 1);
			assert(bin_index < m_bins.size());
			m_bins[bin_index].s_sum += s;
			m_bins[bin_index].v_sum += v;
			m_bins[bin_index].count++;

			if (m_bins[bin_index].count > best_bin_count)
			{
				best_bin_count = m_bins[bin_index].count;
				best_bin_index = bin_index;
			}
		}

		const auto &best_bin = m_bins[best_bin_index];
		const float h = best_bin_index / static_cast<float>(m_bins.size());
		const float s = best_bin.s_sum / best_bin.count;
		const float v = best_bin.v_sum / best_bin.count;
		// float v = 1.f;
		// float s = 1.f;

		float r, g, b;
		xambi::hsv_to_rgb(h, s, v, r, g, b);
		
		rgb_color color;
		color.r = r * 255.f;
		color.g = g * 255.f;
		color.b = b * 255.f;
		return color;
	}

private:
	struct bin_type
	{
		float s_sum = 0;
		float v_sum = 0;
		int count = 0;
	};

	std::vector<bin_type> m_bins;
};

}
