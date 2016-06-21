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

#include "str/Manip.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <climits>
#include <cstdio>

void str::trim(std::string & s)
{
    std::string::size_type i;
    for (i = 0; i < s.length(); i++)
    {
        if (!isspace(s[i]))
            break;
    }
    s.erase(0, i);

    for (i = s.length() - 1; (int) i >= 0; i--)
    {
        if (!isspace(s[i]))
            break;

    }
    if (i + 1 < s.length())
        s.erase(i + 1);
}

bool str::endsWith(const std::string & s, const std::string & match)
{
    const std::string::size_type mLen = match.length();
    const std::string::size_type sLen = s.length();
    for (std::string::size_type i = 0; i < sLen && i < mLen; ++i)
        if (!(s[sLen - i - 1] == match[mLen - i - 1]))
            return false;
    return sLen >= mLen;
}

bool str::startsWith(const std::string & s, const std::string & match)
{
    const std::string::size_type mLen = match.length();
    const std::string::size_type sLen = s.length();
    for (std::string::size_type i = 0; i < sLen && i < mLen; ++i)
        if (!(s[i] == match[i]))
            return false;
    return sLen >= mLen;
}

size_t str::replace(std::string& str, 
                    const std::string& search,
                    const std::string& replace,
                    size_t start)
{
    const std::string::size_type index = str.find(search, start);

    if (index != std::string::npos)
    {
        str.replace(index, search.length(), replace);
        start = index;
    }
    else
    {
        start = str.length();
    }

    return start;        
}

void str::replaceAll(std::string& string, 
                     const std::string& search,
                     const std::string& replace)
{
    std::string::size_type start = 0;
    while (start < string.length())
    {
        start = str::replace(string, 
                             search, 
                             replace, 
                             start);
        // skip ahead --
        // avoids inifinite loop if replace contains search 
        start += replace.length();                             
    }
}

bool str::contains(const std::string& str, const std::string& match)
{
    return str.find(match) != std::string::npos;
}

bool str::isAlpha(const std::string& s)
{
    typedef std::string::const_iterator StringIter;
    for (StringIter it = s.begin(); it != s.end(); ++it)
    {
        if (!isalpha(*it))
            return false;
    }
    return !s.empty();
}

bool str::isAlphaSpace(const std::string& s)
{
    typedef std::string::const_iterator StringIter;
    for (StringIter it = s.begin(); it != s.end(); ++it)
    {
        if (!isalpha(*it) && *it != ' ')
            return false;
    }
    return !s.empty();
}

bool str::isNumeric(const std::string& s)
{
    typedef std::string::const_iterator StringIter;
    for (StringIter it = s.begin(); it != s.end(); ++it)
    {
        if (!isdigit(*it))
            return false;
    }
    return !s.empty();
}

bool str::isNumericSpace(const std::string& s)
{
    typedef std::string::const_iterator StringIter;
    for (StringIter it = s.begin(); it != s.end(); ++it)
    {
        if (!isdigit(*it) && *it != ' ')
            return false;
    }
    return !s.empty();
}

bool str::isWhitespace(const std::string& s)
{
    typedef std::string::const_iterator StringIter;
    for (StringIter it = s.begin(); it != s.end(); ++it)
    {
        if (!isspace(*it))
            return false;
    }
    return true;
}

bool str::isAlphanumeric(const std::string& s)
{
    typedef std::string::const_iterator StringIter;
    for (StringIter it = s.begin(); it != s.end(); ++it)
    {
        if (!isalpha(*it) && !isdigit(*it))
            return false;
    }
    return !s.empty();
}

bool str::isAsciiPrintable(const std::string& s)
{
    typedef std::string::const_iterator StringIter;
    for (StringIter it = s.begin(); it != s.end(); ++it)
    {
        char c = *it;
        if (c < 32 || c > 126)
            return false;
    }
    return true;
}

bool str::containsOnly(const std::string& s, const std::string& validChars)
{
    typedef std::string::const_iterator StringIter;
    std::vector<bool> chars(255, false);
    for (StringIter it = validChars.begin(); it != validChars.end(); ++it)
        chars[(unsigned int)*it] = true;
    for (StringIter it = s.begin(); it != s.end(); ++it)
        if (!chars[(unsigned int)*it])
            return false;
    return true;
}

std::vector<std::string> str::split(const std::string& s,
        const std::string& splitter, size_t maxSplit)
{
    std::vector < std::string > vec;
    const std::string::size_type str_l = s.length();
    const std::string::size_type split_l = splitter.length();
    std::string::size_type pos = 0;
    std::string::size_type nextPos;
    while (pos < str_l && maxSplit != 1)
    {
        nextPos = s.find(splitter, pos);
        if (nextPos == std::string::npos)
            nextPos = str_l;
        if (nextPos != pos)
            vec.push_back(s.substr(pos, nextPos - pos));
        pos = nextPos + split_l;
        if (maxSplit != std::string::npos && vec.size() >= maxSplit - 1)
            break;
    }

    if (pos < str_l)
        vec.push_back(s.substr(pos));

    return vec;
}

static char transformCheck(int c, int (*transform)(int))
{
    // Ensure the character can be represented
    // as an unsigned char or is 'EOF', as the
    // behavior for all other characters is undefined
    int retval;
    if ((c >= 0 && c <= UCHAR_MAX) || c == EOF)
    {
        retval = transform(c);
    }
    else
    {
        // Invalid char for transform: no-op
        retval = c;
    }
    return static_cast<char>(c);
}

static char tolowerCheck(int c)
{
    return transformCheck(c, tolower);
}

static char toupperCheck(int c)
{
    return transformCheck(c, toupper);
}

void str::lower(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), tolowerCheck);
}

void str::upper(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), toupperCheck);
}

