#include "AudioManager.hpp"
#include "Errors.hpp"




namespace MyEngine
{

	void SoundEffect::play(int loop) // -1 - бесконечная петля, в других случаях - число повторов
	{
		if (Mix_PlayChannel(-1, m_chunk, loop) == -1)
		{
			if(Mix_PlayChannel(0, m_chunk, loop) == -1)
				fatal_error("Ошибка Mix_PlayChannel(): " + std::string(Mix_GetError()));
		}
			
	}

	void Music::play(int loop) // -1 - бесконечная петля, в других случаях - число повторов
	{
		if (Mix_PlayMusic(m_music, loop) == -1)
			fatal_error("Ошибка Mix_PlayMusic(): " + std::string(Mix_GetError()));
	}

	void Music::pause()
	{
		Mix_PauseMusic();
	}

	void Music::stop()
	{
		Mix_HaltMusic();
	}

	void Music::resume()
	{
		Mix_ResumeMusic();
	}



	AudioManager::AudioManager()
	{

	}

	AudioManager::~AudioManager()
	{
		destroy();
	}

	void AudioManager::init()
	{
		if (Mix_Init(MIX_INIT_MP3) == -1)  //флаги для различных типов файлов
			fatal_error("Ошибка в функции Mix_Init(): " + std::string(Mix_GetError()));
	
		if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY * 2, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
			fatal_error("Ошибка в функции Mix_OpenAudio(): " + std::string(Mix_GetError()));

		m_initialized = true;
	}

	void AudioManager::destroy()
	{
		if (m_initialized)
		{
			m_initialized = false;

			Mix_Quit();
		}
	}

	SoundEffect AudioManager::load_sound_effect(const std::string& sound_effect_filepath)
	{
		auto iterator = m_effect_map.find(sound_effect_filepath);
		SoundEffect effect;

		if (iterator == m_effect_map.end())
		{
			Mix_Chunk* chunk = Mix_LoadWAV(sound_effect_filepath.c_str());
			if (chunk == nullptr)
				fatal_error("Ошибка Mix_LoadWAV(): " + std::string(Mix_GetError()));

			m_effect_map[sound_effect_filepath] = chunk;

			effect.m_chunk = chunk;
		}

		else
		{
			effect.m_chunk = iterator->second;
		}

		return effect;
	}

	Music AudioManager::load_music(const std::string& music_filepath)
	{
		auto iterator = m_music_map.find(music_filepath);
		Music music;

		if (iterator == m_music_map.end())
		{
			Mix_Music* chunk = Mix_LoadMUS(music_filepath.c_str());
			if (chunk == nullptr)
				fatal_error("Ошибка Mix_LoadMUS(): " + std::string(Mix_GetError()));

			m_music_map[music_filepath] = chunk;

			music.m_music = chunk;
		}

		else
		{
			music.m_music = iterator->second;
		}

		return music;
	}
}

