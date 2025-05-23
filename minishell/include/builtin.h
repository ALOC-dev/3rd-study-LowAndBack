#ifndef BUILTIN_H
#define BUILTIN_H

#include "execResult.h"
#include "parser.h"

ExecResult executeBuiltin(ParsedCommand* command);

#endif