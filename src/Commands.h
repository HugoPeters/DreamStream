/********************************************************************************
* Copyright (c) 2018 by Hugo Peters. http://hugo.fyi/                           *
*                                                                               *
* Permission is hereby granted, free of charge, to any person obtaining a copy  *
* of this software and associated documentation files (the "Software"), to deal *
* in the Software without restriction, including without limitation the rights  *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     *
* copies of the Software, and to permit persons to whom the Software is         *
* furnished to do so, subject to the following conditions:                      *
*                                                                               *
* The above copyright notice and this permission notice shall be included in    *
* all copies or substantial portions of the Software.                           *
*                                                                               *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     *
* THE SOFTWARE.                                                                 *
*********************************************************************************/

#ifndef _Commands_H_
#define _Commands_H_

#include <cstdint>
#include "Utils.h"
#include "Command.h"
#include "Defines.h"

struct CommandDescriptor
{
    CommandDescriptor()
        : m_cmd({ 0,0 })
        , m_enum_type(-1)
        , m_name("")
    {}

    CommandDescriptor(const char* name, int enumType, uint8_t upper, uint8_t lower)
        : m_cmd({ upper, lower })
        , m_enum_type(enumType)
        , m_name(name)
    {}

    CMD m_cmd;
    int m_enum_type;
    const char* m_name;
};

namespace Commands
{
    enum Type
    {
    #define DECLARE_COMMAND(enumType, ...) enumType,
    #include "CommandsList.h"
    #undef DECLARE_COMMAND
        NUM
    };

    static CommandDescriptor Descriptors[NUM]
    {
    #define DECLARE_COMMAND(enumType, name, cmdUpper, cmdLower)\
		CommandDescriptor(name, int(Type::enumType), cmdUpper, cmdLower),
    #include "CommandsList.h"
    #undef DECLARE_COMMAND
    };
}

#endif // _Commands_H_
