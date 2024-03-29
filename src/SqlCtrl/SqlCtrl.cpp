#include "SqlCtrl.h"

NAMESPACE_UPP

void SqlLoad(MapConvert& cv, Sql& sql) {
	cv.Clear();
	while(sql.Fetch())
		cv.Add(sql[0], sql[1]);
}

void SqlLoad(MapConvert& cv, const SqlSelect& set, SqlSession& ss) {
	Sql sql(ss);
	ss.ClearError();
	sql * set;
	if(ShowError(sql)) return;
	SqlLoad(cv, sql);
}

#ifndef NOAPPSQL
void operator*=(MapConvert& cv, const SqlSelect& set) {
	SqlLoad(cv, set);
}
#endif

void SqlLoad(DropList& dl, Sql& sql) {
	dl.ClearList();
	while(sql.Fetch())
		if(sql.GetColumns() == 1)
			dl.Add(sql[0]);
		else
			dl.Add(sql[0], sql[1]);
}

void SqlLoad(DropList& dl, const SqlSelect& set, SqlSession& ss) {
	Sql sql(ss);
	ss.ClearError();
	sql * set;
	if(ShowError(sql)) return;
	SqlLoad(dl, sql);
}

#ifndef NOAPPSQL
void operator*=(DropList& dl, const SqlSelect& set) {
	SqlLoad(dl, set);
}
#endif

void  SqlOption::SetData(const Value& data) {
	String s = data;
	Set(!(IsNull(s) || s == "0"));
}

Value SqlOption::GetData() const {
	return Get() ? "1" : "0";
}

void SqlCtrls::Add(SqlId id, Ctrl& ctrl) {
	Item& m = item.Add();
	m.id = id;
	m.ctrl = &ctrl;
}

SqlSet SqlCtrls::Set() const {
	SqlSet set;
	for(int i = 0; i < item.GetCount(); i++)
		set.Cat(item[i].id);
	return set;
}

void SqlCtrls::Read(Sql& sql)
{
	for(int i = 0; i < item.GetCount(); i++) {
		Item& m = item[i];
		m.ctrl->SetData(sql[m.id]);
	}
}

bool SqlCtrls::Fetch(Sql& sql) {
	if(!sql.Fetch()) return false;
	Read(sql);
	return true;
}

void SqlCtrls::Insert(SqlInsert& insert) const {
	for(int i = 0; i < item.GetCount(); i++)
		insert(item[i].id, item[i].ctrl->GetData());
}

void SqlCtrls::Update(SqlUpdate& update) const {
	for(int i = 0; i < item.GetCount(); i++)
		update(item[i].id, item[i].ctrl->GetData());
}

void SqlCtrls::UpdateModified(SqlUpdate& update) const {
	for(int i = 0; i < item.GetCount(); i++)
		if(item[i].ctrl->IsModified())
			update(item[i].id, item[i].ctrl->GetData());
}

SqlInsert SqlCtrls::Insert(SqlId table) const {
	SqlInsert insert(table);
	Insert(insert);
	return insert;
}

SqlUpdate SqlCtrls::Update(SqlId table) const {
	SqlUpdate update(table);
	Update(update);
	return update;
}

SqlUpdate SqlCtrls::UpdateModified(SqlId table) const {
	SqlUpdate update(table);
	UpdateModified(update);
	return update;
}

bool SqlCtrls::Accept()
{
	for(int i = 0; i < item.GetCount(); i++)
		if(!item[i].ctrl->Accept()) return false;
	return true;
}

void SqlCtrls::ClearModify() {
	for(int i = 0; i < item.GetCount(); i++)
		item[i].ctrl->ClearModify();
}

bool SqlCtrls::IsModified() {
	for(int i = 0; i < item.GetCount(); i++)
		if(item[i].ctrl->IsModified()) return true;
	return false;
}

void SqlCtrls::Enable(bool b)
{
	for(int i = 0; i < item.GetCount(); i++)
		item[i].ctrl->Enable(b);
}

void SqlCtrls::SetNull()
{
	for(int i = 0; i < item.GetCount(); i++)
		item[i].ctrl->SetData(Null);
}

Callback SqlCtrls::operator<<=(Callback cb)
{
	for(int i = 0; i < item.GetCount(); i++)
		item[i].ctrl->WhenAction = cb;
	return cb;
}

bool SqlCtrls::Load(Sql& sql, SqlId table, SqlBool where)
{
	sql * Select(*this).From(table).Where(where);
	return Fetch(sql);
}

#ifndef NOAPPSQL
bool SqlCtrls::Load(SqlId table, SqlBool set)
{
	return Load(SQL, table, set);
}
#endif

END_UPP_NAMESPACE
