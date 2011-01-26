#ifndef _MyRichEdit_h_
#define _MyRichEdit_h_

#include "shared.h"

#include <RichEdit/RichEdit.h>
#include <CtrlLib/CtrlLib.h>
#include <Web/Web.h>
#include "Script.h"
#include "LogWin.h"

class Connection;

//==============================================================================================
struct MyPopUpTable : public PopUpTable {
	MyPopUpTable() : PopUpTable() {
	}
	
	String schemaName;
	String tableName;
};

//==============================================================================================
struct WordTests : public Moveable<WordTests> {
	WordTests() {
		Zap();
	}

	//==========================================================================================	
	void Zap() {
		startsWithLetter = false;
		containsLetters = false;
		containsDigits = false;
		firstWord = false;
		precededBy = "";
		followedBy = "";
	}
	bool startsWithLetter;
	bool containsLetters;
	bool containsDigits;
	bool firstWord;
	String followedBy; // Junk between last searched word and this one (depending on search direction
	String precededBy;
};

//==============================================================================================
struct MyRichEdit : public RichEdit, public Script {
	typedef MyRichEdit CLASSNAME;
	MyPopUpTable	    tablelist // Popup helpers
	,				    columnlist;
	Connection         *connection;
	int                 connId;  // connection id for this connection, so we can save it in configuration
	//int                 scriptId; // If brought from sqllist, then this is set.  Saved in configuration to make test creation easier
	//int                 originalScriptId; // If script content changes, this is set and scriptId is cleared
	//Script             *sob; // More sophisticated object that tracks output parameters along with the text
	bool                createdSobPresent; // Set if we had to create an sob object, so we can destroy it
	Callback            WhenScriptContentChanged;  // Set from main to detect when to update the toolbar
	Callback            WhenToolBarMayBeAffected; // Control keys affect the meaning of various buttons, so we want to update their colurs, tips, etc
	double              zoomlevel;
	enum PopupInfoRequestType {
		IR_NOMATCH
	,	IR_TABLESINSCHEMA
	,	IR_COLUMNSINTABLEALIAS
	,	IR_TABLESMRU
	,	IR_INSERTALLCOLUMNSATFROMPOS
	};
	
	// What do we do after our custom keyhandler runs?
	enum KeyProcessorResponse {
		ALLOWDEFAULTKEYPROCESSORTORUN
	,	SIGNALWEPROCESSEDKEY
	,	SIGNALWEDIDNOTPROCESSKEY
	};
	
	                MyRichEdit();
	virtual        ~MyRichEdit();
	
	//  Added a zoom text feature.
	virtual void    MouseWheel(Point p, int zdelta, dword keyflags);
	void            Layout();
	//  Turn a set of lines into a stream for IN clause use
	void            PasteWithApostrophe();
	void            PasteAsLineMacro();
	//  Turn a set of lines into series of text with the clipboard text pasted in
	String          PasteAsLineMacro(
		Vector<String> lines, String macroMarker, String inBetweenNewLine, String title, 
		String furtherInstructions = Null, String wrapperString = Null);
	void            PasteAsWrappedUnion(); //  Turn a set of lines into series of union alls (for ex:)
	static String   DumpSpecial(String s); // Came with the web search below.  Untested.	
	void            SearchSharePoint(); // Doesn't work.
	void            DeFormatSelection(); // Strip Qtf codes off of text.
	void            DeFormatScript(); // Clean entire thing of Qtf.
	// Stolen from the RichEdit since I needed to manipulated non-exposed components.
	virtual void    RightDown(Point p, dword flags);
	bool            IsLegalWordChar(char cc);
	String          GetPreviousWord(int &pc, WordTests &wordTests);
	void            ProcessPaste();
	MyRichEdit::KeyProcessorResponse 
	                PopupRequested(bool ShowAllColumns);
	virtual bool    Key(dword key, int count);
	// User selected an item from the popup grid, which closes it and sets the Cursor to the selected item.
	// We now paste it into the script at the current script cursor.
	void            SelectedPopUpTable();
	void            SelectedPopUpColumn();
	void            AddColumns(String schema, String table);
	void            AddTables(const String &schema);
	void            ScriptContentChanged(dword key);
	void            SetScript(Connection *pconnection, int pconnId, int pscriptId, String pplainText); // Script is always text, not Rich text.
	// Enhanced version with target management
	void            SetScript(Connection *pconnection, int pconnId, Script &psob); 
	const RichText &GetScriptRichTextRef();
	String          GetScriptRichTextInStrForm();
    void            SetScriptRichTextFromStrForm(String prichTextStrForm);
	RichText        GetScriptRichText();
	void            SetScriptRichText(RichText prichText); // Should create Rich (Qtf) string.
	String          GetScriptQTFText();
	void            SetScriptQTFText(const char *pQTFText);
	String          GetScriptPlainText(); // Returns plain text
	void            SetScriptPlainText(const char *pplainText);
	// Save script id with script so "Add test for script" button works from toolbar.
	void            Log();
	virtual void    Xmlize(XmlIO xml);
	static String   FromPlainTextToQTFText(const char *pplainText); // Improved name for DeQtfLf
	String          GetSelectionAsPlainText(); // My little helper
	void            SetConn(Connection *mconnection);
	int             GetConnId();
	
};


#endif

