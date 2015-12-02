#include "sound3d.h"
//commented for the move to OpenAL 1.17.0
/*
#ifdef _WIN32
#define PLAT_WIN 1
#endif
*/


int sound3d::sounds = 0;
map<string, ALuint> sound3d::buffers;
ALCdevice *sound3d::device= NULL;
ALCcontext *sound3d::context = NULL;
int sound3d::table = -1;

sound3d::sound3d() {
//commented for the move to OpenAL 1.17.0
/*
const MOB_ConfigKeyValue soundConfig[] =
{
#if PLAT_WIN
  // The default sound output on Windows can't be forced to 44.1 KHz. Outputting at 44.1 KHz is essential to support HRTF, so adding this is on Windows is a good idea
	{ MOB_ConfigKey_root_drivers , "dsound" }, 
#endif // #if PLAT_WIN
	// If you want to use HRTFs, you should be outputting to Stereo sound
	{ MOB_ConfigKey_root_channels, "stereo" },
	//{ MOB_ConfigKey_root_hrtf    , ((const char*)1) },
	//{MOB_ConfigKey_root_hrtf_tables, "1052.mhr"},
//	{MOB_ConfigKey_root_wide_stereo, MOB_ConfigValue_Int(1)},
	{ MOB_ConfigKey_NULL         , 0 }, // This is the terminator for the config array
};
alSetConfigMOB( soundConfig );
*/
	if(!device) {
alcGetError(device);
device = alcOpenDevice(NULL);
if(alcGetError(device) != AL_NO_ERROR) {
log("error making device!");
return;
}
}
if(!context) {
alcGetError(device);
context = alcCreateContext(device, NULL);
if(alcGetError(device) != AL_NO_ERROR) {
log("error creating context!");
return;
}
alcGetError(device);
alcMakeContextCurrent(context);
if(alcGetError(device) != AL_NO_ERROR) {
log("error making context current!");
return;
}
}
alGetError();
alGenSources(1, &source);
if(alGetError() != AL_NO_ERROR) {
log("error generating source!");
return;
}
sounds++;
active=false;
}

sound3d::~sound3d() {
sounds--;
stop();
alDeleteSources(1, &source);
if(sounds == 0) {
map<string, ALuint>::iterator it = buffers.begin();
while(it!=buffers.end()) {
alDeleteBuffers(1, &(it->second));
buffers.erase(it++);
}
if(context) {
alcDestroyContext(context);
}
if(device) {
alcCloseDevice(device);
}
}
}
void sound3d::log(string s) {
//printf(s.c_str());
}

bool sound3d::play() {
if(!is_active()) {
log("not active!");
return false;
}
else {
alGetError();
alSourcePlay(source);
if(alGetError() != AL_NO_ERROR) {
log("error!");
return false;
}
}
return true;
}

bool sound3d::stop() {
if(!is_active()) {
return false;
}
else {
alGetError();
alSourceStop(source);
if(alGetError() != AL_NO_ERROR) {
return false;
}
}
return true;
}

bool sound3d::pause() {
if(!is_active()) {
return false;
}
else {
alGetError();
alSourcePause(source);
if(alGetError != AL_NO_ERROR) {
return false;
}
}
return true;
}

bool sound3d::set_coords(float x, float y, float z) {
if(!is_active()) {
log("not active for coords!");

	return false;
}
else {
alGetError();
alSource3f(source, AL_POSITION, x, y, z);
if(alGetError() != AL_NO_ERROR) {
return false;
}
}
return true;
}

bool sound3d::get_coords(float *x, float *y, float *z) {
if(!is_active()) {
return false;
}
else {
alGetError();
alGetSource3f(source, AL_POSITION, x, y, z);
if(alGetError() != AL_NO_ERROR) {
return false;
}
}
return true;
}

bool sound3d::get_paused() {
if(!is_active()) {
return false;
}
else {
alGetError();
ALenum state;
alGetSourcei(source, AL_SOURCE_STATE, &state);
if(state == AL_PAUSED) {
return true;
}
else {
return false;
}
if(alGetError() != AL_NO_ERROR) {
return false;
}
}
}
bool sound3d::get_playing() {
if(!is_active()) {
return false;
}
else {
alGetError();
ALenum state;
alGetSourcei(source, AL_SOURCE_STATE, &state);
if(state == AL_PLAYING) {
return true;
}
else {
return false;
}
if(alGetError() != AL_NO_ERROR) {
return false;
}
}
}

