#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "execResult.h"
#include "parser.h"

ExecResult executeExternal(ParsedCommand* command);

#endif