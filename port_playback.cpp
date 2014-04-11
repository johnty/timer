// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright: (C) 2010 RobotCub Consortium
 * Author: Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 * Modified by Johnty Wang (johntywang@gmail.com) 2014 HPlus Tech
 */

#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>
#include <yarp/os/all.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>


using namespace yarp::os;
using namespace std;

bool cont = true;

int main(int argc, char *argv[]) {
    Network yarp;
    Port output;
	string from_port;
	string to_port;
    string fn;

	if (argc >=4) {
		from_port = string(argv[1]);
		to_port = string(argv[2]);
        fn = string(argv[3]);
	}
	else {
        if (argc < 2) {
            cout<<"usage: port_playback /thisPort /destPort filename"<<endl;
            return 0;
        }
        
	}
    
    //parse input file
    // ...
    
	output.open(from_port.c_str());

	yarp::os::Network::connect(from_port.c_str(),to_port.c_str());
	
    std::clock_t start;
	std::clock_t elapsed;

	ifstream file(fn.c_str());
	string line;
	if (file.is_open()) {
		start = std::clock();
		while (cont) {
			while (getline(file, line)) {
				cout<<"read line: "<<line<<endl;
				Bottle bot;
				bot.fromString(line.c_str());
				long t = bot.get(bot.size()-1).asInt(); //last value is timestamp
				cout<<"t="<<t<<endl;
				elapsed = std::clock() - start;
				while (elapsed <= t) {
					elapsed = std::clock() - start;
				};

				
				bot.pop(); //remove that timestamp!

				//wait, until we get past time.
				// of course, we should handle cases where there is nothing, in which case
				// it will hang!
				cout<<"sending! t="<<t<<endl;
				output.write(bot);
			}
			cont = false;
		}
    }
    output.close();
    return 0;
}
