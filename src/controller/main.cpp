#include "../../include/controller/main.h"

//local
#include "../../include/input/input.h"
#include "../../include/sprite_table/console_interpreter.h"

#include <ldv/bitmap_representation.h>
#include <ldv/box_representation.h>
#include <ldv/line_representation.h>
#include <lm/log.h>

#include <functional>

using namespace controller;

main::main(
	lm::logger& _log,
	ldtools::ttf_manager& _ttfman,
	sprite_table::session_data& _sesd,
	unsigned int _cw,
	unsigned int _ch,
	unsigned int _dsw,
	unsigned int _dsh,
	unsigned int _mf,
	ldv::rgba_color& _background_color
):
	log{_log},
	ttfman{_ttfman},
	session_data(_sesd),
	background_color{_background_color},
	camera{
		{0,0, _cw, _ch}, //pointing at world 0,0.
		{0,0}           //at 0.0 in the screen...
	},
	last_message_rep{
		_ttfman.get("consola-mono", 12),
		ldv::rgba8(255, 255, 255, 255),
	},
	mouse_pos{0,0},
	default_w{_dsw},
	default_h{_dsh},
	movement_factor{_mf}
{

	last_message_rep.set_max_width(_cw);
	set_message("welcome, f1 for help, esc to exit");
}

void main::loop(dfw::input& _input, const dfw::loop_iteration_data& lid) {

	mouse_pos=get_mouse_position(_input);

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

	if(!console_mode) {

		if(_input.is_input_down(input::mode_presentation)) {

			set_state(state_presentation);
			return;
		}

		if(_input.is_input_down(input::center_camera)) {

			center_camera();
		}
	}

	if(_input.is_input_down(input::tab)) {

		console_mode=true;
		_input().set_keydown_control_text_filter(true);
		_input().start_text_input();
	}

	console_mode
		? console_input(_input)
		: workspace_input(_input);
}

void main::workspace_input(dfw::input& _input) {

	bool lctrl=_input.is_input_pressed(input::left_control);
	bool lshift=_input.is_input_pressed(input::left_shift);

	if(_input.is_input_down(input::save)) {

		//These two cases have to be handled separatedly.
		if(lctrl) {

			session_data.set_file_browser_action(sprite_table::session_data::file_browser_action::save);
			//Unfortunately, there's no way to jump now to the state and flow down to save below...
			//save must be called from the state_driver.
			//TODO: could there be a way of recursively calling the main loop, but let's leave it for now.
			push_state(state_file_browser);
		}
		else {
			save();
		}
	}

	if(_input.is_input_down(input::background_selection)) {

		session_data.set_file_browser_action(sprite_table::session_data::file_browser_action::background);
		push_state(state_file_browser);
		return;
	}


	if(_input.is_input_down(input::toggle_ids)) {

		show_ids=!show_ids;
	}

	if(_input.is_input_down(input::load)) {

		session_data.set_file_browser_action(sprite_table::session_data::file_browser_action::load);
		push_state(state_file_browser);
		return;
	}

	if(_input.is_input_down(input::zoom_in)) {

		lctrl
			? select_next()
			: zoom_in();

		return; 
	}
	else if(_input.is_input_down(input::zoom_out)) {

		lctrl
			? select_prev()
			: zoom_out();
		return;
	}

	if(_input.is_input_down(input::space)) {

		clear_selection();
		return;
	}

	if(_input.is_input_down(input::insert)) {

		if(lctrl) {

			if(_input.is_input_pressed(input::up)) {

				duplicate_current(duplicate_pos::up);
			}
			else if(_input.is_input_pressed(input::down)) {

				duplicate_current(duplicate_pos::down);
			}
			else if(_input.is_input_pressed(input::left)) {

				duplicate_current(duplicate_pos::left);
			}
			else if(_input.is_input_pressed(input::right)) {

				//duplicate_current(duplicate_pos::right);
				duplicate_current(duplicate_pos::right);
			}
			else {

				//Pointless to duplicate in the same position, right???
				//duplicate_current(duplicate_pos::same);
				duplicate_current(duplicate_pos::right);
			}
		}
		else {

			create_sprite();
		}
		return;
	}

	if(_input.is_input_down(input::left_click)) {

		const auto it=find_by_position(mouse_pos);
		if(it!=std::end(session_data.get_sprites())) {

			selected_index=it->first;
			set_message(std::string{"selected index "}+std::to_string(selected_index));
			return;
		}

		set_message(std::string{"selection cleared"});
		selected_index=-1;

		return;
	}

	if(_input.is_input_down(input::pagedown) ) {

		select_next();
		return;
	}
	else if(_input.is_input_down(input::pageup)) {

		select_prev();
		return;
	}

	if(_input.is_input_down(input::del)) {

		delete_current();
	}

	if(_input.is_input_down(input::resize)) {

		rotate_frame(lctrl);
	}

	if(_input.is_input_down(input::flip)) {

		lshift
			? cycle_font_size()
			: flip_frame(lctrl);
	}

	if(!lctrl) {

		typedef  bool (dfw::input::*input_fn)(int) const;
		input_fn movement_fn=lshift
			? &dfw::input::is_input_down
			: &dfw::input::is_input_pressed;

		const int factor=lshift ? 1 : movement_factor;
		int movement_x=0,
			movement_y=0;

		if(std::invoke(movement_fn, _input, input::up)) {

			movement_y=-1*factor;
		}
		if(std::invoke(movement_fn, _input, input::down)) {

			movement_y=1*factor;
		}

		if(std::invoke(movement_fn, _input, input::left)) {

			movement_x=-1*factor;
		}
		if(std::invoke(movement_fn, _input, input::right)) {

			movement_x=1*factor;
		}

		if(movement_x || movement_y) {

			perform_movement(
				movement_x,
				movement_y,
				_input.is_input_pressed(input::resize),
				_input.is_input_pressed(input::align)
			);
			return;
		}
	}
}

