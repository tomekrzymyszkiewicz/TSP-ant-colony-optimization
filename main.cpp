#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <chrono>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "structures.h"
using namespace std;
using namespace std::chrono;

vector<vector<string>> tasks;
vector<string> results;
string results_file_name = "";
int number_of_current_graph_vertices = 0;
adjacency_matrix current_graph_adjacency_matrix = adjacency_matrix();
int *path_array;

struct Result
{
    string graph_name;
    string calculated_path;
    string defined_path;
    int calculated_path_weight;
    int defined_path_weight;
    double time;
    int number_of_repeats;
    float alpha;
    float b;
    int era_length;
    string cooling_method;
    string neighborhood_method;
    Result(string graph_name, string calculated_path, int calculated_path_weight, string defined_path, int defined_path_weight, double time, int number_of_repeats, float alpha, float b, int era_length, string cooling_method, string neighborhood_method)
    {
        this->graph_name = graph_name;
        this->calculated_path = calculated_path;
        this->calculated_path_weight = calculated_path_weight;
        this->defined_path = defined_path;
        this->defined_path_weight = defined_path_weight;
        this->time = time;
        this->number_of_repeats = number_of_repeats;
        this->alpha = alpha;
        this->b = b;
        this->era_length = era_length;
        this->cooling_method = cooling_method;
        this->neighborhood_method = neighborhood_method;
    }
    string toString()
    {
        return (graph_name + "," + calculated_path + "," + to_string(calculated_path_weight) + "," + defined_path + "," + to_string(defined_path_weight) + "," + to_string(time) + "," + to_string(number_of_repeats) + "," + to_string(alpha) + "," + to_string(b) + "," + to_string(era_length) + "," + cooling_method + "," + neighborhood_method);
    }
};

void save_results(string results_file_name)
{
    std::cout << "Saving results" << endl;
    fstream fout;
    fout.open(results_file_name, ios::out);
    fout << "graph_name,calculated_path,calculated_path_weight,defined_path,defined_path_weight,time,number_of_repeats,alpha,b,era_length,cooling_method,neighborhood_method" << endl;
    for (long unsigned int i = 0; i < results.size(); i++)
    {
        fout << results[i] << endl;
    }
    fout.close();
    std::cout << "Correctly saved " << results.size() << " results" << endl;
}

template <typename Out>
void split(const std::string &s, char delim, Out result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        *result++ = item;
    }
}
std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

bool load_data(string file_name)
{
    std::cout << "Loading data from " << file_name << " file" << endl;
    ifstream fin;
    fin.open(file_name, ios::in);
    if (!fin.good())
    {
        std::cout << "Data file " << file_name << " not exist" << endl;
        fin.close();
        return false;
    }
    string loaded_source, loaded_destination, loaded_weight;
    string loaded_number_of_vertices;
    getline(fin, loaded_number_of_vertices);
    number_of_current_graph_vertices = stoi(loaded_number_of_vertices);
    current_graph_adjacency_matrix = adjacency_matrix(number_of_current_graph_vertices);
    for (int i = 0; i < number_of_current_graph_vertices; i++)
    {
        string loaded_line_of_matrix = "";
        getline(fin, loaded_line_of_matrix);
        ltrim(loaded_line_of_matrix);
        rtrim(loaded_line_of_matrix);
        vector<string> single_line = split(loaded_line_of_matrix, ' ');
        std::vector<std::string>::iterator it = single_line.begin();
        while (it != single_line.end())
        {
            if (it->length() == 0)
            {
                it = single_line.erase(it);
            }
            else
            {
                ++it;
            }
        }
        for (long unsigned int j = 0; j < single_line.size(); j++)
        {
            ltrim(single_line[j]);
            rtrim(single_line[j]);
            current_graph_adjacency_matrix.add_edge_dir(i, j, stoi(single_line[j]));
        }
    }
    std::cout << "Loaded correctly graph with " << number_of_current_graph_vertices << " vertices" << endl
              << "Graph:" << endl;
    // current_graph_adjacency_matrix.print();
    fin.close();
    return true;
}

