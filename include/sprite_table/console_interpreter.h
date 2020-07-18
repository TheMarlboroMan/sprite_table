#pragma once

#include "session_data.h"

#include <ldtools/sprite_table.h>

#include <string>
#include <map>

namespace sprite_table {

class console_interpreter {

	public:

	using container=std::map<std::size_t, ldtools::sprite_frame>;

	                        console_interpreter(container&, unsigned int, unsigned int);
	bool                    perform(const std::string&);
	const std::string&      get_message() const {return message;}

	private:

	bool                    create(const std::string&);
	bool                    erase(const std::string&);
	bool                    move(const std::string&);
	bool                    move_range(const std::string&);
	bool                    swap(const std::string&);
	bool                    copy(const std::string&);
	bool                    set(const std::string&);

	container&              sprites;
	std::string             message;
	const unsigned int      default_w,
	                        default_h;
};

}