#include "../../include/controller/file_browser.h"

#include <tools/json.h>
#include <tools/file_utils.h>

#include <ldv/ttf_representation.h>

//local
#include "../../include/input/input.h"

//TODO
#include <iostream>

using namespace controller;

file_browser::file_browser(
	lm::logger& plog,
	ldtools::ttf_manager& _ttfman
):
log(plog),
ttf_manager{_ttfman},
current_directory{std::filesystem::current_path()},
selected_index{0} {

	//Mount layout...
	layout.map_font("default_font", ttf_manager.get("consola-mono", 12));

	auto root=tools::parse_json_string(
		tools::dump_file(
			"data/layouts.json"
		)
	);

	first_selection_y=layout.get_int("first_selection_y");

	layout.parse(root["file_browser"]);

	//Setup data...
	set_title("file browser");
	extract_entries();
	refresh_list_view();
}

void file_browser::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal() || _input.is_input_down(input::escape)) {
		set_leave(true);
		return;
	}

	//TODO: input moves selection.
}

void file_browser::draw(ldv::screen& screen, int /*fps*/) {

	screen.clear(ldv::rgba8(0, 0, 0, 255));
	layout.draw(screen);
}

void file_browser::extract_entries() {

	contents.clear();

	if(current_directory!=current_directory.parent_path()) {
		contents.push_back({
			"[..]"
		});
	}

	for(const auto& dir_entry : std::filesystem::directory_iterator(current_directory)) {

		const auto& path=dir_entry.path();
		std::string filename=path.filename();

		if(std::filesystem::is_directory(path)) {

			contents.push_back({
				std::string{"["}+filename+"]"
			});
		}
		//TODO: just else??? Is there not a is_file()???
		else {

			contents.push_back({
				filename
			});
		}
	}
}

void file_browser::refresh_list_view() {

	std::string files;
	for(const auto& e : contents) {
		files+=e.display_name+"\n";
	}

	static_cast<ldv::ttf_representation *>(
		layout.get_by_id("list")
	)->set_text(files);

	//TODO: This will go somewhere else...
	static_cast<ldv::ttf_representation *>(
		layout.get_by_id("selection")
	)->go_to({0, first_selection_y});
}

void file_browser::set_title(const std::string& _title) {

	std::string final_title{_title};
	final_title+=" : "+current_directory.parent_path().string();

	static_cast<ldv::ttf_representation *>(
		layout.get_by_id("title")
	)->set_text(final_title);
}
