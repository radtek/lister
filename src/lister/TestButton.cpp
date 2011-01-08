#include "TestButton.h"

TestButton::TestButton() : Button() {
	WhenPush = THISBACK(BeginTest); // Forces this to be a nested class
	wasClicked = false;
}

// Only called if we use WhenCtrlsAction, NOT WhenCtrlAction.	
void TestButton::BeginTest() {
	wasClicked = true; // TestGrid will pick up
}
