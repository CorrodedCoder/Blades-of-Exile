#include <Windows.h>
#include <cmath>
#include <mmsystem.h>

#include <cstdio>

#include "global.h"
#include "exlsound.h"
#include "text.h"
#include "fields.h"
#include "game_globals.hpp"

#define	NUM_SOUNDS	100

HGLOBAL sound_handles[NUM_SOUNDS];
LPCSTR snds[NUM_SOUNDS];

extern HINSTANCE store_hInstance;

extern Boolean play_sounds,in_startup_mode;
extern HWND mainPtr;
extern Boolean gInBackground;
extern party_record_type party;
extern Boolean cat,cow,chicken,dog,sheep,monsters_going;
extern short overall_mode;

short last_played = 10000;
short store_last_sound_played = 0;

static const Boolean always_asynch[100] = {FALSE,FALSE,FALSE,FALSE,FALSE,
							TRUE,TRUE,FALSE,FALSE,FALSE,
							TRUE,FALSE,FALSE,FALSE,FALSE, // 10
							FALSE,FALSE,FALSE,FALSE,FALSE,
							FALSE,FALSE,TRUE,FALSE,TRUE, // 20
							TRUE,FALSE,FALSE,FALSE,FALSE,
							FALSE,FALSE,FALSE,FALSE,TRUE,  // 30
							FALSE,FALSE,TRUE,FALSE,TRUE,
							FALSE,TRUE,TRUE,TRUE,TRUE, // 40
							TRUE,TRUE,TRUE,TRUE,TRUE,
							TRUE,FALSE,FALSE,TRUE,FALSE, // 50
							TRUE,FALSE,FALSE,FALSE,FALSE,
							FALSE,TRUE,FALSE,FALSE,FALSE, // 60
							FALSE,FALSE,FALSE,FALSE,FALSE,
							FALSE,FALSE,FALSE,FALSE,FALSE, // 70
							FALSE,TRUE,TRUE,TRUE,TRUE,
							TRUE,TRUE,TRUE,TRUE,FALSE, // 80
							TRUE,FALSE,FALSE,FALSE,FALSE,
							FALSE,TRUE,FALSE,FALSE,FALSE, // 90
							FALSE,FALSE,FALSE,FALSE,FALSE};
static const Boolean load_when_play[100] = {
0,0,1,1,1,0,0,1,1,1,
0,0,0,1,0,1,1,1,1,1,
1,1,0,1,1,1,1,0,1,1,
1,1,1,1,0,1,1,0,1,1,
1,1,1,1,1,1,1,0,0,0,
0,1,1,0,1,0,1,1,1,1, // 50
1,0,1,1,1,1,1,1,1,0,
0,0,0,0,0,0,0,0,0,0, // 70
1,1,1,1,1,0,0,0,0,0,
1,0,1,1,1,1,1,1,1,0
};

static const short can_ignore[100] = {
0,0,0,0,0,5,3,0,0,0,
5,5,5,0,5,0,0,0,0,0,
0,0,5,0,0,0,0,0,0,2,
2,2,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,
1,0,0,5,0,1,0,0,0,0, // 50
0,0,0,0,5,0,0,0,0,0,
0,0,0,0,0, 0,5,5,5,5, // 70
4,4,4,4,0,0,0,0,0,0,
0,5,5,0,0,0,0,0,0,0};
// 1 - polite asych, 1 or 2 or 3- lose easily when pref is set for fewer snds
// 3 can be async
// 4 - nostop asynch ... when 4, this sound is NOSTOP, i.e. when played, is played
//     asynch, and refuses all other sounds. Sounds that come in are ignored, and
//     disappear into the ether
// 5 - finally, bold asynch ... when 5, this sound is NOSTOP, i.e. when played, is played
//     asynch, and refuses all other sounds. When a sound is played on top of this, game
//     hangs on until sound is done, and then and only then plays later sound.


short num_devs;
Boolean sounds_fucked = FALSE;
long intro_music_start_time = -1;

