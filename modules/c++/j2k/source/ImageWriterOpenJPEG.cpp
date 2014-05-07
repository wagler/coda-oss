

#include "j2k/ImageWriter.h"

#if defined(USE_OPENJPEG)

#include "j2k/Common.h"
#include "j2k/Header.h"

#include "mem/ScopedArray.h"

#include <algorithm>


namespace { // io functions
    // We will use the OpenJPEG typedefs here in case their headers change.
    // As of 10 April 2014, these are very compatible with CODA sys types.

    // This code is inspired by implStreamRead/Seek/Skip in OpenJPEGImpl.c of NITRO

    // read data from stream into buffer, returns bytes left to read
    OPJ_SIZE_T ioRead(void* buf, OPJ_SIZE_T bytes, void *data)
    {
        io::SeekableInputStream* stream = static_cast<io::SeekableInputStream*>(data);
        sys::byte* buffer = static_cast<sys::byte*>(buf);

        // our streams classes handle only reading what is available.
        sys::SSize_T bytesRead = stream->read(buffer, bytes);

        if (-1 == bytesRead)
            throw except::IOException(Ctxt("Error reading from stream."));

        return bytes - bytesRead;
    }

    // seek to a "file" position, returns 1 for success
    OPJ_BOOL ioSeek(OPJ_OFF_T bytes, void *data)
    {
        // we may have to keep some notion of absolute position in file,
        // if these calls are all relative... not sure what issues we'll
        // run into when multi-threading.
        io::SeekableInputStream* stream = static_cast<io::SeekableInputStream*>(data);
        stream->seek(bytes, io::Seekable::START);
        return 1;
    }

    // skips reading some bytes, returns number of bytes skipped
    OPJ_OFF_T ioSkip(OPJ_OFF_T bytes, void *data)
    {
        if (bytes < 0)
        {
            return 0;
        }

        io::SeekableInputStream* stream = static_cast<io::SeekableInputStream*>(data);
        stream->seek(bytes, io::Seekable::CURRENT);
        return bytes;
    }

    OPJ_SIZE_T ioWrite(void *buf, OPJ_SIZE_T bytes, void *data)
    {
        //CC: at first I had this as an InputStream, but the virtual function
        //    kept getting resolved as available() instead of write().  Couldn't
        //    figure out why, but it worked great when I casted as ByteStream.
        io::ByteStream* stream = static_cast<io::ByteStream*>(data);

        if (bytes == 0)
        {
            return 0;
        }

        sys::byte* buffer = static_cast<sys::byte*>(buf);
        stream->write(buffer, bytes);

        return bytes;
    }

    // helper function
    void openJPEG_errorHandler(const char* msg, void* data)
    {
        throw except::Exception(Ctxt(msg));
    }
}

namespace j2k
{
    ImageWriterOpenJPEG::ImageWriterOpenJPEG(io::SeekableOutputStream& output, Header header) :
        ImageWriterInterface(output, header), mOpenJPEG_Initialized(false),
        mCodec(NULL), mStream(NULL), mImage(NULL)
    {

    }

    ImageWriterOpenJPEG::~ImageWriterOpenJPEG()
    {
        if (mCodec)
        {
            opj_destroy_codec(mCodec);
            mCodec = NULL;
        }

        if (mStream)
        {
            opj_stream_destroy(mStream);
            mStream = NULL;
        }

        if (mImage)
        {
            opj_image_destroy(mImage);
            mImage = NULL;
        }
    }

    void ImageWriterOpenJPEG::openJPEG_CreateIO()
    {
        const OPJ_BOOL isInput = 0;
        mStream = opj_stream_create(OPENJPEG_STREAM_SIZE, isInput);
        if (!mStream)
        {
            throw except::Exception(Ctxt("Error creating openjpeg stream."));
        }
        else
        {
            // The OpenJPEG library needs our IO functions defined, and they
            // must follow a certain footprint.
            opj_stream_set_user_data(mStream, mCompressedStream);
            opj_stream_set_read_function(mStream, ioRead);
            opj_stream_set_seek_function(mStream, ioSeek);
            opj_stream_set_skip_function(mStream, ioSkip);
            opj_stream_set_write_function(mStream, ioWrite);
        }
    }

