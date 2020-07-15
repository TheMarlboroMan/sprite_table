#include "../../include/controller/main.h"

//local
#include "../../include/input/input.h"

#include <ldv/bitmap_representation.h>
#include <ldv/box_representation.h>
#include <ldv/line_representation.h>

#include <functional>

using namespace controller;

main::main(
	lm::logger& plog,
	ldtools::ttf_manager& _ttfman,
	sprite_table::session_data& _sesd,
	int _cw,
	int _ch
):
	log{plog},
	ttfman{_ttfman},
	session_data(_sesd),
	camera{
		{0,0, _cw, _ch}, //pointing at world 0,0.
		{0,0}           //at 0.0 in the screen...
	},
	last_message_rep{
		_ttfman.get("consola-mono", 12),
		ldv::rgba8(255, 255, 255, 255),
	} {

	set_message("welcome");
}

void main::loop(dfw::input& _input, const dfw::loop_iteration_data& lid) {

	if(last_message.time > 0.f) {

		last_message.time-=lid.delta;
	}

	if(_input().is_exit_signal() || _input.is_input_down(input::escape)) {
		set_leave(true);
		return;
	}

	if(_input.is_input_down(input::help)) {

		push_state(state_help);
		return;
	}

	if(_input.is_input_down(input::background_selection)) {

		session_data.set_file_browser_action(sprite_table::session_data::file_browser_action::background);
		push_state(state_file_browser);
		return;
	}

	if(_input.is_input_down(input::load)) {

		session_data.set_file_browser_action(sprite_table::session_data::file_browser_action::load);
		push_state(state_file_browser);
		return;
	}

	if(_input.is_input_down(input::zoom_in)) {

		zoom_in();
	}
	else if(_input.is_input_down(input::zoom_out)) {

		zoom_out();
	}

	const int movement_factor=_input.is_input_pressed(input::left_control)
		? 1
		: 10;

    typedef  bool (dfw::input::*input_fn)(int) const;
	input_fn movement_fn=_input.is_input_pressed(input::left_control)
		? &dfw::input::is_input_down
		: &dfw::input::is_input_pressed;

	/*(_input.*camera_movement_fn)(input::up);*/

	//TODO: actually, the camera is only in "camera mode"

	if(std::invoke(movement_fn, _input, input::up)) {

		camera.move_by(0,-1*movement_factor);
	}
	if(std::invoke(movement_fn, _input, input::down)) {

		camera.move_by(0, 1*movement_factor);
	}

	if(std::invoke(movement_fn, _input, input::left)) {

		camera.move_by(-1*movement_factor, 0);
	}
	if(std::invoke(movement_fn, _input, input::right)) {

		camera.move_by(1*movement_factor, 0);
	}
}

void main::draw(ldv::screen& _screen, int /*fps*/) {

	_screen.clear(ldv::rgba8(0, 0, 0, 255));

	draw_background(_screen);
	draw_sprites(_screen);
	draw_messages(_screen);
}

void main::set_message(const std::string& _message) {

	last_message.message=_message;
	last_message.time=30.0f;

	//TODO: as crazy as it sounds, we should have a "fit to rect" thing for the
	//strings: this shit is crazy as it is, with long lines.

	last_message_rep.set_text(last_message.message);
}


void main::draw_background(ldv::screen& _screen) {

	if(nullptr!=session_data.get_texture()) {

		const auto& tex=*(session_data.get_texture());

		ldv::bitmap_representation bgpic{
			tex,
			{0, 0, tex.get_w(), tex.get_h()},
			{0, 0, tex.get_w(), tex.get_h()}
		};

		bgpic.draw(_screen, camera);
	}
}

void main::draw_sprites(ldv::screen& _screen) {

	for(const auto pair : session_data.get_sprites()) {

		const auto& sprite=pair.second;
		const auto color=ldv::rgba8(0, 255, 0, 255);

		//Draw the block...
		ldv::box_representation box{
			{{sprite.x, sprite.y}, sprite.w, sprite.h},
			color,
			ldv::polygon_representation::type::line
		};

		box.draw(_screen, camera);

		//Add the axes... first the horizontal one...
		auto hor_axis=ldv::line_representation{
			{sprite.x, sprite.y},
			{sprite.x+(sprite.w / 2), sprite.y},
			ldv::rgba8(255,0,0, 255)
		};
		hor_axis.draw(_screen, camera);

		//Then the vertical.
		auto ver_axis=ldv::line_representation{
			{sprite.x, sprite.y},
			{sprite.x, sprite.y+(sprite.h/2)},
			ldv::rgba8(0,0, 255, 255)
		};
		ver_axis.draw(_screen, camera);

		//And the id...
		ldv::ttf_representation id_rep{
			ttfman.get("consola-mono", 14),
			color,
			std::to_string(pair.first)
		};

		id_rep.align(
			box,
			ldv::representation_alignment{
				ldv::representation_alignment::h::center,
				ldv::representation_alignment::v::center
			}
		);
		id_rep.draw(_screen, camera);
	}
}

void main::draw_messages(ldv::screen& _screen) {

	if(last_message.time > 0.f) {

		last_message_rep.align(
		_screen.get_rect(),
			ldv::representation_alignment{
				ldv::representation_alignment::h::center,
				ldv::representation_alignment::v::inner_bottom
			}
		);

		last_message_rep.draw(_screen);
	}
}

void main::zoom_in() {

	auto zoom=camera.get_zoom();

	if(zoom > 0.1) {

		camera.set_zoom(zoom/2.);
	}
}

void main::zoom_out() {

	auto zoom=camera.get_zoom();

	if(zoom < 4) {

		camera.set_zoom(zoom*2.);
	}
}

void main::adjust_camera_limits() {

	if(nullptr!=session_data.get_texture()) {

		const auto& tex=*(session_data.get_texture());
		camera.set_limits({ {0,0}, tex.get_w(), tex.get_h()});
	}
}