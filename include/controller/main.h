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

								main(lm::logger&, ldtools::ttf_manager&, sprite_table::session_data&);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/) {}
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}
	
	void                        set_message(const std::string&);
	
	private:

	struct {
		std::string             message;
		float                   time;		
	}                           last_message{"", 0.0f};

	//references...
	lm::logger&					log;
	ldtools::ttf_manager&       ttfman;
	sprite_table::session_data& session_data;

	//properties
	ldv::ttf_representation     last_message_rep;
};

}
