#include "../../include/controller/file_browser.h"

#include <tools/json.h>
#include <tools/file_utils.h>

//local
#include "../../include/input/input.h"

using namespace controller;

file_browser::file_browser(
	lm::logger& plog,
	ldtools::ttf_manager& _ttfman
):
	log(plog),
	ttf_manager{_ttfman},
	current_directory{std::filesystem::current_path()}
	{

	//Mount layout...
	layout.map_font("default_font", ttf_manager.get("consola-mono", 12));

	auto root=tools::parse_json_string(
		tools::dump_file(
			"data/layouts.json"
		)
	);

	layout.parse(root["file_browser"]);

	//Setup data...
	extract_entries();
	refresh_list_view();
}

void file_browser::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal() || _input.is_input_down(input::escape)) {
		set_leave(true);
		return;
	}
}

void file_browser::draw(ldv::screen& screen, int /*fps*/) {

	screen.clear(ldv::rgba8(0, 0, 0, 255));
	layout.draw(screen);
}

void file_browser::extract_entries() {

}

void file_browser::refresh_list_view() {

}
