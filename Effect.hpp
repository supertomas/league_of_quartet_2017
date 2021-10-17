# include <Siv3D.hpp>
#include"Title.hpp"
struct Snow : IEffect
{
	struct Particle
	{
		Vec2 pos, v0;
	};

	Array<Particle> m_particles;

	Snow(const Point& pos, int count)
		: m_particles(count)
	{
		for (auto& particle : m_particles)
		{
			const Vec2 v = Circular(Random(10.0, 80.0), Random(TwoPi));
			particle.pos = pos + v;
			particle.v0 = v * 2.0;
		}
	}

	bool update(double t) override
	{
		for (const auto& particle : m_particles)
		{
			const Vec2 pos = particle.pos + particle.v0 * t + 0.5* t*t * Vec2(0, 320);

			Circle(pos, 1).draw(HSV(pos.y / 4.0, 0.6, 1.0).toColorF(1.0 - t));
		}

		return t < 0.5;
	}
};

struct TextEffect : IEffect
{
	std::vector<Images*> images;
	const Font m_font;

	const int m_value;

	const Vec2 m_from;
	TextEffect(const Font& font, int value, const Vec2& from)
		: m_font(font)
		, m_value(value)
		, m_from(from) {}

	bool update(double t) override
	{
		if (t >= 0.5)
		{
			return false;
		}

		const double alpha = 1.0 - t;

		const double h = m_value > 80 ? 0.0 : m_value > 50 ? 20.0 : 60.0;

		m_font(m_value).drawCenter(m_from + Vec2(0, -40 * t), HSV(h).toColorF(alpha));
		for (auto& image : images)
		{
			image->draw();
		}
		return true;
	}
};

