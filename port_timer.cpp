#include <yarp/os/all.h>
#include <iostream>
#include <string>
#include <ctime>
#include <stdlib.h>

using namespace std;
using namespace yarp::os;

int main(int argc, char *argv[]) {
    Network yarp;
    BufferedPort<Bottle> port;

    string pname = "/timer";
    double t = 0;
    bool verbose = false;
    
    if (argc < 3) {
        cout <<"usage: port_timer [/port_name] [t (seconds)] [-v )optional]"<<endl;
        return 0;
    }
    
    else { //ok, lets go on...
        pname = string(argv[1]);
        t = atof(argv[2]);
        if (argc == 4) {
            verbose = true; // ha ha hack
        }
    }
    port.open(pname.c_str());
    cout << "Port Timer set up for " <<t << "s intervals";
    if (verbose)
        cout <<"; verbose=ON"<<endl;
    else
        cout <<endl;
    
    //set clock start
    std::clock_t start;
    double duration;
    start = std::clock();
    long count = 0;
    while (true) {
        //find current time;
        duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
        if (port.getPendingReads() > 0) {
            Bottle *input = port.read();
            if (input!=NULL) {
                count++;
                if (verbose)
                    cout << "got " << input->toString().c_str() << endl;
                double total = 0;
                for (int i=4; i<input->size(); i++) {
                    total += input->get(i).asDouble();
                }
            }
        }
        
        //if reached polling interval, reset count + measure FPS
        if (duration >= t) {
            start = std::clock();
            cout <<"received " << count << " frames in "<< t <<" seconds; FPS = "<<count/t <<endl<<endl;
            count = 0; //reset counter
            
        }
    }
    port.close();
    return 0;
}
