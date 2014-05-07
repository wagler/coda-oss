

#ifndef __J2K_OPENJPEG_IMAGE_WRITER_H__
#define __J2K_OPENJPEG_IMAGE_WRITER_H__

#if defined (USE_OPENJPEG)

#include <cstdio> // needed for openjpeg.h

//extern "C" {
    #include <openjpeg.h>
//}

#include "j2k/ImageWriterInterface.h"



namespace j2k
{
	
	class ImageWriterOpenJPEG : public ImageWriterInterface
	{
	public:
		explicit ImageWriterOpenJPEG(io::SeekableOutputStream& output, Header header);
		virtual ~ImageWriterOpenJPEG();

        void setTile(sys::Uint32_T tileX, sys::Uint32_T tileY,
            sys::ubyte **buf, sys::Uint32_T bufSize);

        //sys::Uint64_T readRegion(sys::Uint32_T x0, sys::Uint32_T y0,
        //    sys::Uint32_T x1, sys::Uint32_T y1, sys::ubyte **buf);

        void write();

	private:
		void openJPEG_Setup();
        void openJPEG_CreateIO();
		void readHeader();
        
        // actually used for writer
        void initImage();

		// member variables
		//io::InputStream& mInputStream;
		//size_t offset;
		ImageWriterOpenJPEG();

		// openJPEG library member structs
		opj_codec_t* mCodec;
        opj_stream_t *mStream;
        opj_image_t *mImage;

		bool mOpenJPEG_Initialized;

		// member variables from OpenJPEGReaderImpl
		//opj_dparameters_t mDecodingParameters;
		sys::Off_T ioOffset;

        // const vars
        static const size_t OPENJPEG_STREAM_SIZE = 1024;

        // buffer to hold the compressed image data
        io::ByteStream* mCompressedStream;
    
   
   // j2k_Container *container;


	

	};

}

#endif // USE_OPENJPEG

#endif // __J2K_OPENJPEG_IMAGE_WRITER_H__