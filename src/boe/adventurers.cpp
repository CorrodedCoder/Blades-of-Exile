#include "boe/adventurers.hpp"

bool cave_lore_present(const Adventurers& adven)
{
	short i;
	for (i = 0; i < 6; i++)
		if ((adven[i].main_status == 1) && (adven[i].traits[4] > 0))
			return true;
	return false;
}

bool woodsman_present(const Adventurers& adven)
{
	short i;
	for (i = 0; i < 6; i++)
		if ((adven[i].main_status == 1) && (adven[i].traits[5] > 0))
			return true;
	return false;
}

short mage_lore_total(const Adventurers& adven)
{
	short total = 0, i;

	for (i = 0; i < 6; i++)
		if (adven[i].main_status == 1)
			total += adven[i].skills[11];

	return total;
}
