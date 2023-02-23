/* CSCI 200: Final Project
 *
 * Author: Peter Wetherell
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     // list here any outside assistance you used/received while following the
 *     // CS@Mines Collaboration Policy and the Mines Academic Code of Honor
 *
 * This class deals with creating the motion profile and drawing the robot as it follows the path
 */

#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <SFML/Graphics.hpp>
using namespace sf;

#include <vector>
#include <chrono>
#include <iostream>
#include "Point.hpp"
#include "Spline.hpp"
#include "Pose.hpp"
#include "LinkedList.hpp"
using namespace std;


class Timer { //Timer class copied from L6C
public:
    Timer() { mClockState = CLOCK_UNSTARTED; }
    void startClock() { mStartTime = std::chrono::high_resolution_clock::now(); mClockState = CLOCK_RUNNING; }
    void stopClock() { mEndTime = std::chrono::high_resolution_clock::now(); mClockState = CLOCK_STOPPED; }
    double elapsedTime() { 
        if(mClockState == CLOCK_UNSTARTED) { return 0.f; }
        if(mClockState == CLOCK_RUNNING) {
            mEndTime = std::chrono::high_resolution_clock::now();
        }
        return std::chrono::duration<double>(mEndTime - mStartTime).count(); 
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> mEndTime;
    unsigned short mClockState;
    const short CLOCK_UNSTARTED = 0;
    const short CLOCK_RUNNING = 1;
    const short CLOCK_STOPPED = 2;
};

class Robot {
    private:
        LinkedList<Point> points;
        vector<Pose> targetPoints;
        double x,y,heading;
        int width, height;
        Vertex* line;
        Timer timer;

        void update(){
            while (targetPoints.size() > 1 && targetPoints[0].t < timer.elapsedTime()){ //removing points that were passed
                targetPoints.erase(targetPoints.begin());
            }
            if (targetPoints.size() > 1){ //Interpolating between points
                double t = timer.elapsedTime();
                double mx = (targetPoints[1].p.getX() - targetPoints[0].p.getX())/(targetPoints[1].t - targetPoints[0].t) * (t - targetPoints[0].t);
                double my = (targetPoints[1].p.getY() - targetPoints[0].p.getY())/(targetPoints[1].t - targetPoints[0].t) * (t - targetPoints[0].t);
                double headingError = targetPoints[1].p.getHeading() - targetPoints[0].p.getHeading();
                double PI = 3.14159265358979323846;
                while (abs(headingError) > PI){//means you don't get weird 360 effects
                    headingError -= 2 * PI * ((headingError >= 0) - (headingError < 0)); //This is instead of signum
                }
                double mh = (headingError)/(targetPoints[1].t - targetPoints[0].t) * (t - targetPoints[0].t);
                x = targetPoints[0].p.getX() + mx;
                y = targetPoints[0].p.getY() + my;
                heading = targetPoints[0].p.getHeading() + mh;
            }
            else{//the case when their is only 1 point left => no need to interpolate between 2 points
                x = targetPoints[0].p.getX();
                y = targetPoints[0].p.getY();
                heading = targetPoints[0].p.getHeading();
            }
        }

