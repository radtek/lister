#include "UrpString.h"

//==========================================================================================	
// Crazy idea: Function name maps to the parameters, especially their order.
/*static*/ String UrpString::ReplaceInWhatWith(const String in, const String what, const String with) {
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

//==========================================================================================	
/*static*/ String UrpString::Apostrophize(String in) {
	return "'" + in + "'";
}

//==========================================================================================	
// Trim x number of characters off the end, (IN PLACE)
/*static*/ void UrpString::TrimOff(String &in, int cnt) {
	int l = in.GetLength();
	in.Trim(l - cnt);
}

//==========================================================================================	
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

//==========================================================================================	
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

//==========================================================================================	
// Difference of opinion between me and U++ designers: If you assign a string to an int, it
// should try and convert it, and not error out inexplicably.  So, I didn't change Value,
// I just added my own function.  I decided I should always pass both types.  Should be a
// template with specialization.
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
		throw Exc(CAT << "Unrecognized Value type: " << in.GetType() << " in StringHandler::FromTo where in of Value type, out of int type");
	}
}

//==========================================================================================	
// Support char *in since string constants won't convert to "&" String type.
/*static*/ bool UrpString::FromTo(const char *in, int &out) {
	out = atoi(in);
	return true;
}

//==========================================================================================	
// Use "&" to avoid a string copy op.
/*static*/ bool UrpString::FromTo(const String &in, int &out) {
	out = atoi(in.ToString());
	return true;
}

//==========================================================================================	
// This allows function calls to be completely oblivious to data types
/*static*/ bool UrpString::FromTo(const int in, int &out) {
	out = in;
	return true;
}

//==========================================================================================	
// This should work, right?
/*static*/ bool UrpString::FromTo(const byte in, int &out) {
	out = in;
	return true;
}

//==========================================================================================	
// Ok, most cases this works without an annoying warning!  int is the default for flags
// that store 1 and 0, for christ's sake!
/*static*/ bool UrpString::FromTo(const unsigned int in, int &out) {
	if (in < INT_MAX / 2 && in > INT_MIN / 2) {
		out = in;
	} else {
		throw Exc(CAT << "Input value into FromTo is greater than out can take. " << in);
	}
	return true;
}

//==========================================================================================	
/*static*/ String UrpString::GetWord(String spaceDelimStrings, int wordNo) {
	Vector <String> splitString = Split(spaceDelimStrings, ' ');
		return splitString[wordNo];
}

//==========================================================================================	
/*static*/ String UrpString::ToString(int in) {
	return Format("%d", in);
}

//==========================================================================================	
/*static*/ String UrpString::ToString(byte in) {
	return Format("%d", in);
}

//==========================================================================================	
String Trim(const String& str) {
	return TrimBoth(str); // in String.h
}

//==========================================================================================	
// Fun function to strip things like wrapping apostrophes, quotes, spaces, brackets, tabs, etc.
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

//==========================================================================================	
Value IfNull(Value in, Value defval) {
	if (in.IsNull()) {
		return defval;
	}
	return in;
}

//==========================================================================================	
int AsInt(Value in, Value defval) {
	Value lval = IfNull(in, defval);
	return (int)lval;
}