void load_config()
{
    std::cout << "Loading config.ini" << endl;
    ifstream fin;
    fin.open("config.ini", ios::in);
    if (!fin.good())
    {
        std::cout << "Config.ini not found" << endl;
        fin.close();
        return;
    }
    string loaded_line_of_task = "";
    getline(fin, results_file_name);
    ltrim(results_file_name);
    rtrim(results_file_name);
    std::cout << "Loaded result file name: " << results_file_name << endl;
    while (getline(fin, loaded_line_of_task))
    {
        vector<string> single_line = split(loaded_line_of_task, ' ');
        std::vector<std::string>::iterator it = single_line.begin();
        while (it != single_line.end())
        {
            if (it->length() == 0)
            {
                it = single_line.erase(it);
            }
            else
            {
                ltrim(*it);
                rtrim(*it);
                ++it;
            }
        }
        string graph_file_name, number_of_repeats, alpha, b, era_length, cooling_method, neighborhood_method, shortest_path_weight, shortest_path;
        if (single_line.size() >= 8)
        {
            graph_file_name = single_line[0];
            number_of_repeats = single_line[1];
            alpha = single_line[2];
            b = single_line[3];
            era_length = single_line[4];
            cooling_method = single_line[5];
            neighborhood_method = single_line[6];
            shortest_path_weight = single_line[7];
            for (long unsigned int i = 8; i < single_line.size(); i++)
            {
                shortest_path += single_line[i];
                shortest_path += " ";
            }
        }
        if (graph_file_name.size() == 0 || number_of_repeats.size() == 0 || alpha.size() == 0 || b.size() == 0 || era_length.size() == 0 || cooling_method.size() == 0 || neighborhood_method.size() == 0 || shortest_path_weight.size() == 0)
        {
            std::cout << "Cannot load this task: " << loaded_line_of_task << endl;
        }
        else
        {
            vector<string> task;
            task.push_back(graph_file_name);
            task.push_back(number_of_repeats);
            task.push_back(alpha);
            task.push_back(b);
            task.push_back(era_length);
            task.push_back(cooling_method);
            task.push_back(neighborhood_method);
            task.push_back(shortest_path_weight);
            task.push_back(shortest_path);
            tasks.push_back(task);
            std::cout << "Correclty loaded task: ";
            for (long unsigned int i = 0; i < task.size(); i++)
                std::cout << task[i] << " ";
            std::cout << endl;
        }
    }
    fin.close();
    std::cout << "Config loaded correctly" << endl;
    return;
}

int cost_of_permutation(vector<int> permutation)
{
    int cost = 0;
    for(int i = 0; i < permutation.size()-1; i++)
    {
        cost += current_graph_adjacency_matrix.matrix[permutation[i]][permutation[i + 1]];
    }
    // int cost = current_graph_adjacency_matrix.matrix[0][permutation[0]];
    // for (int i = 0; i < number_of_current_graph_vertices - 2; i++)
    // {
    //     cost += current_graph_adjacency_matrix.matrix[permutation[i]][permutation[i + 1]];
    // }
    // cost += current_graph_adjacency_matrix.matrix[permutation[permutation.size() - 1]][0];
    return cost;
}

vector<int> initial_permutation()
{
    vector<int> permutation;
    vector<int> unused;
    for (int i = 2; i < number_of_current_graph_vertices; i++)
        unused.push_back(i);
    int prev_v = 1;
    permutation.push_back(prev_v);
    for (int i = 1; i < number_of_current_graph_vertices - 1; i++)
    {
        int min_v = 1;
        int min_cost = INT32_MAX;
        auto it = unused.begin();
        while (it != unused.end())
        {
            if (current_graph_adjacency_matrix.matrix[prev_v][(*it)] < min_cost && i != (*it))
            {
                min_cost = current_graph_adjacency_matrix.matrix[i][(*it)];
                min_v = (*it);
            }
            ++it;
        }
        permutation.push_back(min_v);
        it = unused.begin();
        while (it != unused.end())
        {
            if ((*it) == min_v)
            {
                it = unused.erase(it);
            }
            else
            {
                ++it;
            }
        }
        prev_v = min_v;
    }
    return permutation;
}


string print_vector(vector<int> printed_vector)
{
    string answer;
    for (long unsigned int i = 0; i < printed_vector.size(); i++)
    {
        answer += to_string(printed_vector[i]);
        answer += " ";
    }
    return answer;
}


float approximated_solution_cost(vector<int> permutation, float alpha){
    return cost_of_permutation(permutation) * alpha;
}

