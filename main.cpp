#include "image.h"

int main() {
    Image img;
    if (!img.load("assets/test_input.ppm")) return 1;

    img.to_grayscale();
    img.save("output/gray.ppm");

    return 0;
}
