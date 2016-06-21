/* =========================================================================
 * This file is part of str-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 *
 * str-c++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this program; If not, 
 * see <http://www.gnu.org/licenses/>.
 *
 */


#include "str/Tokenizer.h"


str::Tokenizer::Tokenizer(const std::string& str, const std::string& delim)
{
    const std::string::size_type str_l = str.length();
    std::string::size_type pos = 0;
    std::string::size_type start=0, end;
    while (start != std::string::npos) // npos is -1
    {
        start = str.find_first_not_of(delim, pos);
        if (start == std::string::npos)
        {
            break;
        }
        end = str.find_first_of(delim, start);
        if (end == std::string::npos)
        {
            end = str_l;
        }
        vec.push_back(str.substr(start, end - start));
        pos = end;

    }

}
