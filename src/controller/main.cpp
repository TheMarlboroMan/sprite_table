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
		return;
	}
	else if(_input.is_input_down(input::zoom_out)) {

		zoom_out();
		return;
	}

	if(_input.is_input_down(input::left_click)) {

		//Of course, the camera position also has something to say...
		auto pos=_input().get_mouse_position();
		pos.x+=camera.get_x();
		pos.y+=camera.get_y();

		//...and so does the camera zoom...
		pos.x/=camera.get_zoom();
		pos.y/=camera.get_zoom();

		//TODO: Now search for a box with the given position!!!
		//TODO: if a box exist, the current selection index should go to its
		//index, else should go to -1.
		//TODO: The interesting thing here should be tu return an iterator....
		//set_message(std::string{"LEFT CLICK!!! ON "}+std::to_string(pos.x)+","+std::to_string(pos.y));
		return;
	}

	if(_input.is_input_down(input::pagedown)) {

		select_next();
	}
	else if(_input.is_input_down(input::pageup)) {

		select_prev();
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
		const auto color=pair.first==selected_index
			? ldv::rgba8(0, 0, 255, 32)
			: ldv::rgba8(0, 255, 0, 32);

		//Draw the block...
		ldv::box_representation box{
			{{sprite.x, sprite.y}, sprite.w, sprite.h},
			color,
			ldv::polygon_representation::type::fill
		};
		box.set_blend(ldv::representation::blends::alpha);
		box.draw(_screen, camera);

		//Draw a solid border...
		box.set_filltype(ldv::polygon_representation::type::line);
		box.set_color(ldv::rgba8(255, 255, 0, 192));
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
			ldv::rgba8(255, 255, 255, 192),
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

void main::select_first_index() {

	const auto& sprites=session_data.get_sprites();
	if(std::empty(sprites)) {

		selected_index=-1;
		return;
	}

	selected_index=std::begin(sprites)->first;
}

void main::select_prev() {

	const auto& sprites=session_data.get_sprites();
	if(std::empty(sprites)) {

		selected_index=-1;
		return;
	}

	const auto current=sprites.find(selected_index);
	if(current==std::begin(sprites)) {
		//turns out that the previous to the last is... the first.

		return;
	}

	selected_index=std::prev(current)->first;
	set_message(std::string{"selected (previous) index "}+std::to_string(selected_index));
}

void main::select_next() {

	const auto& sprites=session_data.get_sprites();
	if(std::empty(sprites)) {

		selected_index=-1;
		return;
	}

	const auto current=sprites.find(selected_index);
	selected_index=std::next(current)->first; //the next of the last seems to be the last.
	set_message(std::string{"selected (next) index "}+std::to_string(selected_index));
}

sprite_table::session_data::container::const_iterator main::find_by_position(int _x, int _y) const {

	const auto& sprites=session_data.get_sprites();
	return std::find_if(
		std::begin(sprites),
		std::end(sprites),
		[_x, _y](const std::pair<size_t, ldtools::sprite_frame> _pair) {

			//TODO: make a rect.
			//TODO: use our own calculations.

			return false;
		}
	);
}