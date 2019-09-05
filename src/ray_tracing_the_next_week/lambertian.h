#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_ 1

#include "material.h"
#include "texture.h"

class lambertian : public material {
public:

  lambertian(texture* a) : albedo(a) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = ray(rec.p, target - rec.p, r_in.time());
    attenuation = albedo->value(0, 0, rec.p);
    return true;
  }

   texture* albedo;
};

#endif // _LAMBERTIAN_H_

