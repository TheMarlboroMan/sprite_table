#include "../../include/controller/main.h"

//local
#include "../../include/input/input.h"

#include <ldv/bitmap_representation.h>

using namespace controller;

main::main(lm::logger& plog, sprite_table::session_data& _sesd)
:
	log(plog),
	session_data(_sesd) {

}

void main::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal() || _input.is_input_down(input::escape)) {
		set_leave(true);
		return;
	}

	if(_input.is_input_down(input::help)) {

		set_state(state_help);
		return;
	}
	
	if(_input.is_input_down(input::enter)) {

		//TODO: We need some scaffolding here...
		/*
		file_request_type:setbackground

		then in the manager:
			set_new(false)

		when returning:
			if(ok) {
				switch(type) {
					do stuff
				}

		*/


		set_state(state_file_browser);
	}
}

void main::draw(ldv::screen& screen, int /*fps*/) {

	screen.clear(ldv::rgba8(0, 0, 0, 255));

	if(nullptr!=session_data.get_texture()) {

		const auto& tex=*(session_data.get_texture());

		ldv::bitmap_representation bgpic{
			tex,
			{0, 0, tex.get_w(), tex.get_h()},
			{0, 0, tex.get_w(), tex.get_h()}
		};

		bgpic.draw(screen);
	}
}
