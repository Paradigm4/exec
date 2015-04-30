/*
 *    _____      _ ____  ____
 *   / ___/_____(_) __ \/ __ )
 *   \__ \/ ___/ / / / / __  |
 *  ___/ / /__/ / /_/ / /_/ / 
 * /____/\___/_/_____/_____/  
 *
 *
 * BEGIN_COPYRIGHT
 *
 * This file is part of SciDB.
 * Copyright (C) 2008-2014 SciDB, Inc.
 *
 * SciDB is free software: you can redistribute it and/or modify
 * it under the terms of the AFFERO GNU General Public License as published by
 * the Free Software Foundation.
 *
 * SciDB is distributed "AS-IS" AND WITHOUT ANY WARRANTY OF ANY KIND,
 * INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,
 * NON-INFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE. See
 * the AFFERO GNU General Public License for the complete license terms.
 *
 * You should have received a copy of the AFFERO GNU General Public License
 * along with SciDB.  If not, see <http://www.gnu.org/licenses/agpl-3.0.html>
 *
 * END_COPYRIGHT
 */

#define _XOPEN_SOURCE
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <math.h>

#include <vector>

#include <boost/assign.hpp>

#include "query/FunctionLibrary.h"
#include "query/FunctionDescription.h"


using namespace std;
using namespace scidb;
using namespace boost::assign;

static void
danger(const Value** args, Value *res, void*)
{
  res->setInt32( ::system ( args[0]->getString() ));
}

REGISTER_FUNCTION(exec, list_of("string"), "int32", danger);
