#ifndef _MyTool002_MyTool002_h
#define _MyTool002_MyTool002_h

#include "shared.h"
#include "CursorHandler.h"
#include "ConnHandler.h"
#include "ConnGrid.h"
#include "ConnState.h"
#include "Connection.h"
#include "SqlUtil.h"
#include "MiscUtilities.h"
#include "Err.h"
#include "SoundHandler.h"
#include "TestGrid.h"
#include "MyRichEdit.h"
#include "MyDropGrid.h"
#include "Constants.h"
#include "MySplitter.h"

#include <PostgreSQL/PostgreSQL.h> // Need to move to Connection manager

typedef VectorMap<String,Connection *> ConnVector;

//==========================================================================================	
class MyTool002 : public WithMyTool002Layout<TopWindow> {
public:
	ConnVector connVector;
	PostgreSQLSession controlDb;
	MyDropGrid sqlList;
	MySplitter vertSplitter, horizSplitter;
	StaticRect leftPane, rightPane, bottomPane;
	ToolBar toolbar;
	StatusBar status;
	GridCtrl mainGrid;
	ConnGrid connGrid;
	TestGrid testGrid;
	MyRichEdit cmdScript;
	Connection *activeConn;
	ConnHandler connHandler; // Set from ConnButton->ConnHandler when grid changes so sql execution will know source
	Button executeSQL;
	typedef MyTool002 CLASSNAME;
	enum EnumScreenZoom { ZOOM_NORMALSCREEN, ZOOM_FULLSCREEN, ZOOM_ALLSCREENS };
	EnumScreenZoom enumScreenZoom;
	String configFile;
	
	//==========================================================================================	
	MyTool002() {
		
		activeConn = NULL;
		
		// Main screen
		
		CtrlLayout(*this, "MyTool for SQL Execution");
		Sizeable().Zoomable();

		enumScreenZoom = ZOOM_NORMALSCREEN; // Should take from config
		
		// Vertical Splitter
		
		Add(vertSplitter);
		vertSplitter.Vert(horizSplitter, bottomPane);
		// Set default space reserved for connection list and command script
		vertSplitter.SetPos(2000);
		horizSplitter.Horz(leftPane, rightPane);	
		
		// Horizontal Splitter
		
		horizSplitter.SetPos(3000);
		vertSplitter.Zoom(-1);
		horizSplitter.Zoom(-1);

		// Construct Main Output Grid

		mainGrid.SizePos().HSizePosZ(1, 3).VSizePosZ(1, 2); // 2 bits from bottom
		mainGrid.Description("mainGrid");
		mainGrid.WhenCursor = THISBACK(ToolBarRefresh);
		bottomPane.Add(mainGrid);

		// Construct Execute SQL button

		Add(executeSQL);
		executeSQL.TopPos(2, 19).RightPos(18, 22).SetForeground();
		executeSQL.SetImage(MyImages::lightning_icon16());
		executeSQL.WhenPush = THISBACK(ExecuteSQLActiveConn);

		// Connect to our metadata control database

		if(!controlDb.Open("host=localhost dbname=postgres user=postgres password=postgres")) {
			Exclamation(Format("Error in controlDb open: %s", DeQtf(controlDb.GetLastError())));
			exit(-1);
		}

		// Build a statement object

		Sql controlSql(controlDb);

		// Construct Script List Drop Down Grid

		sqlList.AddColumn("ID", 0);	// Hidden Column
		sqlList.AddColumn(""); // No label so it doesn't show up 
		sqlList.AddColumn("RichScript", 0); // Hidden column
		sqlList.Header(false);
		sqlList.ColorRows();
		//sqlList.  .WhenDrop = THISBACK(SelectedScriptFromDropDown);
		sqlList.SetDropLines(30);
		sqlList.DropEnter();
		sqlList.DataAction();
		sqlList.Searching();
		sqlList.SearchHideRows(); // Very important.  Reduce size as you search, otherwise you have to scroll through non-matches
		sqlList.AddPlus(THISBACK(PushScriptToEditor));
		sqlList.SetKeyColumn(0);
		sqlList.SetFindColumn(1);
		sqlList.AddValueColumn(1);
		int ecy = EditField::GetStdHeight();
		rightPane.Add(sqlList.BottomPos(0, ecy).HSizePos(0, 0)); // Cross entire pane

		if (!controlSql.Execute("select scriptid, script, richscript from scripts order by addtimestamp")) {
			Exclamation(Format("Error: %s", DeQtf(controlSql.GetLastError())));		
		} else {
			while(controlSql.Fetch()) {
				sqlList.Add(controlSql[0], controlSql[1], controlSql[2]);
			}
		}

		// Construct Script Editor

		rightPane.Add(cmdScript.VSizePos(0, ecy).HSizePos(0,0));
		toolbar.Set(THISBACK(MyToolBar));
		toolbar.VSizePos(0, 10);
		rightPane.AddFrame(toolbar);

		// Construct Connection Grid
		
		connGrid.Description("connGrid");
		connGrid.WhenCtrlsAction = THISBACK(ClickedConnect);
		leftPane.Add(connGrid);
		connGrid.Build();
		connGrid.Load(&controlDb, controlSql); // Grid can create connections
		connGrid.outputGrid = &mainGrid;
		connGrid.SizePos();

		configFile = GetHomeDirFile("scriptsure.xml");
		
		// Call all Xmlize functions
		LoadFromXMLFile(*this, configFile);

		// By spinning this off as a callback, we get the screen displayed while autoconnecting, and plus the cursor on conn grid is properly set to center
		//SetTimeCallback(100, THISBACK(AutoConnect));		
	}

