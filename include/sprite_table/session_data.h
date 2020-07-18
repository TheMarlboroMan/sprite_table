#pragma once

#include <ldtools/sprite_table.h>
#include <ldv/texture.h>
#include <string>
#include <memory>

namespace sprite_table {

class session_data {

	public:

	using container=std::map<std::size_t, ldtools::sprite_frame>;

	enum class file_browser_action {background, load, save};

	void                                set_texture_by_path(const std::string&);
	void                                load_sprites_by_path(const std::string&);
	file_browser_action                 get_file_browser_action() const {return browser_action;}
	void                                set_file_browser_action(file_browser_action _v) {browser_action=_v;}
	void                                set_session_filename(const std::string& _v) {session_filename=_v;}
	const std::string&                  get_session_filename() const {return session_filename;}
	bool                                is_session_open() const {return session_filename.size();}

	ldv::texture *                      get_texture() const {

		return texture.get();
	}

	container&                          get_sprites() {

		return table;
	}

	private:

	//!Background texture.
	std::unique_ptr<ldv::texture>       texture;

	//!Main table container: the ldtools::sprite table is read only.
	container table;

	//!Filename for the current container table.
	std::string                         session_filename;

	file_browser_action                 browser_action=file_browser_action::background;


};

}
