#pragma once
#include "hitable.h"

class hitable_list : public hitable {
public:
    hitable** list;
    int list_size;
    
    hitable_list() {}
    hitable_list(hitable** l, int n) { list = l; list_size = n; }
    
    virtual ~hitable_list() override
    {
        for (int i = 0; i < list_size; i++)
            delete list[i];
    }

    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec, material& closest_mat) const override;
    virtual bool bounding_box(aabb& box) const override;

};

/**
* Finds nearest object the ray intersects, returns false if nothing is intersected.
*/
bool hitable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec, material& closest_mat) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (int i = 0; i < list_size; i++) {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec, closest_mat)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool hitable_list::bounding_box(aabb& box) const
{
    if (list_size == 0)
        return false;

    aabb temp_box;

    for (int i = 0; i < list_size; i++) 
    {
        if (!list[i]->bounding_box(temp_box)) 
            return false;
        
        box = (i == 0) ? temp_box : enclose_boxes(box, temp_box);
    }

    return true;
}