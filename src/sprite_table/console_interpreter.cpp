#include "../../include/sprite_table/console_interpreter.h"

#include <sstream>
#include <algorithm>

using namespace sprite_table;

console_interpreter::console_interpreter(
	container& _container,
	unsigned int _default_w,
	unsigned int _default_h,
	ldv::rgba_color& _background_color,
	int& _selected_index
)
	:sprites{_container},
	background_color{_background_color},
	selected_index{_selected_index},
	default_w{_default_w},
	default_h{_default_h}
{ }

bool console_interpreter::select(const std::string& _parameters) {

	std::stringstream ss{_parameters};
	std::string dummystr;
	ss>>dummystr;

	std::size_t index=0;
	ss>>index;

	if(ss.fail()) {

		message="invalid syntax, use f1 for help";
		return false;
	}

	if(!sprites.count(index)) {

		message="index does not exist";
		return false;
	}

	selected_index=index;
	message="index selected";
	return true;
}


bool console_interpreter::perform(const std::string& _command) {

	std::stringstream ss{_command};

	std::string command;
	ss>>command;

	if(command=="select") {

		return select(ss.str());
	}

	if(command=="new") {

		return create(ss.str());
	}

	if(command=="delete") {

		return erase(ss.str());
	}

	if(command=="move") {

		return move(ss.str());
	}

	if(command=="move_range") {

		return move_range(ss.str());
	}

	if(command=="swap") {

		return swap(ss.str());
	}

	if(command=="copy") {

		return copy(ss.str());
	}

	if(command=="set") {

		return set(ss.str());
	}

	if(command=="bgcolor") {

		return bg_color(ss.str());
	}

	message=std::string{"unknown command '"}+command+"', try f1 for help";
	return false;
}

bool console_interpreter::create(const std::string& _parameters) {

	std::stringstream ss{_parameters};
	std::string dummystr;
	ss>>dummystr;

	std::size_t index=0;
	ss>>index;

	if(ss.fail()) {

		message="invalid syntax, use f1 for help";
		return false;
	}

	if(sprites.count(index)) {

		message="index already exists";
		return false;
	}

	//Try and extract optional parameters...
	int x=0, y=0, disp_x=0, disp_y=0, w=default_w, h=default_h;

	auto extract=[&ss](int& _val) {

		int dummy=0;
		ss>>dummy;
		if(!ss.fail()) {
			_val=dummy;
		}
	};

	extract(x);
	extract(y);
	extract(w);
	extract(h);
	extract(disp_x);
	extract(disp_y);

	ldtools::sprite_frame fr{
		{
			{x, y},
			static_cast<unsigned int>(w),
			static_cast<unsigned int>(h),
		},
		disp_x,
		disp_y,
		0
	};
	sprites.insert({index, fr});

	message="frame created";
	return true;
}

bool console_interpreter::erase(const std::string& _parameters) {

	std::stringstream ss{_parameters};
	std::string dummystr;
	ss>>dummystr;

	std::size_t index=0;
	ss>>index;

	if(ss.fail()) {

		message="invalid syntax, use f1 for help";
		return false;
	}

	if(!sprites.count(index)) {

		message="index does not exist";
		return false;
	}

	sprites.erase(index);

	message="frame deleted";
	return true;
}

bool console_interpreter::move(const std::string& _parameters) {

	std::stringstream ss{_parameters};
	std::string dummystr;
	ss>>dummystr;

	std::size_t from=0;
	ss>>from;

	if(ss.fail()) {

		message="invalid syntax, use f1 for help";
		return false;
	}

	if(!sprites.count(from)) {

		message="index from does not exist";
		return false;
	}

	std::size_t to=0;
	ss>>to;

	if(sprites.count(to)) {

		message="index to already exists";
		return false;
	}

	auto sprite=sprites.at(from);
	sprites.erase(from);
	sprites.insert({to, sprite});

	message="frame moved";
	return true;
}

