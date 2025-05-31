#include "image.h"

int main() {
    Image img;
    if (!img.load("assets/test_input_9x9.ppm")) return 1;

    img.to_grayscale();
    img.save("output/gray.ppm");

    img.invert_colors();
    img.save("output/inverted.ppm");

    img.box_blur();
    img.save("output/blurred.ppm");


    return 0;
}
