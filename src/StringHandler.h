#ifndef _lister_MyTool002_StringHandler_h_
#define _lister_MyTool002_StringHandler_h_

//==========================================================================================	
String ReplaceStr(const String& s1, const String& find, const String replace) {
        String string_result;
        
        int start_pos = 0;
        int found_pos = 0;
        int find_len = find.GetCount();
        int s1_count = s1.GetCount();
        
        while(((found_pos=s1.Find(find, start_pos))!=-1)){
                string_result.Cat(s1.Mid(start_pos, found_pos - start_pos));
                string_result.Cat(replace);
                start_pos = found_pos + find_len;
        };
                
        if(start_pos<s1.GetCount())
                string_result.Cat(s1.Mid(start_pos));
                
        return (string_result);
}

#endif
