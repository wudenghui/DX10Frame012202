#include"Mp3.h"

Mp3& GetMp3()
{
	static Mp3 m;
	return m;
}

Mp3::Mp3()
{
	//*************************************************
	// mp3
	// initialize COM
    //
    // initialize the COM library on the current thread and identifies the concurrency model as single-thread
    // apartment (STA).
    CoInitialize(0);

    // create the DirectMusic performance object
    //
    // creates a single uninitialized object of the class associated with a specified CLSID.
    if(FAILED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, 
                               (void**)&g_graph_builder)))
    {
        MessageBox(NULL, "Unable to create DirectShow Graph Builder object.", "Error", MB_OK);
        return;
    }

    // Query for the media control and event objects
    g_graph_builder->QueryInterface(IID_IMediaControl, (void**)&g_media_control);
    g_graph_builder->QueryInterface(IID_IMediaEvent, (void**)&g_media_event);
}

Mp3::~Mp3()
{
	ReleaseCOM(g_media_event);
	ReleaseCOM(g_media_control);
	ReleaseCOM(g_graph_builder);
}

void Mp3::PlayMP3(char* filename)
{
	// convert filename to wide-character string
    WCHAR w_filename[MAX_PATH] = {0};

    mbstowcs(w_filename, filename, MAX_PATH);

    // render the file
    g_graph_builder->RenderFile(w_filename, NULL);

    // play the file, switches the entire filter graph into a running state.
    g_media_control->Run();
}