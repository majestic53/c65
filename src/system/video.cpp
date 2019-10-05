/**
 * C65
 * Copyright (C) 2019 David Jolly
 *
 * C65 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * C65 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../include/system/video.h"
#include "./video_type.h"

namespace c65 {

	namespace system {

		video::video(void) :
			m_renderer(nullptr),
			m_texture(nullptr),
			m_window(nullptr)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		video::~video(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		void
		video::frame_rate(
			__in float rate
			)
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Rate=%.02f", rate);

			result << m_title << " [" << (int)rate << " fps]";
			SDL_SetWindowTitle(m_window, STRING(result.str()));

			TRACE_EXIT();
		}

		void
		video::on_initialize(void)
		{
			color_t background = COLOR(BACKGROUND_COLOR);

			TRACE_ENTRY();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Video initializing");

			m_color.resize(WINDOW_WIDTH * WINDOW_WIDTH, BACKGROUND_COLOR);
			m_pixel.resize(WINDOW_WIDTH * WINDOW_WIDTH, background);
			m_title = WINDOW_TITLE;

			m_window = SDL_CreateWindow(STRING(m_title), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					WINDOW_WIDTH * WINDOW_SCALE, WINDOW_WIDTH * WINDOW_SCALE, SDL_WINDOW_FLAGS);

			if(!m_window) {
				THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(C65_SYSTEM_VIDEO_EXCEPTION_EXTERNAL,
					"SDL_CreateWindow failed! %s", SDL_GetError());
			}

			m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_FLAGS);
			if(!m_renderer) {
				THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(C65_SYSTEM_VIDEO_EXCEPTION_EXTERNAL,
					"SDL_CreateRenderer failed! %s", SDL_GetError());
			}

			if(SDL_RenderSetLogicalSize(m_renderer, WINDOW_WIDTH, WINDOW_WIDTH)) {
				THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(C65_SYSTEM_VIDEO_EXCEPTION_EXTERNAL,
					"SDL_RenderSetLogicalSize failed! %s", SDL_GetError());
			}

			if(SDL_SetRenderDrawColor(m_renderer, background.red, background.green, background.blue, background.alpha)) {
				THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(C65_SYSTEM_VIDEO_EXCEPTION_EXTERNAL,
					"SDL_SetRenderDrawColor failed! %s", SDL_GetError());
			}

			if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, SDL_RENDERER_QUALITY) == SDL_FALSE) {
				THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(C65_SYSTEM_VIDEO_EXCEPTION_EXTERNAL,
					"SDL_SetHint failed! %s", SDL_GetError());
			}

			m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
					WINDOW_WIDTH, WINDOW_WIDTH);

			if(!m_texture) {
				THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(C65_SYSTEM_VIDEO_EXCEPTION_EXTERNAL,
					"SDL_CreateTexture failed! %s", SDL_GetError());
			}

			if(SDL_RenderClear(m_renderer)) {
				THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(C65_SYSTEM_VIDEO_EXCEPTION_EXTERNAL,
					"SDL_RenderClear failed! %s", SDL_GetError());
			}

			SDL_RenderPresent(m_renderer);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Video initialized");

			TRACE_EXIT();
		}

		c65_byte_t
		video::on_read(
			__in c65_address_t address
			) const
		{
			c65_byte_t result = MEMORY_ZERO;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address.word, address.word);

			switch(address.word) {
				case ADDRESS_VIDEO_BEGIN ... ADDRESS_VIDEO_END:
					result = m_color.at(address.word - ADDRESS_VIDEO_BEGIN);
					break;
				default:
					THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(C65_SYSTEM_VIDEO_EXCEPTION_ADDRESS_INVALID, "%u(%04x)",
						address.word, address.word);
			}

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		video::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Video uninitializing");

			if(m_texture) {
				SDL_DestroyTexture(m_texture);
				m_texture = nullptr;
			}

			if(m_renderer) {
				SDL_DestroyRenderer(m_renderer);
				m_renderer = nullptr;
			}

			if(m_window) {
				SDL_DestroyWindow(m_window);
				m_window = nullptr;
			}

			m_title.clear();
			m_pixel.clear();
			m_color.clear();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Video uninitialized");

			TRACE_EXIT();
		}

		void
		video::on_write(
			__in c65_address_t address,
			__in c65_byte_t value
			)
		{
			size_t index;

			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address.word, address.word, value, value);

			switch(address.word) {
				case ADDRESS_VIDEO_BEGIN ... ADDRESS_VIDEO_END:
					index = (address.word - ADDRESS_VIDEO_BEGIN);
					m_color.at(index) = value;
					m_pixel.at(index) = COLOR(value);
					break;
				default:
					THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(C65_SYSTEM_VIDEO_EXCEPTION_ADDRESS_INVALID, "%u(%04x)",
						address.word, address.word);
			}

			TRACE_EXIT();
		}

		void
		video::render(void)
		{
			TRACE_ENTRY();

			if(SDL_UpdateTexture(m_texture, nullptr, &m_pixel[0], WINDOW_WIDTH * sizeof(color_t))) {
				THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(C65_SYSTEM_VIDEO_EXCEPTION_EXTERNAL,
					"SDL_UpdateTexture failed! %s", SDL_GetError());
			}

			if(SDL_RenderClear(m_renderer)) {
				THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(C65_SYSTEM_VIDEO_EXCEPTION_EXTERNAL,
					"SDL_RenderClear failed! %s", SDL_GetError());
			}

			if(SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr)) {
				THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(C65_SYSTEM_VIDEO_EXCEPTION_EXTERNAL,
					"SDL_RenderCopy failed! %s", SDL_GetError());
			}

			SDL_RenderPresent(m_renderer);

			TRACE_EXIT();
		}
	}
}
