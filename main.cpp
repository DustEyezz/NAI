#include <iostream>
#include <functional>
#include <map>
#include <math.h>
#include <random>
#include <chrono>

using namespace std;
using myfunction_t = std::function<double(double, double)>;
std::random_device rd;
std::mt19937 mt_generator(rd());


double optimize(myfunction_t function, vector<double> domain, int maxIterations=10000){

    FILE *fp = NULL;
    fp = fopen("data.txt", "a");

    auto start = std::chrono::high_resolution_clock::now();
    uniform_real_distribution<double> dist(domain.at(0), domain.at(1));
    double currentBest = function(domain.at(0), domain.at(1));

    for(int i = 0; i < maxIterations; i++){
        double rand1 = dist(mt_generator);
        double rand2 = dist(mt_generator);
        double temp;
        temp = function(rand1,rand2);

        if(temp < currentBest){
            currentBest = temp;
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    fprintf(fp, "%f %f\n", (double)duration.count(), currentBest);
    cout<<"Iterations: "<<maxIterations<<" Time taken: "<<duration.count()<<" microseconds Lowest: ";
    return currentBest;
}

int main(int argc, char **argv){
    FILE *gnupipe = NULL;
    char *gnuCommands [] = {"set title \"Optimization function graph\"", "f(x) = mean_y", "fit f(x) 'data.txt' via mean_y", "set xlabel\"time in microseconds\"", "set ylabel \"lowest generated\"", "plot 'data.txt', \ f(x)"};
    gnupipe = _popen("gnuplot -persistent", "w");

    map<string, myfunction_t> myFunctions;
    map<string, vector<double>> domain;

    myFunctions["beale"] = [](double x, double y) {
        return (pow((1.5 - x + (x * y)),2)) + (pow(2.25 - x + (x * pow(y,2)),2))+(pow(2.625 - x + x * pow(y,3),2));
    };
    myFunctions["himmel"] = [](double x, double y) {
        return pow(pow(x,2) + y - 11,2) + pow(x + pow(y,2) - 7,2);
    };
    myFunctions["threeHumpCamel"] = [](double x, double y) {
        return ((2 * pow(x,2)) - (1.05 * pow(x,4)) + ((pow(x,6))/6) + (x*y) + (pow(y,2)));
    };
    myFunctions["matyas"] = [](double x, double y) {
        return (0.26 * (pow(x, 2) + pow(y, 2)) - (0.48 * x * y));
    };

   domain["beale"] = {-4.5,4.5};
   domain["himmel"] = {-5,5};
   domain["threeHumpCamel"] = {-5,5};
   domain["matyas"] = {-10,10};

   if( remove( "data.txt" ) != 0 )
       cout << "Creating file\n";

   try {
       vector<string> arguments(argv, argv + argc);
       auto selectedFunction = arguments.at(1);
       for(int i = 0; i < 20; i++){
           cout<<optimize(myFunctions.at(selectedFunction),domain.at(selectedFunction),10000)<<endl;
       }
   }
   catch (std::out_of_range aor) {
       cout << "Blad. Podaj poprawny argument. Dostepne to: ";
       for (auto [k, v] : myFunctions) cout << k << ", ";
       cout << endl;
       return 1;
   }
    for (int i = 0; i < 7; i++) {
        fprintf(gnupipe, "%s\n", gnuCommands[i]);
    }
   return 0;
}