void main::console_input(dfw::input& _input) {

	if(_input().is_event_text()) {

		console_txt=_input().get_text_input();
		return;
	}

	if(_input.is_input_down(input::enter)) {

		_input().clear_text_input();

		if(!console_txt.size()) {

			return;
		}

		if(console_txt=="exit") {

			_input().stop_text_input();
			console_mode=false;
			console_txt.clear();

			//Did we get rid of the current frame?
			if(!session_data.get_sprites().count(selected_index)) {

				selected_index=-1;
			}

			return;
		}

		sprite_table::console_interpreter ci{session_data.get_sprites(), default_w, default_h, background_color};
		ci.perform(console_txt);

		console_txt.clear();
		set_message(ci.get_message());
	}
}

void main::draw(ldv::screen& _screen, int /*fps*/) {

	_screen.clear(background_color);

	draw_background(_screen);
	draw_sprites(_screen);
	draw_messages(_screen);

	console_mode
		? draw_console(_screen)
		: draw_hud(_screen);
}

void main::set_message(const std::string& _message) {

	last_message.message=_message;
	last_message.time=30.0f;
	last_message_rep.set_text(last_message.message);
}

void main::draw_background(ldv::screen& _screen) {

	if(nullptr!=session_data.get_texture()) {

		const auto& tex=*(session_data.get_texture());

		ldv::bitmap_representation bgpic{
			tex,
			{0, 0, tex.get_w(), tex.get_h()},
			{0, 0, tex.get_w(), tex.get_h()}
		};

		bgpic.draw(_screen, camera);
	}
}

