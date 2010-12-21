#ifndef _lister_MyTool002_SoundHandler_h_
#define _lister_MyTool002_SoundHandler_h_
/*
					MessageBeep(MB_ICONINFORMATION); // 40
					MessageBeep(MB_ICONASTERISK); //40
					MessageBeep(MB_ICONEXCLAMATION); //30, Be-dunk!
					MessageBeep(MB_ICONQUESTION); // 20, silent
					MessageBeep(MB_ICONSTOP); // 10, Horrible crash!
					MessageBeep(MB_OK); // 00 -- Quiet, nice
					
*/

#include <CtrlLib/CtrlLib.h>

#include <mmsystem.h> // PlaySound

#define SOUND_TOAD "C:\\TOAD\\toadload.wav"
#define SOUND_SWISH "C:\\Documents and Settings\\nbkwdkv\\My Documents\\Visual Studio 2008\\Projects\\ora\\92010NT_CLT\\autorun\\click.wav"
#define SOUND_DOINK "C:\\blp\\Wintrv\\gecko\\res\\samples\\test.wav"
#define SOUND_BWZZZ "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\fs_asterisk.wav"
#define SOUND_TWDLZP "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\C801.wav"
#define SOUND_WCHITA "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\C802.wav"
#define SOUND_WEEWEE "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\C804.wav"
#define SOUND_NYANYANOOW "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\C807.wav"
#define SOUND_BOODOOP "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\C810.wav"
#define SOUND_DDDP "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\C813.wav"
#define SOUND_YADEEDAH "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\C825.wav"
#define SOUND_BZZZZDT "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\C826.wav"
#define SOUND_BRRDEET "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\C827.wav"
#define SOUND_ZEEZEEZEET "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\C828.wav"
#define SOUND_DEEDOP "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\C829.wav"
#define SOUND_LASER "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Laser SFX.wav"
#define SOUND_CLINK "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Clink.wav"
#define SOUND_DIT "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Dit.wav"
#define SOUND_GLISS "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Gliss.wav"
#define SOUND_LASER2 "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Laser.wav"
#define SOUND_LOWWARP "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Lownote.wav"
#define SOUND_WHIZ "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Whiz-1.wav"
#define SOUND_WHIZ2 "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Whiz-2.wav"
#define SOUND_PLUCK "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Pluck.wav"
#define SOUND_LANG "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\1note7.wav"
#define SOUND_BWAIOP "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\1note4.wav"
#define SOUND_LOWBWAIOP "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\1note2.wav"
#define SOUND_BEAT "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\1beat2.wav"
#define SOUND_LOWBEAT "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\1beat1.wav"
#define SOUND_BELL "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Ding2.wav"
#define SOUND_BERP "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Hoot.wav"
#define SOUND_LONGDING "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Longding.wav"
#define SOUND_RINGNOTE "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Ringnote.wav"
#define SOUND_QUICKDING "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Quikding.wav"
#define SOUND_SYNTH "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Synth-3.wav"
#define SOUND_WHISTLE "C:\\Documents and Settings\\nbkwdkv\\Desktop\\MyTool\\Sounds\\Whistle.wav"

enum EnumEventSound { EVS_CONNECT_SUCCESSFUL, EVS_CONNECT_FAILED, EVS_EXECUTE_COMPLETED
	, EVS_EXECUTE_FAILED };

//==========================================================================================	
void Speak(EnumEventSound enumEventSound) {
	switch (enumEventSound) {
		case EVS_CONNECT_SUCCESSFUL:
			PlaySound(SOUND_BWZZZ, NULL, SND_ASYNC|SND_NODEFAULT);
			break;
		case EVS_EXECUTE_COMPLETED:
			PlaySound(SOUND_TWDLZP, NULL, SND_ASYNC|SND_NODEFAULT);
			break;
		case EVS_EXECUTE_FAILED:
			PlaySound(SOUND_CLINK, NULL, SND_ASYNC|SND_NODEFAULT);
			break;
		default:
			MessageBeep(MB_OK);
	}
}
#endif
