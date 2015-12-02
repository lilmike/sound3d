#define _CRT_SECURE_NO_WARNINGS 1
#include "wavdecoder.h"
#include <malloc.h>


unsigned char* load_wav_file(const char *fname, int16_t * chans, int32_t *rate, int32_t *length, int16_t *bits) 
{ 
FILE *fp; 

fp = fopen(fname,"rb"); 
if (fp) 
{ 
char id2[5];
char id[4]; //four bytes to hold 'RIFF' 
unsigned char *sound_buffer; 
int32_t size; //32 bit value to hold file size 
int16_t format_tag, channels, block_align, bits_per_sample; //our 16 values 
int32_t format_length, sample_rate, avg_bytes_sec, data_size; //our 32 bit values 

fread(id, sizeof(char), 4, fp); //read in first four bytes 
memcpy(id2, id, 4);
id2[4]='\0';
if (!strcmp(id2, "RIFF")) 

{ //we had 'RIFF' let's continue 
fread(&size, sizeof(int32_t), 1, fp); //read in 32bit size value 
fread(id, sizeof(char), 4, fp); //read in 4 byte string now 
memcpy(id2, id, 4);
id2[4]='\0';
if (!strcmp(id2,"WAVE")) 
{ //this is probably a wave file since it contained "WAVE" 
fread(id, sizeof(char), 4, fp); //read in 4 bytes "fmt "; 
fread(&format_length, sizeof(int32_t),1,fp); 
fread(&format_tag, sizeof(int16_t), 1, fp); //check mmreg.h (i think?) for other 
// possible format tags like ADPCM 
if(format_tag == 1) {
fread(&channels, sizeof(int16_t),1,fp); //1 mono, 2 stereo 
*chans = channels;
fread(&sample_rate, sizeof(int32_t), 1, fp); //like 44100, 22050, etc... 
*rate = sample_rate;
fread(&avg_bytes_sec, sizeof(int32_t), 1, fp); //probably won't need this 
fread(&block_align, sizeof(int16_t), 1, fp); //probably won't need this 
fread(&bits_per_sample, sizeof(int16_t), 1, fp); //8 bit or 16 bit file? 
*bits = bits_per_sample;
fread(id, sizeof(char), 4, fp); //read in 'data' 
fread(&data_size, sizeof(int32_t), 1, fp); //how many bytes of sound data we have 
*length = data_size;
sound_buffer = (unsigned char *) malloc (sizeof(unsigned char) * data_size); //set aside sound buffer space 
fread(sound_buffer, sizeof(unsigned char), data_size, fp); //read in our whole sound data chunk 
return sound_buffer;
}
else {
return NULL;
}
} 
else {
return NULL;
}
} 
else {
return NULL;
}
} 
else {
	return NULL;
}
} 
