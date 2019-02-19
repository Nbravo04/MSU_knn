/*
Nicholas Bravata
10/30/2017
CSE 232 Section 003
Project 07 knn
*/

#include <iostream>
using std::ostream; using std::cout; using std::cin; using std:: endl; using std::fixed;
#include<cmath>
#include<sstream>
using std::ostringstream;
#include<string>
using std::string;
#include<vector>
using std::vector;
#include<map>
using std::map;
#include<fstream>
using std::ifstream;
#include <iomanip>
using std::setprecision;
#include<utility>
using std::pair;
#include<algorithm>
using std::find; using std::search; using std::find_if;
#include "proj07_functions.h"

//Takes in a string, splits it into its parts as separated by the character delim.
vector<string> split(const string &s, char delim)
{
    string::size_type start = 0;
    vector<string> v;
    auto pos = s.find(delim,start);
    while (pos!= string::npos)
    {
        v.push_back(s.substr(start, pos - start));
        start = pos + 1;
        pos = s.find(delim, start);
    }
    if (start != s.size())
        v.push_back(s.substr(start));
    
    return v;
}

//Reads in every line of data from an already opened ifstream reference and fills in map m. Each line of the file consists of feature_count features (for iris, 4). Each feature is a double and each is separated by a comma. The last element is a string, the class type for this feature combination. An empty map is passed in by reference and filled by the function. 
void read_data(map<vector<double>, string> &m, unsigned int feature_count, ifstream &inf)
{
    string line;
    while (getline(inf,line))
    {
        vector<double> key;
        auto x = split(line, ',');
        string map_str = x[4];
        x.erase(x.end()-1);
        for (unsigned int i=0; i<x.size();i++)
        {
            double val = std::stod(x[i]);
            key.push_back(val);
        }
        m[key]=map_str;
    }
}

//Takes in a single element of the map, a pair<vector<double>, string> , and returns a string. 
string pair_to_string(const pair<vector<double>, string> &p)
{
    ostringstream oss;
    oss<< fixed << setprecision(3);
    auto p1=p.first;
    auto p2=p.second;
    for(unsigned int i=0; i<p1.size();i++)
    {
        oss<< p1[i]<< " ";
    }
    oss<< p2;
    
    return oss.str();
}

//Takes in our map and prints it to the already opened ostream & out
void print_map(const map<vector<double>, string> &m, ostream &out)
{
    for (auto iter=m.cbegin(); iter !=  m.cend(); iter++)
    {
        string p_map = pair_to_string(*iter);
        out<< p_map<< endl;
    }
}

//Takes in two feature vectors, both of the size feature_count, and returns the Euclidean distancebetween those to vectors as a double.
double distance(const vector<double> &v1, const vector<double> &v2, unsigned int feature_count)
{
    double dist;
    double pre_sqrt;
    for (unsigned int i=0;i< feature_count;i++)
    {
        double sum = v2[i]-v1[i];
        pre_sqrt += pow(sum,2);
    }
    dist = sqrt(pre_sqrt);
    return dist;
}

//Takes in our map m (containing the contents of the file we read in) and a feature vector<double> called test, and returns a smaller version of the original map which contains only the k elements, the k nearest neighbors, found in m when measured against test excluding the case test.
map<vector<double>, string> k_neighbors(const map<vector<double>, string> &m, const vector<double> &test, int k)
{
    map<vector<double>,string> k_neigh;
    map<vector<double>, string> new_m=m;
    map<double, vector<double>> holder;
    auto it = new_m.find(test);
    if(it != new_m.end())
        new_m.erase(it);
    for(auto iter=new_m.begin(); iter != new_m.end(); iter++)
    {
        auto x = iter ->first;
        auto y = distance(x, test, 4);
        holder[y] = x;
    }
    int count =0;
    for (auto iterat=holder.begin(); count < k; iterat++)
    {
        auto a = iterat->second;
        auto b = m.find(a);
        k_neigh[a]= b->second;
        count++;
    }
    return k_neigh;
}

//Takes in our map m and one particular map element (a pair) test. Runs k_neighbors to find the k nearest neighbors. Measures whether each neighbor found in the k-neighbors matches with the label of the test element. Returns a double which is the percentage of times the near-neighbors were the correct label (the one that came in with test)
double test_one(const map<vector<double>, string> &m, pair<vector<double>, string> test, int k)
{
    auto k_neigh = k_neighbors( m, test.first, k);
    double p_sum;
    for (auto iterat=k_neigh.begin(); iterat != k_neigh.end(); iterat++)
    {
        auto x= iterat ->second;
        string p= test.second;
        if (x==p)
            p_sum++;
    }
    double sum = p_sum /k ;
    return sum;
}
