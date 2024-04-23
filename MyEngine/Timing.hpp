#pragma once

#include <SDL.h>
#include <glew.h>

#include <iostream>
#include <vector>


namespace MyEngine
{

	class FPS_Limiter
	{
		int m_fps;
		int m_max_fps;
		int m_frame_time;
		GLuint m_start_ticks;

		void calculate_fps(int samples_num);

	public:
		void set_max_fps(int fps);
		void init(int max_fps);

		void frame_begin();
		int frame_end();

	};

}