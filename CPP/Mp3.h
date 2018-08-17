#ifndef MP3
#define MP3

#include"d3dUtil.h"
#include<DShow.h>

class Mp3
{
public:
	friend Mp3& GetMp3();

	void PlayMP3(char* filename);

private:
	Mp3();
	~Mp3();

	// mp3
	// DirectShows components
	IGraphBuilder*  g_graph_builder;
	IMediaControl*  g_media_control;
	IMediaEvent*    g_media_event;

};

Mp3& GetMp3();

#endif