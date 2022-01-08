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

vector<vector<std::string>> tasks;
vector<std::string> results;
std::string results_file_name = "";
int number_of_current_graph_vertices = 0;
adjacency_matrix current_graph_adjacency_matrix = adjacency_matrix();
struct Result
{
    std::string graph_name;
    std::string calculated_path;
    std::string defined_path;
    int calculated_path_weight;
    int defined_path_weight;
    double time;
    int number_of_repeats;
    float alpha;
    float beta;
    float rho;
    int iterations;
    int number_of_ants;
    float init_tau_param;
    int quantity_of_pheromone;
    std::string evaporation_method;
    Result(std::string graph_name, std::string calculated_path, int calculated_path_weight, std::string defined_path, int defined_path_weight, double time, int number_of_repeats, float alpha, float beta, float rho,
           int iterations, int number_of_ants, float init_tau_param, int quantity_of_pheromone, std::string evaporation_method)
    {
        this->graph_name = graph_name;
        this->calculated_path = calculated_path;
        this->calculated_path_weight = calculated_path_weight;
        this->defined_path = defined_path;
        this->defined_path_weight = defined_path_weight;
        this->time = time;
        this->number_of_repeats = number_of_repeats;
        this->alpha = alpha;
        this->beta = beta;
        this->rho = rho;
        this->iterations = iterations;
        this->number_of_ants = number_of_ants;
        this->init_tau_param = init_tau_param;
        this->quantity_of_pheromone = quantity_of_pheromone;
        this->evaporation_method = evaporation_method;
    }
    std::string toString()
    {
        return (graph_name + "," + calculated_path + "," + to_string(calculated_path_weight) + "," + defined_path + "," + to_string(defined_path_weight) + "," + to_string(time) + "," + to_string(number_of_repeats) + "," + to_string(alpha) + "," + to_string(beta) + "," + to_string(rho) + "," + to_string(iterations) + "," + to_string(number_of_ants) + "," + to_string(init_tau_param) + "," + to_string(quantity_of_pheromone) + "," + evaporation_method);
    }
};

