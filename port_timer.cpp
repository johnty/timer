#include <yarp/os/all.h>
#include <iostream>
#include <string>
#include <ctime>

using namespace std;
using namespace yarp::os;

int main(int argc, char *argv[]) {
    Network yarp;
    BufferedPort<Bottle> port;

    string pname = "/timer";
    if (argc == 2) {
        pname = string(argv[1]);
        
    }
    port.open(pname.c_str());
    cout << "waiting for input" << endl;
    
    std::clock_t start;
    double duration;
    start = std::clock();
    long count = 0;
    while (true) {
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        if (port.getPendingReads() > 0) {
            Bottle *input = port.read();
            if (input!=NULL) {
                count++;
                cout << "got " << input->toString().c_str() << endl;
                //double total = 0;
                //for (int i=0; i<input->size(); i++) {
                //    total += input->get(i).asDouble();
                //}
            }
        }
        if (duration >= 2.0) {
            start = std::clock();
            cout <<"received " << count << " frames in 2 seconds; FPS = "<<count/2.0 <<endl<<endl;
            count = 0; //reset counter
            
        }
    }
    port.close();
    return 0;
}
