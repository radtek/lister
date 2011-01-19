#include "Urp.h"

//==============================================================================================
/*virtual*/ void UrpFxdWdthFntDisplay::Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
		           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
{
	Font newfnt(FontZ(Font::COURIER, 11));
	GridDisplay::Paint(w, x, y, cx, cy, val, style, fg, bg, newfnt, found, fs, fe);
}

//==============================================================================================
/*virtual*/ void TightFontDisplayForGridCtrl::Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
		           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
{
	Font newfnt(FontZ(Font::COURIER, 9));
	GridDisplay::Paint(w, x, y, cx, cy, val, style, fg, bg, newfnt, found, fs, fe);
}

//==============================================================================================
/*virtual*/ void TightFontDisplayForArrayCtrl::Paint(Draw& w, const Rect& r, const Value& q,
	               Color ink, Color paper, dword style) const
{
	Font fnt(FontZ(Font::COURIER, 9));
	String txt = q;
	w.DrawRect(r, paper);
	w.DrawText(r.left + 2, r.top + (r.Height() - GetTextSize(txt, fnt).cy) / 2, txt, fnt, ink); // <- Change is here - "w." no more needed
}



////==============================================================================================
///*virtual*/ void DispPM::Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
//		           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
//{
//	if(!val.IsNull())
//	{
//		int t = int(val) < 0 ? 0 : 1;
////		SetCenterImage(t ? Images::Plus : Images::Minus);
//	}
//	else
//		SetCenterImage(Null);
//	GridDisplay::Paint(w, x, y, cx, cy, Value(""), style, fg, bg, fnt, found, fs, fe);
//}
//
////==============================================================================================

//bool NumbersOnRed::numberString(String s)
//{
//  if(s.GetCount() == 0)
//    return false;
//  
//  int i = 0; 
//  if(s[0] == '+' || s[0] == '-')
//    i++;  
//  
//  while(i < s.GetCount() && s[i] >= '0' && s[i] <= '9')
//    i++;
//  
//  return i == s.GetCount();
//}
//
////==============================================================================================
//void NumbersOnRed::PaintBackground(Draw& w, const Rect& r, const Value& q,
//	                               Color ink, Color paper, dword style) const
//{
//	if(IsNumber(q) || (IsString(q) && numberString(AsString(q))))
//	  paper = Color(255, 150, 150);
//	
//	Display::PaintBackground(w, r, q, ink, paper, style);
//}


/*
struct FontFaceDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const
	{
		Font fnt = Font(q, r.Height() - 2);
		String txt = Font::GetFaceName(q);
		w.DrawRect(r, paper);
		w.DrawText(r.left + 2, r.top + (r.Height() - GetTextSize(txt, fnt).cy) / 2, txt, fnt, ink); // <- Change is here - "w." no more needed
	}

*/
	
//		Add(dl.HSizePos().TopPos(5, Ctrl::STDSIZE));
//		dl.SetDisplay(Single<FontFaceDisplay>());
//		for(int i = 0; i < Font::GetFaceCount(); i++)
//			dl.Add(i);
//		SetRect(0, 0, 200, 70);
	
//};


/*
class DisplayMoney : public Display {
public:
	void Paint(Draw& w, const Rect& r, const Value& q,
		       Color ink, Color paper, dword style) const;

protected:
	Font   font;
	bool   red;

public:
	static DisplayMoney Normal;
	static DisplayMoney Bold;
	static DisplayMoney NormalRed;


	DisplayMoney& SetFont(Font _font)  { font = _font; return *this; }
	DisplayMoney& Red(bool b)          { red = b; return *this; }
	DisplayMoney& NoRed()              { return Red(false); }

	DisplayMoney(Font font = GuiFont(), bool red = true) : font(font), red(red) {}
};
*/

/*
void DisplayMoney::Paint(Draw& w, const Rect& r, const Value& q,
						 Color ink, Color paper, dword s) const {
	String text;
	if(!IsNull(q)) text = FormatMoney(q);
	int cx = GetSmartTextSize(w, text, font).cx;
	w.DrawRect(r, paper);
	DrawSmartText(w, r.left + r.Width() - cx, r.top, r.Width(), text, font,
		          double(q) < 0 ? (s & CURSOR) && (s & FOCUS) ? SLtRed: SRed : ink);
}

inline Font GuiFont() { return StdFont(); }

String FormatMoney(double money) {
	return IsNull(money) ? EmptyString : Format("%.2f", money);
}
*/

/*
String CommaFormatter(const Formatting& f)
{
  int    commaSpacing = 3;  // fix at 3 for now
  String res          = Format( "%" + f.format + "f", f.arg );  // replace our format with "f"
  int    period       = res.Find( '.' );
  int    idx          = (period<0) ? res.GetLength() : period;
  while( (idx-=commaSpacing)>0 )
    res.Insert( idx, ',' );	
  return res;
}

// Test it
RegisterNumberFormatter("C",  CommaFormatter);
String test = Format( "%.2C", 35829123.4253 );
// 35,829,123.43
*/


/*
void GridDisplay::Paint(..)
{
if(parent->Get(MYCOL) == 1)
bg = Red;
else
bg = White;
}
*/

/*
class ColoredStatus : public GridDisplay
{
public:
    void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
				           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe);
	
};

void ColoredStatus::Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
				           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
{
    if( AsString(val) == "1" )	bg = Color(0, 255, 0);
    else bg = Color(255, 0, 0);	
    GridDisplay::Paint(w, x, y, cx, cy, val, style, fg, bg, fnt, found, fs, fe);
} 
...// then set the column
	arrPlayer.AddColumn("Status", t_("Status"), 4).Edit(optStatusP).SetDisplay(Single<ColoredStatus>());   
...
*/



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
	


	char *font_name = "";
	GOBJ().g_object_get(GTK().gtk_settings_get_default(), "gtk-font-name", &font_name, NULL);
	int xdpi;
	GOBJ().g_object_get(GTK().gtk_settings_get_default(), "gtk-xft-dpi", &xdpi, NULL);

	int fontname = Font::ARIAL;
	int fontheight = 13;
	
	const char *q = strrchr(font_name, ' ');
	if(q) {
		int h = atoi(q);
		if(h)
			fontheight = h;
		String face(font_name, q);
		if(Font::FindFaceNameIndex(face) < 0)
			if(ToUpper(face[0]) == 'M')
				fontname = Font::COURIER;
			else
			if(ToUpper(face[0]) == 'S' && ToUpper(face[1]) == 'e')
				fontname = Font::ROMAN;
	}
		
	Draw::SetStdFont(Font(fontname, (fontheight * xdpi) / (1024*72)));	

	ChLookFn(GtkLookFn);
	
*/
