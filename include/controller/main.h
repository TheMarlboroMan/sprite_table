#pragma once

//local
#include "states.h"
#include "../sprite_table/session_data.h"

//framework
#include <dfw/controller_interface.h>
#include <ldv/ttf_representation.h>

//libs
#include <lm/logger.h>
#include <ldtools/ttf_manager.h>

//std
#include <cmath>

namespace controller {

class main:
	public dfw::controller_interface {

	public:

								main(lm::logger&, ldtools::ttf_manager&, sprite_table::session_data&, int, int);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/) {}
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	void                        set_message(const std::string&);
	void                        adjust_camera_limits();
	void                        select_first_index();

	private:

	void                        zoom_in();
	void                        zoom_out();
	void                        select_next();
	void                        select_prev();

	void                        draw_background(ldv::screen&);
	void                        draw_messages(ldv::screen&);
	void                        draw_sprites(ldv::screen&);

	sprite_table::session_data::container::const_iterator find_by_position(int, int) const;

	struct {
		std::string             message;
		float                   time;
	}                           last_message{"", 0.0f};

	//references...
	lm::logger&					log;
	ldtools::ttf_manager&       ttfman;
	sprite_table::session_data& session_data;

	//properties
	ldv::camera                 camera;
	ldv::ttf_representation     last_message_rep;
	int                         selected_index=-1;
};

}