bool sound3d::is_active() {
if(!active) {
return false;
}
else {
return true;
}
}

bool sound3d::load(string filename) {
if(buffers.find(filename)!=buffers.end()) {
alGetError();
alSourcei(source, AL_BUFFER, buffers.find(filename)->second);
if(alGetError() != AL_NO_ERROR) {
	return false;
}
active=true;
return true;
}
else {
ALuint buffer;
const char *s = filename.c_str();
char s2[5];
int index = 0;
for(unsigned int x = filename.length()-4; x < filename.length(); x++) {
s2[index] = s[x];
index++;
}
s2[4]='\0';
if(strcmp(s2, ".ogg") == 0) {
int error = 0;
char *s3 = new char[filename.length()+1];
for(unsigned int x = 0; x < filename.length(); x++ ) {
	s3[x] = filename[x];
}
s3[filename.length()] = '\0';
stb_vorbis *vorbis = stb_vorbis_open_filename(s3, &error, NULL);
if(error != 0) {
	return false;
}
	stb_vorbis_info info = stb_vorbis_get_info(vorbis);
	unsigned int rate = info.sample_rate;
	unsigned int samples = stb_vorbis_stream_length_in_samples(vorbis);
	int channels = info.channels;
	short *buff = new short[samples*channels];
	int samps = stb_vorbis_get_samples_short_interleaved(vorbis, channels, buff, samples*channels);
	stb_vorbis_close(vorbis);
alGetError();
alGenBuffers(1, &buffer);
if(alGetError() != AL_NO_ERROR) {
	free(buff);
return false;
}
ALenum format;
if(channels >= 2 ) {
format = AL_FORMAT_STEREO16;
}
else {
format = AL_FORMAT_MONO16;
}
alBufferData(buffer, format, buff, samps*channels*2, rate);
if(alGetError() != AL_NO_ERROR) {
	free(buff);
alDeleteBuffers(1, &buffer);
return false;
}
buffers.insert(pair<string, ALuint>(filename, buffer));
alSourcei(source, AL_BUFFER, buffer);
if(alGetError() != AL_NO_ERROR) {
	free(buff);
return false;
}
free(buff);
active = true;
return true;
}
else if(strcmp(s2, ".wav") == 0) {
int16_t channels, bits = 0;
int32_t size, rate = 0;
unsigned char*buff = load_wav_file(filename.c_str(), &channels, &rate, &size, &bits);
if(!buff) {
return false;
}
alGetError();
alGenBuffers(1, &buffer);
if(alGetError() != AL_NO_ERROR) {
free(buff);
return false;
}
ALenum format;
	 if(channels == 2) {
		 if(bits == 16) {
			 format = AL_FORMAT_STEREO16;
		 }
		 else {
			 format = AL_FORMAT_STEREO8;
		 }
	 }
else {
		if(bits == 16) {
			format = AL_FORMAT_MONO16;
}
		else {
			format = AL_FORMAT_MONO8;
		}
	 }
alBufferData(buffer, format, buff, size, rate);
if(alGetError() != AL_NO_ERROR) {
free(buff);
alDeleteBuffers(1, &buffer);
return false;
}
buffers.insert(pair<string, ALuint>(filename, buffer));
alSourcei(source, AL_BUFFER, buffer);
if(alGetError() != AL_NO_ERROR) {
free(buff);
return false;
}
free(buff);
active = true;
return true;
}
else {
	return false;
}
}
}

bool sound3d::set_listener_ori(float x, float y, float z, float up_x, float up_y, float up_z) {
alGetError();
float ori[6] = {x, y, z, up_x, up_y, up_z};
alListenerfv(AL_ORIENTATION, ori);
if(alGetError() != AL_NO_ERROR) {
return false;
}
return true;
}

bool sound3d::set_listener_coords(float x, float y, float z) {
alGetError();
alListener3f(AL_POSITION, x, y, z);
if(alGetError() != AL_NO_ERROR) {
return false;
}
return true;
}

