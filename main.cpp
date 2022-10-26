#include <iostream>
#include <functional>
#include <map>
#include <math.h>
#include <random>
#include <chrono>

using namespace std;
using myfunction_t = std::function<double(pair<double, double>)>;
std::random_device rd;
std::mt19937 mt_generator(rd());

auto randomSingularXY = [](double a, double b) {
    uniform_real_distribution<> dis(a, b);
    return pair<double, double>(dis(mt_generator), dis(mt_generator));
};

auto randomVectorXY = [](pair<double, double> p, int a, int b) -> vector<pair<double, double>> {
    uniform_real_distribution<> dis(a, b);
    return {pair<double, double>(dis(mt_generator), dis(mt_generator))};
};

double hillClimbing(myfunction_t function, vector<double> domain, int maxIterations=1000){

    FILE *fp = NULL;
    fp = fopen("climbing.txt", "a");

    int checkpoint = maxIterations / 25;
    double domainStart = domain.at(0);
    double domainEnd = domain.at(1);

    pair<double,double> sk = randomSingularXY(domain.at(0), domain.at(1));


    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < maxIterations; i++){
        auto tk = randomVectorXY(sk, domain.at(0), domain.at(1));
        auto bestNeighbour = *min_element(
                tk.begin(),
                tk.end(),
                [function](auto domainStart, auto domainEnd) {
                    return function(domainStart) > function(domainEnd);
                }
        );
        if (function(bestNeighbour) < function(sk))
            sk = bestNeighbour;

        if (i % checkpoint == 0){
            auto stop = std::chrono::high_resolution_clock::now();
            double currentBest = function(sk);
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            fprintf(fp, "%d %f\n", (int)duration.count(), currentBest);
        }
    }

    fclose(fp);
    return function(sk);
}

double simulatedAnnealing (myfunction_t function, vector<double> domain, int maxIterations=1000) {
    FILE *fp = NULL;
    fp = fopen("annealing.txt", "a");

    int checkpoint = maxIterations / 25;

    vector<pair<double, double>> ArrayOfXY;
    uniform_real_distribution<double> rand01(0, 1);
    double uk = rand01(mt_generator);

    auto sk = randomSingularXY(domain.at(0), domain.at(1));
    ArrayOfXY.push_back(sk);
    auto prevSK = sk;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < maxIterations; i++) {
        auto tk = randomSingularXY(domain.at(0), domain.at(1));
        if (function(tk) <= function(sk)) {
            sk = tk;
            ArrayOfXY.push_back(sk);
        } else {
            if (uk < exp(-(abs(function(tk) - function(prevSK))) / ((abs(domain.at(0)) + (abs(domain.at(1)))) / i / (maxIterations/10)))) {
                sk = tk;
                ArrayOfXY.push_back(sk);
            }
            else{
                sk = prevSK;
            }
        }
        prevSK = sk;

        if (i % checkpoint == 0){
            auto stop = std::chrono::high_resolution_clock::now();
            double currentBest = function(sk);
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            fprintf(fp, "%d %f\n", (int)duration.count(), currentBest);
        }
    }
    fclose(fp);
    return function(sk);
};

int main(int argc, char **argv){
    //FILE *gnupipe = NULL;
    //char *gnuCommands [] = {"set title \"Optimization function graph\"", "f(xy.front()) = mean_y", "g(xy.back()) = mean_x", "fit f(xy.front()) 'data.txt' using 1:2 via mean_y", "fit g(xy.front()) 'data.txt' using 2:1 via mean_x", "set xlabel\"time in microseconds\"", "set ylabel \"lowest generated\"", "plot 'data.txt', \ f(xy.front()), \ g(xy.front())"};
    //gnupipe = _popen("gnuplot -persistent", "w");

    map<string, myfunction_t> myFunctions;
    map<string, vector<double>> domain;

    myFunctions["beale"] = [](pair<double, double> xy) {
        return (pow((1.5 - xy.first + (xy.first * xy.second)),2)) + (pow(2.25 - xy.first + (xy.first * pow(xy.second,2)),2))+(pow(2.625 - xy.first + xy.first * pow(xy.second,3),2));
    };
    myFunctions["himmel"] = [](pair<double, double> xy) {
        return pow(pow(xy.first,2) + xy.second - 11,2) + pow(xy.first + pow(xy.second,2) - 7,2);
    };
    myFunctions["threeHumpCamel"] = [](pair<double, double> xy) {
        return ((2 * pow(xy.first,2) - (1.05 * pow(xy.first,4)) + ((pow(xy.first,6))/6) + (xy.first*xy.second + (pow(xy.second,2)))));
    };
    myFunctions["matyas"] = [](pair<double, double> xy) {
        return (0.26 * (pow(xy.first, 2) + pow(xy.second, 2)) - (0.48 * xy.first * xy.second));
    };

   domain["beale"] = {-4.5,4.5};
   domain["himmel"] = {-5,5};
   domain["threeHumpCamel"] = {-5,5};
   domain["matyas"] = {-10,10};

   if( remove( "climbing.txt" ) != 0 )
       cout << "Creating file\n";
   if( remove( "annealing.txt" ) != 0 )
       cout << "Creating file\n";

   try {
       vector<string> arguments(argv, argv + argc);
       auto selectedFunction = arguments.at(1);
       for (int i = 0; i < 1; i++) {
           cout << "HillClimbing: ";
           cout << hillClimbing(myFunctions.at(selectedFunction), domain.at(selectedFunction), 10000) << endl;
           cout << "Simulated Annealing: ";
           cout << simulatedAnnealing(myFunctions.at(selectedFunction), domain.at(selectedFunction), 10000) << endl;
       }
   }
   catch (std::out_of_range aor) {
       cout << "Blad. Podaj poprawny argument. Dostepne to: ";
       for (auto [k, v] : myFunctions) cout << k << ", ";
       cout << endl;
       return 1;
   }
    //for (int i = 0; i < 8; i++) {
    //    fprintf(gnupipe, "%s\n", gnuCommands[i]);
    //}
   return 0;
}