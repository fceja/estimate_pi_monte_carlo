#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
#include <sstream>

using namespace std;
mutex myMutex;

void checkIfInCircle(double& pointsInside, int start, int end){

    double x, y;

    for(int i = start; i < end; i++){
        x = ((double)rand()) / ((double)RAND_MAX);
        y = ((double)rand()) / ((double)RAND_MAX);

        double dist = (x*x) + (y*y);

        if(dist <= 1) {
            lock_guard<mutex> myLock(myMutex);
            pointsInside += 1;
        }
    }
}

int main(int argc, char** argv){

    int threads;
    int randPoints;

    if(argc != 3){
        cout << "usage: monte [1...10] [10...1000000" << endl;
        exit(EXIT_FAILURE);
    }
    else if(argc >= 2){
        istringstream iss1(argv[1]);
        int val;

        if(iss1 >> val){
            if(val >= 1 && val <= 10) {
                threads = val;
            }else{
                cout << "usage: monte [1...10] [10...1000000" << endl;
                exit(EXIT_FAILURE);
            }
        }

        istringstream iss2(argv[2]);
        if(iss2 >> val){
            if(val >= 10 && val <= 1000000) {
                randPoints = val;
            }else{
                cout << "usage: monte [1...10] [10...1000000" << endl;
                exit(EXIT_FAILURE);
            }
        }

    }

    double pointsInside = 0;

    auto start = chrono::system_clock::now();

    thread t[threads];
    int partition = randPoints/threads;
    int startIndex = 0;

    for(int i = 0; i < threads; i++){
        cout << "Thread[" << i << "] - partition ["
             << startIndex << ":" << startIndex+partition-1 << "]" << endl;

        t[i] = thread(checkIfInCircle, ref(pointsInside), startIndex, startIndex+partition-1);
        startIndex += partition;
    }

    for(int i = 0; i < threads; i++){
        t[i].join();
    }

    double ratio = pointsInside/randPoints;
    double pi = ratio * 4;

    chrono::duration<double> dur = chrono::system_clock::now() - start;
    cout << "Time: " << dur.count() << "seconds" << endl;
    cout << "Estimated Pi: " << pi << endl;

}
