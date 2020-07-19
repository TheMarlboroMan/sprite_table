#pragma once

//local
#include "states.h"

//external
#include <dfw/controller_interface.h>
#include <ldtools/ttf_manager.h>
#include <ldv/ttf_representation.h>
#include <ldv/camera.h>
#include <lm/logger.h>

//std
#include <cmath>

namespace controller {

class help:
	public dfw::controller_interface {

	public:

								help(lm::logger&, ldtools::ttf_manager& _ttfman, unsigned int, unsigned int);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/) {}
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	private:

	//references...
	lm::logger&					log;

	//properties
	ldv::camera					camera;
	ldv::ttf_representation		help_txt_rep;
};

}
