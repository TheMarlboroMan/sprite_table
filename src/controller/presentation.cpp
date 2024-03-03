#include "../../include/controller/presentation.h"

#include <ldv/bitmap_representation.h>
#include <ldv/ttf_representation.h>

#include <algorithm>

//local
#include "../../include/input/input.h"

using namespace controller;

presentation::presentation(
	lm::logger& plog,
	ldtools::ttf_manager& _ttfman,
	sprite_table::session_data& _session_data,
	unsigned int _screen_w,
	unsigned int _screen_h,
	ldv::rgba_color& _background_color
):
	log(plog),
	ttfman{_ttfman},
	session_data{_session_data},
	background_color{_background_color},
	camera{
		{0,0, _screen_w, _screen_h},
		{0,0}
	},
	screen_w{_screen_w},
	screen_h{_screen_h}
{

}

void presentation::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	bool lshift=_input.is_input_pressed(input::left_shift);

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

	if(_input.is_input_down(input::escape)) {

		set_state(state_main);
		return;
	}

	if(_input.is_input_down(input::space)) {

		display_ids=!display_ids;
		return;
	}

	if(lshift && _input.is_input_down(input::flip)) {

		cycle_font_size();
	}

	if(_input.is_input_pressed(input::left)) {

		camera.move_by(-5, 0);
	}
	else if(_input.is_input_pressed(input::right)) {

		camera.move_by(5, 0);
	}

	if(_input.is_input_pressed(input::down)) {

		camera.move_by(0, 5);
	}
	else if(_input.is_input_pressed(input::up)) {

		camera.move_by(0, -5);
	}

	if(_input.is_input_down(input::zoom_in)) {

		zoom_in();
		return;
	}
	else if(_input.is_input_down(input::zoom_out)) {

		zoom_out();
		return;
	}
}

void presentation::awake(dfw::input& /*input*/) {

	calculate();
}

void presentation::draw(ldv::screen& _screen, int /*fps*/) {

	_screen.clear(background_color);

	for(const auto& item : items) {

		draw_item(_screen, item);
	}
}

void presentation::draw_item(ldv::screen& _screen, const presentation_item& _item) {

	const auto& tex=*(session_data.get_texture());

	ldv::bitmap_representation bmp{
		tex,
		{_item.x, _item.y, _item.frame.box.w+2, _item.frame.box.h+2},
		_item.frame.box
	};

	bmp.set_invert_horizontal(_item.frame.flags & 1);
	bmp.set_invert_vertical(_item.frame.flags & 2);

	bmp.draw(_screen, camera);

	if(display_ids) {

		ldv::ttf_representation id_rep{
			ttfman.get("consola-mono", font_size),
			ldv::rgba8(255, 255, 255, 192),
			std::to_string(_item.index)
		};

		id_rep.align(
			bmp,
			ldv::representation_alignment{
				ldv::representation_alignment::h::center,
				ldv::representation_alignment::v::outer_bottom,
				0,
				4
			}
		);
		id_rep.draw(_screen, camera);
	}
}

void presentation::calculate() {

	int x=0,
		y=0,
		max_y=0;

	const int   margin_x=4, //Horizontal margin between sprites.
				margin_y=32; //Vertical margin between sprites.

	for(const auto& pair : session_data.get_sprites()) {

		const auto& sprite{pair.second};
		int bottom_y=y+sprite.box.h;
		max_y=std::max(bottom_y, max_y);

		if(x+sprite.box.w > screen_w) {

			x=0;
			y=max_y+margin_y;
			max_y=y;
		}

		items.push_back({
			pair.first,
			sprite,
			x,
			y
		});

		x+=sprite.box.w+margin_x;
	}

	camera.set_limits({
		{0,0},
		screen_w,
		static_cast<unsigned int>(max_y)
	});

	camera.go_to({0,0});
}

void presentation::zoom_out() {

	auto zoom=camera.get_zoom();

	if(zoom > 0.1) {

		camera.set_zoom(zoom/2.);
	}
}

void presentation::zoom_in() {

	auto zoom=camera.get_zoom();

	if(zoom < 8) {

		camera.set_zoom(zoom*2.);
	}
}

void presentation::cycle_font_size() {

	switch(font_size) {

		case 8:
			font_size=10;
			return;
		case 10:
			font_size=12;
			return;
		case 12:
			font_size=14;
			return;
		case 14:
			font_size=8;
			return;
	}
}
