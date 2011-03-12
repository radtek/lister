#ifndef _lister_lister_ContactGrid_h_
#define _lister_lister_ContactGrid_h_

#include "shared.h"
#include "shared_db.h"

class Connection;

//==============================================================================================
class ContactGrid : public UrpSqlGrid {
public:
	EditString  contactName
			,	emailAddress
			,	homePhoneNo
			,	workPhoneNo
			,	cellPhoneNo
			,	personalEmailAddress
			,	note
	;
	
	Connection *connection; // Our metadata about the query we will be running
	
	            ContactGrid();
	void        Load(Connection *pconnection);
	static void LoadContact(Connection *connection, DropGrid &dropGrid);
	static void BuildContactList(DropGrid &contactList);
};

#endif
