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

#ifndef __J2K_IMAGE_WRITER_INTERFACE_H__
#define __J2K_IMAGE_WRITER_INTERFACE_H__

#include <import/io.h>

#include "j2k/Header.h"

namespace j2k
{

/**
 *********************************************************************
 * @class ImageWriterInterface
 * @brief JPEG 2000 Writer
 * 
 * Use the ImageWriterInterface by first creating a fully-populated 
 * header object, then constructing a derived class with the output
 * stream and header.  Next, adjust any JPEG2000 compression options,
 * write uncompressed data to it with setTile() or setRegion() and 
 * then write out the final compressed image.
 *********************************************************************/
class ImageWriterInterface
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
    ImageWriterInterface(io::SeekableOutputStream& output, Header& header) :
       mOutput(output), mHeader(header),
                mNextOffset(0), mBytePosition(0), mStripIndex(0),
                mElementSize(0), mReverseBytes(false), mLossy(false),
                mCompressionRatio(4.0), mResolutions(INT_MAX)
    {
    }

    //! Destructor
    virtual ~ImageWriterInterface()
    {
    }

    // writer options
    bool isLossy() const {
        return mLossy;
    }

    void setLossy(bool lossy) {
        mLossy = lossy;
    }

    double getCompressionRatio() const {
        return mCompressionRatio;
    }

    void setCompressionRatio(double compRatio) {
        mCompressionRatio = compRatio;
    }

    sys::Uint32_T getNumResolutions() const {
        return mResolutions;
    }

    void setNumResolutions(sys::Uint32_T numRes) {
        mResolutions = numRes;
    }




protected:

    //! Points to the output file stream.
	io::SeekableOutputStream& mOutput;

    Header mHeader;

    //! The offset to the next IFD.
    sys::Uint32_T mNextOffset;

    //! Used to keep track of the current read position in the file.
    sys::Uint32_T mBytePosition;
    
    sys::Uint32_T mStripIndex;

    //! The element size of the image.
    unsigned short mElementSize;

    //! Whether to reverse bytes when reading.
    bool mReverseBytes;

    // option for irreversible (lossy) DWT 9-7 compression
    bool mLossy;

    // option for compression ratio
    double mCompressionRatio;

    // option for number of resolutions
    sys::Uint32_T mResolutions;
};

} // End namespace.

#endif // __J2K_IMAGE_WRITER_INTERFACE_H__
