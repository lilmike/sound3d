#define AL_ALEXT_PROTOTYPES
#include "alConfigMob.h"
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
static void set_hrtf(bool hrtf, char *hrtf_table);
private:
ALuint source;
static map<string, ALuint> buffers;
bool active;
static ALCdevice *device;
static ALCcontext *context;
static int sounds;
void log(string s);
};
