

#include "j2k/ImageReader.h"

#if defined(USE_OPENJPEG)


#include "j2k/Header.h"


namespace { // io functions
    // We will use the OpenJPEG typedefs here in case their headers change.
    // As of 10 April 2014, these are very compatible with CODA sys types.

    // This code is inspired by implStreamRead/Seek/Skip in OpenJPEGImpl.c of NITRO

    // read data from stream into buffer, returns bytes read
    OPJ_SIZE_T ioRead(void* buf, OPJ_SIZE_T bytes, void *data)
    {
        io::SeekableInputStream* stream = static_cast<io::SeekableInputStream*>(data);
        sys::byte* buffer = static_cast<sys::byte*>(buf);
       
        // our streams classes handle only reading what is available.
        sys::SSize_T bytesRead = stream->read(buffer, bytes);

        if (-1 == bytesRead)
            throw except::IOException(Ctxt("Error reading from stream."));

        return bytesRead;
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



    // helper function
      void openJPEG_errorHandler(const char* msg, void* data)
    {
        throw except::Exception(msg);
    }

    
}

namespace j2k
{

    /* from Reader.h
    typedef struct _j2k_IReader
    {
    J2K_IREADER_CAN_READ_TILES  canReadTiles;
    J2K_IREADER_READ_TILE       readTile;
    J2K_IREADER_READ_REGION     readRegion;
    J2K_IREADER_GET_CONTAINER   getContainer;
    J2K_IREADER_DESTRUCT        destruct;
    } j2k_IReader;

    typedef struct _j2k_Reader
    {
    j2k_IReader *iface;
    J2K_USER_DATA *data;
    } j2k_Reader;

    From nrt.h stuff, J2K_USER_DATA is a typedef for void.  so what's in void* data?

    From Container.h

    typedef struct _j2k_IContainer
    {
    J2K_ICONTAINER_GET_GRID_WIDTH getGridWidth;
    J2K_ICONTAINER_GET_GRID_HEIGHT getGridHeight;
    J2K_ICONTAINER_GET_NUM_COMPONENTS getNumComponents;
    J2K_ICONTAINER_GET_COMPONENT getComponent;

    J2K_ICONTAINER_GET_TILESX getTilesX;
    J2K_ICONTAINER_GET_TILESY getTilesY;
    J2K_ICONTAINER_GET_TILE_WIDTH getTileWidth;
    J2K_ICONTAINER_GET_TILE_HEIGHT getTileHeight;

    J2K_ICONTAINER_GET_IMAGE_TYPE getImageType;
    J2K_ICONTAINER_DESTRUCT destruct;
    } j2k_IContainer;

    typedef struct _j2k_Container
    {
    j2k_IContainer *iface;
    OpenJPEGReaderImpl *data;
    } j2k_Container;
    */

    ImageReaderOpenJPEG::~ImageReaderOpenJPEG()
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

        if (mImage && mImage)
        {
            opj_image_destroy(mImage);
            mImage = NULL;
        }
    }


    /*typedef struct _OpenJPEGReaderImpl
    {
    opj_dparameters_t parameters;
    nrt_Off ioOffset;
    nrt_IOInterface *io;
    int ownIO;
    j2k_Container *container;
    IOControl userData;
    } OpenJPEGReaderImpl;*/


  

    ImageReaderOpenJPEG::ImageReaderOpenJPEG(io::SeekableInputStream& input) :
        //mInputStream(input)
        ImageReaderInterface(input), mOpenJPEG_Initialized(false),
        mCodec(NULL), mStream(NULL), mImage(NULL)
    {
        // maps to OpenJPEGImpl.c::j2k_Reader_open, j2k_Reader_openIO

        // nrt_IOHandleAdapter_open should be handled in FileReader

        // new OpenJPEGReaderImpl
        // new j2k_Reader
        // ReaderInterface was static ... defining canReadTiles, readTile, readRegion, etc.

        // save handle to inputStream
        // save ioOffset
        //offset = input.

        // 

        readHeader();
    }

