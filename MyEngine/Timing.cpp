#include "Timing.hpp"

namespace MyEngine
{

	void FPS_Limiter::init(int max_fps)
	{
		set_max_fps(max_fps);
	}

	void FPS_Limiter::set_max_fps(int max_fps)
	{
		m_max_fps = max_fps;
	}

	void FPS_Limiter::frame_begin()
	{
		m_max_fps = SDL_GetTicks();
	}

	int FPS_Limiter::frame_end()
	{
		calculate_fps(1000);

		float frame_ticks = SDL_GetTicks() - m_start_ticks;
		if (1000.0f / m_max_fps > frame_ticks)
			SDL_Delay(1000.0f / m_max_fps - frame_ticks);

		return m_fps;
	}

	void FPS_Limiter::calculate_fps(int samples_num)
	{

		static std::vector<float> frame_times;
		frame_times.resize(samples_num);
		static int current_frame = 0;

		static float prev_ticks = SDL_GetTicks();

		float current_ticks;
		current_ticks = SDL_GetTicks(); 

		m_frame_time = current_ticks - prev_ticks;
		frame_times.at(current_frame % samples_num) = m_frame_time;

		prev_ticks = current_ticks;

		int count;
		current_frame++;

		if (current_frame < samples_num)
			count = current_frame;
		else count = samples_num;

		float averange_frame_time = 0;
		for (int i = 0; i < count; i++)
			averange_frame_time += frame_times.at(i);

		averange_frame_time /= count;
		if (averange_frame_time)
			m_fps = 1000.0f / averange_frame_time;
		else m_fps = 60.0f;

	}
}