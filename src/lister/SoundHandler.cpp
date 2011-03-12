#include "SoundHandler.h"

/*
					MessageBeep(MB_ICONINFORMATION); // 40
					MessageBeep(MB_ICONASTERISK); //40
					MessageBeep(MB_ICONEXCLAMATION); //30, Be-dunk!
					MessageBeep(MB_ICONQUESTION); // 20, silent
					MessageBeep(MB_ICONSTOP); // 10, Horrible crash!
					MessageBeep(MB_OK); // 00 -- Quiet, nice
					
*/

#include <CtrlLib/CtrlLib.h>
#include "shared.h"

#include <mmsystem.h> // PlaySound
//#include <stdio.h>

/*
 * some good values for block size and count
 */
#define BLOCK_SIZE 8192
#define BLOCK_COUNT 20
/*
 * function prototypes
 */ 
static void CALLBACK waveOutProc(HWAVEOUT, UINT, DWORD, DWORD, DWORD);
static WAVEHDR* allocateBlocks(int size, int count);
static void freeBlocks(WAVEHDR* blockArray);
static void writeAudio(HWAVEOUT hWaveOut, LPSTR data, int size);
/*
 * module level variables
 */
static CRITICAL_SECTION waveCriticalSection;
static WAVEHDR* waveBlocks;
static volatile int waveFreeBlockCount;
static int waveCurrentBlock;

#define SOUND_TOAD "C:/TOAD/toadload.wav"
#define SOUND_SWISH "C:/Documents and Settings/nbkwdkv/My Documents/Visual Studio 2008/Projects/ora/92010NT_CLT/autorun/click.wav"
#define SOUND_DOINK "C:/blp/Wintrv/gecko/res/samples/test.wav"
//#define SOUND_BWZZZ "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/fs_asterisk.wav"
#define SOUND_WCHITA "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/C802.wav"
#define SOUND_TWEEYAH "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/C809.wav"
#define SOUND_BOODOOP "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/C810.wav"
#define SOUND_DDDP "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/C813.wav"
#define SOUND_LASER "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Laser SFX.wav"
#define SOUND_DIT "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Dit.wav"
#define SOUND_GLISS "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Gliss.wav"
#define SOUND_LASER2 "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Laser.wav"
#define SOUND_LOWWARP "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Lownote.wav"
#define SOUND_WHIZ "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Whiz-1.wav"
#define SOUND_WHIZ2 "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Whiz-2.wav"
#define SOUND_PLUCK "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Pluck.wav"
#define SOUND_LANG "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/1note7.wav"
#define SOUND_BWAIOP "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/1note4.wav"
#define SOUND_LOWBWAIOP "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/1note2.wav"
#define SOUND_BEAT "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/1beat2.wav"
#define SOUND_LOWBEAT "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/1beat1.wav"
#define SOUND_BELL "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Ding2.wav"
#define SOUND_LONGDING "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Longding.wav"
#define SOUND_RINGNOTE "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Ringnote.wav"
#define SOUND_QUICKDING "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Quikding.wav"
#define SOUND_SYNTH "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Synth-3.wav"
#define SOUND_WHISTLE "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Whistle.wav"

//=========== Sounds in use ==================
// Created by Windows XP Sound Recorder from 11Khz-16-stereo.  Same pitch, but pops at the beginning.  May be player.  More hiss added.
#define SOUND_BWZZZ  "C:/Tools/fs_asterisk.raw.44-16-stereo.wav" // Nasty pop
#define SOUND_TWDLZP "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/C801.wav"
#define SOUND_BERP   "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Hoot.wav"
#define SOUND_CLINK  "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Clink.wav"
#define SOUND_DRESSERDRAWERCLOSE   "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/Dresser Drawer Close 01.wav"
#define SOUND_WOOP   "C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds/fs_menu_command.wav"

//"C:/Documents and Settings/nbkwdkv/Desktop/MyTool/Sounds
// WAVPCM
// Wav to raw PCM converter
// Written by WolfCoder(2006)

#define WIN32_LEAN_AND_MEAN
#include <windows.h> // Use windows
#include <mmsystem.h> // Use multimedia system
#include <iostream> // Use console IO
using namespace std;

