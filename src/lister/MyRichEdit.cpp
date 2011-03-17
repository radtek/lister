#include <RichEdit/RichEdit.h>
#include <lister/Urp/UrpString.h>
#include <Web/Web.h>

#include "MyRichEdit.h"
#include "Connection.h"
#include "Script.h"
#include "KeyHandler.h"

static const String MACRO = "[[INPUT]]";

//==============================================================================================	
MyRichEdit::MyRichEdit() {
	connection = NULL;
	scriptId = UNKNOWN;
	connId = UNKNOWN;
	tablelist.AddColumn("Table", 100);
	tablelist.Normal();
	columnlist.AddColumn("Column", 100);
	columnlist.Normal();
	NoRuler();
	ShowCodes(Null);
	ViewBorder(0);
	zoomlevel = 8.0; // default zoom is about 9 pt on my screen
}

//==============================================================================================	
/*virtual */MyRichEdit::~MyRichEdit() {
}

//==============================================================================================	
//  Added a zoom text feature.
/*virtual*/ void MyRichEdit::MouseWheel(Point p, int zdelta, dword keyflags) {
	if (keyflags == K_CTRL) {
		// Zooms font
		double fzdelta = (double)zdelta;
		zoomlevel+= ((double)fzdelta / 240.0); // One bump on the mouse wheel is 120 on my machine
		RefreshLayoutDeep();
	} else {
		// Scrolls down
		RichEdit::MouseWheel(p, zdelta, keyflags);		
	}
}

//==============================================================================================	
void MyRichEdit::Layout() {
	
	RichEdit::Layout();
	long editor_cx = GetSize().cx;
	long adaptive_cx = (long)(editor_cx * zoomlevel); // Smaller the number, the bigger the text
	SetPage(Size(adaptive_cx, INT_MAX));
}

//==============================================================================================	
void MyRichEdit::SplitOnComma() {
	String ci = ReadClipboardText();
	String co;

	Vector<String> elems = Split(ci, ","); // In Util.cpp: Vector<String> Split(const char *s, const String& delim, bool ignoreempty)
	
	for (int i = 0; i < elems.GetCount(); i++) {
		if (i) co << "\n";
		String elem = StripWrapper(Trim(elems.At(i)), "'");
		co << elem;
	}
	
	WriteClipboardText(co);
	Paste();
	WriteClipboardText(ci); // Restore clipboard so it doesn't keep expanding

}

//==============================================================================================	
// Our system is partitioned by date, with no global indexes, so to search across dates,
// You need to supply a list of dates across the range
void MyRichEdit::PasteRunOfOraDates() {
	VectorMap<String, Date> limits;
	String inputDate; // Manually?
	bool ok = UrpDateInputBox(inputDate, "Enter range of dates", "Enter range of dates to generate an in clause of oracle formatted strings", limits);
	if (ok) {
		if (limits.Find("fromdate") < 0 || limits.Find("todate") < 0) {
			Exclamation("Must enter from and to dates");
			return;
		}
		
		Date fromdate = limits.Get("fromdate");
		Date todate = limits.Get("todate");
		
		String ci = ReadClipboardText();
		String co;
		int i = 0;
		while (fromdate <= todate) {
			if (!In(DayOfWeek(fromdate), 0, 6)) { // Don't generate dates for the weekend
				if (!i) co << "IN(";
				if (i) co << ", ";
				co << Format("'%02d-%s-%04d'", fromdate.day, ToUpper(MonName(fromdate.month-1 /* MonName is zero-based! */)), fromdate.year);
				i++;
			}
			if (i && fromdate == todate) co << ")";
			fromdate+= 1;
		}
		
		WriteClipboardText(co);
		Paste();
		WriteClipboardText(ci); // Restore clipboard so it doesn't keep expanding
	}
}

//==============================================================================================	
void MyRichEdit::PasteWithNoWrapper() {
	PasteJoinLinesWithWrapper();
}

//==============================================================================================	
// My helper, unintentionally hides the RichText version
void MyRichEdit::PasteText(const String &s) {
	RichEdit::PasteText(AsRichText(s.ToWString()));
}

//==============================================================================================	
// Re-visiblize
void MyRichEdit::PasteText(const RichText& text) {
	RichEdit::PasteText(text);
}

//==============================================================================================	
//  Turn a set of lines into a stream for IN clause use
void MyRichEdit::PasteWithApostrophe() {
	PasteJoinLinesWithWrapper("'", "'");
}

//==============================================================================================	
void MyRichEdit::PasteJoinLinesWithWrapper(const String& wrapper, const String &wrapperRight) {
	
	String ci = ReadClipboardText();
	String co;
	
	if (ci.Find('\n') > 0) {
		Vector<String> cil = Split(ci, '\n');
		Sort(cil);
		for (int i = 0; i < cil.GetCount(); i++) {
			if (i) co << ", ";
			co << wrapper << TrimRight(cil[i]) << wrapperRight;
		}
	} else {
		co << wrapper << ci << wrapperRight;
	}
	
	WriteClipboardText(co);
	Paste();
	WriteClipboardText(ci); // Restore clipboard so it doesn't keep expanding
}

