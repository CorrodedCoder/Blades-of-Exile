
#include <Dialogs.h>
#include <TextUtils.h>
#include "Global.h"
#include "dialogutils.h"

DialogPtr make_dialog(int resource_num)
{
	DialogPtr the_dialog;

	the_dialog = GetNewDialog (resource_num, 0, IN_FRONT);
	if (the_dialog == NULL) 
		SysBeep(50);
	ShowWindow (the_dialog);

	return the_dialog;
}

void put_num_in_text(short num_to_put,DialogPtr the_dialog, short item_num)
{
	Str255		text_entry;
	short	the_type;
	Handle	the_handle = NULL;
	Rect	the_rect;
	
	NumToString ((long) num_to_put , text_entry);
	GetDItem( the_dialog, item_num, &the_type, &the_handle, &the_rect );
	SetIText ( the_handle, text_entry);	
}

long get_text_item_num(short item_num,DialogPtr the_dialog)
{
	long storage;
	long		number_given = 0;

	short	the_type;
	Handle	the_handle = NULL;
	Rect	the_rect;
	Str255 	the_string;	
	
	GetDItem( the_dialog, item_num, &the_type, &the_handle, &the_rect );
	GetIText ( the_handle, the_string);
					
	StringToNum( the_string, &number_given);	
	storage = number_given;

	return storage;
}

