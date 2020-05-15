#include "../../include/sprite_table/session_data.h"

#include <ldv/image.h>

#include <filesystem>
#include <stdexcept>

using namespace sprite_table;

void session_data::set_texture_by_path(const std::string& _path) {

	if(!std::filesystem::exists(_path)) {

		throw std::runtime_error("file "+_path+" does not exist");
	}

	try {
		ldv::image img(_path);
		texture.reset(new ldv::texture(img));
	}
	catch(std::exception& e) {

		throw std::runtime_error(std::string{"could not set texture: "}+e.what());
	}
}