void save_results(std::string results_file_name)
{
    std::cout << "Saving results" << endl;
    fstream fout;
    fout.open(results_file_name, ios::out);
    fout << "graph_name,calculated_path,calculated_path_weight,defined_path,defined_path_weight,time,number_of_repeats,alpha,beta,rho,iterations,number_of_ants,init_tau_param,quantity_of_pheromone,evaporation_method" << endl;
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

bool load_data(std::string file_name)
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
    std::string loaded_source, loaded_destination, loaded_weight;
    std::string loaded_number_of_vertices;
    getline(fin, loaded_number_of_vertices);
    number_of_current_graph_vertices = stoi(loaded_number_of_vertices);
    current_graph_adjacency_matrix = adjacency_matrix(number_of_current_graph_vertices);
    for (int i = 0; i < number_of_current_graph_vertices; i++)
    {
        std::string loaded_line_of_matrix = "";
        getline(fin, loaded_line_of_matrix);
        ltrim(loaded_line_of_matrix);
        rtrim(loaded_line_of_matrix);
        vector<std::string> single_line = split(loaded_line_of_matrix, ' ');
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
    std::string loaded_line_of_task = "";
    getline(fin, results_file_name);
    ltrim(results_file_name);
    rtrim(results_file_name);
    std::cout << "Loaded result file name: " << results_file_name << endl;
    while (getline(fin, loaded_line_of_task))
    {
        vector<std::string> single_line = split(loaded_line_of_task, ' ');
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
        std::string graph_file_name, number_of_repeats, alpha, beta, rho, iterations, number_of_ants, init_tau_param, quantity_of_pheromone, evaporation_method, shortest_path_weight, shortest_path;
        if (single_line.size() >= 10)
        {
            graph_file_name = single_line[0];
            number_of_repeats = single_line[1];
            alpha = single_line[2];
            beta = single_line[3];
            rho = single_line[4];
            iterations = single_line[5];
            number_of_ants = single_line[6];
            init_tau_param = single_line[7];
            quantity_of_pheromone = single_line[8];
            evaporation_method = single_line[9];
            shortest_path_weight = single_line[10];
            for (long unsigned int i = 11; i < single_line.size(); i++)
            {
                shortest_path += single_line[i];
                shortest_path += " ";
            }
        }
        if (graph_file_name.size() == 0 || number_of_repeats.size() == 0 || alpha.size() == 0 || beta.size() == 0 || rho.size() == 0 || iterations.size() == 0 || number_of_ants.size() == 0 || number_of_ants.size() == 0 || init_tau_param.size() == 0 || quantity_of_pheromone.size() == 0 || evaporation_method.size() == 0 || shortest_path_weight.size() == 0)
        {
            std::cout << "Cannot load this task: " << loaded_line_of_task << endl;
        }
        else
        {
            vector<std::string> task;
            task.push_back(graph_file_name);
            task.push_back(number_of_repeats);
            task.push_back(alpha);
            task.push_back(beta);
            task.push_back(rho);
            task.push_back(iterations);
            task.push_back(number_of_ants);
            task.push_back(init_tau_param);
            task.push_back(quantity_of_pheromone);
            task.push_back(evaporation_method);
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
    int cost = current_graph_adjacency_matrix.matrix[0][permutation[0]]+current_graph_adjacency_matrix.matrix[permutation[permutation.size()-1]][0];
    for (long unsigned int i = 0; i < permutation.size() - 1; i++)
    {
        cost += current_graph_adjacency_matrix.matrix[permutation[i]][permutation[i + 1]];
    }
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

std::string print_vector(vector<int> printed_vector)
{
    std::string answer;
    for (long unsigned int i = 0; i < printed_vector.size(); i++)
    {
        answer += to_string(printed_vector[i]);
        answer += " ";
    }
    return answer;
}

float **init_pheromone_matrix(int number_of_ants, int number_of_vertices, float approximated_sol, float init_tau_param)
{
    float **matrix = new float *[number_of_vertices];
    float init_tau = (number_of_ants / approximated_sol) * init_tau_param;
    for (int i = 0; i < number_of_vertices; i++)
    {
        matrix[i] = new float[number_of_vertices];
        for (int j = 0; j < number_of_vertices; j++)
        {
            matrix[i][j] = init_tau;
        }
    }
    return matrix;
}

vector<ant> init_colony(int number_of_ants)
{
    vector<ant> colony;
    for (int i = 0; i < number_of_ants; i++)
    {
        colony.push_back(ant(number_of_current_graph_vertices, (i % (number_of_current_graph_vertices - 1)) + 1));
    }
    return colony;
}

bool sort_by_sec(const pair<int, float> &a, const pair<int, float> &b)
{
    return (a.second < b.second);
}

int select_vertex(ant current, float alpha, float beta, float **pheromone_matrix)
{
    vector<pair<int, float>> probabilities;
    float denominator = 0;
    float nominator = 0;
    for (vector<int>::iterator it = current.unvisited.begin(); it != current.unvisited.end(); it++)
    {
        if (current_graph_adjacency_matrix.matrix[current.current_vertex][(*it)] > 0)
        {
            denominator += (float)pow(pheromone_matrix[current.current_vertex][(*it)], alpha) * (float)pow(1 / (float)current_graph_adjacency_matrix.matrix[current.current_vertex][(*it)], beta);
        }
        else // 0 costs edges case
        {
            denominator += (float)pow(pheromone_matrix[current.current_vertex][(*it)], alpha) * (float)pow(1 / 0.1, beta);
        }
    }
    for (int i = 1; i < number_of_current_graph_vertices; i++)
    {
        if (find(current.path.begin(), current.path.end(), i) != current.path.end() || i == current.current_vertex)
        {
            probabilities.push_back(make_pair(i, 0));
            continue;
        }
        if (current_graph_adjacency_matrix.matrix[current.current_vertex][i] > 0)
        {
            nominator = (float)pow(pheromone_matrix[current.current_vertex][i], alpha) * (float)pow(1 / (float)current_graph_adjacency_matrix.matrix[current.current_vertex][i], beta);
        }
        else // 0 costs edges case
        {
            nominator = (float)pow(pheromone_matrix[current.current_vertex][i], alpha) * (float)pow(1 / 0.1, beta);
        }
        probabilities.push_back(make_pair(i, nominator / denominator));
    }
    sort(probabilities.begin(), probabilities.end(), sort_by_sec);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);
    float probability = dis(gen);
    float probabilities_sum = 0; //probabilites sum up to 1
    for (long unsigned int i = 0; i < probabilities.size(); i++)
    {
        probabilities_sum += probabilities[i].second;
        if (probabilities_sum > probability)
            return probabilities[i].first;
    }
    return INT32_MAX; // shouldn`t never be reached
}

float **evaporateCAS(float **pheromone_matrix, vector<ant> colony, float rho, int quantity_of_pheromone)
{
    for (int i = 0; i < number_of_current_graph_vertices; i++)
    {
        for (int j = 0; j < number_of_current_graph_vertices; j++)
        {
            pheromone_matrix[i][j] *= rho;
        }
    }
    for (vector<ant>::iterator it = colony.begin(); it != colony.end(); it++)
    {
        int cost_of_path = cost_of_permutation(it->path);
        for (long unsigned int i = 0; i < it->path.size() - 1; i++)
        {
            pheromone_matrix[it->path[i]][it->path[i + 1]] += (float)quantity_of_pheromone / cost_of_path;
        }
    }
    return pheromone_matrix;
}


float **evaporateQAS(float **pheromone_matrix, float rho)
{
    for (int i = 0; i < number_of_current_graph_vertices; i++)
    {
        for (int j = 0; j < number_of_current_graph_vertices; j++)
        {
            pheromone_matrix[i][j] *= rho;
        }
    }
    return pheromone_matrix;
}

float **add_pheromone_QAS(float **pheromone_matrix, int i, int j, int quantity_of_pheromone)
{
    if(current_graph_adjacency_matrix.matrix[i][j] > 0){
        pheromone_matrix[i][j] += (float)quantity_of_pheromone/current_graph_adjacency_matrix.matrix[i][j];
    }
    else{
        pheromone_matrix[i][j] += (float)quantity_of_pheromone/0.1;
    }
    return pheromone_matrix;
}
/**
 * TSP solving with simulated annealing method
 * 
 * @return pair of calculated path and weight
 */
pair<vector<int>, int> TSP_solve(float alpha = 1, float beta = 3, float rho = 0.6, int iterations = 100, int number_of_ants = number_of_current_graph_vertices, float init_tau_param = 1, int quantity_of_pheromone = 100, string evaporation_method = "CAS", int defined_cost = 0)
{
    vector<int> permutation = initial_permutation();
    int cost = cost_of_permutation(permutation);
    float **pheromone_matrix = init_pheromone_matrix(number_of_ants, number_of_current_graph_vertices, cost, init_tau_param);
    // defined number of iterations
    for (int iteration = 0; iteration < iterations; iteration++)
    {
        vector<ant> colony = init_colony(number_of_ants);
        // travel of every ant
        int ant_count = 1;
        for (vector<ant>::iterator ant_it = colony.begin(); ant_it != colony.end(); ant_it++)
        {
            std::cout << "Iteration:" << std::right << std::setw(4) << iteration+1 <<"|Ant:" << std::right << std::setw(4) << ant_count << "| Cost: " << std::right << std::setw(8) << cost << "| Error: " << std::right << std::setw(8) << 100 * (cost - defined_cost) / (float)defined_cost << "%" << "\t\r" << std::flush;
            ant_count++;
            // travel of ant_it (N-2 moves)
            for (int i = 0; i < number_of_current_graph_vertices - 2; i++)
            {
                int next_vertex = select_vertex((*ant_it), alpha, beta, pheromone_matrix);
                if(evaporation_method == "QAS")
                    pheromone_matrix = add_pheromone_QAS(pheromone_matrix,(*ant_it).current_vertex,next_vertex,quantity_of_pheromone);
                (*ant_it).go_to_vertex(next_vertex);
            }
            int ant_cost = cost_of_permutation(ant_it->path);
            if (ant_cost < cost)
            {
                cost = ant_cost;
                permutation = ant_it->path;
            }
        }
        if(evaporation_method == "CAS"){
            pheromone_matrix = evaporateCAS(pheromone_matrix, colony, rho, quantity_of_pheromone);
        }
        else if(evaporation_method == "QAS"){
            pheromone_matrix = evaporateQAS(pheromone_matrix, rho);
        }
    }
    for (int i = 0; i < number_of_current_graph_vertices; i++)
    {
        delete[] pheromone_matrix[i];
    }
    delete[] pheromone_matrix;
    permutation.insert(permutation.begin(), 0);
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
            std::cout << endl;
            std::cout << "##################################################" << endl;
            std::cout << endl;
            std::string graph_file_name = tasks[i][0];
            int number_of_repeats = stoi(tasks[i][1]);
            float alpha = stof(tasks[i][2]);
            float beta = stof(tasks[i][3]);
            float rho = stof(tasks[i][4]);
            int iterations = stoi(tasks[i][5]);
            int number_of_ants = stoi(tasks[i][6]);
            float init_tau_param = stoi(tasks[i][7]);
            int quantity_of_pheromone = stoi(tasks[i][8]);
            std::string evaporation_method = tasks[i][9];
            std::string shortest_path_weight = tasks[i][10];
            std::string shortest_path = tasks[i][11];
            ltrim(shortest_path);
            rtrim(shortest_path);
            if (!load_data(graph_file_name))
            {
                std::cout << "Cannot load graph from " << graph_file_name << " file." << endl;
            }
            std::cout << "Computing TSP in " << graph_file_name << " graph repeated " << number_of_repeats << " times" << endl;
            std::cout << "alpha: " << alpha << endl;
            std::cout << "beta: " << beta << endl;
            std::cout << "rho: " << rho << endl;
            std::cout << "iterations: " << iterations << endl;
            std::cout << "number of ants: " << number_of_ants << endl;
            std::cout << "init tau parameter: " << init_tau_param << endl;
            std::cout << "quantity_of_pheromone: " << quantity_of_pheromone << endl;
            std::cout << "evaporation method: " << evaporation_method << endl;
            std::cout << "defined weight: " << shortest_path_weight << endl
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
                    answer = TSP_solve(alpha, beta, rho, iterations, number_of_ants, init_tau_param, quantity_of_pheromone, evaporation_method, stoi(shortest_path_weight));
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
                    Result result = Result(graph_file_name, path, weight, shortest_path, stoi(shortest_path_weight), time_span.count(), number_of_repeats, alpha, beta, rho, iterations, number_of_ants, init_tau_param, quantity_of_pheromone, evaporation_method);
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