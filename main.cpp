#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <set>
#include <map>
#include <functional>

using namespace std;

bool generateRandomBool(){
    mt19937 rng(std::random_device{}());
    return uniform_int_distribution<>{0, 1}(rng);
}

int genotype_length(int index){
    int length = 100 + (index % 10) * 2;
    return length;

}

vector<int> generate_genotype(int length){
    vector<int> bits;
    bits.reserve(length);
    for(int i = 0; i < length; i++){
        bits.push_back(int(generateRandomBool()));
    }
    return bits;
}

double binaryToDecimal(int n){
    int num = n;
    double dec_value = 0;
    int base = 1;
    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
        dec_value += last_digit * base;
        base = base * 2;
    }
    return dec_value;
}

vector<double> genotype_into_fenotype(const vector<int>& bits, int length = 110){
    vector<double> fenotype;
    string x;
    string y;
    for(int i = 0; i < length; i++){
        if(i % 20 == 0){
            if(i<55){
                x += to_string(bits[i]);
            }else{
                y += to_string(bits[i]);
            }
        }
    }
    if(x.at(0)=='1'){
        fenotype.push_back(binaryToDecimal(stoi(x)));
    }else{
        fenotype.push_back(binaryToDecimal(-1 * stoi(x)));
    }if(x.at(0)=='1'){
        fenotype.push_back(binaryToDecimal(stoi(y)));
    }else{
        fenotype.push_back(binaryToDecimal(-1 * stoi(y)));
    }

    cout << fenotype.at(0) << endl << fenotype.at(1);
    return fenotype;
}

vector<double> populate(int ammount){
    vector<double> population;
    for(int i = 0; i < ammount; i++){
        vector<int> bits = generate_genotype(genotype_length(23205));
        vector<double> fenotype = genotype_into_fenotype(bits);
        population.push_back(fenotype[0]);
        population.push_back(fenotype[1]);
    }
    return population;
}

///-4.5 <= x, <= 4.5
double beale_function(double x, double y){
    if(-10 > x || x > 10 || -10 > y || y > 10){
        return numeric_limits<double>::max();
    }
    return pow(1.5 - x + (x * y, 2) +
               pow(2.25 - x + x * y, 3), 2) +
           pow(2.625 - x + (pow(x * y, 3)), 2);
}

///-5 <= x, <= 5
double himmelblau_function(double x, double y){
    if(-5>x || x>5 || -5>y || y>5){
        return numeric_limits<double>::max();
    }
    return pow(pow(x,2)+y-11,2) + pow(x+pow(y,2)-7,2);
}

//int calculate_chances(double individual){
//
//}
//
//map<double, vector<double>> fitness(int ammount, const function<double(double , double )>& fun){
//    vector<double> population = populate(ammount);
////    map<double, vector<double>> resultsMap;
//    vector<double> results;
//    for(int i = 0; i < population.size(); i+=2){
//        double result = fun(population[i], population[i + 1]);
//        if(result != numeric_limits<double>::max() && check_if_contains(result, results)){
//            results.push_back(result);
//            resultsMap[result] = {population[i],population[i+1]};
//            cout << "wynik:" << result << " x:" << population[i] << " y:" << population[i+1] << endl;
//        }
//    }
//    return resultsMap;
//}

int main() {
    vector<double> population = populate(100);
    genotype_into_fenotype(population.at(1));   
    return 0;
}

//bool check_if_contains(double number, const vector<double>& list){
//    for(double i : list){
//        if(i == number){
//            return false;
//        }
//    }
//    return true;
//}
//map<double, vector<double>> sortByKey(map<double, vector<double>> inputMap, int amount){
//    map<double, vector<double>> returnMap;
//    for(int i = 0 ; i < amount ; i++){
//        map<double, vector<double>>::iterator pointer = inputMap.begin();
//        for(map<double, vector<double>>::iterator it = inputMap.begin(); it != inputMap.end() ; it.operator++()){
//            if(pointer -> first > it -> first){
//                pointer = it;
//                inputMap.erase(pointer->first);
//            }
//        }
//        returnMap[pointer -> first] = pointer -> second;
//    }
//    return returnMap;
//}
//
//void printMap(const map<double, vector<double>>& inputMap){
//    map<double, vector<double>>::iterator itr;
//    for(auto [a, b]:inputMap){
//    cout<<a<<" "<<a<<endl;
//    }
//}
//int add_randomness(){
//    mt19937 rng(std::random_device{}());
//    return uniform_int_distribution<>{-1, 1}(rng);
//}
