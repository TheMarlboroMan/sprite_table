#include "../../include/controller/help.h"

#include <tools/file_utils.h>

//local
#include "../../include/input/input.h"

using namespace controller;

help::help(
	lm::logger& plog,
	ldtools::ttf_manager& _ttfman,
	const env::env_interface& _env,
	unsigned int _w,
	unsigned int _h
):
	log(plog),
	camera{
		{0,0, _w, _h},
		{0,0}
	},
	help_txt_rep{
		_ttfman.get("consola-mono", 12),
		ldv::rgba8(255, 255, 255, 255)
	}
{
	help_txt_rep.set_text(tools::dump_file(_env.build_data_path("help.txt")));
	help_txt_rep.go_to({0,0});
	camera.set_limits(help_txt_rep.get_view_position());

	//TODO :A library enhancemente, the limits could be set
	//vertically, horizontally or both!.
	camera.go_to({0,0});
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

	if(_input.is_input_pressed(input::down)) {

		camera.move_by(0, 5);
	}
	else if(_input.is_input_pressed(input::up)) {

		camera.move_by(0, -5);
	}
}

void help::draw(ldv::screen& _screen, int /*fps*/) {

	_screen.clear(ldv::rgba8(0, 0, 0, 255));
	help_txt_rep.draw(_screen, camera);
}
