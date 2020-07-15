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
	},
	mouse_pos{0,0} {

	set_message("welcome");
}

void main::loop(dfw::input& _input, const dfw::loop_iteration_data& lid) {

	mouse_pos=get_mouse_position(_input);

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

		const auto it=find_by_position(mouse_pos);
		if(it!=std::end(session_data.get_sprites())) {

			selected_index=it->first;
			set_message(std::string{"selected index "}+std::to_string(selected_index));
			return;
		}

		set_message(std::string{"selection cleared"});
		selected_index=-1;

		return;
	}

	if(_input.is_input_down(input::pagedown)) {

		select_next();
		return;
	}
	else if(_input.is_input_down(input::pageup)) {

		select_prev();
		return;
	}

	if(_input.is_input_down(input::del)) {

		delete_current();
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
	draw_hud(_screen);
}

void main::set_message(const std::string& _message) {

	last_message.message=_message;
	last_message.time=30.0f;

	//TODO: as crazy as it sounds, we should have a "fit to width" thing for the
	//strings: this shit is crazy as it is, with long lines. I am not sure of
	//how would it even work... I guess we could call "fit_to_width" and the 
	//text would be recomposed, a word at a time, calculating the current w
	//until it does not fit (unless it is a single, long word in the line).

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

void main::draw_hud(ldv::screen& _screen) {

	ldv::ttf_representation txt_hud{
		ttfman.get("consola-mono", 12),
		ldv::rgba8(255, 255, 255, 192),
		std::to_string(mouse_pos.x)+","+std::to_string(mouse_pos.y)
	};

	txt_hud.go_to({0,0});
	txt_hud.draw(_screen);

	//TODO: Draw current index, total items.
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

sprite_table::session_data::container::const_iterator main::find_by_position(ldt::point_2d<int> _pos) const {

	const auto& sprites=session_data.get_sprites();

	return std::find_if(
		std::begin(sprites),
		std::end(sprites),
		[_pos](const std::pair<size_t, ldtools::sprite_frame> _pair) {

//TODO: When zoomed in, something' s wrong.

			ldt::box<int, int> box{ 
				{_pair.second.x, _pair.second.y},
				_pair.second.w,
				_pair.second.h
			};

			return box.point_inside(_pos);
		}
	);
}

void main::delete_current() {

	if(-1==selected_index) {
		set_message("nothing selected, can't delete");
		return;
	}

	session_data.get_sprites().erase(selected_index);
	set_message(std::string{"deleted index "}+std::to_string(selected_index));
	selected_index=-1;
}

ldt::point_2d<int> main::get_mouse_position(dfw::input& _input) const {

	auto pos=_input().get_mouse_position();
	pos.x+=camera.get_x();
	pos.y+=camera.get_y();

	pos.x/=camera.get_zoom();
	pos.y/=camera.get_zoom();

	return {pos.x, pos.y};
}
