#include "image.h"
#include <fstream>
#include <iostream>


bool Image::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
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
    return true;
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
    auto get = [&](int x, int y, int c) -> uint8_t {  //[&] is the capture clause, so taht we can use width,height,original inside without passing them explicitly
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


