#include "image.h"
#include <fstream>
#include <iostream>
#include <cmath> // for sqrt

//File size check, if raw format without any extra bytes even for commenting
void debug_file_size(std::ifstream& file, int width, int height);




bool Image::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary); //open file
    if (!file) return false;

    std::string header;
    file >> header; //retrieve header
    if (header != "P6") return false;  //if not P6(binary RGB), we reject the file

    file >> width >> height; //read dimensions
    int max_val;
    file >> max_val; //retrieve maximum color value(should be 255)
    file.get(); // consume the newline character after max_val

    data.resize(width * height * 3); //allocate space for all pixel data (3 bytes per pixel: R, G, B)
    
    //"reinterpret_cast<char*>"" tells C++ to treat uint8_t* as a char*, which read() expects
    file.read(reinterpret_cast<char*>(data.data()), data.size());  //Read the binary data into 'data'
    
    debug_file_size(file, width, height);

    return true;
}



void debug_file_size(std::ifstream& file, int width, int height) {
    std::streampos current = file.tellg();  // where we are in file
    file.seekg(0, std::ios::end);   // move to end of file
    std::streampos total = file.tellg();   // get total size
    file.seekg(current);   // return to previous read position

    std::cout << "Input file size: " << total << " bytes\n";
    std::cout << "Expected: " << (width * height * 3) << " bytes + header text bytes\n";
}



bool Image::save(const std::string& filename) {
    //open output file for writing in binary mode
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;

    file << "P6\n" << width << " " << height << "\n255\n"; //write the correct header for P6 format

    file.write(reinterpret_cast<char*>(data.data()), data.size()); //Write all the RGB binary data to the file

    
    return true;
}



void Image::to_grayscale() {
    for (int i = 0; i < width * height; ++i) {
        int idx = i * 3;
        uint8_t r = data[idx];
        uint8_t g = data[idx + 1];
        uint8_t b = data[idx + 2];

        //"static_cast<uint8_t>"" here explicitly converts expression output of double/float to uint8_t
        uint8_t gray = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
        data[idx] = data[idx + 1] = data[idx + 2] = gray;
    }
}



void Image::invert_colors() {
    for (int i = 0; i < width * height * 3; ++i) {
        data[i] = 255 - data[i];
    }
}



void Image::box_blur() {
    std::vector<uint8_t> original = data;

    /**
     * This function gets a pixel's R/G/B value
     * It handles edges, so (x+dx) and (y+dy) doesn't go out-of-bounds
     */
    auto get = [&](int x, int y, int c) -> uint8_t {  //[&] is the capture clause, so that we can use width,height,original inside without passing them explicitly
        // clamp edges (ensures not out-of-bound)
        x = std::max(0, std::min(width - 1, x));  //if x=-1, clamp to 0, if x=width, clamp to width-1
        y = std::max(0, std::min(height - 1, y));  //same as above
        
        /*
        * "y*width+x" gives the pixel index
        * "*3" accounts for 3 color channels per pixel
        * "+c" indices 0 for R, 1 for G, 2 for B
        */ 
        return original[(y * width + x) * 3 + c];
    };

    for (int y = 0; y < height; ++y) {  //for every row
        for (int x = 0; x < width; ++x) { //for every column

            /*
             Each RGB channel is blurred independently
             R channels are averaged with R values
             G with G
             B with B

            pseudocode:
            
             for (int c = 0; c < 3; ++c) {
                sum = sum of channel c for 9 surrounding pixels
                result = sum / 9
                assign to pixel's channel c
            }

            */
            for (int c = 0; c < 3; ++c) { // for each color channel (R,G,B)
                int sum = 0;
                for (int dy = -1; dy <= 1; ++dy) { // for 3x3 neighborhood rows
                    for (int dx = -1; dx <= 1; ++dx) {  // for 3x3 neighborhood cols
                        // sum up the surrounding 3x3 neighborhood values
                        sum += get(x + dx, y + dy, c);
                    }
                }
                data[(y * width + x) * 3 + c] = static_cast<uint8_t>(sum / 9); //average to get the output
            }
        }
    }
}





void Image::sobel_edge_detect() {
    // Step 1: Convert to grayscale first, one channel only no need to deal with R,G,B separately
    std::vector<uint8_t> gray(width * height);
    for (int i = 0; i < width * height; ++i) {
        int idx = i * 3;
        uint8_t r = data[idx], g = data[idx + 1], b = data[idx + 2];
        gray[i] = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
    }

    // Step 2: Prepare output buffer
    std::vector<uint8_t> edges(width * height);

    // Step 3: Define Sobel kernels, left-right contrast / top-bottom contrast
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    // Step 4: Apply Sobel
    
    //Looping Order y -> x is conventional in CV and mathces memory layout 'data[(y * width + x) * 3 + c]'
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int sumX = 0;
            int sumY = 0;

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int pixel = gray[(y + dy) * width + (x + dx)];
                    sumX += Gx[dy + 1][dx + 1] * pixel;
                    sumY += Gy[dy + 1][dx + 1] * pixel;
                }
            }
            //true magnitude = sqrt(Gx^2 + Gy^2), sqrt() is expensive plus abs(a) + abs(b) is a fast approximation of sqrt(a² + b²)
            //int magnitude = std::min(255, static_cast<int>(std::sqrt(sumX * sumX + sumY * sumY)));
            int magnitude = std::min(255, std::abs(sumX) + std::abs(sumY));
            edges[y * width + x] = static_cast<uint8_t>(magnitude);
        }
    }

    // Step 5: Convert to RGB output (grayscale output to R=G=B)
    for (int i = 0; i < width * height; ++i) {
        data[i * 3 + 0] = edges[i];
        data[i * 3 + 1] = edges[i];
        data[i * 3 + 2] = edges[i];
    }    
}


//used int instead of uint8_t to avoid temporary overflow
void Image::add_brightness(int amount) {
    for (int i = 0; i < width * height * 3; ++i) {
        int value = static_cast<int>(data[i]) + amount;
        data[i] = static_cast<uint8_t>(std::min(255, std::max(0, value)));
    }
}



void Image::draw_pixel(int x, int y, RGB color) {
    if (x < 0 || x >= width || y < 0 || y >= height) return; // bounds check
    int idx = (y * width + x) * 3;
    data[idx] = color.r;
    data[idx + 1] = color.g;
    data[idx + 2] = color.b;
}



void Image::draw_line(int x0, int y0, int x1, int y1, RGB color) {
    int dx = std::abs(x1 - x0), dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        draw_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
    }
}



void Image::draw_circle(int cx, int cy, int radius, RGB color) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    auto plot_circle_points = [&](int xc, int yc, int x, int y) {
        draw_pixel(xc + x, yc + y, color);
        draw_pixel(xc - x, yc + y, color);
        draw_pixel(xc + x, yc - y, color);
        draw_pixel(xc - x, yc - y, color);
        draw_pixel(xc + y, yc + x, color);
        draw_pixel(xc - y, yc + x, color);
        draw_pixel(xc + y, yc - x, color);
        draw_pixel(xc - y, yc - x, color);
    };

    while (y >= x) {
        plot_circle_points(cx, cy, x, y);
        ++x;
        if (d > 0) {
            --y;
            d += 4 * (x - y) + 10;
        } else {
            d += 4 * x + 6;
        }
    }
}
