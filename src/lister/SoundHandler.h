#ifndef _SoundHandler_h_
#define _SoundHandler_h_

enum EnumEventSound { EVS_CONNECT_SUCCEEDED, EVS_CONNECT_FAILED, EVS_EXECUTE_SUCCEEDED
	, EVS_EXECUTE_FAILED, EVS_TEST_SUCCEEDED, EVS_TEST_FAILED };

//==========================================================================================	
//waveOut... API 
//DirectSound
void Speak(EnumEventSound enumEventSound);
#endif

