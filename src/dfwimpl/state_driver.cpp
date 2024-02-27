#include "../../include/dfwimpl/state_driver.h"
#include "../../include/input/input.h"
#include "../../include/controller/states.h"

#include <lm/log.h>
#include <tools/string_utils.h>

#include <algorithm>
#include <sstream>
#include <string>

using namespace dfwimpl;

state_driver::state_driver(
	dfw::kernel& kernel,
	dfwimpl::config& c,
	const env::env_interface& _env
):
	state_driver_interface(controller::t_states::state_main),
	config(c),
	log(kernel.get_log()),
	env{_env},
	background_color{0,0,0,0}
{

	lm::log(log).info()<<"setting state check function..."<<std::endl;

	states.set_function([](int v){
		return v > controller::state_min && v < controller::state_max;
	});

	lm::log(log).info()<<"ready configuration file..."<<std::endl;
	ready_config();

	lm::log(log).info()<<"init state driver building: preparing video..."<<std::endl;
	prepare_video(kernel);

	lm::log(log).info()<<"preparing audio..."<<std::endl;
	prepare_audio(kernel);

	lm::log(log).info()<<"preparing input..."<<std::endl;
	prepare_input(kernel);

	lm::log(log).info()<<"preparing resources..."<<std::endl;
	prepare_resources(kernel);

	lm::log(log).info()<<"registering controllers..."<<std::endl;
	register_controllers(kernel);

	process_parameters(kernel.get_arg_manager());

	lm::log(log).info()<<"virtualizing input..."<<std::endl;
	virtualize_input(kernel.get_input());

	lm::log(log).info()<<"state driver fully constructed"<<std::endl;
}

void state_driver::init(dfw::kernel&) {}

void state_driver::prepare_video(dfw::kernel& kernel) {

	std::stringstream ss;
	ss<<config.string_from_path("video:window_title")<<" v"<<MAJOR_VERSION<<"."<<MINOR_VERSION<<"."<<PATCH_VERSION<<"-"<<BUILD_VERSION;

	kernel.init_video_system({
		config.int_from_path("video:window_w_px"),
		config.int_from_path("video:window_h_px"),
		config.int_from_path("video:window_w_logical"),
		config.int_from_path("video:window_h_logical"),
		ss.str(),
		config.bool_from_path("video:window_show_cursor"),
		config.get_screen_vsync()
	});

	auto& screen=kernel.get_screen();
	screen.set_fullscreen(config.bool_from_path("video:fullscreen"));

	ttf_manager.insert("consola-mono", 12, env.build_data_path("ttf/consola-mono.ttf"));
	ttf_manager.insert("consola-mono", 14, env.build_data_path("ttf/consola-mono.ttf"));
}

void state_driver::prepare_audio(dfw::kernel& kernel) {

	kernel.init_audio_system({
		config.get_audio_ratio(),
		config.get_audio_out(),
		config.get_audio_buffers(),
		config.get_audio_channels(),
		config.get_audio_volume(),
		config.get_music_volume()
	});
}

