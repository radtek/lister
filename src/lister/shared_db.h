/***********************************************************************************************
*  lister - shared_db.h
*  
*  Any system referencing the control database has this in its header or source.
*  sch_source ONLY exists in the main app.cpp file to avoid link errors.
*
*  Author: Jeff Humphreys
*  
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
*
***********************************************************************************************/

#ifndef _lister_lister_shared_db_h_
#define _lister_lister_shared_db_h_

#include <lister/PostgreSQL/PostgreSQL.h>

#define SCHEMADIALECT <lister/PostgreSQL/PostgreSQLSchema.h>
#define MODEL <lister/lister/database.sch>

#include "lister/Sql/sch_header.h"

#endif
