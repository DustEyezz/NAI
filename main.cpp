#include <iostream>
#include <functional>
#include <map>
#include <math.h>
#include <random>
#include <time.h>

using namespace std;
using domain_t = std::vector<double>;
using myfunction_t = std::function<double(double, double)>;
std::random_device rd;
std::mt19937 mt_generator(rd());


//void calc(std::vector<double> numbers, myfunction_t fun) {
  //  using namespace std;
    //cout << fun(numbers) << endl;
//}

double optimize(auto function, auto domain, int maxIterations=1000){
    clock_t startClock, endClock;
    startClock = clock();
    double time;
    uniform_real_distribution<double> dist(domain.at(0), domain.at(1));
    double currentBest = function(domain.at(0), domain.at(1));

    for(int i = 0; i < maxIterations - 1; i++){
        double rand1 = dist(mt_generator);
        double rand2 = dist(mt_generator);
        double temp;
        temp = function(rand1,rand2);

        if(temp < currentBest){
            currentBest = temp;
        }
    }

    endClock = clock();
    time = ((double) (endClock - startClock)) / CLOCKS_PER_SEC;
    cout<<"Iterations: "<<maxIterations<<" Time needed: "<<time<<" Result: ";
    return currentBest;
}

int main(){

    map<string, myfunction_t> myFunctions;
    myFunctions["beal"] = [](double x, double y) { return (pow((1.5 - x + (x * y)),2)) + (pow(2.25 - x + (x * pow(y,2)),2))+(pow(2.625 - x + x * pow(y,3),2));};

    //map<string, function <double (double x, double y)>> functionMap;

    auto beal = [](double x, double y) {
        return (pow((1.5 - x + (x * y)),2)) + (pow(2.25 - x + (x * pow(y,2)),2))+(pow(2.625 - x + x * pow(y,3),2));
    };

    auto himmel = [](double x, double y) {
        return pow(pow(x,2) + y - 11,2) + pow(x + pow(y,2) - 7,2);
    };

    auto threeHumpCamel = [](double x, double y) {
        return ((2 * pow(x,2)) - (1.05 * pow(x,4)) + ((pow(x,6))/6) + (x*y) + (pow(y,2)));
    };

    auto matyas = [](double x, double y) {
        return (0.26 * (pow(x, 2) + pow(y, 2)) - (0.48 * x * y));
    };

    vector<double> domain = {-10,10};

    for(int i = 0; i < 10; i++){
        cout<<optimize(matyas,domain,10000)<<endl;
    }
    return 0;
}