#include <sprite_table/sprite_flip.h>

using namespace sprite_table;

void sprite_flip::flip_horizontal(
	ldtools::sprite_frame& _frame
) {

	if(_frame.flags & 1) {

		_frame.flags &= ~1;
	}
	else {

		_frame.flags |= 1;
	}
}

void sprite_flip::flip_vertical(
	ldtools::sprite_frame& _frame
) {

	if(_frame.flags & 2) {

		_frame.flags &= ~2;

	}
	else {

		_frame.flags |= 2;
	}
}
