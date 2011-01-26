#include "ContactGrid.h"
#include "Connection.h"

//==============================================================================================
ContactGrid::ContactGrid() : UrpSqlGrid() {
	// Build columns
	SetTable(CONTACTS);
	AddKey(CONTACTID);
	AddColumn(CONTACTNAME, "contact", 100).Edit(contactName);
	AddColumn(EMAILADDRESS, "email", 100).Edit(emailAddress);
	AddColumn(HOMEPHONENO, "home", 100).Edit(homePhoneNo);
	AddColumn(WORKPHONENO, "work", 100).Edit(workPhoneNo);
	AddColumn(CELLPHONENO, "cell", 100).Edit(cellPhoneNo);
	AddColumn(NONWORKEMAILADDRESS, "personal email", 100).Edit(personalEmailAddress);
	AddColumn(NOTE, "note", 200).Edit(note);
}

//==============================================================================================
void ContactGrid::Load(Connection *pconnection) {
	connection = pconnection;
	SetSession(*(connection->session));
	SetSortColumn(CONTACTNAME);
	Query();
}

//==============================================================================================
/*static*/ void ContactGrid::LoadContact(Connection *connection, DropGrid &dropGrid) {
	// Load contacts once, maybe F9 can reload?  Should build into UrpDropGrid
	if (!connection->SendQueryDataScript("select contactid, contactname, emailaddress, workphoneno from contacts order by contactname")) {
		return;
	}
	
	dropGrid.Clear();
	while(connection->Fetch()) {
		dropGrid.Add(connection->Get(0), connection->Get(1));
	}

	dropGrid.SetKeyColumn(0);
	dropGrid.Add(Null, Null); // Make sure we support a null value or we won't be able to edit other attributes without assigning a connection, which isn't what we want.
}

//==============================================================================================
/*static*/ void ContactGrid::BuildContactList(DropGrid &contactList) {
	contactList.Width(200);
	contactList.SetDropLines(30);
}