//==============================================================================================	
void MyRichEdit::PasteConcat() {
	String ci = ReadClipboardText();
	String co;
	
	Vector<String> cil = Split(ci, '\n');
	for (int i = 0; i < cil.GetCount(); i++) {
		co << TrimRight(cil[i]);
	}
	
	WriteClipboardText(co);
	Paste();
	WriteClipboardText(ci); // Restore clipboard so it doesn't keep expanding
}


//==============================================================================================	
void MyRichEdit::PasteAsLineMacro() {
	static String wrapperCmd = MACRO;  // Retain any changes the user makes
	
	String ci = ReadClipboardText();
	String co;
	
	if (ci.Find('\n') > 0) {
		Vector<String> cil = Split(ci, '\n');
		Sort(cil);
		String co = PasteAsLineMacro(
			cil // List of lines as input to parser
		,	MACRO
		,	""  // No additional line to insert between
		,   "Enter wrapper text for line macro Build" // title of dialog
		,   cil[0] // Insert one line as an example
		,   wrapperCmd // Saved wrapper command (remembers
		);
		WriteClipboardText(co);
		Paste();
		WriteClipboardText(ci); // Restore clipboard so it doesn't keep expanding
	}
}

//==============================================================================================	
//  Turn a set of lines into series of text with the clipboard text pasted in
String MyRichEdit::PasteAsLineMacro(
	Vector<String> lines, String macroMarker, String inBetweenNewLine, String title, 
	String furtherInstructions /*= Null*/, String wrapperString /*= Null*/) 
{
	String scrubAllOcc;
	String splitOnThisToMacros;
	bool forceInputToUpper;
	bool convertSpacesToUnderscores;
	String expandedLines;
	String instructions = "Wrapper (leave macro or it won't work!!!). Use \\n for new lines, use [[INPUT1]], [[INPUT2]], ... and a spliton value to treat input as fields, and [[INPUT]] to just use the whole thing.";
	
	if (!furtherInstructions.IsVoid()) {
		instructions << "  " << '\n' << furtherInstructions;
	}

	Vector<String> splitInputLine;
	
	if(UrpInputBox(wrapperString, scrubAllOcc, splitOnThisToMacros, forceInputToUpper, convertSpacesToUnderscores, title, instructions)) {
		Vector<String> scrubList;
		if (!scrubAllOcc.IsEmpty()) {
			scrubList = Split(scrubAllOcc, ";");
		}
		
		for (int i = 0; i < lines.GetCount(); i++) {
			String scrubbedInputLine = TrimBoth(lines[i]);
			if (!scrubAllOcc.IsEmpty()) {
				for (int j = 0; j < scrubList.GetCount(); j++) {
			 		scrubbedInputLine = UrpString::ReplaceInWhatWith(scrubbedInputLine, scrubList[j], "");
				}
			}

			if (forceInputToUpper) {
				scrubbedInputLine = ToUpper(scrubbedInputLine);
			}
			
			if (convertSpacesToUnderscores) {
				scrubbedInputLine = UrpString::ReplaceInWhatWith(scrubbedInputLine, " ", "_");
			}
			
			if (!splitOnThisToMacros.IsEmpty()) {
				splitInputLine = Split(scrubbedInputLine, splitOnThisToMacros);
			} else {
				splitInputLine.Clear();
			}
			
			// if not the first line, stuff a separating string, like "UNION ALL" between each input line
			if (i && !inBetweenNewLine.IsEmpty()) expandedLines << "\n" << inBetweenNewLine;
			
			// Converts "Select * from [[INPUT]]" to "Select * from x union all select * from y..."
			String patchedLine = UrpString::ReplaceInWhatWith(wrapperString, macroMarker, TrimRight(scrubbedInputLine));
			for (int j = 0; j < splitInputLine.GetCount(); j++) {
				int oj = j + 1;
				String indexedMarker = "[[INPUT" + FormatInteger(oj) + "]]";
				patchedLine = UrpString::ReplaceInWhatWith(patchedLine, indexedMarker, splitInputLine[j]);
			}

			expandedLines << "\n" << patchedLine;
		}
		
		return expandedLines;
	}
	
	return Null;
}

//==============================================================================================	
//  Turn a set of lines into series of union alls (for ex:)
void MyRichEdit::PasteAsWrappedUnion() {
	static String wrapperCmd = MACRO;  // Retain any changes the user makes
	
	String ci = ReadClipboardText();
	String co;
	String scrubAllOcc;
	String splitOnThisToMacros;
	
//		if (ci.Find('\n') > 0) {
//			if(UrpInputBox(
//					wrapperCmd
//					, scrubAllOcc
//					, splitOnThisToMacros
//					, "Enter wrapper text for UNION ALL Build"
//					, "Wrapper (leave macro or it won't work!!!). Use \\n for new lines, use [[MACRO1]], [[MACRO2]] and a spliton value to treat input as fields:")
//					) 
//			{
//				Vector<String> cil = Split(ci, '\n');
//				Sort(cil);
//				for (int i = 0; i < cil.GetCount(); i++) {
//					if (i) co << "\nunion all\n";
//					
//					// Converts "Select * from [[INPUT]]" to "Select * from x union all select * from y..."
//					String nco = UrpString::ReplaceInWhatWith(wrapperCmd, MACRO, TrimRight(cil[i]));
//					co << nco;
//				}
//			}
//		} else {
//			co << "'" << ci << "'";
//		}
	
	WriteClipboardText(co);
	Paste();
	WriteClipboardText(ci); // Restore clipboard so it doesn't keep expanding
}