void load_sounds ()
{
	short i,t,err;
	HRSRC h;
	WAVEOUTCAPS wavecaps;

#ifdef BOE_DEFAULT_VOLUME
	waveOutSetVolume(nullptr, MAKEWORD(BOE_DEFAULT_VOLUME, BOE_DEFAULT_VOLUME));
#endif

	t = waveOutGetNumDevs();
	if (t == 0) {
		sounds_fucked = TRUE;
		return;
		}
	err = waveOutGetDevCaps(0,&wavecaps,sizeof(WAVEOUTCAPS));
if (err != 0) {
	sounds_fucked = TRUE;
	switch (err) {
		case MMSYSERR_BADDEVICEID:
		MessageBox(mainPtr,"Cannot initialize sounds - No sound device detected. Game can still be played, but quietly.",
	  "Sound Error",MB_OK | MB_ICONEXCLAMATION);
		return;
		case MMSYSERR_NODRIVER:
		MessageBox(mainPtr,"Cannot initialize sounds - No driver installed. Game can still be played, but quietly.",
	  "Sound Error",MB_OK | MB_ICONEXCLAMATION);
		return;
		case MMSYSERR_NOMEM :
		MessageBox(mainPtr,"Cannot initialize sounds - can't find enough memory. Game can still be played, but quietly.",
	  "Sound Error",MB_OK | MB_ICONEXCLAMATION);
		return;
		case MMSYSERR_ALLOCATED:
		MessageBox(mainPtr,"Cannot initialize sounds - sound card already allocated. Game can still be played, but quietly.",
	  "Sound Error",MB_OK | MB_ICONEXCLAMATION);
		return;
		case MMSYSERR_ERROR:
		MessageBox(mainPtr,"Cannot initialize sounds - internal error. Game can still be played, but quietly.",
	  "Sound Error",MB_OK | MB_ICONEXCLAMATION);
		return;
		default:
		MessageBox(mainPtr,"Cannot initialize sounds - unidentified error. Game can still be played, but quietly.",
	  "Sound Error",MB_OK | MB_ICONEXCLAMATION);
		return;

		}

	}

	for (i = 0; i < NUM_SOUNDS; i++) {
		sound_handles[i] = NULL;
		if (load_when_play[i] == FALSE) {
		h = FindResource(store_hInstance, std::format("#{:d}", i + 1).c_str(), "#100");

		sound_handles[i] = LoadResource(store_hInstance,h);
		snds[i] = reinterpret_cast<LPCSTR>(LockResource(sound_handles[i]));
		}
		}

}

void play_sound(short which)  // if < 0, play asynch
{
	if (PSD[304][9] > 0)
		return;
	if (in_startup_mode == TRUE) {
		// put some in foreground check here
		}
	if (play_sounds == TRUE)
		force_play_sound(which);
}


