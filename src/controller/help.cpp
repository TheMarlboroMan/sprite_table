#include "../../include/controller/help.h"

//local
#include "../../include/input/input.h"

using namespace controller;

help::help(lm::logger& plog)
	:log(plog) {

}

void help::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

	if(_input.is_input_down(input::escape)) {

		pop_state();
		return;
	}

}

void help::draw(ldv::screen& screen, int /*fps*/) {

	screen.clear(ldv::rgba8(64, 64, 64, 255));
}