//==============================================================================================
// Came with the web search below.  Untested.	
/*static*/ String MyRichEdit::DumpSpecial(String s) {
    String out;
    for(const char *p = s.Begin(), *e = s.End(); p < e; p++)
        if((byte)*p >= ' ' && *p != '\xFF')
            out.Cat(*p);
        else {
            switch(*p) {
                case '\a': out.Cat("[\\a]"); break;
                case '\b': out.Cat("[\\b]"); break;
                case '\f': out.Cat("[\\f]"); break;
                case '\v': out.Cat("[\\v]"); break;
                case '\t': out.Cat("[\\t]"); break;
                case '\r': out.Cat("[\\r]"); break;
                case '\n': out.Cat("[\\n]\n"); break;
                default:   out.Cat(NFormat("[\\x%02x", (byte)*p)); break;
            }
        }
    return out;
}

//==============================================================================================	
String MyRichEdit::GetSelectionAsPlainText() {
	return GetSelection().GetPlainText().ToString();
}

//==============================================================================================	
// Doesn't work.
void MyRichEdit::SearchSharePoint() {
	String searchString = GetSelectionAsPlainText();
	String proxy = "webproxy.bankofamerica.com:8080";
	proxy = "proxy.ml.com:8083";
	proxy = "rrwebproxy.bankofamerica.com:8080";
	HttpClient client;
	client.URL("http://sharepoint.bankofamerica.com/sites/sharepointcentral/Search%20Center/Pages/Results.aspx?k=test&s=All%20Sites");
	client.URL("http://sharepoint.bankofamerica.com/sites/sharepointcentral/Pages/Default.aspx");

	// WORKS WITH proxy.ml.com:8083:client.URL("http://www.google.com/search?hl=en&source=hp&q=windows+api+control+sound+volume&aq=f&aqi=p-p1g9&aql=&oq=&gs_rfai=");
	client.Proxy(proxy);
	String content = client.ExecuteRedirect();
	String err = client.GetError();
	int stat = client.GetStatusCode();
	String statline = client.GetStatusLine();
	String header = DumpSpecial(client.GetHeaders());
	String dcontent = DumpSpecial(content);

	Pick(AsRichText(dcontent.ToWString()));
}

//==============================================================================================	
// Strip Qtf codes off of text.
void MyRichEdit::DeFormatSelection() {
	WriteClipboardText(GetSelectionAsPlainText());
	Paste();
}

//==============================================================================================	
// Clean entire thing of Qtf.
void MyRichEdit::DeFormatScript() {
	SetScriptPlainText(GetScriptPlainText());
}

//==============================================================================================	
// Useful to count the highlighted selection's comma-separated values, especially when trying
// to determine how many of the in clause values matched up.
void MyRichEdit::CountCommaSepValues() {
	String sel = GetSelectionAsPlainText();
	int itemcount = UrpString::CountOccurInOf(sel, ',');
	Exclamation(ToString(itemcount));
}

