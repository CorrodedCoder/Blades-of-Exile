#include "boe/adventurers.hpp"

short cave_lore_present(const Adventurers& adven)
{
	short i, ret = 0;
	for (i = 0; i < 6; i++)
		if ((adven[i].main_status == 1) && (adven[i].traits[4] > 0))
			ret += 1;
	return ret;
}

short woodsman_present(const Adventurers& adven)
{
	short i, ret = 0;
	for (i = 0; i < 6; i++)
		if ((adven[i].main_status == 1) && (adven[i].traits[5] > 0))
			ret += 1;
	return ret;
}

short mage_lore_total(const Adventurers& adven)
{
	short total = 0, i;

	for (i = 0; i < 6; i++)
		if (adven[i].main_status == 1)
			total += adven[i].skills[11];

	return total;
}
