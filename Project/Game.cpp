#include "Game.hpp"


GameApp::GameApp()
{

}

GameApp::~GameApp()
{

}

void GameApp::on_init() 
{

}

void GameApp::add_screens() 
{
	m_gameplay_screen = new GameplayScreen(&m_window);
	m_screen_list->add_screen(m_gameplay_screen);
	m_screen_list->set_screen(m_gameplay_screen->get_screen_index());
}

void GameApp::on_exit()
{

}
