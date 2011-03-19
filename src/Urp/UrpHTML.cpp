#include "UrpHTML.h"

//==============================================================================================
// Stolen from http://support.microsoft.com/kb/274308
/*
  HTML has its own clipboard format called HTML Format (CF_HTML) that you can use to provide your 
  data to other applications, such as Microsoft Excel, Microsoft Word, or other Microsoft Office 
  applications. 
  CF_HTML is entirely a text-based format that includes a description, a context, and a fragment 
  within that context. When you build data to send to the clipboard, you must include a description of the data 
  to indicate the clipboard version and the offsets for the context and fragment. Calculating the 
  offsets can be the difficult part. However, you can use the following routine to simplify this 
  task: 
  
  for details on CF_HTML: http://msdn.microsoft.com/en-us/library/aa767917.aspx
  /*
 * Predefined Clipboard Formats

	#define CF_TEXT             1
	#define CF_BITMAP           2
	#define CF_METAFILEPICT     3
	#define CF_SYLK             4
	#define CF_DIF              5
	#define CF_TIFF             6
	#define CF_OEMTEXT          7
	#define CF_DIB              8
	#define CF_PALETTE          9
	#define CF_PENDATA          10
	#define CF_RIFF             11
	#define CF_WAVE             12
	#define CF_UNICODETEXT      13
	#define CF_ENHMETAFILE      14
	#if(WINVER >= 0x0400)
	#define CF_HDROP            15
	#define CF_LOCALE           16

	cpp_quote("")
	cpp_quote("// --------------------------------------------------------------------------------")
	cpp_quote("// IMimeMessage IDataObject clipboard formats (also include CF_TEXT)")
	cpp_quote("// --------------------------------------------------------------------------------")
	cpp_quote("STRCONSTA(STR_CF_HTML,               \"HTML Format\");")
	cpp_quote("STRCONSTA(STR_CF_INETMSG,            \"Internet Message (rfc822/rfc1522)\");")
	cpp_quote("STRCONSTA(STR_CF_RFC822,             \"message/rfc822\");")
	cpp_quote("")

*/
void CopyHTMLToClipboard(const char *html) {
    // Create temporary buffer for HTML header...
    char *buf = new char [400 + strlen(html)];
    if(!buf) return;

    // Get clipboard id for HTML format...
    static int cfid = 0;
    if(!cfid) cfid = RegisterClipboardFormat("HTML Format");

    // Create a template string for the HTML header...
    strcpy(buf,
        "Version:0.9\r\n"
        "StartHTML:00000000\r\n"
        "EndHTML:00000000\r\n"
        "StartFragment:00000000\r\n"
        "EndFragment:00000000\r\n"
        "<html><body>\r\n"
        "<!--StartFragment -->\r\n");

    // Append the HTML...
    strcat(buf, html);
    strcat(buf, "\r\n");
    // Finish up the HTML format...
    strcat(buf,
        "<!--EndFragment-->\r\n"
        "</body>\r\n"
        "</html>");

    // Now go back, calculate all the lengths, and write out the
    // necessary header information. Note, wsprintf() truncates the
    // string when you overwrite it so you follow up with code to replace
    // the 0 appended at the end with a '\r'...
    char *ptr = strstr(buf, "StartHTML");
    wsprintf(ptr+10, "%08u", strstr(buf, "<html>") - buf);
    *(ptr+10+8) = '\r';

    ptr = strstr(buf, "EndHTML");
    wsprintf(ptr+8, "%08u", strlen(buf));
    *(ptr+8+8) = '\r';

    ptr = strstr(buf, "StartFragment");
    wsprintf(ptr+14, "%08u", strstr(buf, "<!--StartFrag") - buf);
    *(ptr+14+8) = '\r';

    ptr = strstr(buf, "EndFragment");
    wsprintf(ptr+12, "%08u", strstr(buf, "<!--EndFrag") - buf);
    *(ptr+12+8) = '\r';

    // Now you have everything in place ready to put on the
    // clipboard.

    // Open the clipboard...
    if(OpenClipboard(0)) {
        
        // Empty what's in there...
        EmptyClipboard();
        
        // Allocate global memory for transfer...
        HGLOBAL hText = GlobalAlloc(GMEM_MOVEABLE |GMEM_DDESHARE, strlen(buf)+4);
        
        // Put your string in the global memory...
        char *ptr = (char *)GlobalLock(hText);
        strcpy(ptr, buf);
        GlobalUnlock(hText);
        
        ::SetClipboardData(cfid, hText);
        
        CloseClipboard();
        // Free memory...
        GlobalFree(hText);
    }

    // Clean up...
    delete [] buf;
}

//==============================================================================================
// Create a header for building a clipboard HTML fragment that will paste into Outlook as a 
// nicely formatted grid.  This is important especially for values with leading zeros that
// need to be preserved.  The xtextformat must be used or they will be treated as numbers if
// all characters are digits and the leading zeros will get zapped.  Large numbers that are really ids and need
// to be treated as text should be marked, or they will be converted into floating point.

// Example of usage:  
// 1) First create a string with the head
// 2) then append the following for each column: (note that col tag is not closed, must be MS bug)
	// <col width=122 style='mso-width-source:userset;mso-width-alt:4461;width:92pt'>
	// <tr height=17 style='height:12.75pt'>
	//  <td height=17 class=xquantityformat width=122 style='height:12.75pt;width:92pt'>B_PAYMENT_AMT</td>
	// </tr>
	// <tr height=17 style='height:12.75pt'>
	//  <td height=17 class=xquantityformat align=right style='height:12.75pt'>12,000,000 </td>
	// </tr>
