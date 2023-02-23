/* CSCI 200: Final Project
 *
 * Author: Peter Wetherell
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     // list here any outside assistance you used/received while following the
 *     // CS@Mines Collaboration Policy and the Mines Academic Code of Honor
 *
 * A simple class that stores possitions on the field
 */

#ifndef POINT_HPP
#define POINT_HPP

class Point {
    public:
        Point(){x = 0; y = 0; heading = 0;}
        Point(double x, double y, double heading){
            this->x = x;
            this->y = y;
            this->heading = heading;
        }
        double getX(){
            return x;
        }
        double getY(){
            return y;
        }
        double getHeading(){
            return heading;
        }
    private:
        double x, y, heading;
};

#endif