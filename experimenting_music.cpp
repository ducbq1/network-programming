//This program works under the OSS library
#include <sys/ioctl.h> //for ioctl()
#include <math.h> //sin(), floor()
#include <stdio.h> //perror
#include <fcntl.h> //open, O_WRONLY
#include <linux/soundcard.h> //SOUND_PCM*
#include <iostream>
#include <stdlib.h>
using namespace std;
#define LENGTH 1 //number of seconds
#define RATE 44100 //sampling rate
#define SIZE sizeof(short) //size of sample, in bytes
#define CHANNELS 1 // number of stereo channels
#define PI 3.14159
#define SAMPLE_MAX 32767 // should this end in 8?
#define MIDDLE_C 262
#define SEMITONE 1.05946
enum types {BACK = 0, MAJOR, MINOR};
double getFreq(int index){
   return MIDDLE_C * pow(SEMITONE, index-1);
}
int getInput(){
   char c;
   string str;
   int i;
   while ((c = getchar()) != '\n' && c != EOF)
      str += c;
   for (i = 0; i < str.length(); ++i)
      str.at(i) = tolower(str.at(i));
   if (c == EOF || str == "quit")
      exit(0);
   return atoi(str.c_str());
}
int getIndex()
{   
   int input;
   cout
      << "Choose one of the following:\n"
      << "\t1) C\n"
      << "\t2) C sharp/D flat\n"
      << "\t3) D\n"
      << "\t4) D sharp/E flat\n"
      << "\t5) E\n"
      << "\t6) F\n"
      << "\t7) F sharp/G flat\n"
      << "\t8) G\n"
      << "\t9) G sharp/A flat\n"
      << "\t10) A\n"
      << "\t11) A sharp/B flat\n"
      << "\t12) B\n"
      << "\tor type quit to quit\n";
   input = getInput();
   if (! (input >= BACK && input <= 12))
      return -1;
   return input;
}
void writeToSoundDevice(short buf[], int buffSize, int deviceID) {
   int status;
   status = write(deviceID, buf, buffSize);
   if (status != buffSize)
      perror("Wrote wrong number of bytes\n");
   status = ioctl(deviceID, SOUND_PCM_SYNC, 0);
   if (status == -1)
      perror("SOUND_PCM_SYNC failed\n");
}
int getScaleType(){
   int input;
   cout
      << "Choose one of the following:\n"
      << "\t" << MAJOR << ") for major\n"
      << "\t" << MINOR << ") for minor\n"
      << "\t" << BACK << ") to back up\n"
      << "\tor type quit to quit\n";
   input = getInput();
   return input;
}
void playScale(int deviceID){
   int arraySize, note, steps, index, scaleType;
   int break1; // only one half step to here
   int break2; // only one half step to here
   int t, off;
   double f;
   short *buf;
   arraySize = 8 * LENGTH * RATE * CHANNELS; 
   buf = new short[arraySize];
   while ((index = getIndex()) < 0)
      cout << "Input out of bounds.  Please try again.\n";
   f = getFreq(index);
   while ((scaleType = getScaleType()) < 0)
      cout << "Input out of bounds.  Please try again.\n";
   switch (scaleType) {
      case MAJOR :
         break1 = 3;
         break2 = 7;
         break;
      case MINOR :
         break1 = 2;
         break2 = 5;
         break;
      case BACK :
         return;
      default :
         playScale(deviceID);
   }
   arraySize = LENGTH * RATE * CHANNELS;
   for (note = off = 0; note < 8; ++note, off += t) {
      if (note == 0)
         steps = 0;
      else if (note == break1 || note == break2)
         steps = 1;
      else steps = 2;
      f *= pow(SEMITONE, steps);
      for (t = 0; t < arraySize; ++t)
         buf[t + off] = floor(SAMPLE_MAX*sin(2*PI*f*t/RATE));
   }
   arraySize = 8 * LENGTH * RATE * SIZE * CHANNELS;
   writeToSoundDevice(buf, arraySize, deviceID);
   delete buf;
   return;
}
int main(){
   int deviceID, arg, status, index;
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
      perror("Unable to set sampling rate\n");
   while (true)
            playScale(deviceID);
}
