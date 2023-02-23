/* CSCI 200: Final Project
 *
 * Author: Peter Wetherell
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     // list here any outside assistance you used/received while following the
 *     // CS@Mines Collaboration Policy and the Mines Academic Code of Honor
 *
 * Wrapper for the Point class to give it time and speed information
 */

#ifndef POSE_HPP
#define POSE_HPP

#include "Point.hpp"

class Pose {
    public:
        Point p;
        double t;
        double v;
        Pose(){
            t = 0;
            v = 5;
        }
};

#endif