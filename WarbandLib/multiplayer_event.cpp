#include "multiplayer_event.h"

#include "warband.h"

using namespace wb;

multiplayer_event::multiplayer_event()
{
	time = -1.0;
	size_in_bits = 0;
	cursor_in_bits = 0;
	prev_packet_id = 0;
	cur_packet_id = 0;
	guaranteed = 0;
	priority = 0;
	id = warband->network_manager.cur_event_id++;
	u1 = 0;
};

void multiplayer_event::add_buffer(const network_buffer &nbuf)
{
	warband->basic_game.put_data(this->buffer, this->cursor_in_bits, nbuf.buffer, nbuf.size_in_bits);
	this->size_in_bits += nbuf.size_in_bits;
}