	//==========================================================================================	
	~MyTool002() {
		
		// Release all the connection objects to prevent a memory leak
		for (int i = 0; i < connVector.GetCount(); i++) {
			Connection *conn = connVector[i];
			delete conn;
		}
		
		StoreAsXMLFile(*this, NULL, configFile);
	}

	//==========================================================================================	
	void AutoAddTest() {
		
	}
	
	//==========================================================================================	
	void AutoConnect() {
		// Force auto-connect
		
		if (connGrid.FindConnName("CSDR-UAT")) {
			Connect();
		}
	}
	
	//==========================================================================================	
	void ClickedConnect() {
		if (connGrid.WasConnectionRequested()) {
			Connect();
		}
	}
	
	//==========================================================================================	
	void Connect() {
		String connName = connGrid.GetConnName();
		if (connVector.Find(connName) >= 0) {
			activeConn = (connVector.Get(connName));
		} else {
			connGrid.SetConnState(CONNECTING_START);
			WaitCursor wc;
			Connection *conn = new Connection(connGrid.GetConnId(), connGrid.GetConnName());
			ProcessEvents(); // Necessary to display color change immediately
			EnumConnState enumConnState = connHandler.Connect(connGrid.GetInstanceTypeName(), connGrid.GetLoginStr(), connGrid.GetLoginPwd(), connGrid.GetInstanceAddress()
				, *conn);
			
			connGrid.SetConnState(enumConnState);
			conn->enumConnState = enumConnState; // This is a bit unweildly
			
			if (enumConnState == CON_SUCCEED) {
				Speak(EVS_CONNECT_SUCCESSFUL);
				connVector.Add(connName, conn);
				activeConn = conn;
			} else {
				Speak(EVS_CONNECT_FAILED);
				activeConn = NULL;
				// Leave old connection embedded in script??
			}
		}
		
		cmdScript.SetConn(activeConn);
		ToolBarRefresh();
		ConnectionStatusRefresh();
	}

	//==========================================================================================	
	void AddScriptToHistory() {
		String richscript = cmdScript.GetQTF();
		String script = TrimBoth(cmdScript.Get().GetPlainText().ToString());
		if (script.IsEmpty()) {
			return;
		}
		
		Sql controlSql(controlDb);
		// A little trick to only insert into a table new scripts
		String cmd = Format("insert into scripts(richscript, script) select '%s', '%s' from dual left join scripts on 1=1 where script <> '%s' or script is null limit 1"
			, PGFormat(richscript), PGFormat(script), PGFormat(script));

		if (!controlSql.Execute(cmd)) {
			Exclamation(Format("Error: [* \1%s\1].", DeQtf(controlSql.GetLastError())));
			cmdScript.Tip(DeQtf(controlSql.GetLastError()));
			cmdScript.SetData(cmd);
			return;
		}

		// Add to the list if not a dup
		if (controlSql.GetRowsProcessed() > 0) {		
			int scriptid = GetInsertedId(controlSql, "scripts", "scriptid");
			if (scriptid >= 0) {
				sqlList.Add(scriptid, script, richscript);
				cmdScript.scriptId = scriptid; // Link to script so that it can be uploaded to tests
			}
		}
	}
	
	//==========================================================================================	
	void SelectedScriptFromDropDown() {
		sqlList.Tip(sqlList.Get(1).ToString());
	}
	
	//==========================================================================================	
	// User pressed "+" button on script list dropdown
	//==========================================================================================	
	void PushScriptToEditor() {
		cmdScript.SetQTF(sqlList.Get(2).ToString());
		cmdScript.scriptId = sqlList.Get(0);
		ToolBarRefresh();
	}
	
