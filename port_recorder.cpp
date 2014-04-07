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

    string pname = "/recorder";
    double t = 0;
    bool verbose = false;
    
    if (argc < 2) {
        cout <<"usage: port_recorder /port_name (-v)"<<endl;
        return 0;
    }
    
    else { //ok, lets go on...
        pname = string(argv[1]);
    }
    
    if (argc == 3) {
        //if 3 args, verbose mode
        verbose = true;
    }
    
    port.open(pname.c_str());
    cout << "Port Recorder active. ctrl+c to quit" <<endl;
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
        //read port
        // ...
        
        //parse bottle
        // ...
        
        //write to file
        // ...
        
        //find current time;
        duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
        if (port.getPendingReads() > 0) {
            //if first time, start timer!
            
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
