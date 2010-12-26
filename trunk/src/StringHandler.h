#ifndef _StringHandler_h_
#define _StringHandler_h_

//==========================================================================================	
String ReplaceStr(const String& s1, const String& find, const String replace) {
	if (find == replace) return s1;
	bool blockinfiniterecursion = false;
	
	// If an expansion replace, prevent running more than once
	
	if (replace.Find(find) != -1) {
		blockinfiniterecursion = true;
	}
	
    String string_result = s1;
    int found_pos;
    int find_len = find.GetCount();
    
    // Repeat until no occurrences at all, recursively checking (i.e., for cases of double space to single space)
    
    while(string_result.Find(find) != -1) {
        int start_pos = 0;
        String startFrom = string_result;
        string_result.Clear();
        
        // Replace all occurrences from left to right, which may create new occurrences next pass
        
        while(((found_pos = startFrom.Find(find, start_pos)) != -1)) {
				// Keep portion between strings found
				string_result.Cat(startFrom.Mid(start_pos, found_pos - start_pos));
				string_result.Cat(replace); // add the replacement string
				start_pos = found_pos + find_len;
		}

        if(start_pos < startFrom.GetCount())
        	string_result.Cat(startFrom.Mid(start_pos));
        
        if (blockinfiniterecursion) break;
	}

    return (string_result);
}

#endif
