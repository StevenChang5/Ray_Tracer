#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera{
    public:
        double ASPECT_RATIO     = 1.0; // Ration of image width over height
        int IMG_WIDTH           = 100; // Rendered image width in pixel count
        int samples_per_pixel   = 10;  // Count of random samples for each pixel
        int max_depth           = 10;  // Maximum number of ray bounds into scene

        double vfov = 90;                   // Vertical view angle (fov)
        point3 lookfrom = point3(0,0,0);    // Point camera is looking from
        point3 lookat = point3(0,0,-1);     // Point camera is looking at
        vec3 vup = vec3(0,1,0);             // Camera-relative "up" direction

        double defocus_angle = 0;   // Variational angle of rays through each pixel
        double focus_dist = 10;     // Distance from camera lookfrom point to plane of perfect focus

        void render(const hittable& world){
            initialize();

            std::cout << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n255\n";

            for(int j = 0; j < IMG_HEIGHT; j++){
                std::clog << "\rScanlines remaining: " << (IMG_HEIGHT - j) << ' ' << std::flush;
                for(int i = 0; i < IMG_WIDTH; i++){
                    color pixel_color(0,0,0);
                    for(int sample = 0; sample < samples_per_pixel; sample++){
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }
            std::clog << "\rDone.           \n";
        }
    private:
        int     IMG_HEIGHT;         // Rendered image height
        double pixel_samples_scale; // Color scale factor for a sum of pixel samples
        point3  center;             // Camera Center
        point3  pixel00_loc;        // Location of pixel (0,0)
        vec3    pixel_delta_u;      // Offset of pixel to the right
        vec3    pixel_delta_v;      // Offset of pixel below
        vec3    u, v, w;            // Camera frame basis vectors
        vec3    defocus_disk_u;     // Defocus disk horizontal radius
        vec3    defocus_disk_v;     // Defocus disk vertical radius

        void initialize(){
            IMG_HEIGHT = int(IMG_WIDTH/ASPECT_RATIO);
            IMG_HEIGHT = (IMG_HEIGHT<1) ? 1 : IMG_HEIGHT;

            pixel_samples_scale = 1.0/samples_per_pixel;

            center = lookfrom;

            // Determine viewport dimensions
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2 * h * focus_dist;
            auto viewport_width = viewport_height * (double(IMG_WIDTH)/IMG_HEIGHT);

            // Calculate u, v, w unit basis vectors for the camera coordinate frame
            // Cross product yields vector perpendicular to two vectors
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // Calculate vectors across the horizontal/down the vertical viewport edges
            auto viewport_u = viewport_width * u; // Vector across viewport horizontal edge
            auto viewport_v = viewport_height * -v; // Vector down viewport vertical edge

            // Calculate the horizontal/vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / IMG_WIDTH;
            pixel_delta_v = viewport_v / IMG_HEIGHT;

            // Location of upper left pixel
            auto viewport_upper_left = center - (focus_dist*w) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            // Calculate the camera defocus disk basis vectors
            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;

            
        }

        ray get_ray(int i, int j) const{
            // Construct a camera ray originating from defocus disk and directed at randomly sampled
            // point around the pixel location (i,j)

            auto offset = sample_square();
            auto pixel_sample = pixel00_loc
                                + ((i + offset.x()) * pixel_delta_u)
                                + ((j + offset.y()) * pixel_delta_v);
            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const{
            // Returns the vector to a random point in the [-0.5, -0.5]-[+0.5, +0.5] unit square
            return vec3(random_double()-0.5, random_double()-0.5, 0);
        }

        point3 defocus_disk_sample() const{
            // Returns a random point in the camera defocus disk
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }
        
        color ray_color(const ray& r, int depth, const hittable& world) const{
            // If ray bounce limit is exceeded, no more light is gathered
            if (depth <= 0)
                return color(0,0,0);

            hit_record rec;

            if(world.hit(r, interval(0.001, infinity), rec)){
                ray scattered;
                color attenuation;
                if(rec.mat->scatter(r,rec,attenuation,scattered))
                    return attenuation * ray_color(scattered, depth-1, world);
                return color(0,0,0);
            }
            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            // Linear combination of white and blue 
            return (1.0-a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
        }
};

#endif