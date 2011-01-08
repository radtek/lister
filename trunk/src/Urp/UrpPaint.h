#ifndef _Urp_UrpPaint_h_
#define _Urp_UrpPaint_h_

#include "UrpShared.h"

//==============================================================================================
struct UrpFxdWdthFntDisplay : GridDisplay
{
	virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
			           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
	{
		Font newfnt(FontZ(Font::COURIER, 11));
		GridDisplay::Paint(w, x, y, cx, cy, val, style, fg, bg, newfnt, found, fs, fe);
	}
};

/*
	colsopt.theme.Add(-1, "Flat");
	colsopt.theme.Add(0, "Cairo");		
	colsopt.theme.Add(1, "Gray");
	colsopt.theme.Add(2, "Plastik");		
	colsopt.theme.Add(3, "Lipstik");		
	colsopt.theme.Add(4, "Alloy");		
	colsopt.theme.Add(5, "Unknown 01");
	colsopt.theme.Add(6, "Default");

	case 0: grid->GetDisplay().SetTheme(~colsopt.theme);                  break;

	colsopt.odd_color.NoRampWheel();
	colsopt.even_color.NoRampWheel();
	colsopt.grid_color.NoRampWheel().NotNull();
	colsopt.focus_color.NoRampWheel();
	colsopt.live_color.NoRampWheel();

	WithEditOptionsLayout<ParentCtrl> editopt;
	
class NumbersOnRed : public Display
{
public:
  static bool numberString(String s);
	void PaintBackground(Draw& w, const Rect& r, const Value& q,
	                     Color ink, Color paper, dword style) const;
	
};

bool NumbersOnRed::numberString(String s)
{
  if(s.GetCount() == 0)
    return false;
  
  int i = 0; 
  if(s[0] == '+' || s[0] == '-')
    i++;  
  
  while(i < s.GetCount() && s[i] >= '0' && s[i] <= '9')
    i++;
  
  return i == s.GetCount();
}

void NumbersOnRed::PaintBackground(Draw& w, const Rect& r, const Value& q,
	                               Color ink, Color paper, dword style) const
{
	if(IsNumber(q) || (IsString(q) && numberString(AsString(q))))
	  paper = Color(255, 150, 150);
	
	Display::PaintBackground(w, r, q, ink, paper, style);
}

	
*/

#endif
