/* =========================================================================
 * This file is part of j2k-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2011, General Dynamics - Advanced Information Systems
 *
 * tiff-c++ is free software; you can redistribute it and/or modify
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

#include <import/except.h>
#include <import/io.h>
#include <import/sys.h>
#include <import/j2k.h>

int main(int argc, char **argv)
{
    try
    {
        if (argc < 2)
            throw except::Exception(FmtX("usage: %s <j2k file>", argv[0]));

        sys::OS os;
        std::string path = sys::Path::absolutePath(argv[1]);
        if (!os.exists(path))
            throw except::FileNotFoundException(path);

        // read in the JPEG2000 file header
        io::StandardOutStream outStream;
        io::FileInputStream inStream(path);
        j2k::ImageReader reader(inStream);
        
        // print some image/header information
        reader.print(outStream);
        j2k::Header* header = reader.getHeader();

        // allocate a buffer for the uncompressed image content
        size_t bufSize = reader.getUncompressedSizeInBytes();
        sys::ubyte* buffer = new sys::ubyte[bufSize];


        // read the actual image
        reader.readRegion(0,0,0,0, &buffer);

        
        // set up the writer
        sys::Path::StringPair outPair = sys::Path::splitExt(path);
        std::string outPath = outPair.first + "_out" + outPair.second;
        io::FileOutputStream outFile(outPath);

        // build the header
        j2k::Header outHeader = *header;

        j2k::ImageWriter writer(outFile, outHeader);

        // change some encoding parameters
        writer.setCompressionRatio(20);
        
        // compress the image
        writer.setTile(0, 0, &buffer, bufSize);

        // write to output stream
        writer.write();

        delete [] buffer;
    }
    catch (except::Throwable& t)
    {
        std::cerr << t.toString() << std::endl;
        exit( EXIT_FAILURE);
    }
    catch (...)
    {
        std::cerr << "Caught unnamed exception" << std::endl;
        exit( EXIT_FAILURE);
    }
    return 0;
}