//==============================================================================================	
// Stolen from the RichEdit since I needed to manipulated non-exposed components.
/*virtual*/ void MyRichEdit::RightDown(Point p, dword flags) {
	useraction = true;
	NextUndo();
	MenuBar menu;
	int l, h;
	Rect ocr = GetCaretRect();
	int c = GetMousePos(p);
	int fieldpos = -1;
	Id field;
	String fieldparam;
	String ofieldparam;
	RichObject object, o;
	
	// If user selected a piece of text...
	if(GetSelection(l, h)) {
		if(c >= l && c < h) {
			CopyTool(menu);
			CutTool(menu);
			menu.Add("Search SharePoint", THISBACK(SearchSharePoint));
			menu.Add("De-Format", THISBACK(DeFormatSelection));
			menu.Add("Count Comma-sep", THISBACK(CountCommaSepValues));
		}
		PasteTool(menu);
	}
	else {
		LeftDown(p, flags);
		if(objectpos >= 0) {
			object = GetObject();
			if(!object) return;
			o = object;
			o.Menu(menu, context);
			if(!menu.IsEmpty())
				menu.Separator();
			menu.Add(t_("Object position.."), THISBACK(AdjustObjectSize));
			menu.Separator();
			menu.Add("20 %", THISBACK1(SetObjectPercent, 20));
			menu.Add("40 %", THISBACK1(SetObjectPercent, 40));
			menu.Add("60 %", THISBACK1(SetObjectPercent, 60));
			menu.Add("80 %", THISBACK1(SetObjectPercent, 80));
			menu.Add("90 %", THISBACK1(SetObjectPercent, 90));
			menu.Add("100 %", THISBACK1(SetObjectPercent, 100));
			menu.Break();
			menu.Add(t_("3 pt up"), THISBACK1(SetObjectYDelta, -3));
			menu.Add(t_("2 pt up"), THISBACK1(SetObjectYDelta, -2));
			menu.Add(t_("1 pt up"), THISBACK1(SetObjectYDelta, -1));
			menu.Add(t_("Baseline"), THISBACK1(SetObjectYDelta, 0));
			menu.Add(t_("1 pt down"), THISBACK1(SetObjectYDelta, 1));
			menu.Add(t_("2 pt down"), THISBACK1(SetObjectYDelta, 2));
			menu.Add(t_("3 pt down"), THISBACK1(SetObjectYDelta, 3));
			menu.Separator();
			CopyTool(menu);
			CutTool(menu);
		}
		else {
			RichPos p = cursorp;
			field = p.field;
			fieldparam = p.fieldparam;
			RichPara::FieldType *ft = RichPara::fieldtype().Get(field, NULL);
			if(ft) {
				ofieldparam = fieldparam;
				fieldpos = cursor;
				ft->Menu(menu, &fieldparam);
				if(!menu.IsEmpty())
					menu.Separator();
				CopyTool(menu);
				CutTool(menu);
			}
			else {
				WString w = GetWordAtCursor();
				if(!w.IsEmpty() && !SpellWord(w, w.GetLength(),
				                              fixedlang ? fixedlang : formatinfo.language)) {
					menu.Add(t_("Add to user dictionary"), THISBACK(AddUserDict));
					menu.Separator();
				}
				PasteTool(menu);
				menu.Add("Paste/Join lines as Concat", THISBACK(PasteConcat));
				menu.Add("Paste/Join lines", THISBACK(PasteWithNoWrapper));
				menu.Add("Paste/Join lines/Wrap w/Apostrophe", THISBACK(PasteWithApostrophe)); // Added: JSH, copied from Mouse.cpp
				//menu.Add("Paste as UNION ALL", THISBACK(PasteAsWrappedUnion)); // Added: JSH, copied from Mouse.cpp
				menu.Add("Paste/Run lines thru Line Macro", THISBACK(PasteAsLineMacro));
				menu.Add("Paste/Create line of ORA date strings from range", THISBACK(PasteRunOfOraDates));
				menu.Add("Paste/Split commas to lines", THISBACK(SplitOnComma));
				menu.Add("De-Format", THISBACK(DeFormatScript));
				FindReplaceTool(menu);
				ObjectTool(menu);
			}
		}
	}
	Rect r = GetCaretRect();
	Refresh(r);
	Refresh(ocr);
	paintcarect = true;
	menu.Execute();
	paintcarect = false;
	Refresh(r);
	if(object && o && o.GetSerialId() != object.GetSerialId())
		ReplaceObject(o);
	if(fieldpos >= 0 && fieldparam != ofieldparam) {
		RichText::FormatInfo f = text.GetFormatInfo(fieldpos, 1);
		Remove(fieldpos, 1);
		RichPara p;
		p.Cat(field, fieldparam, f);
		RichText clip;
		clip.Cat(p);
		Insert(fieldpos, clip, false);
		Finish();
	}
}

//==============================================================================================	
bool MyRichEdit::IsLegalWordChar(char cc) {
	return (IsLeNum(cc) || cc == '_');
}

//==============================================================================================	
String MyRichEdit::GetPreviousWord(int &pc, WordTests &wordTests) {
	int c = pc;
	String w = Null; // ERROR!String::GetVoid();

	if (c <= 0) {
		pc = 0;
		return w;
	}
	
	wordTests.Zap();
	
	// Skip back over non-words to first letter/number
	while(c >= 0 && !IsLegalWordChar(Get()[c])) {
		wordTests.followedBy.Insert(0, Get()[c]);  // Save the separation string to identify schemas, etc.
		c--;
	}

	if (c <= 0) {
		pc = 0;
		return w;
	}
	// Now scan through to first let/num
	while(c >= 0 && IsLegalWordChar(Get()[c])) {
		if (IsLetter(Get()[c])) wordTests.containsLetters = true;
		if (IsDigit(Get()[c])) wordTests.containsDigits = true;
		c--;
	}

	if (c >= 0) {
		wordTests.precededBy.Cat(Get()[c]);
	}
	
	c++; // Hit a space jump back onto the first letter, or hit the edge
	int firstchar = c;
	wordTests.startsWithLetter = IsLetter(Get()[c]);
	
	while(c >= 0 && IsLegalWordChar(Get()[c])) {
		w.Cat(ToLower(Get()[c]));
		c++;
	}
	
	pc = firstchar - 1; // Set pointer to character before this
	
	return w;
}

//==============================================================================================	
void MyRichEdit::ProcessPaste() {
	String pasteData = ReadClipboardText();
	ClearClipboard();
	WriteClipboardText(pasteData);
	Paste();
}

