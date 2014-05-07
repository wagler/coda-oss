/* =========================================================================
* This file is part of j2k 
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

#ifndef __J2K_HEADER_H__
#define __J2K_HEADER_H__

#include <io/OutputStream.h>
#include "j2k/Common.h"

namespace j2k
{

    struct ImageComponent
    {
        // An image will consist of at least one component, or channel.
        // An RGB image will have 3.

        // this class maps to OpenJPEG opj_image_comp, but most libraries will
        // likely be similar in nature.

        // horizontal separation of a sample of ith component with respect to the reference grid
        sys::Uint32_T dx;

        // vertical separation of a sample of ith component with respect to the reference grid
        sys::Uint32_T dy;

        // data width
        sys::Uint32_T w;

        // data height
        sys::Uint32_T h;

        // x component offset compared to the whole image
        sys::Uint32_T x0;

        // y component offset compared to the whole image
        sys::Uint32_T y0;

        // precision
        sys::Uint32_T prec;

        // image depth in bits
        sys::Uint32_T bpp;

        // signed (1) / unsigned (0)
        bool sgnd;

        // number of decoded resolution
        sys::Uint32_T resno_decoded;

        // number of division by 2 of the out image compared to the original size of image
        sys::Uint32_T factor;

        // image component data, hope we don't need this
        //sys::Int32_T *data;

    };

    /**
    *********************************************************************
    * @class Header
    * @brief Contains JPEG 2000 header information
    *********************************************************************/
    class Header
    {
    public:

        /**
        *****************************************************************
        * Constructor.  Allows the user to set the values in the header
        * and also provides resonable defaults.
        *****************************************************************/
        Header(sys::Uint32_T gridWidth, sys::Uint32_T gridHeight,
            std::vector<ImageComponent>& components, sys::Uint32_T tileWidth,
            sys::Uint32_T tileHeight, sys::Int32_T colorSpace,
            sys::Uint32_T xOffset=0,sys::Uint32_T yOffset=0) :
        mGridWidth(gridWidth), mGridHeight(gridHeight),
            mComponents(components), mTileWidth(tileWidth),
            mTileHeight(tileHeight), mColorSpace(colorSpace),
            mX_Offset(xOffset), mY_Offset(yOffset)
        {
            mTilesX = gridWidth / tileWidth + (gridWidth % tileWidth == 0 ? 0 : 1);
            mTilesY = gridHeight / tileHeight + (gridHeight % tileHeight == 0 ? 0 : 1);
            mICC_Profile = NULL;
            mICC_ProfileLength = 0;
        }

        Header(const Header& other) :
            mGridWidth(other.getGridWidth()), mGridHeight(other.getGridHeight()),
            mComponents(other.getComponents()), mTileWidth(other.getTileWidth()),
            mTileHeight(other.getTileHeight()), mColorSpace(other.getColorSpace())
        {
            this->mX_Offset = other.mX_Offset;
            this->mY_Offset = other.mY_Offset;

            if (!other.mICC_Profile || other.mICC_ProfileLength == 0)
            {
                mICC_Profile = NULL;
                mICC_ProfileLength = 0;
            }
            else
            {
                mICC_Profile = new sys::ubyte[mICC_ProfileLength];
                mICC_ProfileLength = other.mICC_ProfileLength;
            }

            mTilesX = other.mTilesX;
            mTilesY = other.mTilesY;
        }

        //! Destructor
        ~Header()
        {
            if (mICC_Profile)
                delete [] mICC_Profile;
        }

        /**
        *****************************************************************
        * Prints the JPEG 2000 header to the specified output stream in
        * string format.
        *
        * @param output
        *   the stream to print the header to
        *****************************************************************/
        void print(io::OutputStream& output) const
        {
            std::ostringstream message;
            message << "Image Size:    " << getImageWidth() << " x " << getImageHeight() << std::endl;
            message << "Grid Size:     " << mGridWidth << " x " << mGridHeight << std::endl;
            message << "Components:    " << getNumComponents() << std::endl;
            message << "Colorspace:    ";
            switch (mColorSpace)
            {
            case Const::ColorSpace::UNSPECIFIED :
                message << "Unspecified" << std::endl;
                break;
            case Const::ColorSpace::SRGB :
                message << "sRGB" << std::endl;
                break;
            default:
                message << "Unknown" << std::endl;
            }

            std::vector<ImageComponent>::const_iterator it;
            for (it = mComponents.begin(); it != mComponents.end(); it++)
            {
                message << std::endl;
                message << "Component Info" << std::endl;
                message << "   Width:     " << (*it).w << std::endl;
                message << "   Height:    " << (*it).h << std::endl;
                message << "   Precision: " << (*it).prec << std::endl;
                message << "   BPP:       " << (*it).bpp << std::endl;
                message << "   x0:        " << (*it).x0 << std::endl;
                message << "   y0:        " << (*it).y0 << std::endl;
                message << "   x sep:     " << (*it).dx << std::endl;
                message << "   y sep:     " << (*it).dy << std::endl;
                message << "   signed:    " << (*it).sgnd << std::endl;
            }

            output.write(message.str());
        }

        // accessors

        sys::Uint32_T getGridWidth() const
        {
            return mGridWidth;
        }

        sys::Uint32_T getGridHeight() const
        {
            return mGridHeight;
        }

        sys::Size_T getNumComponents() const
        {
            return mComponents.size();
        }

        std::vector<ImageComponent> getComponents() const
        {
            return mComponents;
        }

        sys::Int32_T getColorSpace() const
        {
            return mColorSpace;
        }

        sys::Uint32_T getTileWidth() const
        {
            return mTileWidth;
        }

        sys::Uint32_T getTileHeight() const
        {
            return mTileHeight;
        }

        sys::Uint32_T getTilesX() const
        {
            return mTilesX;
        }

        sys::Uint32_T getTilesY() const
        {
            return mTilesY;
        }

        sys::Uint32_T getImageWidth() const
        {
            sys::Uint32_T maxWidth = 0;

            std::vector<ImageComponent>::const_iterator it;
            for (it = mComponents.begin(); it != mComponents.end(); it++)
            {
                maxWidth = std::max<sys::Uint32_T>((*it).w, maxWidth);
            }

            return maxWidth;
        }

        sys::Uint32_T getImageHeight() const
        {
            sys::Uint32_T maxHeight = 0;

            std::vector<ImageComponent>::const_iterator it;
            for (it = mComponents.begin(); it != mComponents.end(); it++)
            {
                maxHeight = std::max<sys::Uint32_T>((*it).h, maxHeight);
            }

            return maxHeight;
        }

        // used to compute the uncompressed image size in bytes
        sys::Uint32_T getNumBytesPerPixel() const
        {
            // CC: I think the theory here was to round up
            if (mComponents.size() > 0)
                return (mComponents[0].prec -1) / 8 + 1;
            else
                throw except::IndexOutOfRangeException(Ctxt("No components found in image."));
        }

        void setICCProfile (size_t ICC_Length, const sys::ubyte* ICCProfile)
        {
            if (mICC_Profile)
                delete [] mICC_Profile;
            mICC_Profile = new sys::ubyte[ICC_Length];
            mICC_ProfileLength = ICC_Length;
            memcpy(mICC_Profile, ICCProfile, ICC_Length);
        }

        // TODO: Implement getter function

    private:

        // horizontal offset from the origin of the reference grid to the left side of the image area
        sys::Uint32_T mX_Offset;

        // vertical offset from the origin of the reference grid to the top side of the image area
        sys::Uint32_T mY_Offset;

        // width of the reference grid
        sys::Uint32_T mGridWidth;

        // height of the reference grid
        sys::Uint32_T mGridHeight;

        // color space: sRGB, Greyscale or YUV
        //Const::ColorSpace mColorSpace;
        sys::Int32_T mColorSpace;

        // image components
        std::vector<ImageComponent> mComponents;

        // 'restricted' ICC profile
        sys::ubyte *mICC_Profile;

        // size of ICC profile
        sys::Uint32_T mICC_ProfileLength;

        // from the codestream:

        // tile width
        sys::Uint32_T mTileWidth;

        // tile height
        sys::Uint32_T mTileHeight;

        // number of tiles in the x direction (columns)
        sys::Uint32_T mTilesX;

        // number of tiles in the y direction (rows)
        sys::Uint32_T mTilesY;

    };

} // End namespace.

#endif // __J2K_HEADER_H__
