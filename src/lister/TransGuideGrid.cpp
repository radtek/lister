#include "TransGuideGrid.h"


//==============================================================================================	
TransGuideGrid::TransGuideGrid(GridType gridType) {
	
}

//==============================================================================================	
void TransGuideGrid::Build(Connection *pconnection) {
	//select count (distinct colno) from transguidemap where transguideid = 5 and looped is true;
	//select colno, max(tagname) as tagname from transguidemap where transguideid = 5  and looped is true group by colno order by colno;
	//    for i = 1 to nofcols
	//       AddColumn(tagName, 
	
}

//==============================================================================================	
void TransGuideGrid::Load() {
	//select * from transguidemap where transguideid = 5 and looped is true order by loopno, colno
	// while not eof
	//   fetch 
}


