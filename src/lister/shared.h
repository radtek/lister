/***********************************************************************************************
*  lister - shared.h
*  
*  Not much can be shared across the project.  The factories are defined here so all the other
*  object files can reference them directly.  There is not mutex set up yet, though.
*  I try to keep this the only place for "using namespace Upp", but didn't succeed.
*  The UNKNOWN constant is heavily used, but tangled with INT_NULL value usage.  I should probably
*  pick one or the other.  The problem is database values in integers with NULL come in as INT_NULL
*  but that means you are dependent on the datatype being a 4-byte signed int.  Any other key value
*  would be a different value for INT_NULL.  8-byte integer would be INT64_MIN=INT_NULL.  Which
*  would blow chunks.  Hmm.  -1 is good, but then queries from database have to convert INT_NULL to UNKNOWN.
*  What to do, what to do...
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

#ifndef _shared_h_
#define _shared_h_

#include <lister/Urp/Urp.h>
#include <lister/Urp/UrpWindowFactory.h>

class ConnectionFactory;

using namespace Upp;

// Physically the factory constructor is in lister.cpp
// This allows any of the classes to create windows dynamically with configured persistent positioning.
extern UrpWindowFactory  *windowFactory;

// Not an Urp class, because lister is primarily a Connection God, so Urp has no real connection tools.
extern ConnectionFactory *connectionFactory;

// This must be on this side of using namespace
//typedef PostgreSQLSession ControlSession;

#define UNKNOWN  -1

#define KEY_DECLAREDUNKNOWN  -1       // User must physically select this, attesting that we do not know
#define KEY_MULTIMATCH       -2       // There are multiple values matching the input provided in our lookups, so we can't populate
#define KEY_UNRESOLVED       -3       // The lookup string provided was not found in our reference data
#define KEY_UNSET            -4       // Not set on an user entry screen, so we know they specifically did not set a value
#define KEY_ATTRUNAVAIL      -5       // From a feed, the attribute was not presented
#define KEY_SOURCEUNPOP      -6       // From a feed, the attribute was essentially empty, a blank, or a null
#define KEY_SOURCEDEFLT      -7       // From a feed, the default value was provided, like "99999999" for a SSN, empirically determined
#define KEY_MULTIVALUEDA     -8       // There are legitimately multiple values, stored in standard multivalue table type A


#endif

