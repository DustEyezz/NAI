#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>
using mymap_t = std::map<double, double>;
using myfunction_t = std::function<double(double, double)>;
void calc(mymap_t map1, myfunction_t fun) {
    using namespace std;
    for (auto kv : map1) {
        auto [k, v] = kv;
        cout << fun(k, v) << endl;
    }
}
int main(int argc, char **argv) {
    using namespace std;
    //map<double, double> map1 = {{1, 2}};
    map<string, myfunction_t> formaters;
    formaters["mod"] = [](double x, double y) { return (int)x % (int)y;};
    formaters["add"] = [](double x, double y) { return  x+y; };
    formaters["sin"] = [](double x, double y) { return sin(x+y); };
    try {
        vector<string> arguments(argv, argv + argc);
        //for (auto argument : arguments) cout << " " << argument;
        if (arguments.at(1) != "lab1"){
            cout << "Blad, brakuje lab1";
        }
        auto selected_f = arguments.at(2);
        map<double, double> map1 = {{stof(arguments.at(3)), stof(arguments.at(4))}};
        calc(map1, formaters.at(selected_f));
    } catch (std::out_of_range aor) {
        cout << "Podaj argument. Dostepne to: ";
        for (auto [k, v] : formaters) cout << " " << k;
        cout << endl;
        return 1;
    }
    return 0;
}
