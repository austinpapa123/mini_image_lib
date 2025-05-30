#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <cstdint>

class Image {
public:
    int width = 0, height = 0;
    std::vector<uint8_t> data; // RGB data

    //reads a binary PPM file
    bool load(const std::string& filename);

    //writes a binary PPM file
    bool save(const std::string& filename);

    /*
    Make each pixel's R, G, B values equal using brightness (luminance)

    To mimic human perception we set
    
    🔴 Red → 29.9% 🟢 Green → 58.7% 🔵 Blue → 11.4%
    gray = 0.299 * R + 0.587 * G + 0.114 * B; (luma (Y') formula, defined by the ITU-R BT.601 standard for TV signals.)
    */
    void to_grayscale();
};

#endif // IMAGE_H