float** init_pheromone_matrix(int number_of_ants,float approximated_sol){
    float** matrix = new float*[number_of_ants];
    float init_tau = number_of_ants/approximated_sol;
    for(int i = 0; i < number_of_ants; i++){
        matrix[i] = new float[number_of_ants];
        for(int j = 0; j < number_of_ants; j++){
            matrix[i][j] = init_tau;
        }
    }
    return matrix;
}


vector<ant> init_colony(int number_of_ants){
    vector<ant> colony;
    for(int i = 0; i < number_of_ants; i++){
        colony.push_back(ant(number_of_current_graph_vertices,(i%(number_of_current_graph_vertices-1))+1));
    }
    return colony;
}

bool sort_by_sec(const pair<int,float> &a,
              const pair<int,float> &b)
{
    return (a.second < b.second);
}

int select_vertex(ant current, float alpha, float beta,float** pheromone_matrix){
    vector<pair<int,float>> probabilities; //prawdopodobieństwa wierzchołek prawdopodobieństwo
    float denominator = 0;
    float nominator = 0;
    for(vector<int>::iterator it = current.unvisited.begin(); it != current.unvisited.end(); it++){
        if(current_graph_adjacency_matrix.matrix[current.current_vertex][(*it)] != 0){
            denominator += (float)pow(pheromone_matrix[current.current_vertex][(*it)],alpha)*(float)pow(1/(float)current_graph_adjacency_matrix.matrix[current.current_vertex][(*it)],beta);
        }
        else{
            denominator += (float)pow(pheromone_matrix[current.current_vertex][(*it)],alpha)*(float)pow(1,beta);
        }
        
    }
    for(int i = 1; i < number_of_current_graph_vertices; i++){
        if(find(current.path.begin(),current.path.end(),i) != current.path.end() || i == current.current_vertex){
            probabilities.push_back(make_pair(i,0));
            continue;    
        } 
        nominator = (float)pow(pheromone_matrix[current.current_vertex][i],alpha)*(float)pow(1/(float)current_graph_adjacency_matrix.matrix[current.current_vertex][i],beta);
        probabilities.push_back(make_pair(i,nominator/denominator));
    }
    sort(probabilities.begin(),probabilities.end(),sort_by_sec);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);
    float random = dis(gen);
    float probabilities_sum = 0;
    for(int i = 0; i < probabilities.size(); i++){
        probabilities_sum += probabilities[i].second;
        if(probabilities_sum > random) return probabilities[i].first;
    }
}

float** evaporateCAS(float** pheromone_matrix, vector<ant> colony, float rho,int q_cycles){
    for(int i = 0; i < number_of_current_graph_vertices; i++){
        for(int j = 0; j < number_of_current_graph_vertices; j++){
            pheromone_matrix[i][j] *= rho;
        }
    }
    for(vector<ant>::iterator it = colony.begin(); it != colony.end(); it++){
        int cost_of_path = cost_of_permutation(it->path);
        for(int i = 0; i < it->path.size()-1 ;i++){
            pheromone_matrix[it->path[i]][it->path[i+1]] += (float)q_cycles/cost_of_path;
        }
    }
    return pheromone_matrix;
}
/**
 * TSP solving with simulated annealing method
 * 
 * @return pair of calculated path and weight
 */
pair<vector<int>, int> TSP_solve()
{
    vector<int> permutation = initial_permutation();
    int cost = cost_of_permutation(permutation) + current_graph_adjacency_matrix.matrix[0][permutation[0]] + current_graph_adjacency_matrix.matrix[permutation[permutation.size()-1]][0];
    int iterations = 100;
    int number_of_ants = number_of_current_graph_vertices;
    float alpha = 1; // wpływ feromonu na decyzję
    float rho = 0.6; // współczynnik parowania z przedziału 0,1 0 - wszystko wyparowuje 1 - nic nie wyparowuje
    float beta = 3; // wpływ długości krawędzi na decyzję
    float evaporation = 0.5;
    int q_cycles = 100;
    float approximated_sol = approximated_solution_cost(permutation,alpha);
    float** pheromone_matrix = init_pheromone_matrix(number_of_ants,approximated_sol);

    for(int iteration = 0; iteration < iterations; iteration ++){
        vector<ant> colony = init_colony(number_of_ants);
        for(vector<ant>::iterator ant_it = colony.begin(); ant_it != colony.end(); ant_it++){
        // for(int ant_num = 0; ant_num < number_of_ants; ant_num++){
            for(int i = 0; i < number_of_current_graph_vertices - 2; i++){
                int next_vertex = select_vertex((*ant_it),alpha,beta,pheromone_matrix);
                (*ant_it).go_to_vertex(next_vertex);
            }
            vector<int> ant_permutation = ant_it->path;
            int ant_cost = cost_of_permutation(ant_it->path) + current_graph_adjacency_matrix.matrix[0][ant_it->path[0]] + current_graph_adjacency_matrix.matrix[ant_it->path[ant_it->path.size()-1]][0];
            if(ant_cost < cost){
                cost = ant_cost;
                permutation = ant_it->path;
            }
        }
        pheromone_matrix = evaporateCAS(pheromone_matrix,colony,rho,q_cycles);
    }
    permutation.insert(permutation.begin(),0);
    permutation.push_back(0);
    return make_pair(permutation, cost);
}

