#pragma once

#include <ldtools/sprite_table.h>

namespace sprite_table {

/**
 * logic for sprite flipping
 */
class sprite_flip {

	public:

	void flip_horizontal(ldtools::sprite_frame&);
	void flip_vertical(ldtools::sprite_frame&);
};
}
