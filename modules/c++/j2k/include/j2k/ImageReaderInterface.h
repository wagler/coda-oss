/* =========================================================================
 * This file is part of j2k-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2014, General Dynamics - Advanced Information Systems
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

#ifndef __J2K_IMAGE_READER_INTERFACE_H__
#define __J2K_IMAGE_READER_INTERFACE_H__

#include <import/io.h>

#include "j2k/Header.h"

namespace j2k
{

/**
 *********************************************************************
 * @class ImageReader
 * @brief Reads a j2k image
 *
 * Reads a j2k image and parses out the IFD.  Contains functions for
 * getting data from the image and retrieving the j2k IFD.
 *********************************************************************/
class ImageReaderInterface
{
public:
    /**
     *****************************************************************
     * Constructor.  Sets the image reader to read from the specified
     * file stream.
     *
     * @param input
     *   the stream to read the j2k image from
     *****************************************************************/
    ImageReaderInterface(io::SeekableInputStream& input) :
       mInput(input), mHeader(NULL),
                mNextOffset(0), mBytePosition(0), mStripIndex(0),
                mElementSize(0), mReverseBytes(false)
    {
    }

    //! Destructor
    virtual ~ImageReaderInterface()
    {
    }
	
	bool canReadTiles() const;



    /**
     *****************************************************************
     * Processes the image from the file.  Reads the image's IFD
     * and stores it for later use.
     *****************************************************************/
    void process(const bool reverseBytes = false);

    /**
     *****************************************************************
     * Prints the image's header information to the specified output stream.
     *
     * @param output
     *   the stream to print the info to
     *****************************************************************/
    virtual void print(io::OutputStream &output) const;

    // returns the number of bytes read
    virtual sys::Uint64_T readTile(sys::Uint32_T tileX, sys::Uint32_T tileY,
        sys::ubyte **buf) {return 0;}
    virtual sys::Uint64_T readRegion(sys::Uint32_T x0, sys::Uint32_T y0,
        sys::Uint32_T x1, sys::Uint32_T y1, sys::ubyte **buf) {return 0;}
    sys::Size_T getUncompressedSizeInBytes() const;

    sys::Uint32_T getImageWidth() const;
    sys::Uint32_T getImageHeight() const;
    sys::Uint32_T getGridWidth() const;
    sys::Uint32_T getGridHeight() const;
    sys::Uint32_T getNumComponents() const;
    sys::Uint32_T getTilesX() const;
    sys::Uint32_T getTilesY() const;
    sys::Uint32_T getTileWidth() const;
    sys::Uint32_T getTileHeight() const;
    sys::Int32_T  getColorSpace() const;

    Header* getHeader() const { return mHeader; }

protected:

    //! Points to the input file stream.
    //io::FileInputStream *mInput;
	io::SeekableInputStream& mInput;

    Header* mHeader;

    //! The offset to the next IFD.
    sys::Uint32_T mNextOffset;

    //! Used to keep track of the current read position in the file.
    sys::Uint32_T mBytePosition;
    
    sys::Uint32_T mStripIndex;

    //! The element size of the image.
    unsigned short mElementSize;

    //! Whether to reverse bytes when reading.
    bool mReverseBytes;
};

} // End namespace.

#endif // __J2K_IMAGE_READER_INTERFACE_H__
