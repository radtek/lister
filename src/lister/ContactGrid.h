#ifndef _lister_lister_ContactGrid_h_
#define _lister_lister_ContactGrid_h_

#include "shared.h"
class Connection;

class ContactGrid : public UrpSqlGrid {
public:
	EditString contactName, emailAddress, homePhoneNo, workPhoneNo, cellPhoneNo, personalEmailAddress, note;
	Connection *connection; // Our metadata about the query we will be running
	
	ContactGrid() : UrpSqlGrid() {
		// Build columns
		SetTable(CONTACTS);
		AddKey(CONTACTID);
		AddColumn(CONTACTNAME, "contact", 100).Edit(contactName);
		AddColumn(EMAILADDRESS, "email", 100).Edit(emailAddress);
		AddColumn(HOMEPHONENO, "home", 100).Edit(homePhoneNo);
		AddColumn(WORKPHONENO, "work", 100).Edit(workPhoneNo);
		AddColumn(CELLPHONENO, "cell", 100).Edit(cellPhoneNo);
		AddColumn(NONWORKEMAILADDRESS, "personal email", 100).Edit(personalEmailAddress);
		AddColumn(NOTE, "note", 100).Edit(note);
	}
	
	void Load(Connection *pconnection) {
		connection = pconnection;
		SetSession(*(connection->session));
		SetSortColumn(CONTACTNAME);
		Query();
	}
};

#endif
