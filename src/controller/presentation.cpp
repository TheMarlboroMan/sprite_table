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
	unsigned int _screen_h
):
	log(plog),
	ttfman{_ttfman},
	session_data{_session_data},
	camera{
		{0,0, _screen_w, _screen_h},
		{0,0}
	},
	screen_w{_screen_w},
	screen_h{_screen_h}
{

}

void presentation::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

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

	if(_input.is_input_pressed(input::down)) {

		camera.move_by(0, 5);
	}
	else if(_input.is_input_pressed(input::up)) {

		camera.move_by(0, -5);
	}
}

void presentation::awake(dfw::input& /*input*/) {

	calculate();
}

void presentation::draw(ldv::screen& _screen, int /*fps*/) {

	_screen.clear(ldv::rgba8(0, 0, 0, 255));

	for(const auto& item : items) {

		draw_item(_screen, item);
	}
}

void presentation::draw_item(ldv::screen& _screen, const presentation_item& _item) {

	const auto& tex=*(session_data.get_texture());

	ldv::bitmap_representation bmp{
		tex,
		{_item.x, _item.y, _item.frame.w, _item.frame.h},
		{_item.frame.x, _item.frame.y, _item.frame.w, _item.frame.h}
	};

	bmp.draw(_screen, camera);

	if(display_ids) {

		ldv::ttf_representation id_rep{
			ttfman.get("consola-mono", 14),
			ldv::rgba8(255, 255, 255, 192),
			std::to_string(_item.index)
		};

		id_rep.align(
			bmp,
			ldv::representation_alignment{
				ldv::representation_alignment::h::inner_left,
				ldv::representation_alignment::v::inner_top,
				2,
				2
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
				margin_y=4; //Vertical margin between sprites.

	for(const auto& pair : session_data.get_sprites()) {

		const auto& sprite{pair.second};
		int bottom_y=y+sprite.h;
		max_y=std::max(bottom_y, max_y);

		if(x+sprite.w > screen_w) {

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

		x+=sprite.w+margin_x;
	}

	camera.set_limits({
		{0,0},
		screen_w,
		static_cast<unsigned int>(max_y)
	});

	camera.go_to({0,0});
}