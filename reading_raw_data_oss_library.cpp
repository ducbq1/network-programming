//This program runs under OSS
//The program expects an 8-bit raw sound file.
//You can alter it to read a 16-bit file into short ints
#include <sys/ioctl.h> //for ioctl()
#include <math.h> //sin(), floor()
#include <stdio.h> //perror
#include <fcntl.h> //open
#include <linux/soundcard.h> //SOUND_PCM*
#include <stdlib.h>
#include <unistd.h> 
 
using namespace std;
 
#define TYPE char
#define RATE 44100 //sampling rate
#define SIZE sizeof(TYPE) //size of sample, in bytes
#define CHANNELS 1 //number of stereo channels
#define PI 3.14159
#define SAMPLE_MAX (pow(2,SIZE*8 - 1) - 1) 
 
void writeToSoundDevice(TYPE* buf, int deviceID, int buffSize) {
  int status;
  status = write(deviceID, buf, buffSize);
  if (status != buffSize)
    perror("Wrote wrong number of bytes\n");
  status = ioctl(deviceID, SOUND_PCM_SYNC, 0);
  if (status == -1)
    perror("SOUND_PCM_SYNC failed\n");
}
 
int main(int argc, char* argv[])
{
  int deviceID, arg, status, i, numSamples;
  numSamples = atoi(argv[1]);
 
  TYPE* samples = (TYPE *) malloc((size_t) numSamples * sizeof(TYPE)* CHANNELS);
  FILE *inFile = fopen(argv[2], "rb");
  fread(samples, (size_t)sizeof(TYPE), numSamples*CHANNELS, inFile);
  fclose(inFile);
 
  deviceID = open("/dev/dsp", O_WRONLY, 0);
  if (deviceID < 0)
    perror("Opening /dev/dsp failed\n");
  arg = SIZE * 8;
  status = ioctl(deviceID, SOUND_PCM_WRITE_BITS, &arg);
  if (status == -1)
    perror("Unable to set sample size\n");
  arg = CHANNELS;
  status = ioctl(deviceID, SOUND_PCM_WRITE_CHANNELS, &arg);
  if (status == -1)
    perror("Unable to set number of channels\n");
  arg = RATE;
  status = ioctl(deviceID, SOUND_PCM_WRITE_RATE, &arg);
  if (status == -1)
    perror("Unable to set number of bits\n");
 
  writeToSoundDevice(samples, deviceID, numSamples * CHANNELS);
  FILE *outFile = fopen(argv[3], "wb");
  fwrite(samples, 1, numSamples*CHANNELS, outFile);
  fclose(outFile);
  close(deviceID);
}

