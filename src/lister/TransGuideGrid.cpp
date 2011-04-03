/***********************************************************************************************
*  lister - TransGuideGrid.cpp
*  
*  I can't remember what this was for.  For macro translation?  We have taskdriver and other
*  objects, so we may drop.
*  Author: Jeff Humphreys
*  
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
*
***********************************************************************************************/

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