    // potentially make some of this static if we can reuse the codec for
    // multiple image readers
    void ImageReaderOpenJPEG::openJPEG_Setup()
    {
        // see OpenJPEGImpl.c :: OpenJPEG_setup

        if (mOpenJPEG_Initialized)
            return;

        // initializes the OpenJPEG stream and callbacks
        openJPEG_CreateIO();

        // create the openJPEG decompression structure
        if (!(mCodec = opj_create_decompress(OPJ_CODEC_J2K)))
        {
            throw except::Exception(Ctxt("Error creating OpenJPEG codec"));
        }

        // set the openJPEG error handler
        opj_set_error_handler(mCodec,
            openJPEG_errorHandler,
            NULL);

        // initializes the decoding parameters
        opj_dparameters_t decodingParameters;
        opj_set_default_decoder_parameters(&decodingParameters);

        if (!opj_setup_decoder(mCodec, &decodingParameters))
        {
            throw except::Exception(Ctxt("Error setting up the OpenJPEG decoder."));
        }

        // we have initialized the library.
        mOpenJPEG_Initialized = true;
    }

    

void ImageReaderOpenJPEG::openJPEG_CreateIO()
{
    // opj_stream_t *stream = NULL;
    const OPJ_BOOL isInput = 1;
    mStream = opj_stream_create(OPENJPEG_STREAM_SIZE, isInput);
    if (!mStream)
    {
        throw except::Exception(Ctxt("Error creating openjpeg stream."));
    }
    else
    {
        // The OpenJPEG library needs our IO functions defined, and they
        // must follow a certain footprint.
        opj_stream_set_user_data(mStream, &mInput);
        opj_stream_set_read_function(mStream, ioRead);
        opj_stream_set_seek_function(mStream, ioSeek);
        opj_stream_set_skip_function(mStream, ioSkip);
    }
}

