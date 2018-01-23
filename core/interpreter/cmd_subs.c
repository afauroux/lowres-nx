//
// Copyright 2018 Timo Kloss
//
// This file is part of LowRes NX.
//
// LowRes NX is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// LowRes NX is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LowRes NX.  If not, see <http://www.gnu.org/licenses/>.
//

#include "cmd_subs.h"
#include "core.h"

enum ErrorCode cmd_CALL(struct Core *core)
{
    struct Interpreter *interpreter = core->interpreter;
    
    // CALL
    struct Token *tokenCALL = interpreter->pc;
    ++interpreter->pc;
    
    // Identifier
    if (interpreter->pc->type != TokenIdentifier) return ErrorExpectedSubprogramName;
    struct Token *tokenIdentifier = interpreter->pc;
    ++interpreter->pc;
    
    if (interpreter->pass == PassPrepare)
    {
        struct SubItem *item = tok_getSub(&interpreter->tokenizer, tokenIdentifier->symbolIndex);
        if (!item) return ErrorUndefinedSubprogram;
        tokenCALL->jumpToken = item->token;
        
        return itp_endOfCommand(interpreter);
    }
    else if (interpreter->pass == PassRun)
    {
        enum ErrorCode errorCode = lab_pushLabelStackItem(interpreter, LabelTypeCALL, interpreter->pc);
        if (errorCode != ErrorNone) return errorCode;
        
        interpreter->pc = tokenCALL->jumpToken; // after sub name
    }
    
    return ErrorNone;
}

enum ErrorCode cmd_SUB(struct Core *core)
{
    struct Interpreter *interpreter = core->interpreter;
    
    // SUB
    struct Token *tokenSUB = interpreter->pc;
    ++interpreter->pc;
    
    // Identifier
    if (interpreter->pc->type != TokenIdentifier) return ErrorExpectedSubprogramName;
    ++interpreter->pc;
    
    // Eol
    if (interpreter->pc->type != TokenEol) return ErrorExpectedEndOfLine;
    ++interpreter->pc;
    
    if (interpreter->pass == PassPrepare)
    {
        if (interpreter->numLabelStackItems > 0)
        {
            return ErrorSubCannotBeNested;
        }
        enum ErrorCode errorCode = lab_pushLabelStackItem(interpreter, LabelTypeSUB, tokenSUB);
        if (errorCode != ErrorNone) return errorCode;
    }
    else if (interpreter->pass == PassRun)
    {
        interpreter->pc = tokenSUB->jumpToken; // after END SUB
    }
    
    return ErrorNone;
}

enum ErrorCode cmd_END_SUB(struct Core *core)
{
    struct Interpreter *interpreter = core->interpreter;
    
    // END SUB
    ++interpreter->pc;
    ++interpreter->pc;
    
    // Eol
    if (interpreter->pc->type != TokenEol) return ErrorExpectedEndOfLine;
    ++interpreter->pc;
    
    if (interpreter->pass == PassPrepare)
    {
        struct LabelStackItem *item = lab_popLabelStackItem(interpreter);
        if (!item)
        {
            return ErrorEndSubWithoutSub;
        }
        else if (item->type == LabelTypeSUB)
        {
            item->token->jumpToken = interpreter->pc;
        }
        else
        {
            return ErrorEndSubWithoutSub;
        }
    }
    else if (interpreter->pass == PassRun)
    {
        struct LabelStackItem *itemCALL = lab_popLabelStackItem(interpreter);
        if (!itemCALL) return ErrorEndSubWithoutSub;
        
        if (itemCALL->type == LabelTypeCALL)
        {
            // jump back
            interpreter->pc = itemCALL->token; // after CALL
        }
        else
        {
            return ErrorEndSubWithoutSub;
        }
    }
    return ErrorNone;
}
