#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <cstdint>


struct RGB {
    uint8_t r, g, b;
    RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : r(r), g(g), b(b) {}
};


class Image {
public:
    int width = 0, height = 0;
    std::vector<uint8_t> data; // RGB data, 1D array of interleaved RGB triplets

    Image() {}

    //Constructor for canvas initialization
    Image(int width, int height) {
        this->width = width;
        this->height = height;
        data.resize(width * height * 3); // allocate RGB space
    }

    

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


    /**
     * Sobel Edge Detection
     * 
     * Highlights edges in an image by detecting sudden changes in brightness
     * 
     * We perform grayscale first then sobel kernels calculating result
     */
    void sobel_edge_detect();


    /**
     * brighten or darken the image, simply increase or decrease a 
     * certain amount from the RGB channels, remember to clamp to avoid overlow
     * 
     */
    void add_brightness(int amount);  // amount can be negative to darken



    // draw pixel on Image canvas
    void draw_pixel(int x, int y, RGB color);


    //Bresenham's Line Algorithm ,, draw line on canvas
    void draw_line(int x0, int y0, int x1, int y1, RGB color);


    //Bresenham's Circle Algorithm / Midpoint Circle Algorithm  ,, draws an unfilled circle
    void draw_circle(int cx, int cy, int radius, RGB color);


};

#endif // IMAGE_H