bool console_interpreter::move_range(const std::string& _parameters) {

	std::stringstream ss{_parameters};
	std::string dummystr;
	ss>>dummystr;

	std::size_t from=0;
	ss>>from;

	std::size_t to=0;
	ss>>to;

	std::size_t first_new_index=0;
	ss>>first_new_index;

	if(ss.fail()) {

		message="invalid syntax, use f1 for help";
		return false;
	}

	//First, check that the ranges are in order...
	if(from >= to) {

		message="invalid range, use f1 for help";
		return false;
	}

	//Next, make sure the destination is free...
	std::size_t length=to-from,
	            search=first_new_index,
	            limit=search+length;

	for(; search <= limit; search++) {

		if(sprites.count(search)) {

			message=std::string{"invalid destination, index "}+std::to_string(search)+" is not free";
			return false;
		}
	}

	//And then, just move...
	for(; from <= to; from++) {

		auto sprite=sprites.at(from);
		sprites.erase(from);
		sprites.insert({first_new_index++, sprite});
	}

	message="range moved";
	return true;
}

bool console_interpreter::swap(const std::string& _parameters) {

	std::stringstream ss{_parameters};
	std::string dummystr;
	ss>>dummystr;

	std::size_t a=0;
	ss>>a;

	if(ss.fail()) {

		message="invalid syntax, use f1 for help";
		return false;
	}

	if(!sprites.count(a)) {

		message="index a does not exist";
		return false;
	}

	std::size_t b=0;
	ss>>b;

	if(!sprites.count(b)) {

		message="index b does not exist";
		return false;
	}

	auto sprite_a=sprites.at(a);
	auto sprite_b=sprites.at(b);

	sprites.at(a)=sprite_b;
	sprites.at(b)=sprite_a;

	message="frames swapped";
	return true;
}

bool console_interpreter::copy(const std::string& _parameters) {

	std::stringstream ss{_parameters};
	std::string dummystr;
	ss>>dummystr;

	std::size_t origin=0;
	ss>>origin;

	if(ss.fail()) {

		message="invalid syntax, use f1 for help";
		return false;
	}

	if(!sprites.count(origin)) {

		message="index origin does not exist";
		return false;
	}

	std::size_t destination=0;
	ss>>destination;

	if(sprites.count(destination)) {

		message="index destination already exists";
		return false;
	}

	auto sprite=sprites.at(origin);
	sprites.insert({destination, sprite});

	message="frame copied";
	return true;
}

bool console_interpreter::set(const std::string& _parameters) {

	std::stringstream ss{_parameters};
	std::string dummystr;
	ss>>dummystr;

	std::size_t index=0;
	ss>>index;

	if(ss.fail()) {

		message="invalid syntax, use f1 for help";
		return false;
	}

	if(!sprites.count(index)) {

		message="index does not exist";
		return false;
	}

	std::string prop;
	ss>>prop;
	if(ss.fail()) {

		message="invalid syntax, use f1 for help";
		return false;
	}

	std::vector<std::string> valid_props{"x", "y", "w", "h", "dx", "dy"};
	auto it=std::find(std::begin(valid_props), std::end(valid_props), prop);

	if(valid_props.end()==it) {

		message="invalid property name, use f1 for help";
		return false;
	}

	std::size_t new_value=0;
	ss>>new_value;
	if(ss.fail()) {

		message="invalid value, use f1 for help";
		return false;
	}

	auto& sprite=sprites.at(index);

	if(prop=="x") {

		sprite.box.origin.x=new_value;
	}
	else if(prop=="y") {

		sprite.box.origin.y=new_value;
	}
	else if(prop=="w") {

		sprite.box.w=new_value;
	}
	else if(prop=="h") {

		sprite.box.h=new_value;
	}
	else if(prop=="dx") {

		sprite.disp_x=new_value;
	}
	else if(prop=="dy") {

		sprite.disp_y=new_value;
	}

	message="frame updated";
	return true;
}


bool console_interpreter::bg_color(const std::string& _parameters) {

	std::stringstream ss(_parameters);
	std::string dummy;
	ss>>dummy; //TODO: This is horrible.

	int colors[3]={0,0,0};
	
	for(int i=0; i<3; i++) {

		ss>>colors[i];
		if(ss.fail()) {

			message="syntax error, use bgcolor r g b where r, g and b are between 0 and 255 (read fail)";
			return false;
		}

		if(colors[i] < 0 || colors[i] > 255) {

			message="syntax error, use bgcolor r g b where r, g and b are between 0 and 255 (invalid values)";
			return false;
		}
	}

	background_color.r=colors[0];
	background_color.g=colors[1];
	background_color.b=colors[2];
	message="background color updated";
	return true;
}
