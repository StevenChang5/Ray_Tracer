#include <iostream>

int main(){
    int IMG_WIDTH = 256;
    int IMG_HEIGHT = 256;

    std::cout << "P3\n" << IMG_WIDTH << ' ' << IMG_HEIGHT << "\n255\n";

    for(int j = 0; j < IMG_HEIGHT; j++){
        for(int i = 0; i < IMG_WIDTH; i++){
            auto r = double(i)/(IMG_WIDTH-1);
            auto g = double(j)/(IMG_HEIGHT-1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            std:: cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}