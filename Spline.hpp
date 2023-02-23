/* CSCI 200: Final Project
 *
 * Author: Peter Wetherell
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     // list here any outside assistance you used/received while following the
 *     // CS@Mines Collaboration Policy and the Mines Academic Code of Honor
 *
 * This class deals with creating the splines that the motion profile is added to.
 */

#ifndef SPLINE_HPP
#define SPLINE_HPP

#include <cmath>
#include <vector>
#include "Point.hpp"
#include "Pose.hpp"
using namespace std;

class Spline {
    public:
        Spline(Point a, Point b){
            x = new double[4];
            y = new double[4];
            double dist = 200;//pow(pow(a.getY()-b.getY(),2)+pow(a.getX()-b.getX(),2),0.5);
            generateParametricCurve(x,a.getX(),dist*cos(a.getHeading()),b.getX(),dist*cos(b.getHeading()));
            generateParametricCurve(y,a.getY(),dist*sin(a.getHeading()),b.getY(),dist*sin(b.getHeading()));
        }
        ~Spline(){
            delete x;
            delete y;
        }
        void generateParametricCurve(double*& arr, double p1, double m1, double p2, double m2){ //Solves for one of the components of a cubic spline
            arr[0] = p1;
            arr[1] = m1;
            arr[3] = m2 + 2*p1 + m1 - 2*p2;
            arr[2] = p2 - arr[0] - arr[1] - arr[3];
        }
        void getPoints(vector<Pose>& list, const int FIDELITY){
            for (int i = 0; i < FIDELITY; i ++){
                double t = ((double)i)/FIDELITY;
                Pose p;
                p.p = Point(getX(t),getY(t),getHeading(t));
                list.push_back(p);
            }
        }
    private:
        double *x,*y;
        double getX(double t){
            double retX = 0;
            for (int i = 0; i < 4; i ++){
                retX += pow(t,i) * x[i];
            }
            return retX;
        }
        double getY(double t){
            double retY = 0;
            for (int i = 0; i < 4; i ++){
                retY += pow(t,i) * y[i];
            }
            return retY;
        }
        double getVelX(double t){
            double velX = 0;
            for (int i = 1; i < 4; i ++){
                velX += i * pow(t,i-1) * x[i];
            }
            return velX;
        }
        double getVelY(double t){
            double velY = 0;
            for (int i = 1; i < 4; i ++){
                velY += i * pow(t,i-1) * y[i];
            }
            return velY;
        }
        double getHeading(double t){
            return atan2(getVelY(t),getVelX(t));
        }
};

#endif