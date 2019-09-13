#include <iostream>

#include "sphere.h"
#include "moving_sphere.h"
#include "hitable_list.h"

#include "float.h"
#include "random.h"
#include "camera.h"

#include "material.h"

#include "image_texture.h"

#include "aarect.h"
#include "box.h"
#include "constant_medium.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

vec3 color(const ray& r, hitable *world, int depth) {
  hit_record rec;
  if(world->hit(r, 0.001f, FLT_MAX, rec)) {
    ray scattered;
    vec3 attenuation;
    vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return emitted + attenuation * color(scattered, world, depth + 1);
    }
    else {
      return emitted;
    } 
  }
  else {
    return vec3(0, 0, 0);
    /*vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);*/
  }
}

hitable* two_spheres() {
  texture* checker = new checker_texture(new constant_texture(vec3(0.2f, 0.3f, 0.1f)), new constant_texture(vec3(0.9f, 0.9f, 0.9f)));
  int n = 50;
  hitable** list = new hitable*[n + 1];
  list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
  list[1] = new sphere(vec3(0,  10, 0), 10, new lambertian(checker));

  return new hitable_list(list, 2);
}

hitable* two_perlin_spheres() {
  texture* pertext = new noise_texture(2.0f);

  hitable** list = new hitable*[2];
  list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
  list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
  return new hitable_list(list, 2);
}

hitable* simple_light() {
  texture* pertext = new noise_texture(4.0f);
  hitable** list = new hitable*[4];
  list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
  list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
  list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
  list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
  return new hitable_list(list, 4);
}

hitable* cornell_box() {
  hitable **list = new hitable*[8];
  int i = 0;
  material* red = new lambertian(new constant_texture(vec3(0.65f, 0.05f, 0.05f)));
  material* white = new lambertian(new constant_texture(vec3(0.73f, 0.73f, 0.73f)));
  material* green = new lambertian(new constant_texture(vec3(0.12f, 0.45f, 0.15f)));
  material* light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
  list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
  list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
  list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
  list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
  list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
  list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
  list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
  list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white),  15), vec3(265, 0, 295));
  return new hitable_list(list, i);
}

hitable* cornell_smoke() {
  hitable **list = new hitable*[8];
  int i = 0;
  material* red = new lambertian(new constant_texture(vec3(0.65f, 0.05f, 0.05f)));
  material* white = new lambertian(new constant_texture(vec3(0.73f, 0.73f, 0.73f)));
  material* green = new lambertian(new constant_texture(vec3(0.12f, 0.45f, 0.15f)));
  material* light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
  list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
  list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
  list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
  list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
  list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
  list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

  hitable* b1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
  hitable* b2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));

  list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1.0f, 1.0f, 1.0f)));
  list[i++] = new constant_medium(b2, 0.01, new constant_texture(vec3(0.0f, 0.0f, 0.0f)));

  return new hitable_list(list, i);
}

hitable* random_scene() {
  int n = 50000;
  hitable** list = new hitable*[n + 1];

  texture* checker = new checker_texture(new constant_texture(vec3(0.2f, 0.3f, 0.1f)), new constant_texture(vec3(0.9f, 0.9f, 0.9f)));
  list[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(checker));
  int i = 1;
  for(int a = -10; a < 10; a++) {
    for(int b = -10; b < 10; b++) {
      
      float choose_mat = random_double();
      vec3 center(a + 0.9f * random_double(), 0.2f, b + 0.9f * random_double());
      if((center - vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {

        if(choose_mat < 0.8f) { // diffuse
          list[i++] = new moving_sphere(center, center + vec3(0.0f, 0.5f * random_double(), 0.0f), 0.0f, 1.0f, 0.2f,
            new lambertian(new constant_texture(vec3(random_double() * random_double(), random_double() * random_double(), random_double() * random_double()))));
        }
        else if(choose_mat < 0.95f) { // metal
          list[i++] = new sphere(center, 0.2f,
            new metal(vec3(0.5f * (1.0f + random_double()), 0.5f * (1.0f + random_double()), 0.5f * (1.0f + random_double())), 0.5f * random_double()));
              
        }
        else { // glass
          list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
        }
      }
    }
  }

  list[i++] = new sphere(vec3(0, 1, 0), 1.0f, new dielectric(1.5f));
  list[i++] = new sphere(vec3(-4, 1, 0), 1.0f, new lambertian(new constant_texture(vec3(0.4f, 0.2f, 0.1f))));
  list[i++] = new sphere(vec3(4, 1, 0), 1.0f, new metal(vec3(vec3(0.7f, 0.6f, 0.5f)), 0.0f));

  return new hitable_list(list, i);
}


int main () {
  int nx = 600;
  int ny = 600;
  int ns = 120;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  
  /*hitable* list[5];
  float R = cos(M_PI / 4.0f);
  list[0] = new sphere(vec3(0, 0, -1), 0.5f, new lambertian(vec3(0.1f, 0.2f, 0.5f)));
  list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8f, 0.8f, 0.0f)));
  list[2] = new sphere(vec3(1, 0, -1), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.2f));
  list[3] = new sphere(vec3(-1, 0, -1), 0.5f, new dielectric(1.5f));
  list[4] = new sphere(vec3(-1, 0, -1), -0.45f, new dielectric(1.5f));*/
  //hitable *world = new hitable_list(list, 5);
  hitable *world = cornell_smoke();

  vec3 lookfrom(278.0f, 278.0f, -800.0f);
  vec3 lookat(278.0f, 278.0f, 0.0f);
  float dist_to_focus = 10.0f; //(lookfrom - lookat).length();
  float aperture = 0.0f;
  float vfov = 40.0f;

  camera cam(lookfrom, lookat, vec3(0.0f, 1.0f, 0.0f), vfov, float(nx)/float(ny), aperture, dist_to_focus, 0.0f, 1.0f);

  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {

      vec3 col(0.0f, 0.0f, 0.0f);
      for(int s = 0; s < ns; s++) {

        float u = float(i + random_double()) / float(nx);
        float v = float(j + random_double()) / float(ny);
        ray r = cam.get_ray(u, v);
        col += color(r, world, 0);
      }

      col /= float(ns);
      col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      int ir = int(255.99f * col[0]);
      int ig = int(255.99f * col[1]);
      int ib = int(255.99f * col[2]);

      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }

}