//==============================================================================================	
MyRichEdit::KeyProcessorResponse MyRichEdit::PopupRequested(bool ShowAllColumns) {
	// We are now pointing to the character following the period
	int c = GetCursor();
	char cc = Get()[c];
	int posToInsertAt = -1;
	Vector<String> prevWords;
	Vector<WordTests> prevWordTests;
	
	// Only pop-up if at end of a line or a space follows
	if (cc != 10 && cc != 32) return ALLOWDEFAULTKEYPROCESSORTORUN;

	WordTests wordTests;
	
	String keyWord = GetPreviousWord(c, wordTests);
	if (!wordTests.startsWithLetter) return ALLOWDEFAULTKEYPROCESSORTORUN; // Probably a decimal number or something.
	
	// Ok, have preceeding word
	
	if (TrimBoth(keyWord).GetLength() == 0) {
		// Just a floating ".", so assume we want a list of all tables
		
		Rect caretR = GetCaret();
		Rect editorR = GetScreenView();
		tablelist.WhenSelect = THISBACK(SelectedPopUpTable);
		tablelist.PopUp(
			this							// owner
		,	editorR.left + caretR.left		// x
		,	editorR.top + caretR.bottom		// top
		,	editorR.top + caretR.bottom		// bottom
		,	300								// width
		);
		return ALLOWDEFAULTKEYPROCESSORTORUN; // Absorb the dot, just put tablename
	} else {
		// Is the word an alias or what?
		
		// Lets go back to the first letter and start looking for previous word until 
		// we find: from <x><alias> where keyword = <alias>, table name = <x> if . in front of <x> then grab schema
		
		bool foundFrom = false;
		bool foundAlias = false;
		int wordsBackFromKeyWord = 0; // Count
		PopupInfoRequestType popupInfoRequestType = IR_NOMATCH;
		
		String aliasName, wordBefore, schemaName, tableName, ownerName;
		for (String prevWord = GetPreviousWord(c, wordTests); !prevWord.IsEmpty(); prevWord = GetPreviousWord(c, wordTests)) {
			
			wordsBackFromKeyWord++;
			prevWords.Add(prevWord);
			prevWordTests.Add(wordTests); // Note: zero-index based
			
			if (prevWord == keyWord) {
				// Found alias
				aliasName = prevWord; // select * from cdw.account_dim a where a.
				foundAlias = true;
				if (wordTests.precededBy == ".") { // select * from cdrfile.F641 where f641.rrriwo
					tableName = aliasName;
				}
				
				prevWord = GetPreviousWord(c, wordTests);

				if (prevWord == "from") { // select * from x where x.
					foundFrom = true;
					tableName = aliasName;
				} else if (wordTests.precededBy == ".") { // select * from cdw.account_dim a where a.
					tableName = prevWord;
					prevWord = GetPreviousWord(c, wordTests);
					if (wordTests.precededBy == ".") { // select * from STRAW.dbo.main x where 
						ownerName = prevWord;
						schemaName = GetPreviousWord(c, wordTests);
					} else {
						schemaName = prevWord;  // select * from cdrfile.account_dim a where a.xxxx
					}
					
					// We are done, found it
					popupInfoRequestType = IR_COLUMNSINTABLEALIAS;
					break;
				}
				
			// Check: Is user request a list of tables in the schema?
			} else if (prevWord == "from") {
				if (wordsBackFromKeyWord == 1) { // select * from cdw_stg._
					popupInfoRequestType = IR_TABLESINSCHEMA;
					schemaName = keyWord;
					break;
				} else 
				if (                                                                                
					(wordsBackFromKeyWord == 2 && !foundAlias) // select  from fi_executions t._ (absorb dot)
					) {
					// keyword = alias[.]
					// prevwords[0] = tablename
					
					if (ShowAllColumns) {
						tableName = prevWords[0];
						schemaName = Null;
						aliasName = keyWord;
						posToInsertAt = c;
						popupInfoRequestType = IR_INSERTALLCOLUMNSATFROMPOS;
						break;
					}
						
				} else 
				if (
					(wordsBackFromKeyWord == 3 && !foundAlias && prevWordTests[1].followedBy == ".")
					
				    ) { 
						WordTests wt = prevWordTests[1];
						WordTests wt2 = prevWordTests[0];
						 
					// keyword = alias[.]
					// prevwords[0] = tablename
					// prevwords[1] = schemaname[.]
					
					if (ShowAllColumns) {
						schemaName = prevWords[1];
						tableName = prevWords[0];
						aliasName = keyWord;
						posToInsertAt = c;
						popupInfoRequestType = IR_INSERTALLCOLUMNSATFROMPOS;
						break;
					}
				}
			}
		}
		
		if (popupInfoRequestType == IR_NOMATCH) {
			
		}
		if (popupInfoRequestType == IR_COLUMNSINTABLEALIAS) {
			if (foundAlias) {
				WaitCursor();
				if (ShowAllColumns) {
					Exclamation("Not impl");
					// Fetch all, create comma-delimited list
				} else {
					if (schemaName.IsEmpty()) { // No schema provided
						AddColumns(Null, tableName);
					} else {
						AddColumns(schemaName, tableName);
					}
					Rect caretR = GetCaret();
					Rect editorR = GetScreenView();
					columnlist.WhenSelect = THISBACK(SelectedPopUpColumn);
					PasteText(AsRichText(WString("."), GetFormatInfo()));
					columnlist.PopUp(
						this							// owner
					,	editorR.left + caretR.left		// x
					,	editorR.top + caretR.bottom		// top
					,	editorR.top + caretR.bottom		// bottom
					,	300								// width
					);
				}
				return SIGNALWEDIDNOTPROCESSKEY;	// Leave the dot
			}
		} else 
		if (popupInfoRequestType == IR_TABLESINSCHEMA) {
			WaitCursor();
			AddTables(schemaName);
			Rect caretR = GetCaret();
			Rect editorR = GetScreenView();
			tablelist.WhenSelect = THISBACK(SelectedPopUpTable);
			PasteText(AsRichText(WString("."), GetFormatInfo()));
			tablelist.PopUp(
						this							// owner
					,	editorR.left + caretR.left		// x
					,	editorR.top + caretR.bottom		// top
					,	editorR.top + caretR.bottom		// bottom
					,	300								// width (Show alot of the name)
					);
			return SIGNALWEDIDNOTPROCESSKEY; // Leave the dot
		} else 
			
		// Copy all columns before the FROM clause as comma-delimited list
		if (popupInfoRequestType == IR_INSERTALLCOLUMNSATFROMPOS) {
			if (!connection) return ALLOWDEFAULTKEYPROCESSORTORUN;
			WaitCursor();
			Vector<SqlColumnInfo> columns = connection->session->EnumColumns(schemaName, tableName);
			String columnlist;
			for (int i = 0; i < columns.GetCount(); i++) {
				int datatype = columns[i].valuetype;
				if (In(datatype, 1, 2, 3)) {
					columnlist << (i?", ":"") << aliasName << "." << columns[i].name << "/* " << columns[i].valuetype << " */";
				}
			}
			
			// Check spacing at insert point
			char cc = Get()[posToInsertAt];
			if (!In(cc, K_SPACE, K_ENTER, K_TAB)) {
				columnlist.Cat(' '); // Add a character to push out the "from" word
			}
			
			// Spacing before insertion point
			if (posToInsertAt) {
				cc = Get()[posToInsertAt-1];
				if (!In(cc, K_SPACE, K_ENTER, K_TAB)) {
					columnlist.Insert(0, ' '); // Add a character at front to separate from "select"
				}
			}
			
			Insert(posToInsertAt, AsRichText(columnlist.ToWString()));
			
			return ALLOWDEFAULTKEYPROCESSORTORUN; // Don't eat the dot, let script handler insert it
		}
	}
	return ALLOWDEFAULTKEYPROCESSORTORUN;
}