	//==========================================================================================	
	void ToolBarRefresh() {
		toolbar.Set(THISBACK(MyToolBar));
	}

	//==========================================================================================	
	void ConnectionStatusRefresh() {
		if (activeConn) {
			if (!In(activeConn->enumConnState, CON_SUCCEED, CON_STALE)) {
				Title(CAT << "Scriptor - Not connected to " << activeConn->connName);	
			} else {
				Title(CAT << "Scriptor - Connected to " << activeConn->connName);	
			}
		} else {
			Title("Scriptor - Not connected");
		}
	}
	
	//==========================================================================================
	void CreateTestFromScript() {
		if (!cmdScript.connection) return;
		if (cmdScript.scriptId < 0) return; // Don't try and save tests with no script #, meaning not in database
		
		if (!testGrid.IsOpen()) {
			testGrid.Open();
			testGrid.Build();
			Sql controlSql(controlDb);
			testGrid.Load(&controlDb, controlSql);
		}
		
		testGrid.AddTest(TrimBoth(cmdScript.Get().GetPlainText().ToString()), cmdScript.scriptId, cmdScript.connection->connId);
	}
	
	//==========================================================================================	
	void MyToolBar(Bar& bar) {
		bar.Add(!(cmdScript.Get().GetPlainText().ToString().IsEmpty()), "File", CtrlImg::smalldown(), THISBACK(AddScriptToHistory)).Tip("Memorize Script");
		
		// Only allow test creation if there is a script, a scriptid, and a connection
		bar.Add(
			(cmdScript.Get().GetPlainText().ToString().GetLength() > 0 
			 && cmdScript.connection
			 && cmdScript.scriptId >= 0), "File", MyImages::addtotest(), 
			 THISBACK(CreateTestFromScript)).Tip("Create a Test around this Script");
			 
		// Only allow execution if there is a script and a connection
		bar.Add(
			(cmdScript.Get().GetPlainText().ToString().GetLength() > 0 
			 && cmdScript.connection), "File", MyImages::lightning_icon16(), 
			 THISBACK(ExecuteSQLActiveConn)).Tip("Execute Script");
		cmdScript.FindReplaceTool(bar);
		//CtrlImg::exclamation(), CtrlImg::smallright(), CtrlImg::open(), CtrlImg::undo(), CtrlImg::remove
		//smallcheck, spinup3, smallreporticon, save, Plus, Minus, Toggle, help
	}
	
	//==========================================================================================	
	virtual void Run(bool appmodal = false) {
		ToolBarRefresh();
		ConnectionStatusRefresh();
//		connGrid.HidePrivateColumns();
		String a = connGrid.GetColumnWidths();
		TopWindow::Run(appmodal);
		
		// These are post-create/instantiate tasks, also done after serialization
	}

	//==========================================================================================	
	void ExecuteSQLActiveConn() {
		//String SQLText = cmdScript.GetSelectionData() // .GetPlainText()   WriteClipboardUnicodeText
		String SQLText = cmdScript.Get().GetPlainText().ToString();
		SQLText = TrimBoth(SQLText);
		if (SQLText.IsEmpty()) {
			BeepExclamation();
			return;
		}
		
		if (!activeConn) {
			Exclamation("No connection");
			return;
		}
		
		if (activeConn->enumConnState != CON_SUCCEED) {
			Exclamation("Disconnected");
			return;
		}
		
		CursorHandler cursorHandler(*(activeConn->session));
		bool ran = cursorHandler.Run(&mainGrid, SQLText);
	}
	
	//==========================================================================================	
	bool Key(dword key, int count) {
		static dword style;
		static Rect normalwindowrect;
		
		switch (key) {
			case K_F11:
				// Rotate through 3 states
				if (enumScreenZoom != ZOOM_ALLSCREENS) {
					
					// Get the full size of the screen (single physical screen)
					long cx = GetSystemMetrics(SM_CXSCREEN);
	   				long cy = GetSystemMetrics(SM_CYSCREEN);
					
					if (enumScreenZoom == ZOOM_FULLSCREEN) {
						// Zoom to multiple screens!
						cx = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		   				cy = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		   				enumScreenZoom = ZOOM_ALLSCREENS;
		   				
		   			// Only save the normal style and size, not the full screen size
					} else {
						// Get the current Top window's style bits
						style = GetStyle();
						// Strips the caption bit, which is the title bar
						style = (style & ~WS_CAPTION);
						// Save the "overlapped" or normal window shape
						normalwindowrect = GetRect(); 
						SetWindowLong(GetHWND(), GWL_STYLE, style);  
						enumScreenZoom = ZOOM_FULLSCREEN;
					}
					
	   				// Expand the window to full size
	   				SetWindowPos(GetHWND(),HWND_TOP,0,0,cx,cy,SWP_SHOWWINDOW); 
	   				
					//or use Maximize(false) if you like a delayed resizing, even with zoom effects turned off
				} else {
					// Set the caption bit back so we can get our title bar back
					style |= WS_CAPTION;
					// Pass to windows (Style() method gets confused on SyncCaption0)
					SetWindowLong(GetHWND(), GWL_STYLE, style);
					// Return size to normal; key flag is SWP_FRAMECHANGED
					SetWindowPos(GetHWND(),HWND_TOP, 
						normalwindowrect.left, 
						normalwindowrect.top, 
						normalwindowrect.Width(), 
						normalwindowrect.Height(), SWP_SHOWWINDOW|SWP_FRAMECHANGED);
						enumScreenZoom = ZOOM_NORMALSCREEN;
				}
				break;
		}
		return false;
	}

