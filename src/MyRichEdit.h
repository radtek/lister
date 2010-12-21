#ifndef _MyRichEdit_h_
#define _MyRichEdit_h_

#include "shared.h"
#include "SqlUtil.h"

#include <RichEdit/RichEdit.h>
#include <Web/Web.h>

//==========================================================================================	
struct MyPopUpTable : public PopUpTable {
	MyPopUpTable() : PopUpTable() {
	}
	
	String schemaName;
	String tableName;
};

//==========================================================================================	
struct WordTests {
	WordTests() {
		Zap();
	}

	//==========================================================================================	
	void Zap() {
		startsWithLetter = false;
		containsLetters = false;
		containsDigits = false;
		firstWord = false;
		comesBefore = "";
		comesAfter = "";
	}
	bool startsWithLetter;
	bool containsLetters;
	bool containsDigits;
	bool firstWord;
	String comesAfter; // Junk between last searched word and this one (depending on search direction
	String comesBefore;
};

static const String MACRO = "[[INPUT]]";

//==========================================================================================	
struct MyRichEdit : public RichEdit {
	MyPopUpTable tablelist, columnlist;
	typedef MyRichEdit CLASSNAME;
	Connection *connection;
	int scriptId; // If brought from sqllist, then this is set, until modified
	double zoomlevel;

	//==========================================================================================	
	MyRichEdit() {
		connection = NULL;
		scriptId = -1; // Unknown
		tablelist.AddColumn("Table", 100);
		tablelist.Normal();
		columnlist.AddColumn("Column", 100);
		columnlist.Normal();
		NoRuler();
		ShowCodes(Null);
		ViewBorder(0);
		zoomlevel = 8.0; // default zoom is about 9 pt on my screen
	}
	
