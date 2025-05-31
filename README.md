### Mini Image Library in C++

currently only supports image files in PPM format

A .ppm file in P6 format stores:

A text-based header:\n
P6\n
width height\n
255\n
Followed by raw binary RGB data:\n\n

R1 G1 B1  R2 G2 B2  R3 G3 B3  ...\n
Each pixel takes up 3 bytes — one each for Red, Green, and Blue, in that exact order.\n\n

So, for a 2×2 image:\n

Pixels:      R     G     B\n
Pixel 1:     255   0     0      // Red\n
Pixel 2:     0     255   0      // Green\n
Pixel 3:     0     0     255    // Blue\n
Pixel 4:     255   255   255    // White\n\n

This results in:\n

std::vector<uint8_t> data = {\n
    255, 0, 0,     // Pixel 0 (Top-Left)\n
    0, 255, 0,     // Pixel 1 (Top-Right)\n
    0, 0, 255,     // Pixel 2 (Bottom-Left)\n
    255, 255, 255  // Pixel 3 (Bottom-Right)\n
};\n\n

So, if the image is w × h, then:\n

data.size() = width * height * 3\n\n\n

/assets folder contains a 2x2 RGB color block "test_input.ppm"

compile and run the program using the following command(g++) or using make command\n
1. 'g++ -std=c++11 -Wall -Iinclude -o app.exe src/image.cpp main.cpp'
2. './app' or ".\app.exe" in win
3. output file will be under /output

- Using FileHelper.com to test out the PPM file

#### Features
- Grayscale 
- Invert Colors
- Box Blur



