#pragma once

#include "../../include/env/env.h"

#include <dfw/base_config.h>
#include <dfw/input.h>

#include <iostream>

namespace dfwimpl {

class config:
	public dfw::base_config {
	////////////////////////////////
	// Public interface.

	public:

	config(const env::env_interface&);

	//Application specifics.
	int                 get_default_sprite_w() const {return token_from_path("app:default_sprite_w").GetInt();}
	int                 get_default_sprite_h() const {return token_from_path("app:default_sprite_h").GetInt();}
	int                 get_movement_factor() const {return token_from_path("app:movement_factor").GetInt();}
	int                 get_bg_color_r() const {return token_from_path("app:bg_color_r").GetInt();}
	int                 get_bg_color_g() const {return token_from_path("app:bg_color_g").GetInt();}
	int                 get_bg_color_b() const {return token_from_path("app:bg_color_b").GetInt();}

	//Fullfillment of the kernel interface.
	virtual std::string generate_file_version() const {return "1";}
	virtual std::string get_key_file_version() const {return "meta:v";}
	virtual std::string get_key_screen_double_buffer() const {return "video:double_buffer";}
	virtual std::string get_key_screen_vsync() const {return "video:vsync";}
	virtual std::string get_key_sound_volume() const {return "audio:sound_volume";}
	virtual std::string get_key_music_volume() const {return "audio:music_volume";}
	virtual std::string get_key_audio_ratio() const {return "audio:audio_ratio";}
	virtual std::string get_key_audio_buffers() const {return "audio:audio_buffers";}
	virtual std::string get_key_audio_out() const {return "audio:audio_out";}
	virtual std::string get_key_audio_channels() const {return "audio:audio_channels";}

	///////////////////////////////////
	// Properties.

	private:

	const env::env_interface& env;

	std::string get_file_path() const {return env.build_user_path("config.json");}
	//Hacky constructor version, because the base constructor will need the
	//result of this call.
	std::string get_file_path(const env::env_interface& _env) const {return _env.build_user_path("config.json");}

};
}
