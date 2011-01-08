#include "ConnButton.h"

ConnButton::ConnButton() : Button() {
	WhenPush = THISBACK(Connect); // Forces this to be a nested class
	wasClicked = false;
}

// Only called if we use WhenCtrlsAction, NOT WhenCtrlAction.	
void ConnButton::Connect() {
	wasClicked = true;
}

