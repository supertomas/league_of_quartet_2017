# include "..\Anime.hpp"

using namespace asc;

Anime::Anime() = default;

Anime::Anime(const Texture& texture, size_t size, int32 duration) :
	m_texture(texture),
	m_size(size),
	m_duration(size, duration),
	m_index(0) {}

Anime::Anime(const Texture& texture, size_t size, const Array<int32>& duration) :
	m_texture(texture),
	m_size(size),
	m_duration(duration),
	m_index(0) {}

Anime::~Anime() = default;

void Anime::reset() noexcept
{
	m_index = 0;
	m_stopwatch.reset();
}

void Anime::restart()
{
	m_index = 0;
	m_stopwatch.restart();
}

void Anime::setDuration(int32 duration)
{
	m_duration = Array<int32>(m_size, duration);
}

void Anime::setDuration(const Array<int32>& duration)
{
	m_duration = duration;
}

void Anime::jump(int index, const MillisecondsF& time)
{
	m_index = index;
	m_stopwatch.set(time);
}

void Anime::update()
{
	auto ms = m_stopwatch.ms();

	while (ms > m_duration[m_index])
	{
		ms -= m_duration[m_index];
		m_index >= m_size - 1 ? m_index = 0 : m_index++;
	}

	m_stopwatch.set(MicrosecondsF(ms * 1000));
}

const TextureRegion Anime::get() const
{
	return m_texture.uv(static_cast<double>(m_index) / m_size, 0.0, 1.0 / m_size, 1.0);
}
