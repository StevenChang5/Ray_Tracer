#include "color.h"
#include "vec3.h"

#include <iostream>

int main(){
    int IMG_WIDTH = 256;
    int IMG_HEIGHT = 256;

    std::cout << "P3\n" << IMG_WIDTH << ' ' << IMG_HEIGHT << "\n255\n";

    for(int j = 0; j < IMG_HEIGHT; j++){
        std::clog << "\rScanlines remaining: " << (IMG_HEIGHT - j) << ' ' << std::flush;
        for(int i = 0; i < IMG_WIDTH; i++){
            auto pixel_color = color(double(i)/(IMG_WIDTH-1), double(j)/(IMG_HEIGHT-1),0);
            write_color(std::cout,pixel_color);
        }
    }
    std::clog << "\rDone.           \n";
}