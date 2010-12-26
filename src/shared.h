#ifndef _shared_h_
#define _shared_h_

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
#include <DropGrid/DropGrid.h>

#include <PostgreSQL/PostgreSQL.h>

using namespace Upp;

// This must be on this side of using namespace
typedef PostgreSQLSession ControlSession;

#define CAT String().Cat()
#define UNKNOWN -1

#define LAYOUTFILE <lister/lister.lay>
#include <CtrlCore/lay.h>

#include "image_shared.h"
#include <Draw/iml_header.h>

#endif

