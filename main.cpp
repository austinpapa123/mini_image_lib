#include "image.h"

int main() {
    Image img;
    if (!img.load("assets/test_input.ppm")) return 1;
    //if (!img.load("assets/test_input_9x9.ppm")) return 1;
    //if (!img.load("assets/sobel_test_32x32.ppm")) return 1;

    //img.to_grayscale();
    //img.save("output/gray.ppm");

    //img.invert_colors();
    //img.save("output/inverted.ppm");

    //img.box_blur();
    //img.save("output/blurred.ppm");

    //img.sobel_edge_detect();
    //img.save("output/edges.ppm");

    img.add_brightness(10);
    img.save("output/brightened.ppm");


    return 0;
}
