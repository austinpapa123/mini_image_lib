### Mini Image Library in C++

currently only supports image files in PPM format(raw form, no extra bytes, even comments)

A .ppm file in P6 format stores:

A text-based header:<br />
P6<br />
width height<br />
255<br /><br />

Followed by raw binary RGB data:<br />
R1 G1 B1  R2 G2 B2  R3 G3 B3  ...<br />
Each pixel takes up 3 bytes — one each for Red, Green, and Blue, in that exact order.<br /><br />

So, for a 2×2 image:

Pixels:      R     G     B<br />
Pixel 1:     255   0     0      // Red<br />
Pixel 2:     0     255   0      // Green<br />
Pixel 3:     0     0     255    // Blue<br />
Pixel 4:     255   255   255    // White<br /><br />

This results in:

std::vector<uint8_t> data = {<br />
    255, 0, 0,     // Pixel 0 (Top-Left)<br />
    0, 255, 0,     // Pixel 1 (Top-Right)<br />
    0, 0, 255,     // Pixel 2 (Bottom-Left)<br />
    255, 255, 255  // Pixel 3 (Bottom-Right)<br />
};

So, if the image is w × h, then:

data.size() = width * height * 3

/assets folder contains RGB color blocks for testing<br />
2x2 "test_input.ppm", <br />
9x9 "test_input_9x9.ppm", <br />
32x32 "sobel_test_32x32.ppm"<br />

compile and run the program using the following command(g++) or using make command
1. 'g++ -std=c++11 -Wall -Iinclude -o app.exe src/image.cpp main.cpp'
2. './app' or ".\app.exe" in win
3. output files will be under /output

- Using FileHelper.com to test out the PPM file

#### Features
- Grayscale 
- Invert Colors
- Box Blur
- Sobel Edge Detection (use sobel_test_32x32.ppm under /assets to test)



