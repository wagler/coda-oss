#ifndef __ZIP_ZIP_FILE_H__
#define __ZIP_ZIP_FILE_H__


#include "zip/ZipEntry.h"

/*!
 *  This file is loosely based on libzipfile, from Google's Android OS,
 *  which is open source under the Apache license, from the open source
 *  Java SDK, and from Minizip, which is part of the zlib contrib section.
 *
 *  All of those sources are presumably based on Mark Adler's infozip code
 *  and from zlib.
 *
 *  A note about in/deflation
 *
 *  inflation - decompress
 *  deflation - compression
 */

namespace zip
{
    

    /*!
     *  \class ZipFile
     *  \brief Contains the functionality for reading PKZIP files
     *
     */

    class ZipFile
    {

	//!  This is the container for ZipEntry objects
	std::vector<ZipEntry*> mEntries;

	//!  Zip (apparently) is little-endian
	bool mSwapBytes;

	//!  Compressed data buffer (yes we eat the whole file)
	sys::ubyte* mCompressed;

	sys::Size_T mCompressedLength;
	
	//! More meta-data that isnt used
	unsigned int mCentralDirSize;


	unsigned int mCentralDirOffset;

	std::string mComment;

	//!  Read an integer (little-endian)
	unsigned int readInt(sys::ubyte* buf);

	//!  Read a short (little-endian)
	unsigned short readShort(sys::ubyte* buf);

	//!  Read the top-level zip directory
	void readCentralDir();


	//!  Get the ZipEntry for some element
	ZipEntry* newCentralDirEntry(sys::ubyte** p, sys::SSize_T len);

	//!  Get information for the central dir
	void readCentralDirValues(sys::ubyte* buf, sys::SSize_T len);

	//!  Copy to a string
	//void copyString(const sys::ubyte* buf, sys::SSize_T len);

    public:

	//!  Provide iterator access to the ZipEntry objects
	typedef std::vector<ZipEntry*>::const_iterator Iterator;


	/*!
	 *  We require an input stream for initialization
	 *  This stream should be already initialized, since we
	 *  are planning on reading from it immediately
	 */
	ZipFile(io::InputStream* inputStream) :
	    mCompressed(NULL)
	{
	    mSwapBytes = sys::isBigEndianSystem();
	    mCompressedLength = inputStream->available();
	    mCompressed = new sys::ubyte[mCompressedLength];
	    inputStream->read((sys::byte*)mCompressed, mCompressedLength);
	    
	    readCentralDir();
	}

	/*!
	 *  When the ZipFile object goes out of scope, that
	 *  means its time to delete all of our entries.
	 *
	 *  We do not delete the source InputStream
	 */
	~ZipFile();

	//!  Look for a ZipEntry with the same fileName
	Iterator lookup(std::string fileName) const;

	//!  Iterator to beginning of collection
	Iterator begin() const
	{
	    return mEntries.begin();
	}

	//!  Iterator to end of collection
	Iterator end() const
	{
	    return mEntries.end();
	}

	std::string getComment() const { return mComment; }
    };


}

/*!
 *  Output stream overload
 */
std::ostream& operator<<(std::ostream& os, const zip::ZipFile& zf);

#endif