    void ImageWriterOpenJPEG::setTile(sys::Uint32_T tileX, sys::Uint32_T tileY,
        sys::ubyte **buf, sys::Uint32_T bufSize)
    {
        if (!mOpenJPEG_Initialized)
        {
            initImage();
        }

        //TODO: look at the if(thisTileWidth < tileWidth) condition below.
        //      There seems to be a memory leak.  I'd suggest improving the
        //      signature of this function as well, but this was largely just
        //      ported over from the legacy c code.

        //OPJ_UINT32 bufSize; // looks like the opj_read_tile_header() won't support 64-bit buffer sizes
        const OPJ_UINT32 width = mHeader.getImageWidth();
        const OPJ_UINT32 height = mHeader.getImageHeight();
        const OPJ_UINT32 tileWidth = mHeader.getTileWidth();
        const OPJ_UINT32 tileHeight = mHeader.getTileHeight();
        const OPJ_UINT32 xTiles = mHeader.getTilesX();
        const OPJ_UINT32 yTiles = mHeader.getTilesY();
        size_t numBitsPerPixel = 0;
        size_t numBytesPerPixel = 0;
        sys::Uint64_T fullBufSize = 0;
        const sys::Size_T numComponents = mHeader.getNumComponents();
        sys::Uint32_T nBytes = mHeader.getNumBytesPerPixel();
        sys::Uint32_T tileIndex = tileY * xTiles + tileX;


        // Check for edge case where we may have partial tile
        sys::Uint32_T thisTileWidth = tileWidth;
        sys::Uint32_T thisTileHeight = tileHeight;
        if (tileX == xTiles - 1 && width % tileWidth != 0)
            thisTileWidth = width % tileWidth;
        if (tileY == yTiles - 1 && height % tileHeight != 0)
            thisTileHeight = height % tileHeight;

        sys::Uint32_T thisTileSize = thisTileWidth * thisTileHeight * numComponents * nBytes;

        if(thisTileWidth < tileWidth)
        {
            // TODO: The current approach below only works for single band
            //       imagery.  For RGB data, I believe it is stored as all
            //       red, then all green, then all blue, so we would need
            //       a temp buffer rather than reusing the current buffer.

            if (numComponents != 1)
            {
                throw except::NotImplementedException(Ctxt("Partial tile width not implemented for multi-band"));
            }

            // We have a tile that is wider than it "should" be
            // Need to create smaller buffer to pass to write function
            {
                OPJ_UINT32 ii;
                size_t srcOffset = 0;
                size_t destOffset = 0;
                const size_t srcStride = tileWidth * nBytes;
                const size_t destStride = thisTileWidth * nBytes;

                sys::ubyte* newTileBuf = new sys::ubyte[thisTileSize];
                if(!newTileBuf)
                {
                    throw except::OutOfMemoryException(Ctxt("Could not allocate buffer for tile"));
                }

                for(ii = 0; ii < thisTileHeight; ++ii, srcOffset += srcStride, 
                    destOffset += destStride)
                    memcpy(newTileBuf + destOffset, buf + srcOffset, destStride);
                //cc figure out whats going on here, there's a memory leak
                *buf = newTileBuf;
            }
        }

        // writes the tile to the OpenJPEG library.  In turn, it will compress the data.
        if (!opj_write_tile(mCodec,
            tileIndex,
            (OPJ_BYTE* )*buf,
            thisTileSize,
            mStream))
        {
            // CC: basically, we should check to see if the buffer is full, due to the
            //     compressed image being greater in size than the uncompressed.
            //     If you start catching this exception, dig in and beef up the stream
            //     logic.
            throw except::Exception(Ctxt("Error writing tile"));

            //const nrt_Off currentPos = nrt_IOInterface_tell(impl->compressed, &ioError);
            //const nrt_Off ioSize = nrt_IOInterface_getSize(impl->compressed, &ioError);
            //if (NRT_IO_SUCCESS(currentPos) &&
            //    NRT_IO_SUCCESS(ioSize) &&
            //    currentPos + OPENJPEG_STREAM_SIZE >= ioSize)
            {
                /* The write failed because implStreamWrite() failed because
                * nrt_IOInterface_write() failed because we didn't have enough
                * room left in the buffer that we copy to prior to flushing out
                * to disk in OpenJPEGWriter_write().  The buffer is sized to the
                * uncompressed image size, so this only occurs if the compressed
                * image is actually larger than the uncompressed size.
                * TODO: Handle resizing the buffer on the fly when this occurs
                * inside implStreamWrite().  Long-term if we're able to thread
                * per tile, we won't have to reallocate nearly as much.
                */
                /*  nrt_Error_init(error,
                "Error writing tile: Compressed image is larger "
                "than uncompressed image",
                NRT_CTXT,
                NRT_ERR_INVALID_OBJECT);*/
            }

            /*nrt_Error_init(error, "Error writing tile", NRT_CTXT,
            NRT_ERR_INVALID_OBJECT);
            goto CATCH_ERROR;*/
        }
    }

    void ImageWriterOpenJPEG::write()
    {
        // all the data has been added to the OpenJPEG library, so we tell it
        // to finish up and compress.
        if (!opj_end_compress(mCodec, mStream))
        {
            throw except::Exception(Ctxt("Error ending compression"));
        }

        // copy/write the compressed data to the output IO
        sys::Off_T compressedSize = mCompressedStream->tell();
        mCompressedStream->seek(0, io::Seekable::START);
        mCompressedStream->streamTo(mOutput, compressedSize);
    }

