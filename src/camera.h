#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera{
    public:
        double ASPECT_RATIO = 1.0;
        int IMG_WIDTH = 100;

        void render(const hittable& world){
            initialize();

            std::cout << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n255\n";

            for(int j = 0; j < IMG_HEIGHT; j++){
                std::clog << "\rScanlines remaining: " << (IMG_HEIGHT - j) << ' ' << std::flush;
                for(int i = 0; i < IMG_WIDTH; i++){
                    auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                    auto ray_direction = pixel_center - center;
                    ray r(center, ray_direction);

                    color pixel_color = ray_color(r, world);
                    write_color(std::cout,pixel_color);
                }
            }
            std::clog << "\rDone.           \n";
        }
    private:
    int     IMG_HEIGHT;     // Rendered image height
    point3  center;         // Camera Center
    point3  pixel00_loc;    // Location of pixel (0,0)
    vec3    pixel_delta_u;  // Offset of pixel to the right
    vec3    pixel_delta_v;  // Offset of pixel below
        void initialize(){
            IMG_HEIGHT = int(IMG_WIDTH/ASPECT_RATIO);
            IMG_HEIGHT = (IMG_HEIGHT<1) ? 1 : IMG_HEIGHT;

            center = point3(0,0,0);

            auto focal_length = 1.0;
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (double(IMG_WIDTH)/IMG_HEIGHT);

            // Calculate vectors across the horizontal/down the vertical viewport edges
            auto viewport_u = vec3(viewport_width,0,0);
            auto viewport_v = vec3(0,-viewport_height,0);

            // Calculate the horizontal/vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / IMG_WIDTH;
            pixel_delta_v = viewport_v / IMG_HEIGHT;

            // Location of upper left pixel
            auto viewport_upper_left = 
                center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            
        }
        color ray_color(const ray& r, const hittable& world) const{
            hit_record rec;
            if(world.hit(r, interval(0, infinity), rec)){
                return 0.5 * (rec.normal + color(1,1,1));
            }
            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            // Linear combination of white and blue 
            return (1.0-a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
        }
};

#endif