#include "ImfRgba.h"
#include "ImfRgbaFile.h"

#include <ImfInputFile.h>
#include <ImfOutputFile.h>
#include <ImfChannelList.h>
#include <ImfStringAttribute.h>
#include <ImfFrameBuffer.h>

#include <cassert>
#include <string>

// Function copied (with minior modifications) from pbrt-v3 (https://github.com/mmp/pbrt-v3) which is under BSD-2-Clause License
void write_image_exr(const std::string &name, const float *pixels,
                          int xRes, int yRes, int totalXRes, int totalYRes,
                          int xOffset, int yOffset) {
    using namespace Imf;
    using namespace Imath;

    Rgba *hrgba = new Rgba[xRes * yRes];
    for (int i = 0; i < xRes * yRes; ++i)
        hrgba[i] = Rgba(pixels[3 * i], pixels[3 * i + 1], pixels[3 * i + 2]);

    // OpenEXR uses inclusive pixel bounds.
    Box2i displayWindow(V2i(0, 0), V2i(totalXRes - 1, totalYRes - 1));
    Box2i dataWindow(V2i(xOffset, yOffset),
                     V2i(xOffset + xRes - 1, yOffset + yRes - 1));

    try {
        RgbaOutputFile file(name.c_str(), displayWindow, dataWindow,
                            WRITE_RGB);
        file.setFrameBuffer(hrgba - xOffset - yOffset * xRes, 1, xRes);
        file.writePixels(yRes);
    } catch (const std::exception &exc) {
        throw std::runtime_error("Error writing");
    }

    delete[] hrgba;
}

int main() {
    // write open exr image
    {
        float data [3*100*100];

        for(int y = 0; y < 100; ++y)
            for(int x = 0; x < 100; ++x) {
                data[(y*100+x)*3+0] = 0.f; 
                data[(y*100+x)*3+1] = 1.f; 
                data[(y*100+x)*3+2] = 0.f; 
            }

        write_image_exr("test.exr", data, 100, 100, 100, 100, 0, 0);
    }

    // read open exr image
    {
        std::string filename = "test.exr";

        Imf::InputFile exrIn(filename.c_str());
        const Imf::Header& exrHeader = exrIn.header();
        const Imath::Box2i &exrDataWindow = exrHeader.dataWindow();
        auto width = exrDataWindow.max.x - exrDataWindow.min.x + 1;
        auto height = exrDataWindow.max.y - exrDataWindow.min.y + 1;
        
        std::cout << "width: " << width << " height: " << height << std::endl;
        
        const Imf::ChannelList &exrChannels = exrHeader.channels();

        for (Imf::ChannelList::ConstIterator channel = exrChannels.begin();
                channel != exrChannels.end();
                channel++) {
            std::cout << channel.name() << std::endl;
        }
    }

    return 0;
}