bool sound3d::set_loops(bool looping) {
if(!is_active()) {
return false;
}
else {
alGetError();
alSourcei(source, AL_LOOPING, looping);
if(alGetError() != AL_NO_ERROR) {
return false;
}
return true;
}
}


bool sound3d::get_loops() {
if(!is_active()) {
return false;
}
else {
alGetError();
ALint loop;
alGetSourcei(source, AL_LOOPING, &loop);
if(loop == 1) {
	return true;
}
else {
	return false;
}
if(alGetError() != AL_NO_ERROR) {
return false;
}
}
}

bool sound3d::set_velocity(float x, float y, float z) {
if(!is_active()) {
return false;
}
else {
alGetError();
alSource3f(source, AL_VELOCITY, x, y, z);
if(alGetError() != AL_NO_ERROR) {
return false;
}
return true;
}
}

bool sound3d::get_velocity(float *x, float *y, float *z) {
if(!is_active()) {
return false;
}
else {
alGetError();
alGetSource3f(source, AL_VELOCITY, x, y, z);
if(alGetError() != AL_NO_ERROR) {
return false;
}
return true;
}
}

bool sound3d::set_direction(float x, float y, float z) {
if(!is_active()) {
return false;
}
 else {
alGetError();
alSource3f(source, AL_DIRECTION, x, y, z);
if(alGetError() != AL_NO_ERROR) {
return false;
}
return true;
}
}

bool sound3d::get_direction(float *x, float *y, float *z) {
if(!is_active()) {
return false;
}
else {
alGetError();
alGetSource3f(source, AL_DIRECTION, x, y, z);
if(alGetError() != AL_NO_ERROR) {
return false;
}
return true;
}
}
/*
static inline MOB_ConfigValue MOB_ConfigValue_Int( ALint intVal )
{
	MOB_ConfigValue value;
	value.intVal = intVal;
	return value;
}
*/

//commented for the move to 1.17.0
/*
static inline MOB_ConfigValue MOB_ConfigValue_String( const char *stringVal
)
{
	MOB_ConfigValue value;
	value.stringVal = stringVal;
	return value;
}
*/

bool sound3d::set_hrtf(bool hrtf, int hrtf_table) {
//commented for the move to 1.17.0
/*
	ALboolean stuff = alcDeviceEnableHrtfMOB(device, (hrtf==true)?AL_TRUE:AL_FALSE);
	if(stuff == AL_TRUE) {
		return true;
	}
	else {
		return false;
	}
*/
if(!alcIsExtensionPresent(device, "ALC_HRTF_SOFT")) {
return false;
}
ALCint attr [] = {ALC_HRTF_SOFT, (hrtf)?ALC_TRUE:ALC_FALSE, ALC_HRTF_SPECIFIER_SOFT, hrtf_table, 0};
alcGetError(device);
LPALCRESETDEVICESOFT alcResetDeviceSOFT = (LPALCRESETDEVICESOFT)(alcGetProcAddress(device, "alcResetDeviceSOFT"));
alcResetDeviceSOFT(device, attr);
if(alcGetError(device) != ALC_NO_ERROR) {
return false;
}
table = hrtf_table;
return true;
}

bool sound3d::set_hrtf(bool hrtf, string hrtf_table) {
if(hrtf_table == "") {
if(table == -1)
return false;
return set_hrtf(hrtf, table);
}
else {
LPALCGETSTRINGISOFT alcGetStringiSOFT = (LPALCGETSTRINGISOFT)(alcGetProcAddress(device, "alcGetStringiSOFT"));
ALCint num;
alcGetIntegerv(device, ALC_NUM_HRTF_SPECIFIERS_SOFT, 1, &num);
if(!num) {
return false;
}
else {
ALCint index = -1;
for(ALCint i = 0; i < num; i++) {
            const ALCchar *name = alcGetStringiSOFT(device, ALC_HRTF_SPECIFIER_SOFT, i);
if(strcmp(name, hrtf_table.c_str()) == 0) {
index = i;
}
}
if(index == -1) {
return false;
}
else {
return set_hrtf(hrtf, index);
}
}
}
}

ALCdevice* sound3d::get_device() {
return device;
}
