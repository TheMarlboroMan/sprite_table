#pragma once

//local
#include "states.h"

//framework
#include <dfw/controller_interface.h>

//logger
#include <lm/logger.h>
#include <ldtools/view_composer.h>
#include <ldtools/ttf_manager.h>
#include <tools/pager.h>

//std
#include <filesystem>
#include <cmath>

namespace controller {

class file_browser:
	public dfw::controller_interface {

	public:

	                            file_browser(lm::logger&, ldtools::ttf_manager&);
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/) {}
	virtual void                slumber(dfw::input& /*input*/) {}
	virtual bool                can_leave_state() const {return true;}

	void                        set_title(const std::string& _title) {title=_title;}
	//!Returns if this controller exited with a chosen file.
	bool                        get_result() const {return result;}
	//!Returns the file that was chosen.
	std::string                 get_choice() const {return choice;}

	private:

	struct entry {
		enum class entry_type {file, dir};
		std::string             path_name;
		entry_type              type;
		bool                    is_dir() const {return entry_type::dir==type;}
		bool                    operator<(const entry& _other) const {

			if(_other.is_dir() && !is_dir()) {
				return false;
			}

			if(is_dir() && !_other.is_dir()) {
				return true;
			}

			return path_name < _other.path_name;
		}
	};

	//!Fills up "contents" with the contents of the current directory.
	void						extract_entries();
	//!Refreshes the ttf_representation.
	void						refresh_list_view();
	//!Positions the current selector representation.
	void                        position_selector();
	//!Void composes the title
	void                        compose_title();

	//references...
	lm::logger&                 log;
	ldtools::ttf_manager&       ttf_manager;

	//constants...
	std::size_t                 entries_per_page{10};

	//properties
	std::filesystem::path       current_directory;
	std::string                 title;
	std::vector<entry>          contents;
	int                         first_selection_y,
	                            y_selection_factor;

	bool                        result{false};
	std::string                 choice;

	tools::pager                pager;

	//view properties.
	ldtools::view_composer		layout;

};

}
