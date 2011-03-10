#include "MacroHandler.h"
#include "lister/Urp/UrpString.h"
#include "lister/Urp/UrpTime.h"
// Necessary for RegExp use.  Add package plugin/pcre.
// Had to add folder: C:\Program Files\Microsoft Visual Studio 10.0\VC\include to find stdint.h
#include <plugin/pcre/Pcre.h>

//==============================================================================================
// Convert [[]] codes to values.  Hacky for now.
String ExpandMacros(String inputText, ContextMacros *contextMacros) {
	MacMap &taskMacros = contextMacros->taskMacros;
	if (contextMacros) {
		for (int i = 0; i < taskMacros.GetCount(); i++) {
			String searchMacro;
			String searchFor(taskMacros.GetKey(i));
			searchMacro << "[[" << searchFor << "]]";
			MacPair macPair = taskMacros.Get(searchFor);
			inputText = UrpString::ReplaceInWhatWith(inputText, searchMacro, macPair.expansion);
		}
	}
	
	String macro;
	String expansion;
	
	Date curDate = GetSysDate();

	// Search for macros in the form [[AAAANN]] to transform into values
	// http://www.regextester.com/pregsyntax.html
	// Matches can be TPLUS1, TPLUS5, TMINUS3
	
	String searchFor = "\\[\\[([a-zA-Z]+)(\\d+)\\]\\]"; // Parens force the digits to be inserted into r0[0], r0[1]
	RegExp r0(searchFor);
	
	if (r0.Match(inputText)) {
		String command = r0[0]; 
		String arg1 = r0[1];
	
		ToUpper(Trim(command));
		if (command == "TPLUS") {
			if (arg1 == "") {
				expansion << "{Missing numeric argument for command " << command << "}";
			} else {
				Date endDate = GetDateFromDateBDays(curDate, 1);
				int expansionnum = endDate - curDate;
				expansion = ToString(expansionnum);
			}
		} else {
			expansion << "{Unsupported command " << command << "}";
		}
		inputText = UrpString::ReplaceInWhatWith(inputText, macro, expansion);	
		
	// Command/arg format not found; Just isolate the macro if there and push error message out
	} else {
		searchFor = "\\[\\[(\\w+)\\]\\]";
		RegExp r1(searchFor);
		if (r1.Match(inputText)) {
			expansion << "{Unrecognized macro format: " << command ", must be [[AAAANN]]}";
			inputText = UrpString::ReplaceInWhatWith(inputText, macro, expansion);	
		}
	}

	return inputText;
}

