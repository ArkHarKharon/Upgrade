#pragma once

#include <SDL/SDL_mixer.h>

#include <string>
#include <map>

namespace MyEngine
{
	class SoundEffect
	{
	private:
		friend class AudioManager;
		Mix_Chunk* m_chunk = nullptr;

	public:
		void play(int loop = 0);
	};

	class Music
	{
	private:
		friend class AudioManager;
		Mix_Music* m_music = nullptr;

	public:
		void play(int loop = 0);
		static void pause();
		static void stop();
		static void resume();
	};


	class AudioManager
	{
	private:
		bool m_initialized = false;

		std::map<std::string, Mix_Chunk*> m_effect_map;
		std::map<std::string, Mix_Music*> m_music_map;

	public:
		AudioManager();
		~AudioManager();

		void init();
		void destroy();

		SoundEffect load_sound_effect(const std::string& sound_effect_filepath);
		Music load_music(const std::string& music_filepath);

	};
}

