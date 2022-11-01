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

auto genetic_algorithm = [](
        auto initial_population, auto fitness, auto term_condition,
        auto selection, double p_crossover,
        auto crossover, double p_mutation,  auto mutation){
    using namespace std;
    uniform_real_distribution<double> uniform(0.0,1.0);
    auto population = initial_population;
    vector<double> population_fit = fitness(population);
    while (!term_condition(population,population_fit)) {
        auto parents_indexes = selection(population_fit);
        decltype(population) new_population;
        for (int i = 0 ; i < parents_indexes.size(); i+=2) {
            decltype(initial_population) offspring = {population[i],population[i+1]};
            if (uniform(mt_generator) < p_crossover) {
                offspring = crossover(offspring);
            }
            for (auto chromosome : offspring) new_population.push_back(chromosome);
        }
        for (auto & chromosome : new_population) {
            chromosome = mutation(chromosome,p_mutation);
        }
        population = new_population;
        population_fit = fitness(population);
    }
    return population;
};
using chromosome_t = std::vector<int>;
using population_t = std::vector<chromosome_t>;
std::vector<double> fitness_function(population_t pop){
    return {};
}
std::vector<int> selection_empty(std::vector<double> fitnesses) {
    return {};
}
std::vector<chromosome_t > crossover_empty(std::vector<chromosome_t > parents) {
    return parents;
}
chromosome_t mutation_empty(chromosome_t parents, double p_mutation) {
    return parents;
}

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

   population_t population = {{1,0,1,0,1,0,1}, {1,0,1,0,1,0,1}};

   auto result = genetic_algorithm(population,
                                   fitness_function,
                                   [](auto a, auto b) { return true; },
                                   selection_empty,
                                   1.0,
                                   crossover_empty,
                                   0.01,
                                   mutation_empty);
   for (chromosome_t chromosome: result) {
        cout << "[";
        for (int p: chromosome) {
            cout << p;
        }
        cout << "] ";
   }
   cout << endl;

/*
   try {
       vector<string> arguments(argv, argv + argc);
       auto selectedFunction = arguments.at(1);
       for (int i = 0; i < 25; i++) {
           cout << "BruteForce: ";
           cout << bruteForce(myFunctions.at(selectedFunction), domain.at(selectedFunction), 10000) << endl;
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
   */
    //for (int i = 0; i < 8; i++) {
    //    fprintf(gnupipe, "%s\n", gnuCommands[i]);
    //}
   return 0;
}