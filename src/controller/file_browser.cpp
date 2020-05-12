#include "../../include/controller/file_browser.h"

#include <tools/json.h>
#include <tools/file_utils.h>

#include <ldv/ttf_representation.h>

//local
#include "../../include/input/input.h"

#include <algorithm>

//TODO
#include <iostream>

using namespace controller;

file_browser::file_browser(
	lm::logger& plog,
	ldtools::ttf_manager& _ttfman,
	int _window_height
):
log(plog),
ttf_manager{_ttfman},
current_directory{std::filesystem::current_path()},
pager{0, 0} {

	//Mount layout...
	layout.map_font("default_font", ttf_manager.get("consola-mono", 12));

	auto root=tools::parse_json_string(
		tools::dump_file(
			"data/layouts.json"
		)
	);

	layout.parse(root["file_browser"]);
	first_selection_y=layout.get_int("first_selection_y");
	y_selection_factor=layout.get_int("y_selection_factor");

	//Calculate items per page based on window height...
	int excess_height=_window_height-first_selection_y;
	pager.set_items_per_page(excess_height / y_selection_factor);

	//Setup data...
	set_title("file browser");
	extract_entries();
	refresh_list_view();
	position_selector();
	compose_title();
}

void file_browser::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal() || _input.is_input_down(input::escape)) {

		result=false;
		choice={};
		//TODO: Nooo.
		set_leave(true);
		return;
	}

	bool check_change=false;

	if(_input.is_input_down(input::down)) {

		pager.cycle_item(decltype(pager)::dir::next);
		check_change=true;
	}
	else if(_input.is_input_down(input::up)) {

		pager.cycle_item(decltype(pager)::dir::previous);
		check_change=true;
	}
	if(_input.is_input_down(input::pagedown)) {

		pager.turn_page(decltype(pager)::dir::next);
		check_change=true;
	}
	else if(_input.is_input_down(input::pageup)) {

		pager.turn_page(decltype(pager)::dir::previous);
		check_change=true;
	}
	else if(_input.is_input_down(input::enter)) {

		const auto item=contents[pager.get_current_index()];

		if(item.is_dir()) {

			current_directory/={item.path_name};
			current_directory=current_directory.lexically_normal();

			extract_entries();
			refresh_list_view();
			position_selector();
			compose_title();
		}
		else {

			result=false;
			choice=current_directory.string();
			//TODO: Nooo.
			set_leave(true);
			return;
		}
	}

	if(check_change) {

		if(pager.is_item_cycled()) {
			position_selector();
		}

		if(pager.is_page_turned()) {
			refresh_list_view();
			compose_title();
		}
	}
}

void file_browser::draw(ldv::screen& screen, int /*fps*/) {


	screen.clear(ldv::rgba8(0, 0, 0, 255));
	layout.draw(screen);
}

void file_browser::extract_entries() {

	contents.clear();

	if(current_directory!=current_directory.parent_path()) {
		contents.push_back({
			"..",
			entry::entry_type::dir
		});
	}

	for(const auto& dir_entry : std::filesystem::directory_iterator(current_directory)) {

		const auto& path=dir_entry.path();
		std::string filename=path.filename();

		if(std::filesystem::is_directory(path)) {

			contents.push_back({
				filename,
				entry::entry_type::dir
			});
		}
		else if(std::filesystem::is_regular_file(path)){

			contents.push_back({
				filename,
				entry::entry_type::file
			});
		}
	}

	std::sort(
		std::begin(contents),
		std::end(contents)
	);

	pager.set_item_count(contents.size());
	pager.reset();
}

void file_browser::refresh_list_view() {

	std::string files;
	auto it=std::begin(contents)+(pager.get_current_page() * pager.get_items_per_page());
	auto begin=it;

	//Read the next N items...
	for(; it!=std::end(contents) && std::distance(begin, it) < pager.get_items_per_page(); ++it) {

		files+=it->is_dir()
			? "["+it->path_name+"]\n"
			: it->path_name+"\n";
	}

	static_cast<ldv::ttf_representation *>(
		layout.get_by_id("list")
	)->set_text(files);
}

void file_browser::position_selector() {

	int y_pos=first_selection_y+(pager.get_relative_index()*y_selection_factor);

	static_cast<ldv::ttf_representation *>(
		layout.get_by_id("selection")
	)->go_to({0, y_pos});
}

void file_browser::compose_title() {

	std::string final_title{title};

	final_title+=" : "
		+current_directory.string()
		+" ["
		+std::to_string(pager.get_current_page()+1)
		+"/"
		+std::to_string(pager.get_pages_count())
		+"] at "
		+std::to_string(pager.get_items_per_page());

	static_cast<ldv::ttf_representation *>(
		layout.get_by_id("title")
	)->set_text(final_title);
}
