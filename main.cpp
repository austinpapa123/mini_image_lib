#include "image.h"

int main() {
    //Image img;
    //if (!img.load("assets/test_input.ppm")) return 1;
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

    //img.add_brightness(10);
    //img.save("output/brightened.ppm");

    //Image img(128, 128);
    //img.draw_line(0, 0, 127, 127, RGB(255, 0, 0));     // Red diagonal
    //img.draw_line(0, 127, 127, 0, RGB(0, 255, 0));     // Green diagonal
    //img.save("output/drawn.ppm");


    Image img(128, 128);
    // Draw red circle in the center
    img.draw_circle(64, 64, 30, RGB(255, 0, 0));
    // Optional: draw inner green circle
    img.draw_circle(64, 64, 20, RGB(0, 255, 0));
    img.save("output/circle.ppm");


    return 0;
}
