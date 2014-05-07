


#include "j2k/ImageReaderInterface.h"

namespace j2k
{


void ImageReaderInterface::print(io::OutputStream &output) const
{
    if (mHeader)
    {
        mHeader->print(output);
    }
    else
    {
        output.writeln("Header has not yet been read.");
    }
}

sys::Size_T ImageReaderInterface::getUncompressedSizeInBytes() const
{
    sys::Size_T nComponents = getNumComponents();
    sys::Size_T componentBytes = (mHeader->getComponents()[0].prec - 1) / 8 + 1;
    sys::Size_T bufSize = (sys::Size_T)(getImageWidth()) * getImageHeight() * componentBytes * nComponents;
    return bufSize;
}

sys::Uint32_T ImageReaderInterface::getImageWidth() const
{
        if (mHeader)
            return mHeader->getImageWidth();
        else
            return 0;
    }

    sys::Uint32_T ImageReaderInterface::getImageHeight() const
{
        if (mHeader)
            return mHeader->getImageHeight();
        else
            return 0;
    }

    sys::Uint32_T ImageReaderInterface::getGridWidth() const
    {
        if (mHeader)
            return mHeader->getGridWidth();
        else
            return 0;
    }

    sys::Uint32_T ImageReaderInterface::getGridHeight() const
    {
        if (mHeader)
            return mHeader->getGridHeight();
        else
            return 0;
    }

    sys::Uint32_T ImageReaderInterface::getNumComponents() const
    {
        if (mHeader)
            return (sys::Uint32_T)mHeader->getNumComponents();
        else
            return 0;
    }

    sys::Uint32_T ImageReaderInterface::getTilesX() const
        {
        if (mHeader)
            return mHeader->getTilesX();
        else
            return 0;
    }

    sys::Uint32_T ImageReaderInterface::getTilesY() const
         {
        if (mHeader)
            return mHeader->getTilesY();
        else
            return 0;
    }

    sys::Uint32_T ImageReaderInterface::getTileWidth() const
        {
        if (mHeader)
            return mHeader->getTileWidth();
        else
            return 0;
    }

    sys::Uint32_T ImageReaderInterface::getTileHeight() const
        {
        if (mHeader)
            return mHeader->getTileHeight();
        else
            return 0;
    }

    sys::Int32_T  ImageReaderInterface::getColorSpace() const
{
        if (mHeader)
            return mHeader->getColorSpace();
        else
            return 0;
    }


} // end namespace j2k