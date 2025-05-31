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

