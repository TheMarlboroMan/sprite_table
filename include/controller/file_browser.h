#pragma once

//local
#include "states.h"

//framework
#include <dfw/controller_interface.h>

//logger
#include <lm/logger.h>
#include <ldtools/view_composer.h>
#include <ldtools/ttf_manager.h>

//std
#include <filesystem>
#include <cmath>

namespace controller {

class file_browser:
	public dfw::controller_interface {

	public:

								file_browser(lm::logger&, ldtools::ttf_manager&);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/) {}
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	private:

	//!Fills up "contents" with the contents of the current directory.
	void						extract_entries();
	//!Refreshes the ttf_representation.
	void						refresh_list_view();

	//references...
	lm::logger&					log;
	ldtools::ttf_manager&		ttf_manager;

	//properties
	std::filesystem::path		current_directory;
	std::string					title;
	std::vector<std::string>	contents;

	//view properties.
	ldtools::view_composer		layout;

};

}