// 3) then call CopyHTMLToClipboard.  We should take the <html><body> tag out of there!
// Done!
// See /lister/GridCtrl::SetClipboard for example.
String GetExcelHTMLHead() {
	String head =
		"<html xmlns:o=\"urn:schemas-microsoft-com:office:office\""
		"xmlns:x=\"urn:schemas-microsoft-com:office:excel\""
		"xmlns=\"http://www.w3.org/TR/REC-html40\">"
		"<head>"
		"<meta http-equiv=Content-Type content=\"text/html; charset=utf-8\">"
		"<meta name=ProgId content=Excel.Sheet>"
		"<meta name=Generator content=\"Microsoft Excel 12\">"
		"<link id=Main-File rel=Main-File"
		"<!--table"
		"	{mso-displayed-decimal-separator:\"\.\";"
		"	mso-displayed-thousand-separator:\"\,\";}"
		"@page"
		"	{margin:1.0in .75in 1.0in .75in;"
		"	mso-header-margin:.5in;"
		"	mso-footer-margin:.5in;}"
		"tr"
		"	{mso-height-source:auto;}"
		"col"
		"	{mso-width-source:auto;}"
		"br"
		"	{mso-data-placement:same-cell;}"
		"td"
		"	{padding:0px;"
		"	mso-ignore:padding;"
		"	color:windowtext;"
		"	font-size:10.0pt;"
		"	font-weight:400;"
		"	font-style:normal;"
		"	text-decoration:none;"
		"	font-family:Arial;"
		"	mso-generic-font-family:auto;"
		"	mso-font-charset:0;"
		"	mso-number-format:General;"
		"	text-align:general;"
		"	vertical-align:bottom;"
		"	border:none;"
		"	mso-background-source:auto;"
		"	mso-pattern:auto;"
		"	mso-protection:locked visible;"
		"	white-space:nowrap;"
		"	mso-rotate:0;}"
		".xdatetimeformat"
		"	{mso-number-format:\"General Date\";}"
		".xlongdatetimeformat"
		"	{mso-number-format:\"mm\/dd\/yyyy\\ hh\:mm\:ss\";}"
		".xdateformat"
		"	{mso-number-format:\"mm\/dd\/yyyy\";}"
		".xshortdateformat"
		"	{mso-number-format:\"mm\/dd\";}"
		".xtextformat"
		"	{font-family:\"Arial Unicode MS\";mso-number-format:\"\@\";}"
		".xcurrencyformat"
		"	{mso-number-format:\"\0022$\0022\#\,\#\#0\.00\";}"
		".xquantityformat"
		"	{mso-number-format:\"\#\,\#\#0_\)\;\[Red\]\\\(\#\,\#\#0\\\)\";}"
		".xpriceformat"
		"	{mso-number-format:\"\0022$\0022\#\,\#\#0\.00000\";}"
		"-->"
		"</style>"
		"</head>"
	;
	return head;
}

//==============================================================================================
// Aborted experiment; will be useful to output paste output directly to a file that can be saved and then 
// the clipboard is set to the path so that it can be attached to tools like JIRA or Test Director, etc.
String GridCtrl::ConstructExcelXML(int rowcount, int colcount) {
	String headerTemplate = 
		"	<?xml version=\"1.0\"?>"
		"<?mso-application progid=\"Excel.Sheet\"?>"
		"<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\""
		" xmlns:o=\"urn:schemas-microsoft-com:office:office\""
		" xmlns:x=\"urn:schemas-microsoft-com:office:excel\""
		" xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\""
		" xmlns:html=\"http://www.w3.org/TR/REC-html40\">"
		" <DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">"
		"  <Author> </Author>"
		"  <LastAuthor> </LastAuthor>"
		"  <Created>2011-03-17T17:46:22Z</Created>"
		"  <LastSaved>2011-03-18T14:30:29Z</LastSaved>"
		"  <Company> </Company>"
		"  <Version>12.00</Version>"
		" </DocumentProperties>"
		" <ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">"
		"  <WindowHeight>11640</WindowHeight>"
		"  <WindowWidth>22860</WindowWidth>"
		"  <WindowTopX>0</WindowTopX>"
		"  <WindowTopY>60</WindowTopY>"
		"  <ProtectStructure>False</ProtectStructure>"
		"  <ProtectWindows>False</ProtectWindows>"
		" </ExcelWorkbook>"
		" <Styles>"
		"  <Style ss:ID=\"Default\" ss:Name=\"Normal\">"
		"   <Alignment ss:Vertical=\"Bottom\"/>"
		"   <Borders/>"
		"   <Font ss:FontName=\"Arial\" x:Family=\"Swiss\" ss:Color=\"#000000\"/>"
		"   <Interior/>"
		"   <NumberFormat/>"
		"   <Protection/>"
		"  </Style>"
		"  <Style ss:ID=\"s63\">"
		"   <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Bottom\"/>"
		"   <Font ss:FontName=\"Calibri\" x:Family=\"Swiss\" ss:Size=\"11\" ss:Color=\"#000000\"/>"
		"  </Style>"
		"  <Style ss:ID=\"s64\">"
		"   <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Bottom\"/>"
		"  </Style>"
		"  <Style ss:ID=\"s65\">"
		"   <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Bottom\"/>"
		"   <NumberFormat ss:Format=\"Short Date\"/>"
		"  </Style>"
		"  <Style ss:ID=\"s67\">"
		"   <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Bottom\"/>"
		"   <NumberFormat ss:Format=\"h:mm:ss\"/>"
		"  </Style>"
		" </Styles>"
		" <Worksheet ss:Name=\"Sheet1\">"
		"  <Table ss:ExpandedColumnCount=\"%d\" ss:ExpandedRowCount=\"%d\" x:FullColumns=\"1\""
		"   x:FullRows=\"1\">"
		"}";
	
	return Format(headerTemplate, rowcount, colcount);
}
