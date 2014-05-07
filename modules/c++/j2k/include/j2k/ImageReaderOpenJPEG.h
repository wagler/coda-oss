

#ifndef __J2K_OPENJPEG_IMAGE_READER_H__
#define __J2K_OPENJPEG_IMAGE_READER_H__

#if defined (USE_OPENJPEG)

#include <cstdio> // needed for openjpeg.h

//extern "C" {
    #include <openjpeg.h>
//}

#include "j2k/ImageReaderInterface.h"



namespace j2k
{
	
	class ImageReaderOpenJPEG : public ImageReaderInterface
	{
	public:
		explicit ImageReaderOpenJPEG(io::SeekableInputStream& input);
		virtual ~ImageReaderOpenJPEG();

        sys::Uint64_T readTile(sys::Uint32_T tileX, sys::Uint32_T tileY,
        sys::ubyte **buf);

        sys::Uint64_T readRegion(sys::Uint32_T x0, sys::Uint32_T y0,
            sys::Uint32_T x1, sys::Uint32_T y1, sys::ubyte **buf);

	private:
		void openJPEG_Setup();
        void openJPEG_CreateIO();
		void readHeader();

		// member variables
		//io::InputStream& mInputStream;
		//size_t offset;
		ImageReaderOpenJPEG();

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

        
    
   
   // j2k_Container *container;


	

	};

}

#endif // USE_OPENJPEG

#endif // __J2K_OPENJPEG_IMAGE_READER_H__