#ifndef _lister_Urp_UrpGridCommon_h_
#define _lister_Urp_UrpGridCommon_h_

class Connection; // Defined in lister

class UrpGridCommon {
public:
	bool                  built;
	bool                  loaded;
	
	// This is kept, however, it may make no sense for screens that are loaded differently,
	// like the grids within the taskdef window.  Therefore the row is not applied and left to
	// the code to apply where appropriate.
	int                   lastKnownRow;
	Connection           *connection;
	
	                      UrpGridCommon() {	
	                      	built = loaded = false;	
	                      	connection = NULL;
	                      }
};

#endif