int main()
{
    load_config();
    std::cout << "Tasks loaded.\nPress any key to continue...";
    getchar();
    if (tasks.size() == 0)
    {
        std::cout << "No tasks found to be performed." << endl;
    }
    else
    {
        for (long unsigned int i = 0; i < tasks.size(); i++)
        {
            std::cout << endl
                      << "##################################################" << endl
                      << endl;
            string graph_file_name = tasks[i][0];
            int number_of_repeats = stoi(tasks[i][1]);
            float alpha = stof(tasks[i][2]);
            float b = stof(tasks[i][3]);
            int era_length = stoi(tasks[i][4]);
            bool cooling_method = true;
            if (tasks[i][5] == "geo")
                cooling_method = false;
            else if (tasks[i][5] == "boltzmann")
                cooling_method = true;
            bool neighborhood_method = true;
            if (tasks[i][6] == "swap")
                neighborhood_method = false;
            else if (tasks[i][6] == "invert")
                neighborhood_method = true;
            string shortest_path_weight = tasks[i][7];
            string shortest_path = tasks[i][8];
            ltrim(shortest_path);
            rtrim(shortest_path);
            if (!load_data(graph_file_name))
            {
                std::cout << "Cannot load graph from " << graph_file_name << " file." << endl;
            }
            std::cout << "Computing TSP in " << graph_file_name << " graph repeated " << number_of_repeats << " times" << endl
                      << "alpha: " << alpha << endl
                      << "era_length: " << era_length << endl
                      << "cooling method: " << tasks[i][5] << endl;
            if (cooling_method)
                std::cout << "b: " << b << endl;
            std::cout << "neighborhood method: " << tasks[i][6] << endl
                      << "defined weight: " << shortest_path_weight << endl
                      << endl;
            if (number_of_current_graph_vertices < 1)
            {
                std::cout << "Cannot execute task. The array must to have at least 1 element.";
            }
            else if (number_of_repeats < 1)
            {
                std::cout << "Cannot execute task. The minimum number of repetitions is 1.";
            }
            else
            {
                pair<vector<int>, int> answer;
                for (int j = 0; j < number_of_repeats; j++)
                {
                    high_resolution_clock::time_point t_start = high_resolution_clock::now();
                    answer = TSP_solve();
                    high_resolution_clock::time_point t_end = high_resolution_clock::now();
                    duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);
                    int weight = answer.second;
                    string path = "";
                    std::vector<int>::iterator it = answer.first.begin();
                    while (it != answer.first.end())
                    {
                        path += to_string(*it);
                        path += " ";
                        it++;
                    }
                    ltrim(path);
                    rtrim(path);
                    std::cout << "Calculated shortest path: " << path << endl
                              << "Defined shortest path:    " << shortest_path << endl
                              << "Calculated weight: " << weight << endl
                              << "Defined weight:    " << shortest_path_weight << endl
                              << "Error: " << 100 * (weight - stoi(shortest_path_weight)) / stof(shortest_path_weight) << "%"
                              << endl
                              << "Time: " << ((double)time_span.count() / (double)number_of_repeats) << " s" << endl
                              << "Task " << i + 1 << " from " << tasks.size() << " | Repeat " << j + 1 << " from " << number_of_repeats << endl
                              << endl;
                    Result result = Result(graph_file_name, path, weight, shortest_path, stoi(shortest_path_weight), time_span.count(), number_of_repeats, alpha, b, era_length, tasks[i][5], tasks[i][6]);
                    results.push_back(result.toString());
                }
            }
        }
    }
    save_results(results_file_name);
    std::cout << "Press any key to continue...";
    getchar();
    return 0;
}