        void normalizePath(){
            const double MAX_VEL = 500, MAX_ACCEL = 50, MAX_ANG_VEL = 2.15, MAX_ANG_ACCEL = 1.05;
            double vel = 5;
            double t = 0;
            targetPoints[0].t = t;
            targetPoints[0].v = vel;
            for (unsigned int i = 1; i < targetPoints.size(); i ++){ //This adds the acceleration curve
                double d = getDist(targetPoints[i-1].p,targetPoints[i].p); //This is approximately arc length
                double deltaTheta = targetPoints[i-1].p.getHeading()-targetPoints[i].p.getHeading(); //get change in angle
                double PI = 3.14159265358979323846;
                while (abs(deltaTheta) > PI){//means you don't get weird 360 effects
                    deltaTheta -= 2 * PI * ((deltaTheta >= 0) - (deltaTheta < 0)); //This is instead of signum
                }
                deltaTheta = abs(deltaTheta);//absalute change in angle
                double maxSpeed = MAX_VEL;
                double maxAccel = MAX_ACCEL;
                if (deltaTheta >= pow(10,-4)){
                    maxSpeed = max(min(maxSpeed,MAX_ANG_VEL*d/deltaTheta),10.0); //Find linear vel from angular vel | d ~ S = r*theta => d/theta = r
                    maxAccel = max(min(maxAccel,MAX_ANG_ACCEL*d/deltaTheta),5.0); //^ linear accel from angular accel
                }
                double deltaT = (-vel + sqrt(pow(vel,2) + 2 * maxAccel * d))/maxAccel; //Solved quadratic equation for time a = accel/2 b = vel c = -d
                vel = min(maxSpeed, vel + maxAccel * deltaT);
                t += deltaT;
                targetPoints[i].t = t;
                targetPoints[i].v = vel;
            }
            double totalTime = t;
            t = 0;
            vel = 5;
            for (unsigned int i = targetPoints.size()-1; i >= 1; i --){//adding deceleration by appling the process in reverse
                double d = getDist(targetPoints[i-1].p,targetPoints[i].p); //This is approximately arc length
                double deltaTheta = targetPoints[i-1].p.getHeading()-targetPoints[i].p.getHeading(); //get change in angle
                double PI = 3.14159265358979323846;
                while (abs(deltaTheta) > PI){//means you don't get weird 360 effects
                    deltaTheta -= 2 * PI * ((deltaTheta >= 0) - (deltaTheta < 0)); //This is instead of signum
                }
                deltaTheta = abs(deltaTheta);//absalute change in angle
                double maxSpeed = MAX_VEL;
                double maxAccel = MAX_ACCEL;
                if (deltaTheta >= pow(10,-4)){
                    maxSpeed = max(min(maxSpeed,MAX_ANG_VEL*d/deltaTheta),10.0); //Find linear vel from angular vel | d ~ S = r*theta => d/theta = r
                    maxAccel = max(min(maxAccel,MAX_ANG_ACCEL*d/deltaTheta),5.0); //^ linear accel from angular accel
                }
                double deltaT = (-vel + sqrt(pow(vel,2) + 2 * maxAccel * d))/maxAccel; //Solved quadratic equation for time a = accel/2 b = vel c = -d
                vel = min(maxSpeed, vel + maxAccel * deltaT);
                vel = min(vel,targetPoints[i-1].v);
                t += deltaT;
                targetPoints[i-1].t = totalTime - t;
                targetPoints[i-1].v = vel;
            }
            double startT = t - totalTime;
            for (unsigned int i = 0; i < targetPoints.size(); i ++){//normalizing all to start at 0
                targetPoints[i].t += startT;
            }
        }
        double getDist(Point p1, Point p2){
            return pow(pow(p1.getX()-p2.getX(),2) + pow(p1.getY()-p2.getY(),2),0.5);
        }

    public:
        ~Robot(){
            delete line;
            timer.stopClock();
        }
        Robot(){
            x = 0;
            y = 0;
            heading = 0;
            width = 1000;
            height = 1000;
            line = nullptr;
        }
        Robot(const int WIDTH, const int HEIGHT){
            x = 0;
            y = 0;
            heading = 0;
            width = WIDTH;
            height = HEIGHT;
        }
        void push_back(Point p){
            points.insert(points.size(),p);
        }
        void draw(RenderTarget& window){
            update();

            //Body shape of the robot
            CircleShape body;
            body.setPosition(Vector2f((float)(width/2 + x - 50),(float)(height/2 - y - 50))); //Convert from screen coords to field coords
            body.setFillColor(Color(0,0,0));
            body.setOutlineColor(Color(255,255,255));
            body.setOutlineThickness(5);
            body.setRadius(50);
            window.draw(body);

            //Line to indicate driection of the robot
            Vertex line[] =
            {
                Vertex(Vector2f((float)(width/2 + x),(float)(height/2 - y))),
                Vertex(Vector2f((float)(width/2 + x + 50 * cos(heading)),(float)(height/2 - y - 50 * sin(heading))))
            };

            window.draw(line, 2, sf::Lines);
        }
        void renderPath(RenderTarget& window){
            update();
            //draws the line the robot follows
            window.draw(line, targetPoints.size(), sf::Lines);
        }
        void build(){
            Pose start;
            start.p = points.get(0);
            start.t = 0;

            if (targetPoints.size() != 0){ //don't leak memory if you build multiple times
                delete line;
            }

            targetPoints.push_back(start); //pushes the first point to ensure that the robot always goes somewhere even if you don't have a trajectory
            for (unsigned int i = 0; i < points.size()-1; i++){
                Spline a(points.get(i),points.get(i+1));
                a.getPoints(targetPoints, 100); //generates splines
            }

            //creates the line for the robot to follow. It is generated in reverse order to make it easier to exclude parts when we follow the path
            line = new Vertex[targetPoints.size()];
            for (unsigned int i = 0; i < targetPoints.size(); i ++){
                line[targetPoints.size() - 1 - i] = Vertex(Vector2f((float)(width/2 + targetPoints[i].p.getX()),(float)(height/2 - targetPoints[i].p.getY())));
                //This is the path of the trajectory
            }
            normalizePath(); //Applies the motion profiling to the robot
        }
        void startFollowTraj(){
            timer.startClock();
        }
        double pathTime(){
            return targetPoints[targetPoints.size()-1].t;
        }
        int getSize(){
            return targetPoints.size();
        }
};

#endif