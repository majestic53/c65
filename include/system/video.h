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

#ifndef C65_SYSTEM_VIDEO_H_
#define C65_SYSTEM_VIDEO_H_

#include "../interface/bus.h"
#include "../interface/singleton.h"

namespace c65 {

	namespace system {

		class video :
				public c65::interface::bus,
				public c65::interface::singleton<c65::system::video> {

			public:

				~video(void);

				void frame_rate(
					__in float rate
					);

				void render(void);

			protected:

				friend class c65::interface::singleton<c65::system::video>;

				video(void);

				video(
					__in const video &other
					) = delete;

				video &operator=(
					__in const video &other
					) = delete;

				void on_initialize(void) override;

				void on_notify(
					__in const c65_event_t &event
					) const override;

				c65_byte_t on_read(
					__in c65_address_t address
					) const override;

				void on_uninitialize(void) override;

				void on_write(
					__in c65_address_t address,
					__in c65_byte_t value
					) override;

				bool m_changed;

				std::vector<c65_byte_t> m_color;

				std::vector<color_t> m_pixel;

				SDL_Renderer *m_renderer;

				SDL_Texture *m_texture;

				std::string m_title;

				SDL_Window *m_window;
		};
	}
}

#endif // C65_SYSTEM_VIDEO_H_