    void ImageWriterOpenJPEG::initImage()
    {
        // set up the encoder parameters.
        opj_cparameters_t encoderParams;
        opj_set_default_encoder_parameters(&encoderParams);

        if (mLossy)
        {
            encoderParams.irreversible = 1;
        }
        else
        {
            /* TODO: These two lines should not be necessary when using lossless
            *       encoding but appear to be needed (at least in OpenJPEG 2.0) -
            *       otherwise we get a seg fault.
            *       The sample opj_compress.c is doing the same thing with a comment
            *       indicating that it's a bug. */
            ++encoderParams.tcp_numlayers;
            encoderParams.cp_disto_alloc = 1;
        }

        // set the number of resolutions
        { 
            // OpenJPEG defaults this to 6, but that causes the compressor 
            // to fail if the tile sizes are less than 2^6.  So we adjust this
            // down if necessary.
            const double logTwo = log(2);
            const OPJ_UINT32 minX = (OPJ_UINT32)floor(log(mHeader.getTileWidth()) / logTwo);
            const OPJ_UINT32 minY = (OPJ_UINT32)floor(log(mHeader.getTileHeight()) / logTwo);
            const OPJ_UINT32 minXY = std::min<OPJ_UINT32>(minX, minY);
            encoderParams.numresolution = std::min<OPJ_UINT32>(minXY, mResolutions);
        }

        // do we need to set other tcp_rates for other components?
        encoderParams.tcp_rates[0] = mCompressionRatio;

        /* Turn on tiling */
        encoderParams.tile_size_on = 1;
        encoderParams.cp_tx0 = 0;
        encoderParams.cp_ty0 = 0;
        encoderParams.cp_tdx = mHeader.getTileWidth();
        encoderParams.cp_tdy = mHeader.getTileHeight();

        const size_t nComponents = mHeader.getNumComponents();
        mem::ScopedArray<opj_image_cmptparm_t> cmptParams(new opj_image_cmptparm_t[nComponents]);

        if (!(cmptParams.get()))
        {
            throw except::OutOfMemoryException(Ctxt("Error allocating image compression parameters"));
        }

        const std::vector<ImageComponent> components = mHeader.getComponents();

        // populate the OpenJPEG component structures
        for(size_t i = 0; i < nComponents; ++i)
        {
            const ImageComponent component = components[i];
            cmptParams[i].w = component.w;
            cmptParams[i].h = component.h;
            cmptParams[i].prec = component.prec;
            cmptParams[i].x0 = component.x0;
            cmptParams[i].y0 = component.y0;
            cmptParams[i].dx = component.dx;
            cmptParams[i].dy = component.dy;
            cmptParams[i].sgnd = component.sgnd;
            cmptParams[i].bpp = component.bpp; // so far, i've just seen this as 0
        }

        size_t nBytes = mHeader.getNumBytesPerPixel();
        size_t uncompressedSize = mHeader.getImageWidth() * mHeader.getImageHeight() * nComponents * nBytes;
        mCompressedStream = new io::ByteStream(uncompressedSize); // CC: we could let this resize as needed, but may be slower

        // initializes the OpenJPEG stream and callbacks
        openJPEG_CreateIO();

        OPJ_COLOR_SPACE colorSpace = OPJ_COLOR_SPACE(mHeader.getColorSpace());

        // initialize the codec for compression
        if (!(mCodec = opj_create_compress(OPJ_CODEC_J2K)))
        {
            throw except::Exception(Ctxt("Error creating OpenJPEG codec"));
        }

        // initialize the image object
        if (!(mImage = opj_image_tile_create(nComponents, cmptParams.get(),
            colorSpace)))
        {
            throw except::Exception(Ctxt("Error creating OpenJPEG image"));
        }

        // you'd think these would be set in the opj_image_tile_create function...
        mImage->numcomps = nComponents;
        mImage->x0 = 0;
        mImage->y0 = 0;
        mImage->x1 = mHeader.getImageWidth();
        mImage->y1 = mHeader.getImageHeight();
        mImage->color_space = colorSpace;

        if(!opj_set_error_handler(mCodec,
            openJPEG_errorHandler,
            NULL))
        {
            throw except::Exception(Ctxt("Unable to set OpenJPEG error handler"));
        }

        if (!opj_setup_encoder(mCodec, &encoderParams, mImage))
        {
            throw except::Exception(Ctxt("Error setting up OpenJPEG decoder"));

        }

        // this well tell OpenJPEG to start compressing the data we throw at it
        // via the set_tile functions
        if (!opj_start_compress(mCodec, mImage, mStream))
        {
            throw except::Exception(Ctxt("Error starting OpenJPEG compression"));
        }

        mOpenJPEG_Initialized = true;

    }


} // end namespace j2k

#endif
