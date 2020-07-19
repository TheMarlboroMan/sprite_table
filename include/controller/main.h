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

								main(lm::logger&, ldtools::ttf_manager&, sprite_table::session_data&, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/) {}
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	void                        set_message(const std::string&);
	void                        adjust_camera_limits();
	void                        select_first_index();
	void                        save();

	private:

	void                        workspace_input(dfw::input&);
	void                        console_input(dfw::input&);
	void                        create_sprite();
	void                        zoom_in();
	void                        zoom_out();
	void                        select_next();
	void                        select_prev();
	void                        delete_current();
	void                        duplicate_current();
	void                        clear_selection();
	std::size_t                 get_next_index() const;
	void                        perform_movement(int, int, bool, bool);
	void                        draw_background(ldv::screen&);
	void                        draw_messages(ldv::screen&);
	void                        draw_sprites(ldv::screen&);
	void                        draw_hud(ldv::screen&);
	void                        draw_console(ldv::screen&);

	sprite_table::session_data::container::const_iterator find_by_position(ldt::point_2d<int>) const;
	ldt::point_2d<int>			get_mouse_position(dfw::input&) const;

	struct {
		std::string             message;
		float                   time;
	}                           last_message{"", 0.0f};

	//references...
	lm::logger&                 log;
	ldtools::ttf_manager&       ttfman;
	sprite_table::session_data& session_data;

	//properties
	ldv::camera                 camera;
	ldv::ttf_representation     last_message_rep;
	ldt::point_2d<int>			mouse_pos;
	int                         selected_index=-1;
	const unsigned int          default_w,
	                            default_h,
	                            movement_factor;
	bool                        console_mode=false;
	std::string                 console_txt; //we cannot drag the ldv::input to the view to draw this...
};

}
