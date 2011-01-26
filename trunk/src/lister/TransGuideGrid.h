#ifndef _lister_lister_TransGuideGrid_h_
#define _lister_lister_TransGuideGrid_h_

#include "shared.h"
#include <lister/Urp/Urp.h>
class Connection;

class TransGuideGrid : public UrpGrid {
public:
	enum GridType { LOOPED, SHARED };
	
	                TransGuideGrid(GridType gridType);
	void            Build(Connection *pconnection);
	void            Load();
	
};

#endif
