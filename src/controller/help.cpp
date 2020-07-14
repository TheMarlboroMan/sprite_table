#include "../../include/controller/help.h"

#include <tools/file_utils.h>

//local
#include "../../include/input/input.h"

using namespace controller;

help::help(
	lm::logger& plog, 
	ldtools::ttf_manager& _ttfman
):
	log(plog), 
	help_txt_rep{
		_ttfman.get("consola-mono", 12),
		ldv::rgba8(255, 255, 255, 255)
	}
{
	help_txt_rep.set_text(tools::dump_file("data/help.txt"));
	help_txt_rep.go_to({0,0});
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

void help::draw(ldv::screen& _screen, int /*fps*/) {

	_screen.clear(ldv::rgba8(0, 0, 0, 255));
	help_txt_rep.draw(_screen);
}
