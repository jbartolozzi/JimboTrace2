#include "raytrace.h"
#include <stdlib.h>
#include <thread>

using namespace std;

void runMulti();
void runRaytraceThread(int threadNum);
raytrace *r1;

void runRaytraceThread(int threadNum) {
	r1->runRaytrace(threadNum);
}

void runMulti() {
	thread t1(runRaytraceThread,0);
	thread t2(runRaytraceThread,1);
	thread t3(runRaytraceThread,2);
	thread t4(runRaytraceThread,3);
	thread t5(runRaytraceThread,4);
	thread t6(runRaytraceThread,5);
	thread t7(runRaytraceThread,6);
	thread t8(runRaytraceThread,7);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
}

int main (int argc, char** argv) {
	cout << "Welcome to JIMBO-TRACE" << endl;
	int frames = 0;
	int start = 0;
	
	while (frames <= 0) {
		cout << "Input the starting frame: ";
		cin >>start;

		cout << "Input the number of frames: ";
		cin >> frames;
		cout << endl;
	}
	r1 = new raytrace();

	for (int f = start; f < start+frames; f++ ){
		r1->setFrame(f);
		runMulti();
		//r1->runRaytrace(0);
		r1->finishRaytrace();
	}

	
	cout << "Render Complete" << endl;

	std::cin.ignore( std::numeric_limits<std::streamsize>::max(), 'q' );

	return 0;
}