//==============================================================================================	
void MyRichEdit::ScriptContentChanged(dword key) {
	
	// Script has changed, so the script id is no longer correct
	if (scriptId != UNKNOWN) {
		// We may be updating the text for an existing script
		originalScriptId = scriptId;
		scriptId = UNKNOWN;
	}
	
	if (WhenScriptContentChanged) WhenScriptContentChanged();
}

//==============================================================================================	
void MyRichEdit::ResetEditorState() {
	ClearModify(); // May zap the Ctrl-Z capability
	columnlist.Clear();
	columnlist.schemaName = Null;
	columnlist.tableName = Null;
	tablelist.Clear();
	tablelist.schemaName = Null;
}

//==============================================================================================	
bool MyRichEdit::Key(dword key, int count) {
	static dword style;
	static Rect normalwindowrect;

	// Request for popup information extended.  For schemas all columns are listed comma-delimited
	// Must capture before RichEdit key processor, which absorbs it.

	KeyProcessorResponse keyProcessorResponse;


	if (In(key, K_CTRL_KEY, K_ALT_KEY, K_SHIFT_KEY
				, K_CTRL_KEY|K_KEYUP, K_ALT_KEY|K_KEYUP, K_SHIFT_KEY|K_KEYUP)) 
	{
		// Update toolbar tips and icons to reflect the new state of keys
		if (WhenToolBarMayBeAffected) WhenToolBarMayBeAffected();
	}

//	if (key != K_CTRL_KEY) {			
//		Exclamation(CAT << key);
//	}
	
	switch (key) {
		
		// Attempt to paste 
		case K_CTRL_V:
			ProcessPaste();
			keyProcessorResponse = SIGNALWEPROCESSEDKEY;
			return true; // Eat keystroke
			
		// Request for popup information
		case K_PERIOD: 
			keyProcessorResponse = PopupRequested(false);
			break;

		// Request all columns spread across
		case K_CTRL_PERIOD: // Defined in AKeys.cpp and Win32Keys.i
			keyProcessorResponse = PopupRequested(true /* ShowAllColumns */);
			break;
		
		// If selection, wrap in asterisk slashes (comments) or unwrap
		case K_CTRL_ASTERISK:
			if (IsSelection()) {
				String saveclip = ReadClipboardText();
				String sel = GetSelectionAsPlainText();
				if (sel.StartsWith("/*") && sel.EndsWith("*/")) {
					// Strip wrapper
					sel.Remove(0, 2); UrpString::TrimOff(sel, 2);
				} else {
					sel.Insert(0, "/*"); sel.Cat("*/");
				}
				WriteClipboardText(sel);
				Paste();
				WriteClipboardText(saveclip); // Restore any previous clipping
				return true;
			}
		default:
			keyProcessorResponse = ALLOWDEFAULTKEYPROCESSORTORUN;
	}

	
	bool didWeProcessKey = RichEdit::Key(key, count);

	if (IsModified()) {
		// Flush out scriptId if any changes are made, ignore control keys
		ScriptContentChanged(key);
	}

	switch (keyProcessorResponse) {
		case SIGNALWEPROCESSEDKEY:
			return true;
		case SIGNALWEDIDNOTPROCESSKEY:
			return false;
		case ALLOWDEFAULTKEYPROCESSORTORUN:
			break;
		default:
			throw Exc("Unrecognized keyProcessorResponse in MyRichEdit: " + keyProcessorResponse);
	}

	return didWeProcessKey;
}

