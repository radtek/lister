#ifndef _shared_h_
#define _shared_h_

#include <lister/Urp/Urp.h>

using namespace Upp;

// This must be on this side of using namespace
//typedef PostgreSQLSession ControlSession;

#define UNKNOWN -1

#define KEY_DECLAREDUNKNOWN  -1       // User must physically select this, attesting that we do not know
#define KEY_MULTIMATCH       -2       // There are multiple values matching the input provided in our lookups, so we can't populate
#define KEY_UNRESOLVED       -3       // The lookup string provided was not found in our reference data
#define KEY_UNSET            -4       // Not set on an user entry screen, so we know they specifically did not set a value
#define KEY_ATTRUNAVAIL      -5       // From a feed, the attribute was not presented
#define KEY_SOURCEUNPOP      -6       // From a feed, the attribute was essentially empty, a blank, or a null
#define KEY_SOURCEDEFLT      -7       // From a feed, the default value was provided, like "99999999" for a SSN, empirically determined
#define KEY_MULTIVALUEDA     -8       // There are legitimately multiple values, stored in standard multivalue table type A

#include "TestGrid.h" // Referenced from layout

#define LAYOUTFILE <lister/lister/lister.lay>
#include <CtrlCore/lay.h>

#endif

