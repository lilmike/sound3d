//#define AL_ALEXT_PROTOTYPES //commented for the move to OpenAL 1.17.0
#ifdef _WIN32
#include <windows.h>
#endif
//#include "alConfigMob.h" //commented for the move.
#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"
#include "stb_vorbis.h"
#include "wavdecoder.h"
#include <map>
#include <string>
using std::map;
using std::string;
using std::pair;
#pragma once
class sound3d {
public:
sound3d(void);
~sound3d(void);
bool play();
bool stop();
bool pause();
bool set_coords(float x, float y, float z);
bool get_coords(float *x, float *y, float *z);
bool get_paused();
bool get_playing();
bool is_active();
bool load(string filename);
static bool set_listener_ori(float x, float y, float z, float up_x, float up_y, float up_z);
static bool set_listener_coords(float x, float y, float z);
bool set_loops(bool looping);
bool get_loops();
bool set_velocity(float x, float y, float z);
bool get_velocity(float *x, float *y, float *z);
bool set_direction(float x, float y, float z);
bool get_direction(float *x, float *y, float *z);
static bool set_hrtf(bool hrtf, string hrtf_table);
static bool set_hrtf(bool hrtf, int hrtf_table);
static ALCdevice* get_device();
private:
ALuint source;
static map<string, ALuint> buffers;
bool active;
static ALCdevice *device;
static ALCcontext *context;
static int sounds;
static int table;
void log(string s);
};
