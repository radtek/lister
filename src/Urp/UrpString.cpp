#include "UrpString.h"

//==============================================================================================
// Crazy idea: Function name maps to the parameters, especially their order.
//==============================================================================================
/*static*/ String UrpString::ReplaceInWhatWith(const String in, const String what, const String with, int *replacementCount /*= NULL*/) {
	if (replacementCount) *replacementCount = 0;
	if (what.IsEmpty()) return in;
	if (what == with) return in;
	bool blockinfiniterecursion = false;
	
	// If an expansion with, prevent running more than once
	
	if (with.Find(what) != -1) {
		blockinfiniterecursion = true;
	}
	
    String out = in;
    int found_pos;
    int what_len = what.GetCount();
    
    // Repeat until no occurrences of "what" at all, recursively checking (i.e., for cases of double space to single space)
    
    while(out.Find(what) != -1) {
        int start_pos = 0;
        String startFrom = out;
        out.Clear();
        
        // Replace all occurrences from left to right, which may create new occurrences next pass
        
        while(((found_pos = startFrom.Find(what, start_pos)) != -1)) {
            if (replacementCount) *replacementCount++;
			// Keep portion between strings found
			out.Cat(startFrom.Mid(start_pos, found_pos - start_pos));
			out.Cat(with); // add the replacement string
			start_pos = found_pos + what_len;
		}

        if(start_pos < startFrom.GetCount())
        	out.Cat(startFrom.Mid(start_pos));
        
        if (blockinfiniterecursion) break;
	}

    return (out);
}

//==============================================================================================
/*static*/int UrpString::CountOccurInOf(const String &in, const char of/*=','*/) {
	const char *s = in; // operator char*()                { return Begin(); }
	int cnt = 0;
	while (*s++) cnt+= (*s == of); // Taking advantage of side affect :)
	return cnt;
}

//==============================================================================================
/*static*/ String UrpString::Apostrophize(String in) {
	return "'" + in + "'";
}

//==============================================================================================
// Trim x number of characters off the end, (IN PLACE)
//==============================================================================================
/*static*/ void UrpString::TrimOff(String &in, int cnt) {
	int l = in.GetLength();
	in.Trim(l - cnt);
}

