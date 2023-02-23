/* CSCI 200: Final Project
 *
 * Author: Peter Wetherell
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     // list here any outside assistance you used/received while following the
 *     // CS@Mines Collaboration Policy and the Mines Academic Code of Honor
 *
 * This class gets user input and then passes that to the robot class. It draws the window and then updates the robot;
 */

#include <SFML/Graphics.hpp>
using namespace sf;

#include <iostream>
#include <fstream>
#include <cmath>
#include "Robot.hpp"
#include "Point.hpp"
using namespace std;

int main(int argc, char **argv) {
    // create a window
    const int WIDTH = 1000, HEIGHT = 1000;
    RenderWindow window( VideoMode(WIDTH, HEIGHT), "Final Project" );

    Robot r(WIDTH, HEIGHT);

    double PI = 3.14159265358979323846;
    
    if (argc > 1){
        ifstream path;
        cout << "Now opening \"" << argv[1] << "\"" << endl;
        path.open(argv[1]);
        if (path.fail()) {
            cerr << "Error opening input file";
            return -1;
        }
        cout << "File opened successfully!" << endl;

        double x, y, heading;
        while (path >> x){
            path >> y; path >> heading;
            r.push_back(Point(min(max(x,-WIDTH/2.0),WIDTH/2.0),min(max(y,-HEIGHT/2.0),HEIGHT/2.0),heading*PI/180.0));
        }

        path.close();
    }
    else{
        cout << "Enter your starting point" << endl;
        char c;
        do {
            cout << "format: x, y, heading (deg)" << endl;
            double x, y, heading;
            cin >> x; cin >> y; cin >> heading;
            r.push_back(Point(min(max(x,-WIDTH/2.0),WIDTH/2.0),min(max(y,-HEIGHT/2.0),HEIGHT/2.0),heading*PI/180.0));
            cout << "Would you like to enter a point? (Y/N)" << endl;
            cin >> c;
        }
        while (c != 'N' && c != 'n');
    }

    r.build();

    cout << "Path takes " << r.pathTime() << " sec" << endl;

    r.startFollowTraj();

    Event event;

    // while the window is open
    while( window.isOpen() ) {
        // clear any existing contents
        window.clear();

        /////////////////////////////////////
        // BEGIN DRAWING HERE

        r.renderPath(window);
        r.draw(window);

        //  END  DRAWING HERE
        /////////////////////////////////////


        // display the current contents of the window
        window.display();

        /////////////////////////////////////
        // BEGIN EVENT HANDLING HERE
        // check if any events happened since the last time checked
        while( window.pollEvent(event) ) {
            // if event type corresponds to pressing window X
            if(event.type == Event::Closed) {
                // tell the window to close
                window.close();
            }
            // check addition event types to handle additional events
        }
        //  END  EVENT HANDLING HERE
        /////////////////////////////////////
    }

    return 0;
}