//==============================================================================================	
// User selected an item from the popup grid, which closes it and sets the Cursor to the selected item.
// We now paste it into the script at the current script cursor.
void MyRichEdit::SelectedPopUpTable() {
	if (!tablelist.IsCursor()) return;
	WString txt = tablelist.Get(0);
	PasteText(AsRichText(txt, GetFormatInfo()));
}

//==============================================================================================	
void MyRichEdit::SelectedPopUpColumn() {
	if (!columnlist.IsCursor()) return;
	WString txt = columnlist.Get(0);
	
	PasteText(AsRichText(txt, GetFormatInfo()));
}

//==============================================================================================	
void MyRichEdit::AddColumns(String schema, String table) {
			
	if (columnlist.schemaName == schema && columnlist.tableName == table) {
		// Already loaded
		return;
	}
	
	// TODO: Keep binary cache and let user force refresh, track age.  Use a flat file
	if (!connection) return;
	
	columnlist.Clear();
	Vector<SqlColumnInfo> columns = connection->session->EnumColumns(schema, table);
	
	columnlist.BackPaint();
	columnlist.BackPaintHint();
	columnlist.PopUpEx().ColumnSortFindKey();
	
	for(int i = 0; i < columns.GetCount(); i++) {
		columnlist.Add(ToLower(columns[i].name));
	}
	columnlist.Sort();
	columnlist.schemaName = schema; columnlist.tableName = table; // Remember this as being cached

}

//==============================================================================================
void MyRichEdit::AddTables(const String &schema) { // owner?
	String schemaName = ToUpper(schema);
	
	
	if (tablelist.schemaName == schemaName) {
		// Already loaded
		return;
	}

	// TODO: Keep binary cache and let user force refresh, track age.  Use a flat file
	if (!connection) return;
	
	tablelist.Clear();
	// Need list of users. SCHEMA NAME IS CASE SENSITIVE!!!!!  I BET POSTGRE has to be lower.
	Vector<String> tables = connection->session->EnumTables(schemaName);
	Vector<String> views = connection->session->EnumViews(schemaName);
	Sort(tables);
	Sort(views);
	tablelist.BackPaint();
	tablelist.BackPaintHint();
	
	for(int i = 0; i < tables.GetCount(); i++) {
		tablelist.Add(ToLower(tables[i]));
	}

	for(int i = 0; i < views.GetCount(); i++) {
		tablelist.Add(ToLower(views[i]));
	}
	
	tablelist.WantFocus();
	tablelist.schemaName = schemaName;
}

//==============================================================================================
// Treat incoming script as plain text.  Underscores in plain text will be converted to
// spaces if treated as QTF, and so DeQtf removes the Qtf interpretation of special
// characters. Legacy.
void MyRichEdit::SetScript(Connection * pconnection, int pconnId, int pscriptId, String pplainText) {
	if (pconnId >= 0) {
		connId = pconnId;
	}
	
	if (pconnection) connection = pconnection; // Else keep any existing connection
	
	if (pscriptId >= 0) {
		scriptId = pscriptId;
	}
	
	SetScriptPlainText(pplainText);
	ResetEditorState(); // New script
}

//==============================================================================================
void MyRichEdit::SetScript(Connection *pconnection, int pconnId, Script &psob) {
	if (pconnId >= 0) {
		connId = pconnId;
	}
	
	if (pconnection) connection = pconnection; // Else keep any existing connection
	
	if (psob.scriptId >= 0) {
		scriptId = psob.scriptId;
	}
	
	//SetScriptPlainText(psob.scriptPlainText);
	SetScriptRichText(psob.scriptRichText);
	ResetEditorState(); // New script
	(Script)*this = psob; // Currently we don't own the script.  Lister main probably does.
}

//==============================================================================================
const RichText &MyRichEdit::GetScriptRichTextRef() {
	return Get();
}