	//==========================================================================================	
	//  Added a zoom text feature.
	virtual void MouseWheel(Point p, int zdelta, dword keyflags) {
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
	
	//==========================================================================================	
	void Layout() {
		RichEdit::Layout();
		long editor_cx = GetSize().cx;
		long adaptive_cx = (long)(editor_cx * zoomlevel); // Smaller the number, the bigger the text
		SetPage(Size(adaptive_cx, INT_MAX));
	}
	
	//==========================================================================================	
	void SetConn(Connection *mconnection) {
		if (connection != mconnection) {
			tablelist.Clear();
		}
		connection = mconnection;
	}
	
	//==========================================================================================	
	//  Turn a set of lines into a stream for IN clause use
	void PasteWithApostrophe() {
		String ci = ReadClipboardText();
		String co;
		
		if (ci.Find('\n') > 0) {
			Vector<String> cil = Split(ci, '\n');
			Sort(cil);
			for (int i = 0; i < cil.GetCount(); i++) {
				if (i) co << ", ";
				co << "'" << TrimRight(cil[i]) << "'";
			}
		} else {
			co << "'" << ci << "'";
		}
		
		WriteClipboardText(co);
		Paste();
		WriteClipboardText(ci); // Restore clipboard so it doesn't keep expanding
	}

	//==========================================================================================	
	//  Turn a set of lines into series of union alls (for ex:)
	void PasteAsWrappedUnion() {
		static String wrapperCmd = MACRO;  // Retain any changes the user makes
		
		String ci = ReadClipboardText();
		String co;
		
		if (ci.Find('\n') > 0) {
			if(MyEditText(wrapperCmd, "Enter wrapper text for UNION ALL Build", "Wrapper (leave macro or it won't work!!!). Use \\n for new lines:")) {
				Vector<String> cil = Split(ci, '\n');
				Sort(cil);
				for (int i = 0; i < cil.GetCount(); i++) {
					if (i) co << "\nunion all\n";
					
					// Converts "Select * from [[INPUT]]" to "Select * from x union all select * from y..."
					// Note that the macro can be doubled, tripled, etc
					String nco = ReplaceStr(wrapperCmd, MACRO, TrimRight(cil[i]));
					co << nco;
				}
			}
		} else {
			co << "'" << ci << "'";
		}
		
		WriteClipboardText(co);
		Paste();
		WriteClipboardText(ci); // Restore clipboard so it doesn't keep expanding
	}

	//==========================================================================================	
	static String DumpSpecial(String s) {
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

	//==========================================================================================	
	void SearchSharePoint() {
		RichText txt = GetSelection();
		String searchString = txt.GetPlainText().ToString();
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

	//==========================================================================================	
	virtual void RightDown(Point p, dword flags) {
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
		if(GetSelection(l, h)) {
			if(c >= l && c < h) {
				CopyTool(menu);
				CutTool(menu);
				menu.Add("Search SharePoint", THISBACK(SearchSharePoint));
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
					menu.Add("Paste/Wrap w/Apostrophe", THISBACK(PasteWithApostrophe)); // Added: JSH, copied from Mouse.cpp
					menu.Add("Paste as UNION ALL", THISBACK(PasteAsWrappedUnion)); // Added: JSH, copied from Mouse.cpp
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

	//==========================================================================================	
	bool IsLegalWordChar(char cc) {
		return (IsLeNum(cc) || cc == '_');
	}
	
	//==========================================================================================	
	String GetPreviousWord(int &pc, WordTests &wordTests) {
		int c = pc;
		String w = String::GetVoid();
	
		if (c <= 0) {
			pc = 0;
			return w;
		}
		
		wordTests.Zap();
		
		// Skip back over non-words to first letter/number
		while(c >= 0 && !IsLegalWordChar(Get()[c])) {
			wordTests.comesAfter.Insert(0, Get()[c]);  // Save the separation string to identify schemas, etc.
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
			wordTests.comesBefore.Cat(Get()[c]);
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
	
	//==========================================================================================	
	bool Key(dword key, int count) {
		static dword style;
		static Rect normalwindowrect;
		
		RichEdit::Key(key, count);

		switch (key) {
			case '.': {
				// We are now pointing to the character following the period
				int c = GetCursor();
				char cc = Get()[c];
				
				// Only pop-up if at end of a line or a space follows
				if (cc != 10 && cc != 32) return false;

				WordTests wordTests;
				
				String keyWord = GetPreviousWord(c, wordTests);
				if (!wordTests.startsWithLetter) return false; // Probably a decimal number or something.
				
				// Ok, have preceeding word
				
				if (TrimBoth(keyWord).GetLength() == 0) {
					// Just a floating ".", so assume we want a list of all tables
					
					Rect caretR = GetCaret();
					Rect editorR = GetScreenView();
					tablelist.WhenSelect = THISBACK(SelectedPopUpTable);
					tablelist.PopUp(this, editorR.left + caretR.left, editorR.top + caretR.bottom, editorR.top + caretR.bottom, editorR.top + caretR.bottom);
					return true; // Absorb the dot, just put tablename
				} else {
					// Is the word an alias or what?
					
					// Lets go back to the first letter and start looking for previous word until 
					// we find: from <x><alias> where keyword = <alias>, table name = <x> if . in front of <x> then grab schema
					
					bool foundFrom = false;
					bool foundAlias = false;
					String aliasName, wordBefore, schemaName, tableName, ownerName;
					for (String prevWord = GetPreviousWord(c, wordTests); !prevWord.IsVoid(); prevWord = GetPreviousWord(c, wordTests)) {
						if (prevWord == keyWord) {
							// Found alias
							aliasName = prevWord; // select * from cdw.account_dim a where a.
							foundAlias = true;
							if (wordTests.comesBefore == ".") { // select * from cdrfile.F641 where f641.rrriwo
								tableName = aliasName;
							}
							
							prevWord = GetPreviousWord(c, wordTests);
	
							if (prevWord == "from") { // select * from x where x.
								foundFrom = true;
								tableName = aliasName;
							} else if (wordTests.comesBefore == ".") { // select * from cdw.account_dim a where a.
								tableName = prevWord;
								prevWord = GetPreviousWord(c, wordTests);
								if (wordTests.comesBefore == ".") { // select * from STRAW.dbo.main x where 
									ownerName = prevWord;
									schemaName = GetPreviousWord(c, wordTests);
								} else {
									schemaName = prevWord;  // select * from cdrfile.account_dim a where a.xxxx
								}
								
								// We are done, found it
								break;
							}
						}
					}
					
					if (foundAlias) {
						WaitCursor();
						if (schemaName.IsEmpty()) { // No schema provided
							AddColumns(Null, tableName);
						} else {
							AddColumns(schemaName, tableName);
						}
						Rect caretR = GetCaret();
						Rect editorR = GetScreenView();
						columnlist.WhenSelect = THISBACK(SelectedPopUpColumn);
						columnlist.PopUp(this, editorR.left + caretR.left, editorR.top + caretR.bottom, editorR.top + caretR.bottom, editorR.top + caretR.bottom);
						return false; // Leave the dot
					}
							           
				}
				
				// Either an alias, a database, schema
			}
			break;
			case K_CTRL_SPACE:
			break;
		}
		
		return false;
	}

	//==========================================================================================	
	void SelectedPopUpTable() {
		WString txt = tablelist.Get(0);
		PasteText(AsRichText(txt, GetFormatInfo()));
	}

	//==========================================================================================	
	void SelectedPopUpColumn() {
		WString txt = columnlist.Get(0);
		PasteText(AsRichText(txt, GetFormatInfo()));
	}

	//==========================================================================================	
	void AddColumns(String schema, String table) {
				
		if (columnlist.schemaName == schema && columnlist.tableName == table) {
			// Already loaded
			return;
		}

		// TODO: Keep binary cache and let user force refresh, track age.  Use a flat file
		if (!connection) return;
		
		columnlist.Clear();
		Vector<SqlColumnInfo> columns = connection->session->EnumColumns(schema, table);
		//Sort(tables);
		columnlist.BackPaint();
		columnlist.BackPaintHint();
		for(int i = 0; i < columns.GetCount(); i++) {
			columnlist.Add(ToLower(columns[i].name));
		}
		columnlist.schemaName = schema; columnlist.tableName = table; // Remember this as being cached

	}
	
	//==========================================================================================	
	void Serialize(Stream &s) {
		RichEdit::Serialize(s);
		int version = 0;
		s / version;
		s % scriptId; // By serializing id, when you start up, the previous script will be present and you'll be able to link up to the test screen.
	}
};


#endif
