#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r){
    // Hit sphere (anywhere on sphere's surface) calculated as 
    //      radius^2 = dot product(center-xyz_position)
    //      If radius^2 == dot product(c-xyz), then xyz is on the sphere
    // Given ray from camera, -b - sqrt(discriminant) / (2*a) gives
    //      the position on the ray where it hits the sphere
    vec3 oc = center - r.origin();

    // Non-simplified
    // auto a = dot(r.direction(), r.direction());
    // auto b = -2.0 * dot(r.direction(), oc);
    // auto c = dot(oc, oc) - radius * radius;
    // auto discriminant = b*b - 4*a*c;

    // Simplified
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = h*h - a*c;

    if(discriminant < 0){
        return -1.0;
    }else{
        // Non-simplified
        // return (-b - std::sqrt(discriminant) / (2.0*a));

        // Simplified
        return (h - std::sqrt(discriminant)) / a;
    }
}

color ray_color(const ray& r){
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if(t > 0.0){
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    // Linear combination of white and blue 
    return (1.0-a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
}

int main(){
    auto ASPECT_RATIO = 16.0/9.0;
    int IMG_WIDTH = 400;

    // Calculate image height based on aspect ratio, ensure that it's at least 1.
    int IMG_HEIGHT = int(IMG_WIDTH/ASPECT_RATIO);
    IMG_HEIGHT = (IMG_HEIGHT<1) ? 1 : IMG_HEIGHT;

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(IMG_WIDTH)/IMG_HEIGHT);
    auto camera_center = point3(0,0,0);

    // Calculate vectors across the horizontal/down the vertical viewport edges
    auto viewport_u = vec3(viewport_width,0,0);
    auto viewport_v = vec3(0,-viewport_height,0);

    // Calculate the horizontal/vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / IMG_WIDTH;
    auto pixel_delta_v = viewport_v / IMG_HEIGHT;

    // Location of upper left pixel
    auto viewport_upper_left = camera_center
                               - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render

    std::cout << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n255\n";

    for(int j = 0; j < IMG_HEIGHT; j++){
        std::clog << "\rScanlines remaining: " << (IMG_HEIGHT - j) << ' ' << std::flush;
        for(int i = 0; i < IMG_WIDTH; i++){
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout,pixel_color);
        }
    }
    std::clog << "\rDone.           \n";
}