void force_play_sound(short which)
{
	short i,num_fails = 0;
	Boolean asyn = FALSE,a_sound_did_get_played = FALSE;
	Boolean not_asyn = FALSE,check_sound;
	HRSRC h;

	if ((sounds_fucked == TRUE) || (play_sounds == FALSE))
		return;
	if (which < 0) {
		asyn = TRUE;
		which = which * -1;
		}
	if (which >= 1000) {
		which -= 1000;
		not_asyn = TRUE;
		}

	if ((which != 22) && (intro_music_start_time > 0) && (GetCurrentTime() - intro_music_start_time < 8000))
		return;
		else if ((intro_music_start_time > 0) && (sound_handles[22] != NULL)) {
			FreeResource(sound_handles[22]);
			sound_handles[22] = NULL;
			}
	if (which == 22)
		intro_music_start_time = GetCurrentTime();

	if (which >= 100)
		return;

	if ((always_asynch[which] == TRUE) &&
	((can_ignore[which] == 1) || (can_ignore[which] >= 3)))
		asyn = TRUE;
	if ((can_ignore[which] > 0) && (can_ignore[which] < 5) && (party.stuff_done[305][5] == 1))
		return;
	if ((can_ignore[which] != 1) && (can_ignore[which] < 3))
		asyn = FALSE;
	if ((party.stuff_done[305][5] == 1) && (can_ignore[which] < 5))
		asyn = FALSE;
	if (not_asyn == TRUE)
		asyn = FALSE;

	//print_nums(1000 + can_ignore[which],which,(short) asyn);

	if ((load_when_play[which] == TRUE) && (sound_handles[which] == NULL)) {
	  //	if (can_ignore[which] != 4)
			asyn = FALSE;
		h = FindResource(store_hInstance, std::format("#{:d}", which + 1).c_str(), "#100");

		sound_handles[which] = LoadResource(store_hInstance,h);
		snds[which] = reinterpret_cast<LPCSTR>(LockResource(sound_handles[which]));

	  //	ASB("Loaded sound:");
	  //	print_nums(0,0,which);
		}

	if (store_last_sound_played == 6) {
		//ASB("Interrupted snd.");
		sndPlaySound(NULL,0);
		}

	if (asyn == TRUE) {
		if (can_ignore[which] >= 4)
			check_sound = sndPlaySound(snds[which],SND_ASYNC | SND_MEMORY | SND_NOSTOP);
			else check_sound = sndPlaySound(snds[which],SND_ASYNC | SND_MEMORY);

		while (check_sound == FALSE) {

			if (can_ignore[store_last_sound_played] == 4) {// then sound goes away
				//ASB("Sound overruled by asynch sound.");
				return;
				}


			num_fails++;
			if (num_fails < 40)
				sound_pause(25);
				else {
					MessageBox(mainPtr,"Cannot play sounds - Sounds stuck error a. Game can still be played, but quietly. Check to make sure your sound drivers are up to date and not corrupted.",
					  "Sound Error",MB_OK | MB_ICONEXCLAMATION);
					print_nums(111,which,num_fails);
					sounds_fucked = TRUE;
					return;
					}
			//ASB ("Asynch clearing buffer!!!");
			//check_sound = sndPlaySound(snds[99],SND_SYNC | SND_MEMORY);
			sndPlaySound(NULL,0);

			if (can_ignore[which] >= 4)
				check_sound = sndPlaySound(snds[which],SND_ASYNC | SND_MEMORY | SND_NOSTOP);
				else check_sound = sndPlaySound(snds[which],SND_ASYNC | SND_MEMORY);
			}
	  a_sound_did_get_played = TRUE;
	  }
		else {
		if (can_ignore[which] >= 4)
			check_sound = sndPlaySound(snds[which],SND_SYNC | SND_MEMORY | SND_NOSTOP);
			else check_sound = sndPlaySound(snds[which],SND_SYNC | SND_MEMORY);
		while (check_sound == FALSE) {
			if (can_ignore[store_last_sound_played] == 4) {// then sound goes away
				//ASB("Sound overruled by asynch sound.");
				return;
				}


			num_fails++;
			if (num_fails < 40)
				sound_pause(25);
				else {
					MessageBox(mainPtr,"Cannot play sounds - Sounds stuck error b. Game can still be played, but quietly. Check to make sure your sound drivers are up to date and not corrupted.",
					 "Sound Error",MB_OK | MB_ICONEXCLAMATION);
					print_nums(222,which,num_fails);
					sounds_fucked = TRUE;
					return;
					}
			//ASB ("Asynch clearing buffer!!!");
			//check_sound = sndPlaySound(snds[99],SND_SYNC | SND_MEMORY);
			sndPlaySound(NULL,0);

			if (can_ignore[which] >= 4)
				check_sound = sndPlaySound(snds[which],SND_SYNC | SND_MEMORY | SND_NOSTOP);
				else check_sound = sndPlaySound(snds[which],SND_SYNC | SND_MEMORY);
			}
		a_sound_did_get_played = TRUE;
	  }

	store_last_sound_played = which;

	if ((load_when_play[which] == TRUE) && (asyn == FALSE)) {
		// deleted a seemingly extraneous LoadResource here
		//ASB("Dumped a sound.");
		FreeResource(sound_handles[which]);
		sound_handles[which] = NULL;
		}
		//else if (load_when_play[which] == TRUE) ASB("Kept a temp. sound.");
	for (i = 0; i < NUM_SOUNDS; i++)
		if ((load_when_play[which] == TRUE) && (sound_handles[which] != NULL)
			&& (a_sound_did_get_played == TRUE) && (i != which)){
		// deleted a seemingly extraneous LoadResource here
		FreeResource(sound_handles[i]);
		sound_handles[i] = NULL;
		//ASB("Kept sound dumped:");
		//print_nums(0,0,i);
		}

}

void kill_sound()
{
			sndPlaySound(NULL,0);
}
void one_sound(short which)
{
	if (which == last_played)
		return;
	play_sound(which);
	last_played = which;
}

void clear_sound_memory()
{
	last_played = 100;
}

void flip_sound()
{
	play_sounds = (play_sounds == TRUE) ? FALSE : TRUE;
}

void sound_pause(long len) {
	long t1,t2;

	t1 = (long) GetCurrentTime();
	t2 = t1;
	while (t2 - t1 < len) {
		t2 = (long)GetCurrentTime();
		}
}
void move_sound(unsigned char ter,short step)
{
	const auto& terrain = scenario.ter_type(ter);

	if ((monsters_going == FALSE) && (overall_mode < 10) && (party.in_boat >= 0)) {
		if (terrain.special == terrain_special::TownEntrance)
			return;
		play_sound(48);
	}
	else if ((monsters_going == FALSE) && (overall_mode < 10) && (party.in_horse >= 0)) {////
		play_sound(85);
	}
	else if ((ter >= 84) && (ter <= 89))
		play_sound(47);
	else if ((terrain.picture == 80) || (terrain.picture == 76)) // already played in special terrain check
		; 
	else if (terrain.picture == 180)
		play_sound(55);
	else if (step % 2 == 0)
		play_sound(49);
	else
		play_sound(50);
}

void incidental_noises()
{}
