/* =========================================================================
 * This file is part of sys-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 *
 * sys-c++ is free software; you can redistribute it and/or modify
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

#ifdef WIN32

#include <limits>
#include <cmath>
#include "sys/File.h"

#ifdef _MSC_VER // Microsoft C++
#pragma warning(disable: 4290) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#endif

void sys::File::create(const std::string& str,
                       int accessFlags,
                       int creationFlags) throw (sys::SystemException)
{
    // If the truncate bit is on AND the file does exist,
    // we need to set the mode to TRUNCATE_EXISTING
    if ((creationFlags & sys::File::TRUNCATE) && sys::OS().exists(str) )
    {
        creationFlags = TRUNCATE_EXISTING;
    }
    else
    {
        creationFlags = ~sys::File::TRUNCATE & creationFlags;
    }

    mHandle = CreateFileA(str.c_str(),
                         static_cast<DWORD>(accessFlags),
                         FILE_SHARE_READ, NULL,
                         static_cast<DWORD>(creationFlags),
                         FILE_ATTRIBUTE_NORMAL, NULL);

    if (mHandle == SYS_INVALID_HANDLE)
    {
        throw sys::SystemException(Ctxt(FmtX("Error opening file: [%s]", str.c_str())));
    }
    mPath = str;
}

void sys::File::readInto(char *buffer, Size_T size) throw (sys::SystemException)
{
    const size_t MAX_READ_SIZE = std::numeric_limits<DWORD>::max();
    size_t bytesRead = 0;
    size_t bytesRemaining = size;

    while (bytesRead < size)
    {
        // Determine how many bytes to read
        const DWORD bytesToRead = static_cast<DWORD>(
                std::min(MAX_READ_SIZE, bytesRemaining));

        // Read from file
        DWORD bytesThisRead = 0;
        if (!ReadFile(mHandle,
                      buffer + bytesRead,
                      bytesToRead,
                      &bytesThisRead,
                      NULL))
        {
            throw sys::SystemException(Ctxt("Error reading from file"));
        }
        else if (bytesThisRead == 0)
        {
            //! ReadFile does not fail when finding the EOF --
            //  instead it reports 0 bytes read, so this stops an infinite loop
            //  from Unexpected EOF
            throw sys::SystemException(Ctxt("Unexpected end of file"));
        }

        bytesRead += bytesThisRead;
        bytesRemaining -= bytesThisRead;
    }
}

void sys::File::writeFrom(const char *buffer, Size_T size) throw (sys::SystemException)
{
    const size_t MAX_WRITE_SIZE = std::numeric_limits<DWORD>::max();
    size_t bytesRemaining = size;
    size_t bytesWritten = 0;

    while (bytesWritten < size)
    {
        // Determine how many bytes to write
        const DWORD bytesToWrite = static_cast<DWORD>(
            std::min(MAX_WRITE_SIZE, bytesRemaining));

        // Write the data
        DWORD bytesThisWrite = 0;
        if (!WriteFile(mHandle,
                       buffer + bytesWritten,
                       bytesToWrite,
                       &bytesThisWrite,
                       NULL))
        {
            throw sys::SystemException(Ctxt("Writing from file"));
        }

        // Accumulate this write until we are done
        bytesRemaining -= bytesThisWrite;
        bytesWritten += bytesThisWrite;
    }
}

sys::Off_T sys::File::seekTo(sys::Off_T offset, int whence) throw (sys::SystemException)
{
    /* Ahhh!!! */
    LARGE_INTEGER largeInt;
    LARGE_INTEGER toWhere;
    largeInt.QuadPart = offset;
    if (!SetFilePointerEx(mHandle, largeInt, &toWhere, static_cast<DWORD>(whence)))
        throw sys::SystemException(Ctxt("SetFilePointer failed"));

    return (sys::Off_T) toWhere.QuadPart;
}

sys::Off_T sys::File::length() throw (sys::SystemException)
{
    DWORD highOff;
    DWORD ret = GetFileSize(mHandle, &highOff);
    sys::Uint64_T off = highOff;
    return (sys::Off_T)(off << 32) + ret;
}

sys::Off_T sys::File::lastModifiedTime() throw (sys::SystemException)
{
    FILETIME creationTime, lastAccessTime, lastWriteTime;
    BOOL ret = GetFileTime(mHandle, &creationTime,
            &lastAccessTime, &lastWriteTime);
    if (ret)
    {
        ULARGE_INTEGER uli;
        uli.LowPart = lastWriteTime.dwLowDateTime;
        uli.HighPart = lastWriteTime.dwHighDateTime;
        ULONGLONG stInMillis(uli.QuadPart/10000);
        return (sys::Off_T)stInMillis;
    }
    throw sys::SystemException(Ctxt(
                    FmtX("Error getting last modified time for path %s",
                            mPath.c_str())));
}

void sys::File::flush()
{
    if (!FlushFileBuffers(mHandle))
    {
        throw sys::SystemException(Ctxt("Error flushing file " + mPath));
    }
}

void sys::File::close()
{
    CloseHandle(mHandle);
    mHandle = SYS_INVALID_HANDLE;
}

#endif
