#ifndef MUSIC
#define MUSIC

#include"d3dUtil.h"
#include<dsound.h>

typedef struct pcm_sound_typ
{
	LPDIRECTSOUNDBUFFER dsbuffer;   // the ds buffer containing the sound
	DWORD state;                      // state of the sound
	int rate;                       // playback rate
	int size;                       // size of sound
	char* name;	// sound 文件的名字
} pcm_sound, *pcm_sound_ptr;

#define MAX_SOUNDS     256 // max number of sounds in system at once 

// digital sound object state defines
#define SOUND_NULL     0 // " "
#define SOUND_LOADED   1
#define SOUND_PLAYING  2
#define SOUND_STOPPED  3





class Music
{
public:
	friend Music& GetMusic();

	void LoadSoundFromDir(char* dirName);

	LPDIRECTSOUNDBUFFER GetSoundBufferByName(char* name);
	LPDIRECTSOUNDBUFFER GetSoundBufferByIndex(int index);
	void PlaySound(int index);
	

	int GetSoundIndexByName(char* name);
public:
	// 加载WAV文件
	int DSound_Load_WAV(char *filename, int control_flags);
private:
	Music();
	~Music();

	// sound
	LPDIRECTSOUND	mDs;
	DSBUFFERDESC		dsbd;           // directsound description
	DSCAPS			dscaps;         // directsound caps
	DSBCAPS			dsbcaps;        // directsound buffer caps

	LPDIRECTSOUNDBUFFER	lpdsbprimary,    // you won't need this normally
                    lpdsbsecondary;  // the sound buffers

	WAVEFORMATEX		pcmwf;          // generic waveformat structure

	pcm_sound			sound_fx[MAX_SOUNDS];    // the array of secondary sound buffers
};

Music& GetMusic();

#endif