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
#ifndef __J2K_IMAGE_WRITER_H__
#define __J2K_IMAGE_WRITER_H__

#if defined(USE_JASPER)


#elif defined(USE_OPENJPEG)

#  include "j2k/ImageWriterOpenJPEG.h"

    namespace j2k
    {
        typedef ImageWriterOpenJPEG ImageWriter;
    } // End namespace.

#endif // which j2k library

#endif // __J2K_IMAGE_WRITER_H__