//==============================================================================================
/*static*/ bool UrpString::IsIntegerString(String testStr) {
	String localizedStr = TrimBoth(testStr);
	const char *s = localizedStr;
	const char *first_s = s;
	bool isInteger = true;
	int charCount = 0;
	
	while (*s) {
		charCount++;
		int c = *s;
		
		if (charCount == 1) {
			if (!In(c, '-', '+', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9')) {
				isInteger = false;
				break;
			}
		} else {
			if (!In(c, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9')) {
				isInteger = false;
				break;
			}
		}
		
		s++;
	}
	
	return isInteger;
}

//==============================================================================================
/*static*/ bool UrpString::IsDecimalString(String testStr) {
	String localizedStr = TrimBoth(testStr);
	const char *s = localizedStr;
	const char *first_s = s;
	bool isInteger = true;
	bool couldBeDecimal = true;
	int charCount = 0;
	int decimalCount = 0;
	
	while (*s) {
		charCount++;
		int c = *s;
		
		if (c == '.')
			decimalCount++;
		
		if (charCount == 1) {
			if (!In(c, '-', '+', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.')) {
				isInteger = false;
				break;
			}
		} else {
			if (!In(c, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.')) {
				isInteger = false;
				break;
			}
		}
		
		s++;
	}
	
	if (!isInteger || decimalCount > 1) couldBeDecimal = false;
	return couldBeDecimal;
}

//==============================================================================================
// Difference of opinion between me and U++ designers: If you assign a string to an int, it
// should try and convert it, and not error out inexplicably.  So, I didn't change Value,
// I just added my own function.  I decided I should always pass both types.  Should be a
// template with specialization.
//==============================================================================================
/*static*/ bool UrpString::FromTo(const Value &in, int &out) {
	if (IsString(in)) {
		return FromTo(in.ToString(), out);
	} else 
	if (IsNumber(in)) {
		// The Value code works, unless its a string!!  Then it tries to convert it as a
		// double. Doh!
		out = in;
		return true;
	} else {
		throw Exc(CAT << "Unrecognized Value type: " << in.GetType() 
		<< " in StringHandler::FromTo where in of Value type, out of int type");
	}
}

//==============================================================================================
// Support char *in since string constants won't convert to "&" String type.
//==============================================================================================
/*static*/ bool UrpString::FromTo(const char *in, int &out) {
	out = atoi(in);
	return true;
}

//==============================================================================================
// Use "&" to avoid a string copy op.
//==============================================================================================
/*static*/ bool UrpString::FromTo(const String &in, int &out) {
	out = atoi(in.ToString());
	return true;
}

//==============================================================================================
// This allows function calls to be completely oblivious to data types
//==============================================================================================
/*static*/ bool UrpString::FromTo(const int in, int &out) {
	out = in;
	return true;
}

//==============================================================================================
// This should work, right?
//==============================================================================================
/*static*/ bool UrpString::FromTo(const byte in, int &out) {
	out = in;
	return true;
}

//==============================================================================================
// Ok, most cases this works without an annoying warning!  int is the default for flags
// that store 1 and 0, for christ's sake!
//==============================================================================================
/*static*/ bool UrpString::FromTo(const unsigned int in, int &out) {
	if (in < INT_MAX / 2 && in > INT_MIN / 2) {
		out = in;
	} else {
		throw Exc(CAT << "Input value into FromTo is greater than out can take. " << in);
	}
	return true;
}

//==============================================================================================
/*static*/ String UrpString::GetWord(String spaceDelimStrings, int wordNo) {
	Vector <String> splitString = Split(spaceDelimStrings, ' ');
		return splitString[wordNo];
}

//==============================================================================================
/*static*/ String UrpString::ToString(int64 i) {
	String s;
	s = Format("%ld", i);
	return s;
}

//==============================================================================================
/*static*/ String UrpString::ToString(int i) {
	String s;
	s = Format("%d", i);
	return s;
}

//==============================================================================================
/*static*/ String UrpString::ToString(byte i) {
	String s;
	s = Format("%d", (int)i);
	return s;
}

//==============================================================================================
String Trim(const String& str) {
	return TrimBoth(str); // in String.h
}

//==============================================================================================
// Fun function to strip things like wrapping apostrophes, quotes, spaces, brackets, tabs, etc.
//==============================================================================================
String StripWrapper(const String& str, const String& wrapper, const String &wrapperRight) {
	String lwrapperRight = wrapperRight;
	if (lwrapperRight.IsEmpty()) { 
		if      (wrapper.IsEqual("[")) lwrapperRight = "]";
		else if (wrapper.IsEqual("{")) lwrapperRight = "}";
		else if (wrapper.IsEqual("(")) lwrapperRight = ")";
		else if (wrapper.IsEqual("`")) lwrapperRight = "'";
		else if (wrapper.IsEqual("/*")) lwrapperRight = "*/";
		else lwrapperRight = wrapper;
	}
	
	if (str.StartsWith(wrapper) && str.EndsWith(lwrapperRight)) {
		return str.Mid(wrapper.GetLength(), str.GetLength() - (lwrapperRight.GetLength() + wrapper.GetLength()));
	}
	return str;
}

//==============================================================================================
// Taken from SQL functions that are useful in code.
//==============================================================================================
Value IfNull(Value in, Value defval) {
	if (in.IsNull()) {
		return defval;
	}
	return in;
}

//==============================================================================================
bool IsNull(int in) {
	return (in == INT_NULL);
}

//==============================================================================================
int IfNull(int in, int defval) {
	if (in == INT_NULL) {
		return defval;
	}
	return in;
}

//==============================================================================================
int AsInt(Value in, Value defval) {
	Value lval = IfNull(in, defval);
	return (int)lval;
}


//==============================================================================================
int ToInt(String in, String defval) {
	return ToInt(in, atoi(defval));
}

//==============================================================================================
int ToInt(String in, int defval) {
	if (in.IsEmpty()) {
		return defval;
	}
	
	return atoi(in);
}

//==============================================================================================
// I know U++ has a function, but ToString works better for me.
//==============================================================================================
String ToString(int in) {
	return Format("%d", in);
}

//==============================================================================================
// Very useful function that takes the U++ Value class and expands it with a way to convert
// natural output from Value Getters to SQL String compliant parameters.
// To be used when build in SQL scripts with a Format command.
// 99% of time for internal control tables, so PGSQL is the expectation for most code.
//==============================================================================================
String ToSQL(const Value &in, int dialect /*=PGSQL*/, bool nestedconstant /*=false*/) {
	if (in.IsNull()) return "NULL";
	
	switch (in.GetType()) {
		//--------------------------------------------------------------------------------------
		case STRING_V: 
			if (in.ToString().IsEmpty()) return "NULL";
			if (nestedconstant)
				return CAT << "E\"" << UrpString::ReplaceInWhatWith(UrpString::ReplaceInWhatWith(in.ToString(), "\\", "\\\\'"), "'", "\\'") << "\"";
			else
				// E escapes slashes and such
				return CAT << "E'" << UrpString::ReplaceInWhatWith(UrpString::ReplaceInWhatWith(in.ToString(), "\\", "\\\\'"), "'", "\\'") << "'";

		//--------------------------------------------------------------------------------------
		case INT_V:
			if (in == INT_NULL) return "NULL";
			return in.ToString();
			
		//--------------------------------------------------------------------------------------
		case BOOL_V:
			if (dialect == PGSQL)
				if (nestedconstant) // We are building a value array insert, so values are quotes instead of aposts
					return (in? "\"1\"" : "\"0\""); // Postrgres flips when you stuff numerics in, unless I suppose I make a cast converter.  Might be best.
				else
					return (in? "'1'" : "'0'"); // Postrgres flips when you stuff numerics in, unless I suppose I make a cast converter.  Might be best.
			else 
				return (in? "1" : "0");
			
		//--------------------------------------------------------------------------------------
		case INT64_V:
			if (in == INT64_NULL) return "NULL";
			return in.ToString();
		
		//--------------------------------------------------------------------------------------
		case DATE_V: {
			Date d = in;	
			if (d.year == 1900 && d.month == 1 && d.day == 1) return NULL; 
			if (!d.year && !d.month && !d.day) return NULL;
			// Assume US.
			if (dialect == PGSQL)
				return Format("'%04d-%02d-%02d'", d.year, d.month, d.day);
			else if (dialect == ORACLE)
				return Format("'%02d-%s-%d'", d.day, Upper(MonName(d.month-1)), d.year);
			else if (dialect == MSSQL)
				return Format("'%02d-%02d-%04d'", d.month, d.day, d.year);
			else 
				return Format("'%02d-%02d-%04d'", d.month, d.day, d.year);
		}
		
		//--------------------------------------------------------------------------------------
		case TIME_V: {
			Time t = in;	
			return Format("'%02d:%02d:%02d'", t.hour, t.minute, t.second);
		}

		//--------------------------------------------------------------------------------------
		case DOUBLE_V:
			if (in == DOUBLE_NULL) return "NULL";
			return in.ToString();
			
		// May not work for multidimensional 
		
		//--------------------------------------------------------------------------------------
		case VALUEARRAY_V: {
			ValueArray va = in;
			String vastr;
			if (!nestedconstant) vastr << "'";
			vastr << "{";
			for (int i = 0; i < va.GetCount(); i++) {
				if (i) vastr << ",";
				vastr << ToSQL(va.Get(i), dialect, true);
			}
			vastr << "}";
			if (!nestedconstant) vastr << "'";
			return Value(vastr);
		}

		//--------------------------------------------------------------------------------------
		case WSTRING_V:
		{
			// For PGSQL, U&"\0441\043B\043E\043D" = Russian word "slon" (elephant) in Cyrillic letters
			WString ws = in;
			String s = "U&\"";
			for (int i = 0; i < ws.GetLength(); i++) {
				wchar wc = ws[i];
				String sh = HexString(ToString((int)wc));
				s << "\\" << sh;
			}
			s << "\"";
			return s;
		}
		
		
		//case TIMESTAMP_V??
		//case VALUEMAP_V:
		// INTERVAL_V??
		// MILLISECONDS??
		// Time zone, epoch, microseconds, doy, dow, millenium, century , weekofyear, weekofmonth
		default:
			throw Exc(Format("Unrecognized value type %lu", (int)in.GetType())); // unsigned long
	}
}

//==============================================================================================
// Trim off a specific string at the beginning of a string.  A convenience function only.
// So RestOf("CatHouse", "Cat") returns "House", so you don't have to deal with lengths.
//==============================================================================================
String RestOf(const String &startwith, const String &skipthis) {
	if (startwith.StartsWith(skipthis)) {
		return startwith.Mid(skipthis.GetLength());
	} else {
		return startwith;
	}
}

//==============================================================================================
// Tired of the "To" prefix :)  U++ function is ToUpper.
//==============================================================================================
String Upper(const String& s) {
	return(ToUpper(s));
}
