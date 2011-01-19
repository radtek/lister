#ifndef _lister_lister_KeyHandler_h_
#define _lister_lister_KeyHandler_h_

// Specific key scan codes from my emachines keyboard: Model # KB-0108 version CV Rev.L

// Taken using http://www.barcodeman.com/altek/mule/kbemulator/

// Not defined in CtrlCore.h
#define K_ALT_CTRL       K_ALT|K_CTRL
#define K_ALT_SHIFT      K_ALT|K_SHIFT
#define K_ALT_SHIFT_CTRL K_ALT|K_SHIFT|K_CTRL
#define K_ALT_CTRL_F8    K_ALT_CTRL|K_F8


/*
Left-Ctrl-down	'14'               # Keynumber 58 down.
Left-Ctrl-up	'F014'             # Keynumber 58 up.
Right-Ctrl-down	'E014'             # Keynumber 64 down.
Right-Ctrl-up	'E0F014'           # Keynumber 64 up.
Sleep-down	'E03F'             # Keynumber 131 down.
Sleep-up	'E0F03F'           # Keynumber 131 up.
Wake-down	'E05E'             # Keynumber 132 down.
Wake-up	'E0F05E'           # Keynumber 132 up.
End-down	'E069'             # Keynumber 81 down. (Numlock:off. Shift:no)
End-up	'E0F069'           # Keynumber 81 up. (Numlock:off. Shift:no)
Numpad-Ins-down	'70'               # Keynumber 99 down.
Numpad-Ins-up	'F070'             # Keynumber 99 up.
Numpad-Enter-down	'E05A'             # Keynumber 108 down.
Numpad-Enter-up	'E0F05A'           # Keynumber 108 up.
Power-down	'E037'             # Keynumber 130 down.
Power-up	'E0F037'           # Keynumber 130 up.
Capslock-down	'58'               # Keynumber 30 down.
Capslock-up	'F058'             # Keynumber 30 up.
Numpad-/-down	'E04A'             # Keynumber 95 down. (Shift:no)
Numpad-/-up	'E0F04A'           # Keynumber 95 up. (Numlock:off. Shift:no)
Midpad-home-down	'E06C'             # Keynumber 80 down. (Numlock:off. Shift:no)
Midpad-home-up	'E0F06C'           # Keynumber 80 up. (Numlock:off. Shift:no)
Numpad-home-down	'6C'               # Keynumber 91 down.
Numpad-home-up	'F06C'             # Keynumber 91 up.
Printscrn-down	'E012E07C'         # Keynumber 124 down. (Shift, Cntrl and Alt are up)
Printscrn-up	'E0F07CE0F012'     # Keynumber 124 down. (Shift, Cntrl and Alt are up)
Scrolllock-down	'46'               # Keynumber 125 down.
Scrolllock-up	'F046'             # Keynumber 125 up.
Pausebreak-down	'E11477E1F014F077' # Keynumber 126 down. (Both cntrl keys are up)
Pausebreak-up	                   # Keynumber 126 up (no scancode).
Menu-down	'E02F'             # Keynumber 129 down.
Menu-up	'E0F02F'           # Keynumber 129 up.
Right-windows-down	'E027'             # Keynumber 128 down.
Right-windows-up	'E0F027'           # Keynumber 128 up.
Right-Alt-down	'E011'             # Keynumber 62 down.
Right-Alt-up	'E0F011'           # Keynumber 62 up.
Left-Alt-down	'11'               # Keynumber 60 down.
Left-Alt-up	'F011'             # Keynumber 60 up.
Left-windows-down	'E01F'             # Keynumber 127 down.
Left-windows-up	'E0F01F'           # Keynumber 127 up.
Left-shift-down	'12'               # Keynumber 44 down.
Left-shift-up	'F012'             # Keynumber 44 up.
Right-shift-down	'59'               # Keynumber 57 down.
Right-shift-up	'F059'             # Keynumber 57 up.
*/

#endif