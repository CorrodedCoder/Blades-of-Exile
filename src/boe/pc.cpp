#include "boe/pc.hpp"

void heal_pc(pc_record_type& pc, short amt)
{
	if (pc.cur_health > pc.max_health)
		return;
	if (pc.main_status != 1)
		return;
	pc.cur_health += amt;
	if (pc.cur_health > pc.max_health)
		pc.cur_health = pc.max_health;

}