void main::draw_sprites(ldv::screen& _screen) {

	for(const auto& pair : session_data.get_sprites()) {

		const auto& sprite=pair.second;
		const auto color=static_cast<int>(pair.first)==selected_index
			? ldv::rgba8(0, 0, 255, 32)
			: ldv::rgba8(0, 255, 0, 32);

		//Draw the block...
		ldv::box_representation box{
			sprite.box,
			color,
			ldv::polygon_representation::type::fill
		};
		box.set_blend(ldv::representation::blends::alpha);
		box.draw(_screen, camera);

		//Draw a solid border...
		box.set_filltype(ldv::polygon_representation::type::line);
		box.set_color(ldv::rgba8(255, 255, 0, 192));
		box.draw(_screen, camera);

		//Add the axes... first the horizontal one...
		int sprite_w=static_cast<int>(sprite.box.w),
			sprite_h=static_cast<int>(sprite.box.h);

		ldt::point_2d<int>  pt{sprite.box.origin.x+sprite.disp_x, sprite.box.origin.y+sprite.disp_y},
		                    hor_pt{pt.x+(sprite_w / 2), pt.y},
		                    ver_pt{pt.x, pt.y+(sprite_h/2)};

		auto hor_axis=ldv::line_representation{
			pt,
			hor_pt,
			ldv::rgba8(255,0,0, 255)
		};
		hor_axis.draw(_screen, camera);

		//Then the vertical.
		auto ver_axis=ldv::line_representation{
			pt,
			ver_pt,
			ldv::rgba8(0,0, 255, 255)
		};
		ver_axis.draw(_screen, camera);

		//And the id...
		if(show_ids) {

			ldv::ttf_representation id_rep{
				ttfman.get("consola-mono", font_size),
				ldv::rgba8(255, 255, 255, 192),
				std::to_string(pair.first)
			};

			id_rep.align(
				box,
				ldv::representation_alignment{
					ldv::representation_alignment::h::center,
					ldv::representation_alignment::v::center
				}
			);
			id_rep.draw(_screen, camera);
		}

		//If flipped, add some mark to the frame...
		if(sprite.flags) {

			std::string flag_text="";

			if(sprite.flags & 1) {

				flag_text+="h";
			}

			if(sprite.flags & 2) {

				flag_text+="v";
			}

			flag_text+=std::to_string(sprite.get_rotation());

			ldv::ttf_representation flag_rep{
				ttfman.get("consola-mono", font_size),
				ldv::rgba8(255, 255, 255, 192),
				flag_text
			};

			flag_rep.align(
				box,
				ldv::representation_alignment{
					ldv::representation_alignment::h::center,
					ldv::representation_alignment::v::inner_bottom
				}
			);

			flag_rep.draw(_screen, camera);
		}
	}
}

void main::draw_hud(ldv::screen& _screen) {

	ldv::ttf_representation txt_hud{
		ttfman.get("consola-mono", 12),
		ldv::rgba8(255, 255, 255, 192),
		""
	};

	std::string txt=std::to_string(mouse_pos.x)+","+std::to_string(mouse_pos.y);
	txt+=" total: "+std::to_string(session_data.get_sprites().size());
	txt+=" current: ";

	if(-1==selected_index) {

		txt+="none";
	}
	else {
		const auto& sprite=session_data.get_sprites().at(selected_index);
		txt+=std::to_string(selected_index)
			+" at "
			+std::to_string(sprite.box.origin.x)+","+std::to_string(sprite.box.origin.y)
			+" ("+std::to_string(sprite.box.w)+" x "+std::to_string(sprite.box.h)
			+") displaced by "+std::to_string(sprite.disp_x)+","+std::to_string(sprite.disp_y);
	}

	txt_hud.go_to({0,0});
	txt_hud.set_text(txt);
	txt_hud.draw(_screen);
}

