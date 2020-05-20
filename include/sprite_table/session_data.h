#pragma once

#include <ldtools/sprite_table.h>
#include <ldv/texture.h>
#include <string>
#include <memory>

namespace sprite_table {

class session_data {

	public:

	void                                set_texture_by_path(const std::string&);
	void                                load_sprites_by_path(const std::string&);

	ldv::texture *                      get_texture() const {

		return texture.get();
	}

	ldtools::sprite_table&              get_sprites() {

		return table;
	}

	private:

	//!Background texture.
	std::unique_ptr<ldv::texture>       texture;

	//!Main table container.
	ldtools::sprite_table               table;

};

}