    void ImageReaderOpenJPEG::readHeader()
    {
        // initialize the decoder
        openJPEG_Setup();

        OPJ_BOOL success;
        try {
            success = opj_read_header(mStream, mCodec, &mImage);
        } catch (except::Exception e) {
            throw except::Exception(Ctxt("Error reading header: " + e.getMessage()));
        }

        if (!success)
        {
            throw except::Exception(Ctxt("Error reading header."));
        } 

        opj_codestream_info_v2_t* codeStreamInfo = NULL;
        try {
            codeStreamInfo = opj_get_cstr_info(mCodec);
        } catch (except::Exception e) {
            throw except::Exception(Ctxt("Error reading code stream: " + e.getMessage()));
        }

        if (!codeStreamInfo)
        {
            throw except::Exception(Ctxt("Error reading code stream."));
        }

        OPJ_UINT32 tileWidth = codeStreamInfo->tdx;
        OPJ_UINT32 tileHeight = codeStreamInfo->tdy;

        // sanity checking 
        if (!mImage)
        {
           throw except::Exception(Ctxt("NULL image after reading header"));
        }

        if (mImage->x0 >= mImage->x1 || mImage->y0 >= mImage->y1)
        {
            throw except::Exception(Ctxt("Invalid image offsets"));
        }

        if (mImage->numcomps == 0)
        {
            throw except::Exception(Ctxt("No image components found"));
        }

        // TODO: We need special handling that's not implemented in readTile() to
        //       accommodate partial tiles with more than one band.
        OPJ_UINT32 imageWidth = mImage->x1 - mImage->x0;
        OPJ_UINT32 imageHeight = mImage->y1 - mImage->y0;
        if (mImage->numcomps > 1 &&
            (imageWidth % tileWidth != 0 || imageHeight % tileHeight != 0))
        {
           // throw except::NotImplementedException(Ctxt(
           //     "Partial tiles not implemented in multi-band imagery"));
        }


        // loop over the available image components and add to header.
        std::vector<ImageComponent> components;
            for(OPJ_UINT32 idx = 0; idx < mImage->numcomps; ++idx)
            {
                opj_image_comp_t cmp = mImage->comps[idx];
                
                ImageComponent ic;
                ic.dx = cmp.dx;
                ic.dy = cmp.dy;
                ic.h = cmp.h;
                ic.w = cmp.w;
                ic.x0 = cmp.x0;
                ic.y0 = cmp.y0;
                ic.prec = cmp.prec;
                ic.bpp = cmp.bpp;
                ic.sgnd = cmp.sgnd > 0;
                ic.resno_decoded = cmp.resno_decoded;
                ic.factor = cmp.factor;
                //ic.data = cmp.data;
                
                components.push_back(ic);
            }

            sys::Int32_T colorSpace = mImage->color_space; //Const::ColorSpace::UNSPECIFIED;

            mHeader = new Header(mImage->x1 - mImage->x0,
                mImage->y1 - mImage->y0,
                components,
                tileWidth, tileHeight, colorSpace);

        {
            opj_destroy_cstr_info(&codeStreamInfo);

            //openJPEG_cleanup(&stream, &image);
        }
    }

sys::Uint64_T ImageReaderOpenJPEG::readTile(sys::Uint32_T tileX, sys::Uint32_T tileY,
        sys::ubyte **buf)
{
    if (!mHeader)
        readHeader();

    OPJ_UINT32 bufSize; // looks like the opj_read_tile_header() won't support 64-bit buffer sizes
    const OPJ_UINT32 tileWidth = mHeader->getTileWidth();
    const OPJ_UINT32 tileHeight = mHeader->getTileHeight();
    size_t numBitsPerPixel = 0;
    size_t numBytesPerPixel = 0;
    sys::Uint64_T fullBufSize = 0;

    // only decode what we want
    if (!opj_set_decode_area(mCodec, mImage, tileWidth * tileX, tileHeight * tileY,
                             tileWidth * (tileX + 1), tileHeight * (tileY + 1)))
    {
        throw except::Exception(Ctxt("Error decoding area"));
    }

    {
        int keepGoing;
        OPJ_UINT32 tileIndex, nComponents;
        OPJ_INT32 tileX0, tileY0, tileX1, tileY1;

        if (!opj_read_tile_header(mCodec, mStream, &tileIndex, &bufSize, &tileX0,
                                  &tileY0, &tileX1, &tileY1, &nComponents,
                                  &keepGoing))
        {
            /*nrt_Error_init(error, "Error reading tile header", NRT_CTXT,
              NRT_ERR_UNK);*/
            //goto CATCH_ERROR;
        }        

        if (keepGoing)
        {
            /* TODO: The way blockIO->cntl->blockOffsetInc is currently
             *       implemented in ImageIO.c corresponds with how a
             *       non-compressed partial block would be laid out in a
             *       NITF - the actual extra columns would have been read.
             *       Not sure how the J2K data is laid out on disk but
             *       OpenJPEG is hiding this from us if the extra columns are
             *       present there.  So whenever we get a partial tile that
             *       isn't at the full width, we need to add in these extra
             *       columns of 0's ourselves.  Potentially we could update
             *       ImageIO.c to not require this instead.  Note that we
             *       don't need to pad out the extra rows for a partial block
             *       that isn't the full height because ImageIO will never try
             *       to memcpy these in - we only need to get the stride to
             *       work out correctly.
             */
            const OPJ_UINT32 thisTileWidth = tileX1 - tileX0;
            const OPJ_UINT32 thisTileHeight = tileY1 - tileY0;
            if (thisTileWidth < tileWidth)
            {
                /* TODO: The current approach below only works for single band
                 *       imagery.  For RGB data, I believe it is stored as all
                 *       red, then all green, then all blue, so we would need
                 *       a temp buffer rather than reusing the current buffer.
                 */
                if (nComponents != 1)
                {
                    throw except::NotImplementedException(Ctxt("Partial tile width not implemented for multi-band"));
                }

                // CC: i'm surprised there is a bpp variable in the ImageComponent that
                //     seems to be 0, and that the prec variable has the bits per pixel...
                numBitsPerPixel = mHeader->getComponents()[0].prec;
                numBytesPerPixel =
                    (numBitsPerPixel / 8) + (numBitsPerPixel % 8 != 0);
                fullBufSize = tileWidth * thisTileHeight * numBytesPerPixel;
            }
            else
            {
                fullBufSize = bufSize;
            }

            if (buf && !*buf)
            {
                *buf = new sys::ubyte[fullBufSize];
                if (!*buf)
                {
                    throw except::OutOfMemoryException(Ctxt("Couldn't allocate enough memory for the image buffer"));
                }
            }

            if (!opj_decode_tile_data(mCodec, tileIndex, *buf, bufSize, mStream))
            {
                /*nrt_Error_init(error, "Error decoding tile", NRT_CTXT,
                  NRT_ERR_UNK);*/
                //goto CATCH_ERROR;
            }

            if (thisTileWidth < tileWidth)
            {
                /* We have a tile that isn't as wide as it "should" be
                 * Need to add in the extra columns ourselves.  By marching
                 * through the rows backwards, we can do this in place.
                 */
                const size_t srcStride = thisTileWidth * numBytesPerPixel;
                const size_t destStride = tileWidth * numBytesPerPixel;
                const size_t numLeftoverBytes = destStride - srcStride;
                OPJ_UINT32 lastRow = thisTileHeight - 1;
                size_t srcOffset = lastRow * srcStride;
                size_t destOffset = lastRow * destStride;
                OPJ_UINT32 ii;
                sys::ubyte* bufPtr = *buf;

                for (ii = 0;
                     ii < thisTileHeight;
                     ++ii, srcOffset -= srcStride, destOffset -= destStride)
                {
                    sys::ubyte* const dest = bufPtr + destOffset;
                    memmove(dest, bufPtr + srcOffset, srcStride);
                    memset(dest + srcStride, 0, numLeftoverBytes);
                }
            }
        }
    }

    return fullBufSize;
}


sys::Uint64_T ImageReaderOpenJPEG::readRegion(sys::Uint32_T x0, sys::Uint32_T y0,
        sys::Uint32_T x1, sys::Uint32_T y1, sys::ubyte **buf)
{
    sys::Uint64_T bufSize;
    sys::Uint64_T offset = 0;
    sys::Uint32_T componentBytes, nComponents;

    if (!mHeader)
        readHeader();

    // if the stop coordinates weren't specified, read the whole image
    if (x1 == 0)
        x1 = mHeader->getImageWidth();
    if (y1 == 0)
        y1 = mHeader->getImageHeight();

    // only decode what we want
    if (!opj_set_decode_area(mCodec, mImage, x0, y0, x1, y1))
    {
        throw except::Exception(Ctxt("Error decoding area"));
    }

    nComponents = mHeader->getNumComponents();
    componentBytes = (mHeader->getComponents()[0].prec - 1) / 8 + 1;
    bufSize = (sys::Uint64_T)(x1 - x0) * (y1 - y0) * componentBytes * nComponents;
    if (buf && !*buf)
    {
        *buf = new sys::ubyte[bufSize];
        if (!*buf)
        {
            throw except::OutOfMemoryException(Ctxt("Couldn't allocate enough memory for the image buffer"));
        }
    }

    {
        int keepGoing;
        OPJ_UINT32 tileIndex, reqSize;
        OPJ_INT32 tileX0, tileY0, tileX1, tileY1;

        do
        {
            if (!opj_read_tile_header(mCodec, mStream, &tileIndex, &reqSize, &tileX0,
                                      &tileY0, &tileX1, &tileY1, &nComponents,
                                      &keepGoing))
            {
                throw except::Exception(Ctxt("Error reading tile header"));
            }

            if (keepGoing)
            {
                if (!opj_decode_tile_data(mCodec, tileIndex, (*buf + offset),
                                          reqSize, mStream))
                {
                    throw except::Exception(Ctxt("Error decoding tile"));
                }
                offset += reqSize;
            }
        }
        while (keepGoing);
    }

    return bufSize;
}

} // end namespace j2k

#endif
