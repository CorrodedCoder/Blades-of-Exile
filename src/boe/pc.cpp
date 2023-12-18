#include "boe/pc.hpp"
#include <algorithm>

void pc_heal(pc_record_type& pc, short amt)
{
	if ((pc.main_status == 1) && (pc.cur_health < pc.max_health))
	{
		pc.cur_health = std::min(static_cast<short>(pc.cur_health + amt), pc.max_health);
	}
}

bool pc_cure(pc_record_type& pc, short amt)
{
	if (pc.main_status != 1)
	{
		return false;
	}
	pc.status[2] = std::max(pc.status[2] - amt, 0);
	return true;
}

void restore_sp_pc(pc_record_type& pc, short amt)
{
	if (pc.cur_sp > pc.max_sp)
		return;
	pc.cur_sp += amt;
	if (pc.cur_sp > pc.max_sp)
		pc.cur_sp = pc.max_sp;
}