//void main(int argn,char *argv[])
//==============================================================================================
void convwavtoraw(String wav)
{
     // Show header
     cout<<"WAV to PCM converter by WolfCoder (2006).\n";
//     if(argn == 1)
//     {
//          cout<<"Usage: wavpcm wavefile.pcm\n";
//          return;
//     }
//     for(int index = 0;index < argn-1;index++)
	 for (int i = 0; i < 1;i++)
     {
          cout<<"Processing "<<wav<<":";
          // Prepare for a WAV file read
          HMMIO hwav;
          MMCKINFO parent,child;
          UCHAR *sound_buffer;
          WAVEFORMATEX wf;
          char outwav[MAX_PATH];
          int outwav_pos = 0;
          HANDLE hpcm;
          DWORD written;
          // Set chunk info
          parent.ckid = (FOURCC)0;
          parent.cksize = 0;
          parent.fccType = (FOURCC)0;
          parent.dwDataOffset = 0;
          parent.dwFlags = 0;
          // Copy to child
          child = parent;
          // Open the WAV file
          if((hwav = mmioOpen((LPSTR)(const char *)wav,NULL,MMIO_READ | MMIO_ALLOCBUF)) == NULL)
          {
               cout<<" FAILED.\nCould not open the WAV file.\n";
               return;
          }
          // Descend into the RIFF
          child.ckid = mmioFOURCC('W','A','V','E');
          if(mmioDescend(hwav,&parent,NULL,MMIO_FINDRIFF))
          {
               mmioClose(hwav,0);
               cout<<" FAILED.\nWAV file section not found.\n";
               return;
          }
          // Descend into the format
          child.ckid = mmioFOURCC('f','m','t',' ');
          if(mmioDescend(hwav,&child,&parent,0))
          {
               mmioClose(hwav,0);
               cout<<"FAILED.\nWAV format not found.\n";
               return;
          }
          // Read WAV format
          if(mmioRead(hwav,(char *)&wf,sizeof(wf)) != sizeof(wf))
          {
               mmioClose(hwav,0);
               cout<<"FAILED.\nWAV file section couldn't be read.\n";
               return;
          }
          // Check for PCM
          if(wf.wFormatTag != WAVE_FORMAT_PCM)
          {
               mmioClose(hwav,0);
               cout<<"FAILED.\nNot a PCM file.\n";
               return;
          }
          // Go up a level
          if(mmioAscend(hwav,&child,0))
          {
               mmioClose(hwav,0);
               cout<<"FAILED.\nBad WAV file.\n";
               return;
          }
          // Descend to data
          child.ckid = mmioFOURCC('d','a','t','a');
          if(mmioDescend(hwav,&child,&parent,MMIO_FINDCHUNK))
          {
               mmioClose(hwav,0);
               cout<<"FAILED.\nWAV file data missing.\n";
               return;
          }
          // Proceed to extract raw WAV data
          sound_buffer = new UCHAR[child.cksize];
          mmioRead(hwav,(char *)sound_buffer,child.cksize);
          mmioClose(hwav,0);
          // Verify WAV file
          cout<<" Hz="<<wf.nSamplesPerSec<<", BitDepth="<<wf.wBitsPerSample<<", Duration="<<child.cksize<<" bytes. (~";
          cout<<(child.cksize/1024)<<"KB)\n";
          // Get a new file name
          ZeroMemory(&outwav[0],sizeof(outwav));
          strcpy(&outwav[0],(const char *)wav);
          outwav[strlen(&outwav[0])-3] = 'p';
          outwav[strlen(&outwav[0])-2] = 'c';
          outwav[strlen(&outwav[0])-1] = 'm';
          // Write the data
          hpcm = CreateFile(&outwav[0],GENERIC_WRITE,NULL,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
          WriteFile(hpcm,&sound_buffer[0],child.cksize,&written,NULL);
          CloseHandle(hpcm);
     }
}



static void CALLBACK waveOutProc(
	HWAVEOUT hWaveOut, 
	UINT uMsg, 
	DWORD dwInstance, 
	DWORD dwParam1,
	DWORD dwParam2 
	)
{
/*
 * pointer to free block counter
 */
int* freeBlockCounter = (int*)dwInstance;
/*
 * ignore calls that occur due to openining and closing the
 * device.
 */
if(uMsg != WOM_DONE)
return;
EnterCriticalSection(&waveCriticalSection);
(*freeBlockCounter)++;
LeaveCriticalSection(&waveCriticalSection);
}

WAVEHDR* allocateBlocks(int size, int count) {
unsigned char* buffer;
int i;
WAVEHDR* blocks;
DWORD totalBufferSize = (size + sizeof(WAVEHDR)) * count;
/*
 * allocate memory for the entire set in one go
 */
if((buffer = (unsigned char *)HeapAlloc(
	GetProcessHeap(), 
	HEAP_ZERO_MEMORY, 
	totalBufferSize
)) == NULL) {
fprintf(stderr, "Memory allocation error\n");
ExitProcess(1);
}
/*
 * and set up the pointers to each bit
 */
blocks = (WAVEHDR*)buffer;
buffer += sizeof(WAVEHDR) * count;
for(i = 0; i < count; i++) {
	blocks[i].dwBufferLength = size;
	blocks[i].lpData = (LPSTR)buffer;
	buffer += size;
}
return blocks;
}

//==============================================================================================
void freeBlocks(WAVEHDR* blockArray) {
	/* 
	 * and this is why allocateBlocks works the way it does
	 */ 
	HeapFree(GetProcessHeap(), 0, blockArray);
}

//==============================================================================================
void writeAudio(HWAVEOUT hWaveOut, LPSTR data, int size) {
	WAVEHDR* current;
	int remain;
	current = &waveBlocks[waveCurrentBlock];
	while(size > 0) {
	/* 
	 * first make sure the header we're going to use is unprepared
	 */
	if(current->dwFlags & WHDR_PREPARED) 
	waveOutUnprepareHeader(hWaveOut, current, sizeof(WAVEHDR));
	if(size < (int)(BLOCK_SIZE - current->dwUser)) {
	memcpy(current->lpData + current->dwUser, data, size);
	current->dwUser += size;
	break;
	}
	remain = BLOCK_SIZE - current->dwUser;
	memcpy(current->lpData + current->dwUser, data, remain);
	size -= remain;
	data += remain;
	current->dwBufferLength = BLOCK_SIZE;
	waveOutPrepareHeader(hWaveOut, current, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, current, sizeof(WAVEHDR));
	EnterCriticalSection(&waveCriticalSection);
	waveFreeBlockCount--;
	LeaveCriticalSection(&waveCriticalSection);
	/*
	 * wait for a block to become free
	 */
	while(!waveFreeBlockCount)
	Sleep(10);
	/*
	 * point to the next block
	 */
	waveCurrentBlock++;
	waveCurrentBlock %= BLOCK_COUNT;
	current = &waveBlocks[waveCurrentBlock];
	current->dwUser = 0;
	}
	}
	
//==============================================================================================
int PlaySound2(String soundFile) {
	HWAVEOUT hWaveOut; /* device handle */
	HANDLE hFile;/* file handle */
	WAVEFORMATEX wfx; /* look this up in your documentation */
	char buffer[1024]; /* intermediate buffer for reading */
	int i;
	/*
	 * initialise the module variables
	 */ 
	waveBlocks = allocateBlocks(BLOCK_SIZE, BLOCK_COUNT);
	waveFreeBlockCount = BLOCK_COUNT;
	waveCurrentBlock= 0;
	InitializeCriticalSection(&waveCriticalSection);
	/*
	 * try and open the file
	 */ 
	if((hFile = CreateFile(
		soundFile,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	)) == INVALID_HANDLE_VALUE) {
		//fprintf(stderr, "%s: unable to open file '%s'\n", argv[0], argv[1]);
		
		DeleteCriticalSection(&waveCriticalSection);
		freeBlocks(waveBlocks);
		return -1;
	}
	/*
	 * set up the WAVEFORMATEX structure.
	 */
	wfx.nSamplesPerSec = 44100; /* sample rate */
	wfx.wBitsPerSample = 16; /* sample size */
	wfx.nChannels= 2; /* channels*/
	wfx.cbSize = 0; /* size of _extra_ info */
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
	/*
	 * try to open the default wave device. WAVE_MAPPER is
	 * a constant defined in mmsystem.h, it always points to the
	 * default wave device on the system (some people have 2 or
	 * more sound cards).
	 */
	 
	// This will fail if Windows Audio service not started; due to error during startup
	if(waveOutOpen(
		&hWaveOut, 
		WAVE_MAPPER, 
		&wfx, 
		(DWORD_PTR)waveOutProc, 
		(DWORD_PTR)&waveFreeBlockCount, 
		CALLBACK_FUNCTION
		) != MMSYSERR_NOERROR) {
		//fprintf(stderr, "%s: unable to open wave mapper device\n", argv[0]);
		DeleteCriticalSection(&waveCriticalSection);
		freeBlocks(waveBlocks);
		CloseHandle(hFile);
		return -2;
	}
	/*
	 * playback loop
	 */
	while(1) {
		DWORD readBytes;
		if(!ReadFile(hFile, buffer, sizeof(buffer), &readBytes, NULL))
		break;
		if(readBytes == 0)
		break;
		if(readBytes < sizeof(buffer)) {
			printf("at end of buffer\n");
			memset(buffer + readBytes, 0, sizeof(buffer) - readBytes);
			printf("after memcpy\n");
		}
		writeAudio(hWaveOut, buffer, sizeof(buffer));
	}
	/*
	 * wait for all blocks to complete
	 */
	while(waveFreeBlockCount < BLOCK_COUNT)
		Sleep(10);
	/*
	 * unprepare any blocks that are still prepared
	 */
	for(i = 0; i < waveFreeBlockCount; i++) 
		if(waveBlocks[i].dwFlags & WHDR_PREPARED)
			waveOutUnprepareHeader(hWaveOut, &waveBlocks[i], sizeof(WAVEHDR));
	
	DeleteCriticalSection(&waveCriticalSection);
	freeBlocks(waveBlocks);
	waveOutClose(hWaveOut);
	CloseHandle(hFile);
	return 0;
}

//==============================================================================================
//waveOut... API 
//DirectSound
void Speak(EnumEventSound enumEventSound) {
	int rtnval;
	
	switch (enumEventSound) {
		case EVS_CONNECT_SUCCEEDED:
			//PlaySound(SOUND_BWZZZ, NULL, SND_ASYNC|SND_NODEFAULT);
			rtnval = PlaySound2(SOUND_BWZZZ);
			break;
		case EVS_CONNECT_FAILED:
			PlaySound(SOUND_BERP, NULL, SND_ASYNC|SND_NODEFAULT);
			break;
		case EVS_EXECUTE_SUCCEEDED:
			PlaySound(SOUND_TWDLZP, NULL, SND_ASYNC|SND_NODEFAULT);
			break;
		case EVS_EXECUTE_FAILED:
			PlaySound(SOUND_CLINK, NULL, SND_ASYNC|SND_NODEFAULT);
			break;
		case EVS_TEST_SUCCEEDED:
			PlaySound(SOUND_TWDLZP, NULL, SND_ASYNC|SND_NODEFAULT);
			break;
		case EVS_TEST_FAILED:
			PlaySound(SOUND_CLINK, NULL, SND_ASYNC|SND_NODEFAULT);
			break;
		case EVS_FETCH_COMPLETED:
			PlaySound(SOUND_DRESSERDRAWERCLOSE, NULL, SND_ASYNC|SND_NODEFAULT);
			break;
		case EVS_INSERT_COMPLETED:
			PlaySound(SOUND_WOOP, NULL, SND_ASYNC|SND_NODEFAULT);
			break;
		case EVS_INSERT_FAILED:
			PlaySound(SOUND_CLINK, NULL, SND_ASYNC|SND_NODEFAULT);
			break;
		case EVS_SAVE_SUCCEEDED:
			PlaySound(SOUND_QUICKDING, NULL, SND_ASYNC|SND_NODEFAULT);
			break;
		default:
			MessageBeep(MB_OK);
	}
}
