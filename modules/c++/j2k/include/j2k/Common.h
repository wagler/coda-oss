/* =========================================================================
* This file is part of j2k-c++ 
* =========================================================================
* 
* (C) Copyright 2004 - 2011, General Dynamics - Advanced Information Systems
*
* j2k-c++ is free software; you can redistribute it and/or modify
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

/**
*********************************************************************
* @file
* @brief Contains classes for handling j2k files.
*********************************************************************/
#ifndef __J2K_COMMON_H__
#define __J2K_COMMON_H__

namespace j2k
{

    /**
    *********************************************************************
    * @class Const
    * @brief Contains constants used by JPEG 2000 files.
    *
    * Constants such as JPEG2000 types, type sizes, and others have been
    * pooled together into this class.  Every member can be accessed
    * statically so there is no need to create in instance of this class
    * to use the constants.
    *********************************************************************/
    class Const
    {
    public:
        //! Default constructor
        Const()
        {
        }

        //! Deconstructor
        ~Const()
        {
        }

        /**
        *****************************************************************
        * @class ColorSpace
        * @brief Contains an enumeration of every ColorSpace type that J2K
        * supports.
        *****************************************************************/
        class ColorSpace
        {
        public:
            // these map to the OpenJPEG OPJ_COLOR_SPACE enum.
            enum
            {
                UNKNOWN = -1,    // not supported by the library 
                UNSPECIFIED = 0, // not specified in the codestream 
                SRGB = 1,		 // sRGB
                GRAY = 2,		 // grayscale
                YUV = 3			 // also known as SYCC
            };
        };
    };

} // End namespace j2k.

#endif // __J2K_COMMON_H__
