/*This program demonstrates how to read in a raw
  data file and write it to the sound device to be played.
  The program uses the ALSA library.
  Use option -lasound on compile line.*/
 
#include </usr/include/alsa/asoundlib.h>
#include <math.h>
#include <iostream>
using namespace std;
 
static char *device = "default";    /*default playback device */
int main(int argc, char* argv[])
{
  int err, numSamples;
  snd_pcm_t *handle;
  snd_pcm_sframes_t frames;
  numSamples = atoi(argv[1]);
  char* samples = (char*) malloc((size_t) numSamples);
  FILE *inFile = fopen(argv[2], "rb");
  int numRead = fread(samples, 1, numSamples, inFile);
  fclose(inFile);
if ((err=snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0){
    printf("Playback open error: %s\n", snd_strerror(err));
    exit(EXIT_FAILURE);
  }
if ((err =
 snd_pcm_set_params(handle,SND_PCM_FORMAT_U8,
   SND_PCM_ACCESS_RW_INTERLEAVED,1,44100, 1, 100000) ) < 0 ){
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
   }
  frames = snd_pcm_writei(handle, samples, numSamples);
  if (frames < 0)
    frames = snd_pcm_recover(handle, frames, 0);
  if (frames < 0) {
    printf("snd_pcm_writei failed: %s\n", snd_strerror(err));
  }
  snd_pcm_close(handle);
  return 0;
}
