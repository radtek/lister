#ifndef _Urp_UrpString_h_
#define _Urp_UrpString_h_

#include "UrpShared.h"

class UrpString {
public:
	                // Crazy idea: Function name maps to the parameters, especially their order.
	static String   ReplaceInWhatWith(const String in, const String what, const String with);
	static String   Apostrophize(String in);
	static void     TrimOff(String &in, int cnt); // Trim x number of characters off the end
	static bool     IsIntegerString(String testStr);
	static bool     IsDecimalString(String testStr);
	                // Difference of opinion between me and U++ designers: If you assign a string to an int, it
	                // should try and convert it, and not error out inexplicably.  So, I didn't change Value,
	                // I just added my own function.  I decided I should always pass both types.  Should be a
	                // template with specialization.
	static bool     FromTo(const Value &in, int &out);
	                // Support char *in since string constants won't convert to "&" String type.
	static bool     FromTo(const char *in, int &out);
	                // Use "&" to avoid a string copy op.
	static bool     FromTo(const String &in, int &out);
	                // This allows function calls to be completely oblivious to data types
	static bool     FromTo(const int in, int &out);
	                // This should work, right?
	static bool     FromTo(const byte in, int &out);
	                // Ok, most cases this works without an annoying warning!  int is the default for flags
	                // that store 1 and 0, for christ's sake!
	static bool     FromTo(const unsigned int in, int &out);
	static String   GetWord(String spaceDelimStrings, int wordNo);
	static String   ToString(int in);
	static String   ToString(byte in);
};

                    // Tired of remembering "TrimBoth"
String              Trim(const String& str);
String              StripWrapper(const String& str, const String& wrapper, const String &wrapperRight = Null);
Value               IfNull(Value in, Value defval);
                    // Xmlize returns INT_MIN when a value is not found in the xml.
                    // Other functions in U++ use INT_MIN as a Null representor
int                 IfNull(int in, int defval);
int                 AsInt(Value in, Value defval);

#endif