void state_driver::prepare_input(dfw::kernel& kernel) {

	using namespace dfw;

	std::vector<input_pair> pairs{
		{{input_description::types::keyboard, SDL_SCANCODE_ESCAPE, 0}, input::escape},
		{input_description_from_config_token(config.token_from_path("input:left")), input::left},
		{input_description_from_config_token(config.token_from_path("input:right")), input::right},
		{input_description_from_config_token(config.token_from_path("input:up")), input::up},
		{input_description_from_config_token(config.token_from_path("input:down")), input::down},
		{input_description_from_config_token(config.token_from_path("input:enter")), input::enter},
		{input_description_from_config_token(config.token_from_path("input:space")), input::space},
		{input_description_from_config_token(config.token_from_path("input:left_control")), input::left_control},
		{input_description_from_config_token(config.token_from_path("input:left_shift")), input::left_shift},
		{input_description_from_config_token(config.token_from_path("input:pageup")), input::pageup},
		{input_description_from_config_token(config.token_from_path("input:pagedown")), input::pagedown},
		{input_description_from_config_token(config.token_from_path("input:help")), input::help},
		{input_description_from_config_token(config.token_from_path("input:mode_presentation")), input::mode_presentation},
		{input_description_from_config_token(config.token_from_path("input:background_selection")), input::background_selection},
		{input_description_from_config_token(config.token_from_path("input:save")), input::save},
		{input_description_from_config_token(config.token_from_path("input:load")), input::load},
		{input_description_from_config_token(config.token_from_path("input:zoom_in")), input::zoom_in},
		{input_description_from_config_token(config.token_from_path("input:zoom_out")), input::zoom_out},
		{input_description_from_config_token(config.token_from_path("input:left_click")), input::left_click},
		{input_description_from_config_token(config.token_from_path("input:del")), input::del},
		{input_description_from_config_token(config.token_from_path("input:insert")), input::insert},
		{input_description_from_config_token(config.token_from_path("input:resize")), input::resize},
		{input_description_from_config_token(config.token_from_path("input:align")), input::align},
		{input_description_from_config_token(config.token_from_path("input:tab")), input::tab},
		{input_description_from_config_token(config.token_from_path("input:toggle_ids")), input::toggle_ids},
		{input_description_from_config_token(config.token_from_path("input:center_camera")), input::center_camera}
	};

	kernel.init_input_system(pairs);
}

void state_driver::prepare_resources(dfw::kernel& /*kernel*/) {

/*
	dfw::resource_loader r_loader(kernel.get_video_resource_manager(), kernel.get_audio_resource_manager());

	r_loader.generate_textures(tools::explode_lines_from_file(std::string("data/resources/textures.txt")));
	r_loader.generate_sounds(tools::explode_lines_from_file(std::string("data/resources/audio.txt")));
	r_loader.generate_music(tools::explode_lines_from_file(std::string("data/resources/music.txt")));
*/
}

void state_driver::register_controllers(dfw::kernel& /*kernel*/) {

	auto reg=[this](ptr_controller& _ptr, int _i, dfw::controller_interface * _ci) {
		_ptr.reset(_ci);
		register_controller(_i, *_ptr);
	};

	unsigned int    screen_w=config.int_from_path("video:window_w_logical"),
					screen_h=config.int_from_path("video:window_h_logical");

	reg(
		c_file_browser,
		controller::t_states::state_file_browser,
		new controller::file_browser(
			log,
			ttf_manager,
			env,
			screen_h
		)
	);

	reg(
		c_main,
		controller::t_states::state_main,
		new controller::main(
			log,
			ttf_manager,
			session_data,
			screen_w,
			screen_h,
			config.get_default_sprite_w(),
			config.get_default_sprite_h(),
			config.get_movement_factor(),
			background_color
		)
	);

	reg(
		c_help,
		controller::t_states::state_help,
		new controller::help(
			log,
			ttf_manager,
			env,
			screen_w,
			screen_h
		)
	);
	reg(
		c_presentation,
		controller::t_states::state_presentation,
		new controller::presentation(
			log,
			ttf_manager,
			session_data,
			screen_w,
			screen_h,
			background_color
		)
	);
	//[new-controller-mark]

}

