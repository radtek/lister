#include "Sql.h"

NAMESPACE_UPP

void td_scalar(SqlSet& set, const String& prefix, const char *x) {
	set.Cat(SqlCol(prefix + x));
}

void td_array(SqlSet& set, const String& prefix, const char *x, int cnt) {
	String name = prefix + x;
	for(int i = 0; i < cnt; i++)
		set.Cat(SqlCol(name + Format("%d", i)));
}

void td_var(SqlSet& set, const String& prefix, const char *x, SqlSet (*f)(const String&)) {
	set.Cat((*f)(prefix + x + '$'));
}

void td_shrink(String *array, int cnt) {
	String *lim = array + cnt;
	while(array < lim) {
		array->Shrink();
		array++;
	}
}

struct NfEqual : FieldOperator {
	ValueArray va;
	
	virtual void Field(const char *name, Ref f) {
		va.Add(f);
	}
};

bool EqualFields(Fields a, Fields b)
{
	NfEqual fa, fb;
	a(fa);
	b(fb);
	return fa.table == fb.table && fa.va == fb.va;
}

END_UPP_NAMESPACE
