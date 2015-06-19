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
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

#include <vector>

#include <boost/assign.hpp>

#include "query/FunctionLibrary.h"
#include "query/FunctionDescription.h"


using namespace std;
using namespace scidb;
using namespace boost::assign;


static void
killer(const Value** args, Value *res, void*)
{
  pid_t pid = (pid_t)args[0]->getInt32();
  int sig = (int)args[1]->getInt32();
  res->setInt32( kill(pid, sig) );
  waitpid(-1, 0, WNOHANG);
}

static void
danger(const Value** args, Value *res, void*)
{
  const char *cmd = args[0]->getString();
  const char *sh[] = { "/bin/sh", "-c", cmd, NULL };
  pid_t pid;
  signal(SIGCHLD, SIG_IGN);
  if ((pid = fork()) < 0)
  {
    res->setInt32(-1);
    return;
  }
  else if (pid == 0)
  {
    // child process
    execv("/bin/sh", (char * const *)sh);
    _exit(0);
  } else
  {
    res->setInt32((int)pid);
  }
}

REGISTER_FUNCTION(exec, list_of("string"), "int32", danger);
REGISTER_FUNCTION(kill, list_of("int32")("int32"), "int32", killer);
