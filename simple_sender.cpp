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
#include <stdio.h>

using namespace yarp::os;

int main() {
    Network yarp;
    Port output;
    output.open("/sender3");
	yarp::os::Network::connect("/sender3","/timer");
    int top = 100000;
    for (int i=1; i<=top; i++) {
        // prepare a message
        Bottle bot;
		bot.addString("testing");
		for (int j=0; j<9; j++) {
			bot.addDouble(1.234);
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