void state_driver::prepare_state(int _next, int _current) {

	auto& fbrowser=*(static_cast<controller::file_browser*>(c_file_browser.get()));
//	auto& help=*(static_cast<controller::help*>(c_help.get()));
	auto& main=*(static_cast<controller::main*>(c_main.get()));

	switch(_next) {
		case controller::t_states::state_file_browser:

			switch(_current) {
				case controller::t_states::state_main:
					fbrowser.set_allow_create(session_data.get_file_browser_action()==decltype(session_data)::file_browser_action::save);
				break;
			}
		break;
	}


	if(_current==controller::t_states::state_file_browser) {

		switch(_next) {

			case controller::t_states::state_main:

				if(fbrowser.get_result()) {

					try {
						switch(session_data.get_file_browser_action()) {

							case decltype(session_data)::file_browser_action::save:
								session_data.set_session_filename(fbrowser.get_choice());
								main.save();
							break;
							case decltype(session_data)::file_browser_action::background:
								session_data.set_texture_by_path(fbrowser.get_choice());
								main.set_message(std::string{"loaded background "}+fbrowser.get_choice());
								main.adjust_camera_limits();
							break;
							case decltype(session_data)::file_browser_action::load:
								session_data.load_sprites_by_path(fbrowser.get_choice());
								session_data.set_session_filename(fbrowser.get_choice());
								main.set_message(std::string{"loaded "}+std::to_string(session_data.get_sprites().size())+ " entries from "+fbrowser.get_choice());
								//main.select_first_index();
								main.clear_selection();
							break;
						}
					}
					catch(std::exception& e) {

						lm::log(log).warning()<<e.what()<<std::endl;
						main.set_message(std::string{"error : "}+e.what());
					}

				}

			break;
		}
	}
}

void state_driver::common_pre_loop_input(dfw::input& input, float /*delta*/) {

	if(input().is_event_joystick_connected()) {
		lm::log(log).info()<<"New joystick detected..."<<std::endl;
		virtualize_input(input);
	}
}

void state_driver::common_loop_input(dfw::input& /*input*/, float /*delta*/) {

}

void state_driver::common_pre_loop_step(float /*delta*/) {

}

void state_driver::common_loop_step(float /*delta*/) {

}

void state_driver::virtualize_input(dfw::input& input) {

	lm::log(log).info()<<"trying to virtualize "<<input().get_joysticks_size()<<" controllers..."<<std::endl;

	for(size_t i=0; i < input().get_joysticks_size(); ++i) {
		input().virtualize_joystick_hats(i);
		input().virtualize_joystick_axis(i, 15000);
		lm::log(log).info()<<"Joystick virtualized "<<i<<std::endl;
	}
}

void state_driver::process_parameters(const tools::arg_manager& _argman) {

	auto& main=*(static_cast<controller::main*>(c_main.get()));

	if(_argman.exists("-f") && _argman.arg_follows("-f")) {

		const auto& session_file=_argman.get_following("-f");
		try {
			session_data.load_sprites_by_path(session_file);
			session_data.set_session_filename(session_file);
			main.set_message(std::string{"using session "}+session_file);
			//main.select_first_index();
			main.clear_selection();
		}
		catch(std::exception &e) {

			lm::log(log).warning()<<"could not load session "<<session_file<<" : "<<e.what()<<std::endl;
			main.set_message("failed to load command line session");
		}
	}

	if(_argman.exists("-i") && _argman.arg_follows("-i")) {

		const auto& image_file=_argman.get_following("-i");
		try {
			session_data.set_texture_by_path(image_file);
			main.set_message(std::string{"using background "}+image_file);
			main.adjust_camera_limits();
		}
		catch(std::exception &e) {

			lm::log(log).warning()<<"could not load background "<<image_file<<" : "<<e.what()<<std::endl;
			main.set_message("failed to load command line background");
		}
	}
}

void state_driver::ready_config() {

	lm::log(log).info()<<"checking configuration file"<<std::endl;
	//check if the configuration contains the background color keys, add
	//default values if not.
	std::vector<std::string> keys={"bg_color_r", "bg_color_g", "bg_color_b"};
	bool added=false;
	for(const auto& key : keys) {

		std::string fullkey="app:"+key;
		lm::log(log).info()<<"checking configuration key "<<fullkey<<std::endl;
		
		if(!config.has_path(fullkey)) {
	
			config.add(fullkey, 0);
			lm::log(log).info()<<"adding missing configuration key "<<fullkey<<std::endl;
			added=true;
		}
	}

	//if not present, leave log line and add it with a default value.
	if(added) {
		
		lm::log(log).info()<<"saving configuration file"<<std::endl;
		config.save();
		config.reload();
	}

	background_color=ldv::rgba8(
		config.int_from_path("app:bg_color_r"),
		config.int_from_path("app:bg_color_g"),
		config.int_from_path("app:bg_color_b"),
		0
	);
}

