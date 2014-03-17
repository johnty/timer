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
#include <string>
#include <ctime>


using namespace yarp::os;
using namespace std;

int main(int argc, char *argv[]) {
    Network yarp;
    Port output;
	string from_port;
	string to_port;

	if (argc >=3) {
		from_port = string(argv[1]);
		to_port = string(argv[2]);
	}
	else {
		from_port = "/sender";
		to_port = "/timer";
	}
	output.open(from_port.c_str());

	yarp::os::Network::connect(from_port.c_str(),to_port.c_str());
    int top = 100000;
	Random::seed(std::clock());
    for (int i=1; i<=top; i++) {
        // prepare a message
        Bottle bot;
		bot.addString("testing");
		bot.addInt(i); //id
		bot.addInt(i); //dev type
		bot.addInt(i); //server type
		bot.addInt(i); //time stamp
		for (int j=0; j<15; j++) { //15 sensor values
			bot.addDouble(Random::uniform());
		}
        // send the message
        output.write(bot);
        //printf("Sent message: %s\n", bot.toString().c_str());
        // wait a while
        //Time::delay(1);
    }
    output.close();
    return 0;
}
