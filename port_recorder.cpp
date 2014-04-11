#include <yarp/os/all.h>
#include <iostream>
#include <string>
#include <ctime>
#include <stdlib.h>

#ifdef WIN32
#include <Windows.h>
#else
#include <csignal>
#endif


bool cont = true;

using namespace std;
using namespace yarp::os;


#ifdef WIN32
BOOL CtrlHandler( DWORD fdwCtrlType ) 
{ 
  switch( fdwCtrlType ) 
  { 
    // Handle the CTRL-C signal; windows
    case CTRL_C_EVENT: 
      printf( "Ctrl-C event\n\n" );
      Beep( 750, 300 ); 
	  cont = false;
      return( TRUE );
    default: 
      return FALSE; 
  } 
}
#else
    // signal, unix
void signalHandler( int signum )
{
    cout << "Interrupt signal (" << signum << ") received.\n";
    cont = false;
    //exit(signum);

}
#endif

int main(int argc, char *argv[]) {
#ifdef WIN32
	if( SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE ) ) { 
		printf( "\nThe Control Handler is installed.\n" ); 
	}
#else
    std::signal(SIGINT, signalHandler);
#endif

    Network yarp;
    BufferedPort<Bottle> port;

    string pname = "/recorder";
	string fname = "rec00.txt";
    double t = 0;
    bool verbose = false;
    
    if (argc != 3) {
        cout <<"usage: port_recorder [/port_name] [output.file]"<<endl;
        return 0;
    }
    
    else { //ok, lets go on...
        pname = string(argv[1]);
        fname = string(argv[2]);
    }
    port.open(pname.c_str());
    cout << "recording on port " << pname << " to " <<fname<<endl;
    
    //set clock start
    std::clock_t start;
	std::clock_t elapsed;
    double duration;
    
    long count = 0;
	bool started = 0;
	port.setStrict(true); //this makes sure we don't lose anything!
	FILE *fout = fopen(fname.c_str(), "w");
	Bottle *input;
    while (cont) {
		//read port
		if (port.getPendingReads() > 0) {
			//on first msg, start the clock!
			if (started == false) {
				cout<<"recording started...\n";
				//start the clock!
				start = std::clock();
				started = true;
			}
			elapsed = std::clock() - start;
			input = port.read();
			input->add((int)elapsed); //too bad this is at end; find out how to add to beginning!
			fprintf(fout,"%s\n", input->toString().c_str());
        }
        //parse bottle
        // ...
        
        //write to file
        // ...
        
        //find current time;
    }
	cout<<"closing file..."<<endl;
	fclose(fout);
	cout<<"closing port..."<<endl;
    port.close();
    return 0;
}