// matching examples:
// [^\W_]           Matches any letter or digit, but not underscore
// [W-]46]          is interpreted as a class of two characters ("W" and "-") followed by a literal string "46]", so it would match "W46]" or "-46]
// [d-m]            matches any letter between d and m
// [^a]             matches a newline
// [aeiou]          matches "A" as well as "a"
// [^aeiou]         does not match "A"
// [aeiou]          matches any lower case vowel
// [^aeiou]         matches any character that is not a lower case vowel
// [\dABCDEF]       matches hexidecimal digits
// gilbert|sullivan matches either "gilbert" or "sullivan"
// the string "the red king" is matched against the pattern the ((red|white) (king|queen)) the captured substrings are "red king", "red", and "king", and are numbered 1, 2, and 3
// (\\w+)           match and capture a word
// \\s              match any space?
// /^abc$/          matches the subject string "def\nabc" in multiline mode, but not otherwise PCRE_MULTILINE
// (?!a)            asserts that the next character is not "a"?
// \d               matches any digit
// a.*?b            matches "ab" in "ababab" (ungreedy, minimum matching first)
// a.*b             matches "ababab" in "ababab" (greedy, longest match first)
// \p{Ps}.*?\p{Pe}  match a string beginning with any "opening punctuation" and ending with any "close punctuation" such as "[abc]"
// a++bc            will match as many "a"s as possible, and never back up to try one less
// /\w+(?=\t)/      matches a word followed by a tab, without including the tab
// (?#comment)      A comment
// \((a*|(?R))*\)   match any combination of balanced parentheses and "a"s
// \p{L}            matches unicode letter
// \bex             in the string "Texts for experts", \bex matches the "ex" in "experts" but not in "Texts" (because the "ex" occurs inside a word and not immediately after a word boundary).
// H(ä|ae?)ndel     matches "Handel", "Händel", and "Haendel"
// H(a|ae|ä)ndel    matches "Handel", "Händel", and "Haendel"
// colou?r          matches both "color" and "colour"
// gr(a|e)y         matches "gray" and "grey"
// ab*c             matches "ac", "abc", "abbc", "abbbc"
// ab+c             "abc", "abbc", "abbbc", and so on, but not "ac"
// [abcx-z]         matches "a", "b", "c", "x", "y", or "z", as does [a-cx-z]
// []abc]           matches ],a,b, or c
// [^abc]           matches any character other than "a", "b", or "c"
// [^a-z]           matches any single character that is not a lowercase letter from "a" to "z"
// a\{3,5\}         matches only "aaa", "aaaa", and "aaaaa"
// [xyz]*           matches "", "x", "y", "z", "zx", "zyx", "xyzzy", and so on
// \(ab\)*          matches "", "ab", "abab", "ababab", and so on
// .at              matches any three-character string ending with "at", including "hat", "cat", and "bat"
// [hc]at           matches "hat" and "cat".
// [^b]at           matches all strings matched by .at except "bat".
// ^[hc]at          matches "hat" and "cat", but only at the beginning of the string or line.
// \[.\]            matches any single character surrounded by "[" and "]" since the brackets are escaped, for example: "[a]" and "[b]".
// <.*>             input Another whale sighting occurred on <January 26>, <2004>. will return "<January 26>, <2004>"
// <.*?>            Another whale sighting occurred on <January 26>, <2004> is "<January 26>" and "<2004>"
// <[^>]*>          Another whale sighting occurred on <January 26>, <2004> is "<January 26>" and "<2004>"
// (.*)\1           may not work.  backreferences or squares match 
// {n}              match exactly n times
// ([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})\\s-\\s-\\s\\[([^\\]]+)\\]  IP Address
// ^(?:(?:31(\/|-|\.)(?:0?[13578]|1[02]))\1|(?:(?:29|30)(\/|-|\.)(?:0?[1,3-9]|1[0-2])\2))(?:(?:1[6-9]|[2-9]\d)?\d{2})$|^(?:29(\/|-|\.)0?2\3(?:(?:(?:1[6-9]|[2-9]\d)?(?:0[48]|[2468][048]|[13579][26])|(?:(?:16|[2468][048]|[3579][26])00))))$|^(?:0?[1-9]|1\d|2[0-8])(\/|-|\.)(?:(?:0?[1-9])|(?:1[0-2]))\4(?:(?:1[6-9]|[2-9]\d)?\d{2})$
//    Validates d/m/y, matches 29/02/1972, 5-9-98, 10-11-2002
//    Non-matches: 29/02/2003, 12/12/2002
// (\d*)'*-*(\d*)/*(\d*)&quot; parses feet and inches.  5'-3/16&quot; | 1'-2&quot; | 5/16&quot
// ^((((31\/(0?[13578]|1[02]))|((29|30)\/(0?[1,3-9]|1[0-2])))\/(1[6-9]|[2-9]\d)?\d{2})|(29\/0?2\/(((1[6-9]|[2-9]\d)?(0[48]|[2468][048]|[13579][26])|((16|[2468][048]|[3579][26])00))))|(0?[1-9]|1\d|2[0-8])\/((0?[1-9])|(1[0-2]))\/((1[6-9]|[2-9]\d)?\d{2})) (20|21|22|23|[0-1]?\d):[0-5]?\d:[0-5]?\d$
//    maths d/m/y h:m:s 29/02/2004 20:15:27
// ^[1-9]{1}$|^[1-4]{1}[0-9]{1}$|^50$  Match any number between 1 and 50, no characters, no empty sets, and not zero. Match numbers greater than 1 and less than 51, no spaces, no characters.
// (^-\d*\.?\d*[1-9]+\d*$)|(^-[1-9]+\d*\.\d*$) Accepts only negative decimal values. Zero and positive numbers are non-matching. Allows zeros after last non-zero numeric value after decimal place for significant digits.
// ^\$(\d{1,3}(\,\d{3})*|(\d+))(\.\d{2})?$    US currency, This re matches US currency format with lead dollar sign. Dollar value must have at least one digit and may or may not be comma separated. Cents value is optional.
// (?!^0*$)(?!^0*\.0*$)^\d{1,5}(\.\d{1,3})?$     This regular expression validates a number NOT 0, with no more than 5 places ahead and 3 places behind the decimal point.
// ^([0-5]?\d?\d?\d?\d|6[0-4]\d\d\d|65[0-4]\d\d|655[0-2]\d|6553[0-5])$  match for 0 - 65535
// ^((\d{1,2})?([.][\d]{1,2})?){1}[%]{1}$     for checking a value is between 99.99% and 00.00%

// ^\$?\-?([1-9]{1}[0-9]{0,2}(\,\d{3})*(\.\d{0,2})?|[1-9]{1}\d{0,}(\.\d{0,2})?|0(\.\d{0,2})?|(\.\d{1,2}))$|^\-?\$?([1-9]{1}\d{0,2}(\,\d{3})*(\.\d{0,2})?|[1-9]{1}\d{0,}(\.\d{0,2})?|0(\.\d{0,2})?|(\.\d{1,2}))$|^\(\$?([1-9]{1}\d{0,2}(\,\d{3})*(\.\d{0,2})?|[1-9]{1}\d{0,}(\.\d{0,2})?|0(\.\d{0,2})?|(\.\d{1,2}))\)$
    // Matching: currency format that allows optional $, optional &quot;-&quot;(MinusSignNegative) OR &quot;()&quot; (ParenNegative) but not both, optional cents, and optional commas separating thousands. Minus sign can be before or after $, but parens must be outside the $. UPDATED: now fails to match a &quot;$&quot; without any further numbers
    

// Introductional commands:
// (*LF)            Newline is a linefeed character. Corresponding linebreaks can be matched with \n.


//http://zez.org/article/articleview/65/
//http://www.regexlib.com/CheatSheet.aspx

