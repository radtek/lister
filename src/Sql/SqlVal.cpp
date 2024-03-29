#include "Sql.h"

NAMESPACE_UPP

SqlCol SqlCol::As(const char *as) const
{
	return name + SqlCase(MSSQL | PGSQL, " as ")(" ") + as;
}

SqlCol SqlId::Of(const char *of) const
{
	return of + ('.' + ToString());
}

SqlCol SqlId::Of(SqlId id) const
{
	return id.IsNull() ? ToString() : id.ToString() + '.' + ToString();
}

SqlId SqlId::operator[](const SqlId& id) const
{
	return id.IsNull() ? ToString() : ToString() + '.' + id.ToString();
}

SqlId SqlId::As(const char *as) const
{
	return id.IsNull() ? ToString() : ToString() + SqlCase(MSSQL | PGSQL, " as ")(" ") + as;
}

SqlId SqlId::operator [] (int i) const
{
	return SqlId(ToString() + FormatInt(i));
}

SqlId SqlId::operator&(const SqlId& s) const
{
	return SqlId(ToString() + "$" + s.ToString());
}

String SqlS::operator()() const
{
	return '(' + text + ')';
}

String SqlS::operator()(int at) const
{
	return at > priority ? operator()() : text;
}

String SqlS::operator()(int at, byte cond) const
{
	if(at <= priority)
		return text;
	StringBuffer out;
	out << SqlCase(cond, "(")() << text << SqlCase(cond, ")")();
	return out;
}

SqlS::SqlS(const SqlS& a, const char *o, const SqlS& b, int pr, int prb) {
	text = a(pr) + o + b(prb);
	priority = pr;
}

SqlS::SqlS(const SqlS& a, const char *o, const SqlS& b, int pr) {
	text = a(pr) + o + b(pr);
	priority = pr;
}

SqlVal SqlVal::As(const char *as) const {
	SqlVal v;
	v.SetHigh(text + ~SqlCase(MSSQL | PGSQL, " as ")(" ") + as);
	return v;
}

SqlVal SqlVal::As(const SqlId& id) const {
	return As(~~id);
}

