#include "boe/pc.hpp"
#include <algorithm>

void heal_pc(pc_record_type& pc, short amt)
{
	if ((pc.main_status == 1) && (pc.cur_health < pc.max_health))
	{
		pc.cur_health = std::min(static_cast<short>(pc.cur_health + amt), pc.max_health);
	}
}

bool pc_cure(pc_record_type& pc, short amt)
{
	if (pc.main_status != 1)
		return false;
	if (pc.status[2] <= amt)
		pc.status[2] = 0;
	else
		pc.status[2] -= amt;
	return true;
}
