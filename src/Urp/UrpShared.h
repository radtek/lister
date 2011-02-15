#ifndef _Urp_UrpShared_h_
#define _Urp_UrpShared_h_

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
#include <DropGrid/DropGrid.h>
#include <SqlCtrl/SqlCtrl.h>
#include <CtrlCore/CtrlCore.h>
#include <plugin/ftp/ftp.h>
#include <plugin/ftp/lib/ftplib.h>
#include <Web/Web.h>
#include <DropGrid/DropGrid.h>

#define CAT String().Cat()

using namespace Upp;

// A popup for inputing a string
#define LAYOUTFILE <lister/Urp/Urp.lay>
#include <CtrlCore/lay.h>

// The following String functions have to be segregated from the templates due to compiler errors
// Compiler confuses these with T, T2 args, but won't use those functions!
namespace More {
	bool In(String i, const char *in1);
	bool In(String i, const char *in1, const char *in2);
	bool In(String i, const char *in1, const char *in2, const char *in3);
	bool In(String i, const char *in1, const char *in2, const char *in3, const char *in4);
};
template <typename T>
bool In(T i, T in1, T in2) { if (i == in1 || i == in2) return true;	return false; }

template <typename T, typename T2>
bool In(T i, T2 in1, T2 in2) { if (i == in1 || i == in2) return true;	return false; }

bool In(int i, int in1, int in2);

template <typename T>
bool In(T i, T in1, T in2, T in3) {	if (i == in1 || i == in2 || i == in3) return true; return false; }

template <typename T, typename T2>
bool In(T i, T2 in1, T2 in2, T2 in3) { if (i == in1 || i == in2 || i == in3) return true; return false; }

bool In(int i, int in1, int in2, int in3);

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4) { if (i == in1 || i == in2 || i == in3 || i == in4) return true; return false; }

bool In(int i, int in1, int in2, int in3, int in4);

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5) { if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5) return true; return false; }

bool In(int i, int in1, int in2, int in3, int in4, int in5);

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6) { if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6) return true; return false; }

bool In(int i, int in1, int in2, int in3, int in4, int in5, int in6);

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6, T in7) { if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6 || i == in7) return true; return false; }
bool In(int i, int in1, int in2, int in3, int in4, int in5, int in6, int in7);

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6, T in7, T in8) { if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6 || i == in7 || i == in8) return true; return false; }
bool In(int i, int in1, int in2, int in3, int in4, int in5, int in6, int in7, int in8);

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6, T in7, T in8, T in9) { if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6 || i == in7 || i == in8 || i == in9) return true; return false; }
bool In(int i, int in1, int in2, int in3, int in4, int in5, int in6, int in7, int in8, int in9);

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6, T in7, T in8, T in9, T in10) { if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6 || i == in7 || i == in8 || i == in9 || i == in10) return true; return false; }
bool In(int i, int in1, int in2, int in3, int in4, int in5, int in6, int in7, int in8, int in9, int in10);

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6, T in7, T in8, T in9, T in10, T in11) { if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6 || i == in7 || i == in8 || i == in9 || i == in10 || i == in11) return true; return false; }
bool In(int i, int in1, int in2, int in3, int in4, int in5, int in6, int in7, int in8, int in9, int in10, int in11);

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6, T in7, T in8, T in9, T in10, T in11, T in12) { if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6 || i == in7 || i == in8 || i == in9 || i == in10 || i == in11 || i == in12) return true; return false; }
bool In(int i, int in1, int in2, int in3, int in4, int in5, int in6, int in7, int in8, int in9, int in10, int in11, int in12);

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6, T in7, T in8, T in9, T in10, T in11, T in12, T in13) { if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6 || i == in7 || i == in8 || i == in9 || i == in10 || i == in11 || i == in12 || i == in13) return true; return false; }
bool In(int i, int in1, int in2, int in3, int in4, int in5, int in6, int in7, int in8, int in9, int in10, int in11, int in12, int in13);

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6, T in7, T in8, T in9, T in10, T in11, T in12, T in13, T in14) { if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6 || i == in7 || i == in8 || i == in9 || i == in10 || i == in11 || i == in12 || i == in13 || i == in14) return true; return false; }
bool In(int i, int in1, int in2, int in3, int in4, int in5, int in6, int in7, int in8, int in9, int in10, int in11, int in12, int in13, int in14);


bool None(bool b1, bool b2);
bool None(bool b1, bool b2, bool b3);
bool None(bool b1, bool b2, bool b3, bool b4);
bool None(bool b1, bool b2, bool b3, bool b4, bool b5);

#endif
