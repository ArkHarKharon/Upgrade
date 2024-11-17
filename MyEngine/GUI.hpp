#pragma once

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <SDL/SDL_events.h>
#include <SDL/SDL_timer.h>

#include <glm/glm.hpp>
#include <string>

namespace MyEngine
{
	class GUI
	{
	private:
		static CEGUI::OpenGL3Renderer* m_renderer;
		CEGUI::GUIContext* m_context = nullptr;
		CEGUI::Window* m_root_window = nullptr;

		unsigned int m_last_time = 0;

	public:

		void init(const std::string& resource_directory);

		void destroy();

		void draw();
		void update();

		void set_mouse_cursor(const std::string& image_file);
		void show_mouse_cursor();
		void hide_mouse_cursor();

		void on_SDL_event(SDL_Event& event);

		void load_scheme(const std::string& scheme_file);

		void set_font(const std::string& font_file);

		CEGUI::Window* create_widget(const std::string& scheme_file,const glm::vec4& dest_rect_percent,
			const glm::vec4& dest_rect_pix,const std::string& name = "");

		static void set_widget_dest_rect(CEGUI::Window* widget, const glm::vec4& dest_rect_percent,
			const glm::vec4& dest_rect_pix);

		static CEGUI::OpenGL3Renderer* get_renderer() { return m_renderer; }
		CEGUI::GUIContext* get_context() { return m_context; }
	};
}
