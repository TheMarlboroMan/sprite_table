#pragma once

//local
#include "states.h"
#include "../sprite_table/session_data.h"

//framework
#include <dfw/controller_interface.h>
#include <ldtools/sprite_table.h>
#include <ldtools/ttf_manager.h>
#include <ldv/camera.h>

//logger
#include <lm/logger.h>

//std
#include <vector>

namespace controller {

class presentation:
	public dfw::controller_interface {

	public:

	using container=std::map<std::size_t, ldtools::sprite_frame>;

								presentation(lm::logger&, ldtools::ttf_manager&, sprite_table::session_data&, int, int);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/);
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	private:

	struct item {
		size_t                  index;
		ldtools::sprite_frame   frame;
		int                     x,
								y;
	};

	void                        calculate();
	void                        draw_item(ldv::screen&, const item&);

	//references...
	lm::logger&					log;
	ldtools::ttf_manager&       ttfman;
	sprite_table::session_data& session_data;

	//properties
	ldv::camera                 camera;
	const int                   screen_w,
	                            screen_h;
	bool                        display_ids=true;

	std::vector<item>           items;
};

}