//==============================================================================================
// This is called to convert the rich text to a persistent text storable form, but not plain text.
String MyRichEdit::GetScriptRichTextInStrForm() {
	return GetQTF();
}

//==============================================================================================
// This is called to convert the rich text to a persistent text storable form, but not plain text.
void MyRichEdit::SetScriptRichTextFromStrForm(String prichTextStrForm) {
	Pick(ParseQTF(prichTextStrForm));
	ResetEditorState(); // New script
}

//==============================================================================================
// When copying from the editor, a proper copy must be made or it may disappear when a a new
// script is loaded into the editor, and some other part of the app has a false reference.
RichText MyRichEdit::GetScriptRichText() {
	return Get().Copy(0, INT_MAX);
}

//==============================================================================================
// Since we save the RichScript, we can restore it and not treat it as plain text.
void MyRichEdit::SetScriptRichText(RichText prichText) {
	// Performance issue: background paint will spin badly if ther eare difficult network shares in the environment variables LIB, INCLUDE or PATH.
	// No idea why rich text is scanning the path every keystroke, but I have to find and kill it
	// Detected from install of Sybase ASE which altered the path with a nasty remote ActiveDir

	//Pick(prichText); 
	
	// Cloned from Pick, except we don't clear our history, which I would like to keep so Ctrl-Z works
	// Note: Ctrl-Z still doesn't work.
	text = prichText;
	
	if(text.GetPartCount() == 0)
		text.Cat(RichPara());
	ReadStyles();
	EndSizeTracking();
	Vector<int> all_lang = text.GetAllLanguages();
	SetupLanguage(all_lang);
	Move(0);
	Update();
	
	ResetEditorState();
}

//==============================================================================================
String MyRichEdit::GetScriptQTFText() {
	return AsQTF(Get());
	ResetEditorState(); // New script
}

//==============================================================================================
void MyRichEdit::SetScriptQTFText(const char *pQTFText) {
	SetQTF(pQTFText);	
	ResetEditorState(); // New script
}

//==============================================================================================
// Script is always text, not Rich text.
String MyRichEdit::GetScriptPlainText() {
	return TrimBoth(Get().GetPlainText().ToString());
}

//==============================================================================================
// Set internal RichText text object from incoming plain text.  Inefficient.  Should be storing
// the rich text version in the database, or QTF version.
void MyRichEdit::SetScriptPlainText(const char *pplainText) {
	//text.Insert(int pos, const RichText& p)
	
	//SetQTF => Pick(ParseQTF(qtf, 0, context)); 
	//Pick => text = t; +RichPara()
	//DeQtf => if((byte)*s > ' ' && !IsDigit(*s) && !IsAlpha(*s) && (byte)*s < 128)	r.Cat('`'); else  r.Cat(*s++);
	//DeQtfLf => 	if(*s == '\n')	r.Cat('&');
	
	// The DeQtf function really converts non-QTF data into QTF format where QTF control characters
	// are escaped.  The term "DeQtf" implies incorrectly that it REMOVES QTF codes
	
	SetScriptQTFText(
		FromPlainTextToQTFText(
			pplainText
		)
	);
	ResetEditorState(); // New script

}

//==============================================================================================
// My improved name for the DeQtfLf function
/*static*/ String MyRichEdit::FromPlainTextToQTFText(const char *pplainText) {
	return DeQtfLf(pplainText);
}

//==============================================================================================
int MyRichEdit::GetConnId() {
	return connId;
}

//==============================================================================================	
void MyRichEdit::SetConn(Connection *mconnection) {
	if (connection != mconnection) {
		tablelist.Clear(); tablelist.schemaName = Null;
		columnlist.Clear(); columnlist.schemaName = Null; columnlist.tableName = Null;
	}
	connection = mconnection;
	
	// If we have a connection then we log the connection id since we can't persist the actual pointer over runs
	// By persisting the id, on startup the test can be created.
	if (connection) {
		connId = connection->connId;
	} else {
		connId = UNKNOWN;
	}
}

//==============================================================================================
void MyRichEdit::Log() {
	LogLine(CAT << "Script `#`:" << scriptId << ", conn `#`:" << connId);
	if (connection) {
		LogLine(CAT << "Connection`: " << connection->connName);
	} else {
		LogLine("Connection ptr is null");
	}

	LogLine(CAT << "Script`: [@B " << MyRichEdit::FromPlainTextToQTFText(scriptPlainText) << "]");
	LogLine(CAT << "rowlimit`: " << rowLimit << ", scriptTarget`: " << (int)scriptTarget
	        << ", targetname`: " << DeQtf(targetName)); // << ", flush?" << sob->fastFlushTarget);
}

//==============================================================================================
// Save script id with script so "Add test for script" button works from toolbar.
/*virtual*/ void MyRichEdit::Xmlize(XmlIO xml) {
	xml
		("scriptid", scriptId)
		("connid", connId)
	;
	// Copied following from Ctrl::Xmlize.  Not sure how it works.
	Value v = GetData();
	UPP::Xmlize(xml, v);
	SetData(v);
	if (xml.IsLoading()) ResetEditorState();
}
