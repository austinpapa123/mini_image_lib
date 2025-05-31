#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <cstdint>

class Image {
public:
    int width = 0, height = 0;
    std::vector<uint8_t> data; // RGB data, 1D array of interleaved RGB triplets

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



    /*
    Turn every pixel into its opposite color
    new_R = 255 - R
    new_G = 255 - G
    new_B = 255 - B
    */
    void invert_colors();


    /*
    RGB averaging
    For each pixel, replaces its value with the average of its neighbors
    makes images appear softer, like out-of-focus or smoothed
    */
    void box_blur();
};

#endif // IMAGE_H
