#include <vector>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec)  + (t1.tv_usec - t0.tv_usec) / 1000000.0f;
}

class Point{
    public:
	Point()
	//default constructor
	{

	};
	
	Point(int x, int y)
	{
	    this->x = x;
	    this->y = y;
	};

	Point(const Point& p)
        //copy constructor
	{
	    this->x = p.x;
	    this->y = p.y;
	};
	int x;
	int y;
};

vector<Point> my_pos;
void my_callback(Point p)
{
    printf("log : x=%d,y=%d\n", p.x, p.y);
    my_pos.push_back(p);
}

void simulator(void(*start_cb)(Point))
{
    Point sampling_pos(rand()%1000, rand()%1000);
    start_cb(sampling_pos);
}

void start_simulation()
{
    struct timeval t0;
    struct timeval t1;
    gettimeofday(&t0, 0);
    gettimeofday(&t1, 0);
    float elapsed;
    elapsed = timedifference_msec(t0, t1);
    srand(time(NULL));
   
    while(elapsed < 5.0)
    {
	//milli sec
        int pollingDelay = 500;
	#ifdef _WIN32
	Sleep(pollingDelay);
	#else
	usleep(pollingDelay*1000);  /* sleep for 100 milliSeconds */
	#endif  
	simulator(my_callback);
	gettimeofday(&t1, 0);
	elapsed = timedifference_msec(t0, t1);
	printf("Execution time = %f\n", elapsed);
    }
    printf("Total Execution time = %f\n", elapsed);
    printf("end of simulation\n");

}
void export_file(vector<Point> p)
{
    ofstream fout("log_pos.txt");
    for (std::vector<Point>::iterator it = my_pos.begin() ; it != my_pos.end(); ++it)
	fout << "time slot " << (it - my_pos.begin())<< ", " << it->x << ", "<< it->y << endl;
    fout << endl;
}
int main()
{ 
    start_simulation();
    export_file(my_pos);

}