	//==========================================================================================	
	void Serialize(Stream& s) {
		SerializePlacement(s);
		testGrid.SerializePlacement(s);		
	}
	
	//==========================================================================================	
	void Xmlize(XmlIO xml) {
		xml
			("vertsplit", vertSplitter)
			("horzsplit", horizSplitter)
			("grid", connGrid)
			("cmdScript", cmdScript)
		;
	}
};

#endif

/*
In RichEdit's constructor page size is set to Size(3968, 6074)
and unit to UNIT_POINT.
When saving to pdf ,a pdf document is 8,5x11 inch. 
How and when is it calculated ? ( 3968 / 72 != 8,5 )
How to set the page size to different sizes in inches, cm, etc. ?

When U++ is dealing with "physical units" (like the real world distances on paper media), it always uses "dots". 1 dot is defined as 1/600 inch (and is equal to single pixel on 600dpi printer).

QTF knows nothing about physical page - QTF document only gets rendered to any page size. If you need to record information about landscape, you have to create some extended format.

Thank you. I used the same way. I have created a style, named Landscape in QTF-document, and using the availability of this style as an indicator of a turn the page. 
Another style will be to specify the size of the page.

All that is needed for the template editor of printed documents for editing which I use a modified version UWord.

I think the editor UWord should understand the turn of the page and the size of paper (if the user wishes to set this).

Therefore, I propose to extend the specification QTF.

1.? How you want to change it to Null? By changeing library code? Not a good idea...

2.
Vector<int> faces;
for(int i = 0; i < Font::GetFaceCount(); i++)
if(Font::GetFaceInfo(i) & Font::SCALEABLE)
faces.Add(i);
editor.FontFaces(faces); 

#include "CtrlLib/CtrlLib.h"

struct FontFaceDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const
	{
		Font fnt = Font(q, r.Height() - 2);
		String txt = Font::GetFaceName(q);
		w.DrawRect(r, paper);
		w.DrawText(r.left + 2, r.top + (r.Height() - w.GetTextSize(txt, fnt).cy) / 2, txt, fnt, ink);
	}
};

struct MyApp : TopWindow {
	DropList dl;
	
	MyApp()
	{
		Add(dl.HSizePos().TopPos(5, Ctrl::STDSIZE));
		dl.SetDisplay(Single<FontFaceDisplay>());
		for(int i = 0; i < Font::GetFaceCount(); i++)
			dl.Add(i);
		SetRect(0, 0, 200, 70);
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
RichEdit& RichEdit::FontFaces(const Vector<int>& face); - just put in the list of font faces you want to use. (See Font).

	String qtf;
    qtf << txText1.GetQTF(CHARSET_ISO8859_1);
	//qtf << "[a NRE PARAGRAPH: &";
	txText1.SetQTF(qtf);

void PullText::OnButton2(void)
{
	RichText clip=txText2.CopyText(0,10000);
	txText2.Clear();
	txText2.PasteText(clip);
}
Supose I have a form with the RichEditWithToolbar (txText) and a button. When I run the app I write "Hello" on the blank RichEdit. Then I push the button and I saw "UTF-8 Hello" on the RichEdit. Is this normal behaviour. If so How do I remove the UTF-8 part of the Text.

struct MyEditorWindow {
    MenuBar  menu;
    RichEdit edit;
.......
    MyEditorWindow() {
       Add(edit.SizePos()); // will fill entire view area
       AddFrame(menu); // will be at the top, reducing the view area
       .....
    }
};


Is it possible to accept files/links/text drag & dropped from an other application? It seems that I can not even drag&drop text from one text input into an other one inside the same app. If these are not yet possible, could we add them to the wishlist?
Thanks 

Well, this is one of things that is still missing in U++. Means you can implement drag&drop in U++, including dropping files from other apps etc... but there is no direct support for doing so.

It is already on ToDo list.

Partial problem here is that solution should involve both X11 and OLE mechanisms, or at least be flexible enough to allow them later. 

just testing with codes pieces from TCtrllib directrory.
drag qtf file and the richeditor will open it.
#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>
#include <shellapi.h>
using namespace Upp;

#define LAYOUTFILE "MainLay.lay"
#include <CtrlCore/lay.h>

//DragAcceptFiles
//DragFinish
//DragQueryFile
//DragQueryPoint


class TestDragDrop : public WithMainGUI<TopWindow>
{
	public:
		TestDragDrop();
		~TestDragDrop();	
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	private:
		void showDroppedFiles(HDROP dropInfo);
	private:
		RichEdit editor;
		typedef TestDragDrop CLASSNAME;
};

TestDragDrop::TestDragDrop()
{
	Add(editor.SizePos());
	editor.SetFocus();
	Open();
	HWND hwnd = GetHWND();
	DragAcceptFiles(hwnd,true);
}

TestDragDrop::~TestDragDrop()
{
}

void TestDragDrop::showDroppedFiles(HDROP hdrop)
{
	//get how many are dropped
	int count = DragQueryFile((HDROP)hdrop, (DWORD)-1, 0, 0);
	Vector<String> list;
	for(int i = 0; i < count; i++) {
		//get length of files name
		int length = DragQueryFile((HDROP)hdrop, i, 0, 0);
		String s;
		DragQueryFile((HDROP)hdrop, i, s.GetBuffer(length), length + 1);
		s.ReleaseBuffer(length);
		//Taken from UWord codes
		editor.SetQTF(LoadFile(s));
		editor.ClearModify();
	}
	DragFinish((HDROP)hdrop);
}

LRESULT TestDragDrop::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = GetHWND();
	switch(message) 
	{
		case WM_DROPFILES:
		{
			showDroppedFiles((HDROP)wParam);
			return 0;
		}
		default:
			return TopWindow::WindowProc(message, wParam, lParam);
	}
	return true;
}

GUI_APP_MAIN
{
	TestDragDrop test;
	
	test.Run();
}

Thanks. Really not that complicated, is it? (What IS complicated is to support all variants, including OLE D&D, in-application D&D, X11 D&D etc....)

BTW, according to "U++ programming paradigm", I do not quite like this part:

mobilehunter wrote on Sat, 10 February 2007 18:46 

just testing with codes pieces from TCtrllib directrory.
drag qtf file and the richeditor will open it.
TestDragDrop::TestDragDrop()
{
	Add(editor.SizePos());
	editor.SetFocus();
	Open();
	HWND hwnd = GetHWND();
	DragAcceptFiles(hwnd,true);
}


 


Opening dialog in constructor is "bad practice" (of course, it works as expected and in experimental code it is OK).

Anyway, notice quite useful methods in Ctrl::

	virtual void    NcCreate(HWND hwnd);
	virtual void    NcDestroy();
	virtual void    PreDestroy();

I removed the Open and the rest from constructor.
And add this codes.
So i have to put codes that need immediate access to hwnd inside NcCreate, right?

void TestDragDrop::NcCreate(HWND hwnd)
{
	Ctrl::NcCreate(hwnd);
	DragAcceptFiles(hwnd,true);
}

Hi i did this inside RichEdit control, RichEdit just an example.
The problem was size of font.

LOGFONT lf;
FontInfo fi = formatinfo.Info();
ImmGetCompositionFont(hIMC,&lf);
lf.lfHeight = -DotToPt(formatinfo.GetHeight());
//font size will follow current font setting
ImmSetCompositionFont(hIMC, &lf);


The result was character inside IME window displayed smaller.
Is there any way to make character size inside IME window to have the same size as what richedit displayed? 

Why DotToPt?

Anyway, IMO, if you use what is used in

FontInfo Draw::Acquire(Font font, HDC hdc, int angle, int device)

you should get equal results (CreateFont is used there, but the formula for font height ougth to be the same).

That is:

font.GetHeight() ? -abs(font.GetHeight()) : -12, font.GetWidth()

Ah, I see - are you supposed to put there points instead of pixels?

Well, even in that case, your formulae is wrong. IMO, you will have to take into acount actual screen DPI (dots are fixed at 600dpi, which is not screen resolution).

Mirek 

I got this formula from MSDN
fontHeight=-MulDiv(DotToPt(formatinfo.GetHeight()), GetDeviceCaps(hdc, LOGPIXELSY), 72);

I just compared the UWord and WordPad for the font size using visual cues only, bad comparison though:).
It seems font size inside UWord bigger than WordPad. I compared using english alphabet.

I also compared the character inside WordPad with character inside UWord's IME window, both have the same size. 

Hi Mirek.

Today a learned another thing called Zoom from RichEdit control.
So it will make character displayed inside UWord bigger. 
I didn't consider this factor when calculating the font height for IME window.

case closed 

#include <RichEdit/RichEdit.h>

 

using namespace Upp;

 

const char *examples[] = {

    "Normal [* bold] [/ italic] [_ underline] [` superscript] [, subscript]",

    "`[ `] \1[escaped]\1 [* bold]",

    "[A Arial (Sans-Serif)] [R Times New Roman (Serif)] [C Courier (Monospace)]",

    "[0 6pt ][1 8pt ][2 10pt ][3 12pt ][4 16pt ][5 20pt ][6 24pt ][7 28pt ][8 36pt ][9 48pt ]",

    "[!Tahoma! Tahoma]",

    "[+500 500dots]",

    "[@4 Green text] [$(255.220.200) Pink background]",

    "[%EN-US English language] [%CS-CZ Czech language]",

    "[^upp.sf.net^ Hyperlink] [Icompiler, linker; Index entry]",

    "[:label: Labeled paragraph]",

    "[< Left paragraph alignment]",

    "[= Center paragraph alignment]",

    "[> Right paragraph alignment]",

    "[# Justify alignment. Just some text to demosntrate it... "

    "Just some text to demonstrate it... Just some text to demonstrate it...]",

    "[l1000 Left margin 1000dots]",

    "[i1000 Indent 1000 dots."

    "Just some text to demonstrate it... Just some text to demonstrate it...]",

    "[r1000 Right margin 1000 dots."

    "Just some text to demonstrate it... Just some text to demonstrate it...]",

    "Paragraph&[b200 Before 200dots]",

    "[a200 After 200dots]&Paragraph",

    "[i200 [O0 bullet&][O1 bullet&][O2 bullet&][O3 bullet]]",

    "[O9i500 text bullet-|"

    "Just some text to demonstrate it... Just some text to demonstrate it..."

    "Just some text to demonstrate it... Just some text to demonstrate it...]",

    "[O9i200 [N1m.; -|level 1&][N1a -|level 2&][N1a -|level 2&][N1 -|level 1&][N1a -|level 2]]",

    "[~300~=.2000~>-3000 -|Normal tab-|Centered tab-|Right tab]",

    "{{1:2 A1:: A2:: B1:: B2}}",

    "{{2:1G4g100F5f50 A1:: A2:: B1:: B2}}",

    "{{1:2 A1::l40/60R6@3 A2::! B1:: B2}}",

    "{{1:1:1|2 A1::-2 A2:: A3:: B1:: B2:: B3}}",

    "{{1:2 A1:: A2:: B1:: {{1:2 a1:: a2:: a1:: a2}}}}",

    NULL

    String qtf = "&";

    for(int i = 0; examples[i]; i++)

        qtf << "{{1 [@3C1* " << DeQtf(AsCString(examples[i])) << "]:: " << examples[i] << "}}&&";

    RichEditWithToolBar e;

    e.SetQTF(qtf);

    TopWindow w;

    w.Sizeable().Zoomable();

    w.Add(e.SizePos());

    w.Run();

With U++ version you have, you would need to implement your own Print.

Anaway, I have just added "automatic landscape" into Print routing for RichText. With it, all you need to do is reverse page dimensions:

RichEdit editor;
editor.SetPage(Size(6074, 3968));

My question is this: My chat and IM views use the RichTextView controls and I need to scroll down to the very bottom every time a new message comes up only IF the view is already scrolled down to the bottom. If the user is viewing some back messages or he is not currently scrolled to the very bottom, the view should only scroll down to where the he was at before the new message came. I did this but I'm not satisfied because sometimes the view still scrolls one line down when it's not at the bottom:

int sb = output_.GetSb(), tsb = output_.GetSbTotal();

output_ <<= ~output_ + qtf;

if (!_sbConst) {
    output_.ScrollEnd();
    sb = output_.GetSb();
    tsb = output_.GetSbTotal();
    if (tsb > sb && tsb > output_.GetSbPage())
        _sbConst = tsb - sb;
} else {
    if (abs(tsb - sb) != _sbConst) output_.SetSb(sb);
    else output_.ScrollEnd();
}


But this now requires that ScrollBar::GetPage() be exposed as RichTextView::GetSbPage().  Can we do this guys?

GetPage()+GetPos(Last Position) = GetTotal()
Rect in same unit - GetScreenRect(of window) - Size and position of window
Rect in same unit - GetScreenRect(of ctrl) - Size and position of ctrl

Try this aprove. 

Included new code for Raster and plugin/Gif.

Virtual funcions added are: 
- Rect GetPageRect(int n). Gets the subimage Rect
- int GetPageDisposal(int n). Gets the way the subimage has to be handled by the renderer

Reference/RasterMultiPage example has been uploaded covering these changes.

Now last disposal method ("Restore to previous") is included in RasterMultiPage Reference demo.

And now is the time for the GIF animation player control. If you tell me the basic class methods I can do it for yourself.

Anyway if GIF animation is not going to be included in CtrlLib I will include it in Bazaar with pleasure 

Hello klrkt

I had left this waiting this post to be solved http://www.ultimatepp.org/forum/index.php?t=msg&goto=280 35&

In fact it is not directly related  

Now plugin/Gif supports animated gifs... but do not play them.

I will post it in few time. 

The new class is called AnimatedClip. 
Is it better RasterPlayer?
Yes, here: "upp\reference\AnimatedClip" 


void App::MyToolBar(Bar& bar)
{
	bar.Add(grid.IsCursor(), "File", Image::Wait() , THISBACK(ClearGrid));
	bar.Add(grid.IsCursor(), "File", Image::Wait() , THISBACK(ClearGrid));
}
void App::ToolBarRefresh()
{
	tool.Set(THISBACK(MyToolBar));
	Beep(1000,100);
}
	ToolBar tool;
		tool.Set(THISBACK(MyToolBar));
		
		AddFrame(tool);

class EditStringLim : public EditString {
	String str;
public:
typedef EditStringLim CLASSNAME;
void OnMax();
	EditStringLim();
	~EditStringLim() {;}
};

void EditStringLim::OnMax(){
	if (GetLength()>=maxlen){  //bad! but if you make only >maxlen the value becomes <error ...
		SetData(str);
		SetSelection(maxlen, maxlen); //to avoid "selected all"
	}
	else 
		str = GetData().ToString(); //.Left(maxlen);
}

EditStringLim::EditStringLim(){
	WhenAction=THISBACK(OnMax);
}


editor.SetCursor(editor.GetPos(editor.GetLineNo(lineno - 1)));
b.pos = editor.GetEditPos();
	EditPos pos;
	pos.sby = sb.Get().y;
	pos.cursor = cursor;
	return pos;

RichText h = ParseQTF(styles + ("[s7; &]" + CreateQtf(link[q], n[q].name, m, true)));

	for(int i = 0; i < txt.GetPartCount(); i++)
		if(txt.IsPara(i)) {
			RichPara p = txt.Get(i);
			if(i >= l && i < h) {
				WString h = p.GetText();
				String nat;
				const wchar *s = h;
				while(*s)
					if(*s == 160 || *s == ' ') {
						nat.Cat(' ');
						while(*s == 160 || *s == ' ') s++;
					}
					else
						nat.Cat(*s++);
				int q = nat.GetCount() ? natural.Find(nat) : -1;
				if(q >= 0) {
					started = true;
					const CppItem& m = n[q];
					RichText h = ParseQTF(styles + ("[s7; &]" + CreateQtf(link[q], n[q].name, m, true)));
					if(h.GetPartCount())
						h.RemovePart(h.GetPartCount() - 1);
					result.CatPick(h);
				}
				else
				if(!started || p.GetLength())
					result.Cat(p);
			}
			else
				result.Cat(p);
		}
		else {
			RichTable b;
			b <<= txt.GetTable(i);
			result.CatPick(b);
		}
	RichPara empty;
	result.Cat(empty);
	editor.BeginOp();
	editor.SetSelection(0, txt.GetLength());
	editor.PasteText(result);

String QtfRichObject::ToString() const
{
	return String("@@#").Cat() << uintptr_t(&obj) << ";";
}

QtfRichObject::QtfRichObject(const RichObject& o)
	: obj(o)
{}

RichText ParseQTF(const char *qtf, int accesskey, void *context)
{
	RichQtfParser p(context);
	try {
		p.Parse(qtf, accesskey);
	}
	catch(RichQtfParser::Exc) {}
	return p.target;
}

It is not clear to me how to add a new line, for example ...

Need to add new paragraph (RichPara).

ParseQtf and ParseRTF 

Maybe suggest what should the example do and I can provide it....

Mirek

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	RichText txt;
	{
		RichPara para;
		RichPara::CharFormat fmt;
		(Font&)fmt = Serif(100);
		para.Cat("Hello world!", fmt);
		txt.Cat(para);
	}
	{
		RichPara para;
		{
			RichPara::CharFormat fmt;
			(Font&)fmt = Monospace(150).Bold();
			fmt.ink = Red;
			fmt.capitals = true;
			para.Cat("Monospace-bold-red-capitals, ", fmt);
		}
		{
			RichPara::CharFormat fmt;
			(Font&)fmt = SansSerif(100);
			fmt.link = "http://www.ultimatepp.org";
			para.Cat("This is some link", fmt);
		}
		txt.Cat(para);
	}
	{
		RichPara para;
		RichPara::CharFormat fmt;
		para.format.align = ALIGN_CENTER;
		para.Cat("Centered", fmt);
		txt.Cat(para);
	}
	{
		RichPara para;
		RichPara::CharFormat fmt;
		para.format.before = 100;
		para.format.lm = 300;
		para.Cat("Before and left margin", fmt);
		txt.Cat(para);
	}
	{
		RichPara para;
		RichPara::CharFormat fmt;
		RichObject obj = CreatePNGObject(CtrlImg::exclamation(), 200, 200);	
		para.Cat("Object: ", fmt);
		para.Cat(obj, fmt);
		txt.Cat(para);
	}
	{
		RichTable table;
		table.AddColumn(1);
		table.AddColumn(1);
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 2; j++) {
				RichText celltext;
				RichPara para;
				RichPara::CharFormat fmt;
				para.Cat(AsString(i) + ':' + AsString(j), fmt);
				celltext.Cat(para);
				table.SetPick(i, j, celltext);
			}
		txt.CatPick(table);
	}
	
	RichTextView view;
	view.Pick(txt);
	TopWindow win;
	win.Add(view.SizePos());
	win.Run();
}

String strQTF = txText.GetQTF(CHARSET_ISO8859_1);
txText.SetQTF(strQTF);

http://www.ultimatepp.org/reference$RichTextObject$en-us.html

INITBLOCK {

    RichObject::Register("mytype", &Single<MyRichObjectType>());

};


editor.SetQTF("Some line&Here comes the RichObject: @@mytype:1200*200`Hello world!`&Next line");

    else

        editor.Pick(ParseQTF(LoadFile(FileName())));


void MyRichEdit::InsertMy()

{

    String txt;

    if(EditText(txt, "New MyRichObject", "Text")) {

        RichObject obj(&Single<MyRichObjectType>(), txt);

        obj.SetSize(1000, 300);

        obj.KeepRatio(false);

        PasteText(AsRichText(obj));

    }

}

 
I was asked by application user to provide some means to insert often repeated texts ("e.g. if I press F4, I want xyz to be inserted here).

This simple module allows you do define selection as macro using Ctrl+F1-10 a insert using F1-10, in EditField, DocEdit, LineEdit, RichEdit:

static WString s_macro[10];

bool MacroKeyHook(Ctrl *ctrl, dword key, int count)
{
	if(key >= K_CTRL_F1 && key <= K_CTRL_F10) {
		WString& txt = s_macro[key - K_CTRL_F1];
		int l, h;
		if(EditField *e = dynamic_cast<EditField *>(ctrl))
			if(e->GetSelection(l, h)) {
				txt = e->GetText().Mid(l, h - l);
				return true;
			}
		if(TextCtrl *e = dynamic_cast<TextCtrl *>(ctrl))
			if(e->GetSelection(l, h)) {
				txt = e->GetW(l, h - l);
				return true;
			}
		if(RichEdit *e = dynamic_cast<RichEdit *>(ctrl)) {
			txt = e->GetSelection().GetPlainText();
			return true;
		}
	}
	if(key >= K_F1 && key <= K_F10) {
		WString txt = s_macro[key - K_F1];
		if(EditField *e = dynamic_cast<EditField *>(ctrl)) {
			e->Insert(txt);
			return true;
		}
		if(TextCtrl *e = dynamic_cast<TextCtrl *>(ctrl)) {
			e->Paste(txt);
			return true;
		}
		if(RichEdit *e = dynamic_cast<RichEdit *>(ctrl)) {
			e->PasteText(AsRichText(txt, e->GetFormatInfo()));
			return true;
		}
	}
	return false;
}

void SerializeTextMacros(Stream& s)
{
	int version = 0;
	s / version;
	for(int i = 0; i < 10; i++)
		s % s_macro[i];
}

INITBLOCK {
	Ctrl::InstallKeyHook(MacroKeyHook);
}

Vector<int> faces;
for(int i = 0; i < Font::GetFaceCount(); i++)
if(Font::GetFaceInfo(i) & Font::SCALEABLE)
faces.Add(i);
editor.FontFaces(faces); 

*/
