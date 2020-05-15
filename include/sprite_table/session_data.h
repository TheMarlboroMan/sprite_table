#pragma once

#include <ldv/texture.h>
#include <string>
#include <memory>

namespace sprite_table {

class session_data {

	public:

	void                                set_texture_by_path(const std::string&);

	ldv::texture *                      get_texture() const {

		return texture.get();
	}

	private:

	std::unique_ptr<ldv::texture>       texture;

};

}