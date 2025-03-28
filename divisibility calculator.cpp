#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <cassert>
#include <chrono>
#include <set>
#include <algorithm>
#include <iterator>

using namespace std;

struct cashe {
    map<int, pair<bool, bool>> simple_perfect;
    map<pair<int, int>, int> gcdcache;
    map<pair<int, int>, long long> lcmcache;
    map<int, vector<int>> divisors;
};

pair<bool, bool> check_perfect_simple(int, cashe&);
int gcd(int, int, cashe&);
long long lcm(int, int, cashe&);
pair<int, int> simplification(int, int, cashe&);
vector<int> find_divisors(int, cashe&);
void analysis (int, int, cashe&);
void calculator(cashe&);
void run_tests();
void test_check_perfect_simple();
void test_gcd();
void test_lcm();
void test_simplification();

int main(){
    run_tests();
    cout << "Введите нужную функцию: analyzer или calculator\nФормат ввода:\nдля анализа делимости\nanalyzer\nчисло_1 число_2\nили для калькулятора\ncalculator\n*/* знак операции */*\n";
    string choice;
    cashe answers;
    do{
        cin >> choice;
        if (choice == "analyzer"){
            int number_1, number_2;
            cin >> number_1 >> number_2;
            analysis(number_1, number_2, answers);
        }
        else if (choice == "calculator")
            calculator(answers);
    } while (choice != "end");
}

vector<int> find_divisors(int number, cashe& answers){
    vector<int> div;
    if (answers.divisors.count(number))
        return answers.divisors[number];
    for (int i = 1; i <= sqrt(number); i++){
        if (number % i == 0){
            div.push_back(i);
            if (number / i != i)
                div.push_back(number / i);
        }
    }
    sort(div.begin(), div.end());
    return answers.divisors[number] = div;
}

pair<bool, bool> check_perfect_simple(int number, cashe& answers){
    if (answers.simple_perfect.count(number))
        return {answers.simple_perfect[number].first, answers.simple_perfect[number].second};
    vector<int> div = find_divisors(number, answers);
    int sum_digits = 0;
    for (auto num: div)
        sum_digits += num;
    return {answers.simple_perfect[number].first = div.size() <= 2, answers.simple_perfect[number].second = (sum_digits == 2 * number && number != 1)};
}

int gcd(int number_1, int number_2, cashe& answers){
    pair<int, int> key = {min(number_1, number_2), max(number_1, number_2)};
    if (answers.gcdcache.count(key))
        return answers.gcdcache[key];
    vector<int> div_num1 = find_divisors(number_1, answers), div_num2 = find_divisors(number_2, answers);
    auto it = div_num1.end();
    while (it >= div_num1.begin()){
        if (count(div_num2.begin(), div_num2.end(), *it))
            return answers.gcdcache[key] = *it;
        it--;
    }
}

long long lcm (int number_1, int number_2, cashe& answers){
    pair<int ,int> key = {min(number_1, number_2), max(number_1, number_2)};
    if (answers.lcmcache.count(key))
        return answers.lcmcache[key];
    return answers.lcmcache[key] = (long long)number_1 / gcd(number_1, number_2, answers) * number_2;
}

pair<int, int> simplification(int num, int den, cashe& answers){
    int simply = gcd(abs(num), abs(den), answers);
    return {num / simply, den / simply};
}

void analysis (int number_1, int number_2, cashe& answers){
    auto start = chrono::high_resolution_clock::now();
    vector<int> numbers = {number_1, number_2};
    for (auto it = numbers.begin(); it != numbers.end(); it++){
        cout << "Число " << *it << (check_perfect_simple(*it, answers).first ? " простое" : " НЕ простое") << endl;
        cout << "Число " << *it << (check_perfect_simple(*it, answers).second ? " совершенное" : " НЕ совершенное") << endl;
    }
    cout << "Наибольший общий делитель => " << gcd(number_1, number_2, answers) << endl;
    cout << "Наименьшее общее кратное => " << lcm(number_1, number_2, answers) << endl;
    auto end = chrono:: high_resolution_clock:: now();
    chrono::duration<double, milli> float_ms = end - start;
    cout << "Время работы кода:" << float_ms.count() << "\n\n";
}

void calculator(cashe& answers){
    int num_1, den_1, num_2, den_2;
    char fraction_1, fraction_2, sign;
    cin >> num_1 >> fraction_1 >> den_1 >> sign >> num_2 >> fraction_2 >> den_2;
    int new_num, new_den;
    auto start = chrono::high_resolution_clock::now();
    if (sign == '+')
        new_num = num_1 * den_2 + num_2 * den_1;
    else 
        new_num = num_1 * den_2 - num_2 * den_1;
    new_den = den_1 * den_2;
    pair<int, int> result = simplification(new_num, new_den, answers);
    cout << result.first << "/" << result.second << "\n";
    auto end = chrono:: high_resolution_clock:: now();
    chrono::duration<double, milli> float_ms = end - start;
    cout << "Время выполнения кода: " << float_ms.count() << "\n";
}

void test_check_perfect_simple() {
    cashe ans;
    pair<bool, bool> result = check_perfect_simple(2, ans);
    assert(result.first == true && result.second == false);
    result = check_perfect_simple(4, ans);
    assert(result.first == false && result.second == false);
    result = check_perfect_simple(6, ans);
    assert(result.first == false && result.second == true);
    result = check_perfect_simple(28, ans);
    assert(result.first == false && result.second == true);
    result = check_perfect_simple(10, ans);
    assert(result.first == false && result.second == false);
    result = check_perfect_simple(1, ans);
    assert(result.first == true && result.second == false);
}

void test_gcd() {
    cashe ans;
    assert(gcd(10, 5, ans) == 5);
    assert(gcd(100, 25, ans) == 25);
    assert(gcd(20, 30, ans) == 10);
}

void test_lcm() {
    cashe ans;
    assert(lcm(4, 6, ans) == 12);
    assert(lcm(5, 10, ans) == 10);
    assert(lcm(7, 3, ans) == 21);
}

void test_simplification(){
    cashe ans;
    pair<int, int> result = simplification(10, 20, ans);
    assert(result.first == 1 && result.second == 2); 
    result = simplification(1, 3, ans);
    assert(result.first == 1 && result.second == 3);
    result = simplification (8, 32, ans);
    assert(result.first == 1 && result.second == 4);
}

void run_tests() {
    test_check_perfect_simple();
    test_gcd();
    test_lcm();
    test_simplification();
    cout << "Все тесты прошли успешно!" << endl;
}
