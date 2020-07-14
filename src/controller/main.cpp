#include "../../include/controller/main.h"

//local
#include "../../include/input/input.h"

#include <ldv/bitmap_representation.h>

using namespace controller;

main::main(
	lm::logger& plog,
	ldtools::ttf_manager& _ttfman,
	sprite_table::session_data& _sesd
):
	log{plog},
	ttfman{_ttfman},
	session_data(_sesd),
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
}

void main::draw(ldv::screen& _screen, int /*fps*/) {

	_screen.clear(ldv::rgba8(0, 0, 0, 255));

	if(nullptr!=session_data.get_texture()) {

		const auto& tex=*(session_data.get_texture());

		ldv::bitmap_representation bgpic{
			tex,
			{0, 0, tex.get_w(), tex.get_h()},
			{0, 0, tex.get_w(), tex.get_h()}
		};

		bgpic.draw(_screen);
	}

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


void main::set_message(const std::string& _message) {

	last_message.message=_message;
	last_message.time=30.0f;

	//TODO: as crazy as it sounds, we should have a "fit to rect" thing for the
	//strings: this shit is crazy as it is, with long lines.

	last_message_rep.set_text(last_message.message);
}