SqlVal::SqlVal(const String& x) {
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(const char *s) {
	if(s && *s)
		SetHigh(SqlFormat(s));
	else
		SetNull();
}

SqlVal::SqlVal(int x) {
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(int64 x) {
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(double x) {
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(Date x) {
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(Time x) {
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(const Value& x) {
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(const Nuller&) {
	SetNull();
}

SqlVal::SqlVal(SqlId id) {
	SetHigh(id.ToString());
}

SqlVal::SqlVal(const SqlId& (*id)())
{
	SetHigh((*id)().ToString());
}

SqlVal::SqlVal(SqlCol id) {
	SetHigh(id.ToString());
}
/*
SqlVal::SqlVal(const SqlSelect& x) {
	SetHigh('(' + ((SqlStatement) x).GetText() + ')');
}
*/

// Jeff restored this function and now Get(row, ID) as SqlBool so that this is called
// from SqlUpdate ect. constructors.  The reason is that PGSQL 9.0 requires boolean to be sent
// wrapped as string, either '1''0' or 'T''F' or 'Y''N'.  The bit type in MSSQL would tolerate 
// 1 and 0 as inputs, but not PGSQL.
SqlVal::SqlVal(const SqlBool& x) {
	SetHigh(SqlCase(PGSQL, "'")() + (x.AsBool() ? "1" : "0") + SqlCase(PGSQL, "'")());
}

SqlVal::SqlVal(const Case& x) {
	SetHigh(~x);
}

SqlVal operator-(const SqlVal& a) {
	return SqlVal('-' + a(SqlS::UNARY), SqlS::UNARY);
}

SqlVal operator+(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a, " + ", b, SqlS::ADD);
}

SqlVal operator-(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a," - ", b, SqlS::ADD, SqlS::ADD + 1);
}

SqlVal operator*(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a, " * ", b, SqlS::MUL);
}

SqlVal operator/(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a, " / ", b, SqlS::MUL, SqlS::MUL + 1);
}

SqlVal operator%(const SqlVal& a, const SqlVal& b) {
	return SqlFunc("mod", a, b);
}

SqlVal operator|(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a, SqlCase(ORACLE, " || ")(PGSQL, " || ")(" + "), b, SqlS::MUL);
}

SqlVal& operator+=(SqlVal& a, const SqlVal& b)     { return a = a + b; }
SqlVal& operator-=(SqlVal& a, const SqlVal& b)     { return a = a - b; }
SqlVal& operator*=(SqlVal& a, const SqlVal& b)     { return a = a * b; }
SqlVal& operator/=(SqlVal& a, const SqlVal& b)     { return a = a / b; }
SqlVal& operator%=(SqlVal& a, const SqlVal& b)     { return a = a % b; }
SqlVal& operator|=(SqlVal& a, const SqlVal& b)     { return a = a | b; }

SqlVal SqlFunc(const char *name, const SqlVal& a) {
	return SqlVal(name + a(), SqlS::FN);
}

SqlVal SqlFunc(const char *n, const SqlVal& a, const SqlVal& b) {
	return SqlVal(String(n) + '(' + ~a + ", " + ~b + ')', SqlS::FN);
}

SqlVal SqlFunc(const char *n, const SqlVal& a, const SqlVal& b, const SqlVal& c) {
	return SqlVal(String(n) + '(' + ~a + ", " + ~b + ", " + ~c + ')', SqlS::FN);
}

SqlVal SqlFunc(const char *n, const SqlVal& a, const SqlVal& b, const SqlVal& c, const SqlVal& d) {
	return SqlVal(String(n) + '(' + ~a + ", " + ~b + ", " + ~c + ", " + ~d + ')', SqlS::FN);
}

SqlVal SqlFunc(const char *name, const SqlSet& set) {
	return SqlVal(name + set(), SqlS::FN);
}

SqlVal Decode(const SqlVal& exp, const SqlSet& variants) {
	ASSERT(!variants.IsEmpty());
	return SqlVal("decode("  + ~exp  + ", " + ~variants + ')', SqlS::FN);
}

SqlVal Distinct(const SqlVal& exp) {
	return SqlVal("distinct " + exp(SqlS::ADD), SqlS::UNARY);
}

SqlSet Distinct(const SqlSet& columns) {
	return SqlSet("distinct " + ~columns, SqlSet::SET);
}

SqlVal All(const SqlVal& exp) {
	return SqlVal("all " + exp(SqlS::ADD), SqlS::UNARY);
}

SqlSet All(const SqlSet& columns) {
	return SqlSet("all " + ~columns, SqlSet::SET);
}

SqlVal Count(const SqlVal& exp)
{
	return SqlFunc("count", exp);
}

SqlVal Count(const SqlSet& exp)
{
	return SqlFunc("count", exp);
}

SqlVal SqlAll()
{
	return SqlCol("*");
}

SqlVal SqlCountRows()
{
	return Count(SqlAll());
}

SqlVal Descending(const SqlVal& exp) {
	return SqlVal(exp(SqlS::ADD) + " desc", SqlS::UNARY);
}

SqlVal SqlMax(const SqlVal& exp) {
	return SqlFunc("max", exp);
}

SqlVal SqlMin(const SqlVal& exp) {
	return SqlFunc("min", exp);
}

SqlVal SqlSum(const SqlVal& exp) {
	return SqlFunc("sum", exp);
}

SqlVal Avg(const SqlVal& a) {
	return SqlFunc("avg", a);
}

SqlVal Stddev(const SqlVal& a) {
	return SqlFunc("stddev", a);
}

SqlVal Variance(const SqlVal& a) {
	return SqlFunc("variance", a);
}

SqlVal Greatest(const SqlVal& a, const SqlVal& b) {
	return SqlFunc(SqlCase
						(SQLITE3, "max")
						("greatest"),
					a, b);
}

SqlVal Least(const SqlVal& a, const SqlVal& b) {
	return SqlFunc(SqlCase
						(SQLITE3, "min")
						("least"),
					a, b);
}

SqlVal ConvertCharset(const SqlVal& exp, const SqlVal& charset) { //TODO Dialect!
	if(exp.IsEmpty()) return exp;
	return exp.IsEmpty() ? exp : SqlFunc("convert", exp, charset);
}

SqlVal ConvertAscii(const SqlVal& exp) { //TODO Dialect!
	return ConvertCharset(exp, "US7ASCII");
}

SqlVal Upper(const SqlVal& exp) {
	return exp.IsEmpty() ? exp : SqlFunc("upper", exp);
}

SqlVal Lower(const SqlVal& exp) {
	return exp.IsEmpty() ? exp : SqlFunc("lower", exp);
}

SqlVal Length(const SqlVal& exp) {
	return exp.IsEmpty() ? exp : SqlFunc("length", exp);
}

SqlVal UpperAscii(const SqlVal& exp) {
	return exp.IsEmpty() ? exp : Upper(ConvertAscii(exp));
}

SqlVal Substr(const SqlVal& a, const SqlVal& b) {
	return SqlFunc("SUBSTR", a, b);
}

SqlVal Substr(const SqlVal& a, const SqlVal& b, const SqlVal& c)
{
	return SqlFunc("SUBSTR", a, b, c);
}

SqlVal Instr(const SqlVal& a, const SqlVal& b) {
	return SqlFunc("INSTR", a, b);
}

SqlVal Wild(const char* s) {
	String result;
	for(char c; (c = *s++) != 0;)
		if(c == '*')
			result << '%';
		else if(c == '?')
			result << '_';
		else if(c == '.' && *s == 0)
			return result;
		else
			result << c;
	result << '%';
	return result;
}

SqlVal SqlDate(const SqlVal& year, const SqlVal& month, const SqlVal& day) {//TODO Dialect!
	return SqlFunc("to_date", year|"."|month|"."|day, "SYYYY.MM.DD");
}

SqlVal AddMonths(const SqlVal& date, const SqlVal& months) {//TODO Dialect!
	return SqlFunc("add_months", date, months);
}

SqlVal LastDay(const SqlVal& date) {//TODO Dialect!
	return SqlFunc("last_day", date);
}

SqlVal MonthsBetween(const SqlVal& date1, const SqlVal& date2) {//TODO Dialect!
	return SqlFunc("months_between", date1, date2);
}

SqlVal NextDay(const SqlVal& date) {//TODO Dialect!
	return SqlFunc("next_day", date);
}

SqlVal Cast(const char* type, const SqlId& a) {
	return SqlFunc(type, a);
}

SqlVal SqlNvl(const SqlVal& a, const SqlVal& b) {
	return SqlFunc(SqlCase
						(PGSQL, "coalesce")
						(MY_SQL|SQLITE3, "ifnull")
						(MSSQL, "isnull")
						("nvl"),
					a, b);
}

SqlVal SqlNvl(const SqlVal& a) {
	return Nvl(a, SqlVal(0));
}

SqlVal Prior(SqlId a) {
	return SqlVal("prior " + ~a, SqlS::UNARY);
}

SqlVal NextVal(SqlId a) {
	return SqlVal(SqlCase
	                 (PGSQL, "nextval('" + ~a + "')")
	                 (~a + ".NEXTVAL")
	              , SqlS::HIGH);
}

SqlVal CurrVal(SqlId a) {
	return SqlVal(SqlCase
				    (PGSQL, "currval('" + ~a + "')")
				    (~a + ".CURRVAL")
				  , SqlS::HIGH);
}

SqlVal SqlRowNum()
{
	return SqlCol("ROWNUM");
}

SqlVal SqlArg() {
	return SqlCol("?");
}

SqlVal OuterJoin(SqlCol col)
{
	return SqlCol(~col + "(+)");
}

SqlVal SqlBinary(const char *s, int l)
{
	SqlVal x;
	x.SetHigh(SqlFormatBinary(s, l));
	return x;
}

SqlVal SqlBinary(const String& data)
{
	return SqlBinary(~data, data.GetCount());
}

END_UPP_NAMESPACE