void main::draw_messages(ldv::screen& _screen) {

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

void main::draw_console(ldv::screen& _screen) {

	//Draw background...
	ldv::box_representation box(
		{0, 0, _screen.get_w(), 40},
		ldv::rgba8(0, 0, 0, 128),
		ldv::polygon_representation::type::fill
	);

	box.draw(_screen);

	//Draw current command...
	ldv::ttf_representation command{
		ttfman.get("consola-mono", 14),
		ldv::rgba8(255, 255, 255, 255),
		std::string{">"}+console_txt
	};

	command.draw(_screen);
}

void main::zoom_out() {

	auto zoom=camera.get_zoom();

	if(zoom > 0.05) {

		camera.set_zoom(zoom/2.);
	}
}

void main::zoom_in() {

	auto zoom=camera.get_zoom();

	if(zoom < 8) {

		camera.set_zoom(zoom*2.);
	}
}

void main::adjust_camera_limits() {

	if(nullptr!=session_data.get_texture()) {

		const auto& tex=*(session_data.get_texture());
		camera.set_limits({ {0,0}, tex.get_w(), tex.get_h()});
	}
}

void main::select_first_index() {

	const auto& sprites=session_data.get_sprites();
	if(std::empty(sprites)) {

		selected_index=-1;
		return;
	}

	selected_index=std::begin(sprites)->first;
}

void main::select_prev() {

	const auto& sprites=session_data.get_sprites();
	if(std::empty(sprites)) {

		selected_index=-1;
		return;
	}

	const auto current=sprites.find(selected_index);
	if(current==std::begin(sprites)) {
		//turns out that the previous to the last is... the first.

		return;
	}

	selected_index=std::prev(current)->first;
	set_message(std::string{"selected (previous) index "}+std::to_string(selected_index));
}

void main::select_next() {

	const auto& sprites=session_data.get_sprites();
	if(std::empty(sprites)) {

		selected_index=-1;
		return;
	}

	const auto current=sprites.find(selected_index);

	const auto last=std::end(sprites);
	if(std::prev(last)==current) {

		return;
	}

	selected_index=std::next(current)->first; //the next of the last seems to be the last.
	set_message(std::string{"selected (next) index "}+std::to_string(selected_index));
}

sprite_table::session_data::container::const_iterator main::find_by_position(ldt::point_2d<int> _pos) const {

	const auto& sprites=session_data.get_sprites();

	return std::find_if(
		std::begin(sprites),
		std::end(sprites),
		[_pos](const std::pair<size_t, ldtools::sprite_frame> _pair) {

			ldt::box<int, unsigned int> box{
				{_pair.second.box.origin.x, _pair.second.box.origin.y},
				_pair.second.box.w,
				_pair.second.box.h
			};

			return box.point_inside(_pos);
		}
	);
}

void main::delete_current() {

	if(-1==selected_index) {
		set_message("nothing selected, can't delete");
		return;
	}

	session_data.get_sprites().erase(selected_index);
	set_message(std::string{"deleted index "}+std::to_string(selected_index));
	selected_index=-1;
}

ldt::point_2d<int> main::get_mouse_position(dfw::input& _input) const {

	auto pos=_input().get_mouse_position();

	pos.x/=camera.get_zoom();
	pos.y/=camera.get_zoom();

	pos.x+=camera.get_x();
	pos.y+=camera.get_y();

	return {pos.x, pos.y};
}

void main::create_sprite() {

	auto& sprites=session_data.get_sprites();

	ldtools::sprite_frame fr{ {{0, 0}, default_w, default_h}, 0, 0, 0};

	auto id=-1==selected_index
		? get_next_index()
		: get_next_index_from_selection();

	sprites.insert({id, fr});

	set_message(std::string{"created new index "}+std::to_string(id));
}

void main::clear_selection() {

	selected_index=-1;
	set_message("selection clear");
}

void main::save() {

	if(!session_data.is_session_open()) {

		set_message("no session is currently open, use ctrl+s to save");
		return;
	}

	lm::log(log).info()<<"will attempt to save into '"<<session_data.get_session_filename()<<"'"<<std::endl;

	std::ofstream file{session_data.get_session_filename()};
	if(!file) {

		lm::log(log).warning()<<"attempt failed"<<std::endl;
		set_message("error, could not open output file");
		return;
	}

	for(const auto& pair : session_data.get_sprites()) {

		const auto& sprite=pair.second;
		file<<pair.first<<"\t"<<sprite.box.origin.x<<"\t"<<sprite.box.origin.y<<"\t"<<sprite.box.w<<"\t"<<sprite.box.h<<"\t"<<sprite.disp_x<<"\t"<<sprite.disp_y<<"\t"<<sprite.flags<<std::endl;
	}

	set_message(std::string{"saved "}+std::to_string(session_data.get_sprites().size())+" entries into "+session_data.get_session_filename());;
}

std::size_t main::get_next_index() const {

	auto& sprites=session_data.get_sprites();
	return sprites.size()
		? sprites.rbegin()->first+1
		: 1;
}

std::size_t main::get_next_index_from_selection() const {

	auto next=selected_index+1;
	auto& sprites=session_data.get_sprites();

	if(!sprites.count(next)) {

		return next;
	}

	return get_next_index();
}

void main::duplicate_current(
	duplicate_pos _pos
) {

	if(-1==selected_index) {

		set_message("cannot duplicate without an active selection");
		return;
	}

	try {
		auto& sprites=session_data.get_sprites();
		auto copy=sprites.at(selected_index);

		switch(_pos) {

			case duplicate_pos::same: break;
			case duplicate_pos::up:     copy.box.origin.y-=copy.box.h; break;
			case duplicate_pos::down:   copy.box.origin.y+=copy.box.h; break;
			case duplicate_pos::left:   copy.box.origin.x-=copy.box.w; break;
			case duplicate_pos::right:  copy.box.origin.x+=copy.box.w; break;
		}

		auto next_index=-1==selected_index
			? get_next_index()
			: get_next_index_from_selection();

		sprites.insert({next_index, copy});

		set_message(std::string{"duplicated "}+std::to_string(selected_index)+" into "+std::to_string(next_index)+", current selection changed");
		selected_index=next_index;

	}
	catch(std::exception& e) {

		lm::log(log).error()<<"error duplicating current: "<<e.what()<<std::endl;
	}
}

void main::perform_movement(int _x, int _y, bool _resize, bool _align) {

	//Move camera.
	if(-1==selected_index) {
		camera.move_by(_x, _y);
		return;
	}

	ldtools::sprite_frame& sprite=session_data.get_sprites().at(selected_index);

	if(_resize) {

		auto perform=[](int _factor, unsigned int& _target) {

			if(_factor > 0) {

				_target+=_factor;
			}
			else if(_factor < 0 && static_cast<int>(_target) > -_factor) {

				_target+=_factor; //factor comes in negative, so to substract, we add.
			}
		};

		perform(_x, sprite.box.w);
		perform(_y, sprite.box.h);

		return;
	}

	if(_align) {

		sprite.disp_x+=_x;
		sprite.disp_y+=_y;
		return;
	}

	//Move.
	sprite.box.origin.x+=_x;
	sprite.box.origin.y+=_y;
}

void main::center_camera() {

	if(-1==selected_index) {

		return;
	}

	const auto& sprite=session_data.get_sprites().at(selected_index);
	camera.center_on(sprite.box);
}

void main::flip_frame(bool _vertical) {

	if(-1==selected_index) {

		return;
	}

	ldtools::sprite_frame& sprite=session_data.get_sprites().at(selected_index);

	if(_vertical) {

		if(sprite.flags & 2) {

			sprite.flags &= ~2;
		}
		else {

			sprite.flags |= 2;
		}
	}
	else {

		if(sprite.flags & 1) {

			sprite.flags &= ~1;
		}
		else {

			sprite.flags |= 1;
		}
	}
}

void main::rotate_frame(bool _counterclockwise) {

	if(-1==selected_index) {

		return;
	}

	ldtools::sprite_frame& sprite=session_data.get_sprites().at(selected_index);
	int degrees=sprite.get_rotation()+(_counterclockwise ? -90 : 90);

	if(degrees < 0) {

		degrees=270;
	}
	else if(degrees == 360) {

		degrees=0;
	}

	switch(degrees) {
	
		case 0:
			sprite.flags &= ~4;
			sprite.flags &= ~8;
		break;
		case 90:
			sprite.flags |= 4;
			sprite.flags &= ~8;
		break;
		case 180:
			sprite.flags &= ~4;
			sprite.flags |= 8;
		break;
		case 270:
			sprite.flags |= 4;
			sprite.flags |= 8;
		break;
	}
}

void main::cycle_font_size() {

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
