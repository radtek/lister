#ifndef _lister_lister_ContactGrid_h_
#define _lister_lister_ContactGrid_h_

#include "shared.h"
class Connection;

class ContactGrid : public UrpSqlGrid {
public:
	EditString contactName, emailAddress, phoneNo, phone2No, phone3No, personalEmailAddress;
	Connection *connection; // Our metadata about the query we will be running
	
	ContactGrid() : UrpSqlGrid() {
		// Build columns
		SetTable(CONTACTS);
		AddKey(CONTACTID);
		AddColumn(CONTACTNAME, "contact", 100).Edit(contactName);
		AddColumn(EMAILADDRESS, "email", 100).Edit(emailAddress);
		AddColumn(PHONENO, "phone #", 100).Edit(phoneNo);
		AddColumn(PHONE2NO, "phone #", 100).Edit(phoneNo);
		AddColumn(PHONE3NO, "phone #", 100).Edit(phoneNo);
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
