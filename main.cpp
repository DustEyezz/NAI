#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>
using myfunction_t = std::function<double(std::vector<double>)>;
void calc(std::vector<double> numbers, myfunction_t fun) {
    using namespace std;
    cout << fun(numbers) << endl;
}
int main(int argc, char **argv) {
    using namespace std;
    map<string, myfunction_t> formaters;
    formaters["mod"] = [](vector<double> numbers) { return (int)numbers.front() % (int)numbers.back();};
    formaters["add"] = [](vector<double> numbers) { return  numbers.front() + numbers.back();};
    formaters["sin"] = [](vector<double> numbers) { return sin(numbers.front());};
    try {
        vector<string> arguments(argv, argv + argc);
        auto selected_f = arguments.at(2);
        if (arguments.size() > 5){
            cout << "Blad. Zbyt duzo argumentow: " << arguments.size() - 1;
            return 1;
        }
        else if(selected_f == "sin" && arguments.size() > 4){
            cout << "Blad. Zbyt duzo argumentow przy wyborze sin, max to 1 argument ";
            return 1;
        }
        else if (arguments.at(1) != "lab1"){
            cout << "Blad, brakuje lab1 na poczatku";
            return 1;
        }
        vector<double> numbers = {{stod(arguments.at(3)), stod(arguments.back())}};
        calc(numbers, formaters.at(selected_f));
    } catch (std::out_of_range aor) {
        cout << "Blad. Podaj poprawny argument. Dostepne to: ";
        for (auto [k, v] : formaters) cout << " " << k;
        cout << endl;
        return 1;
    }
    return 0;
}
