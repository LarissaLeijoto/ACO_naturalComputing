#include <cstdlib> 
#include <iostream>
#include <string> 
#include <sstream>
#include <math.h>
#include <ctime>  
#include <climits>
#include <cfloat>
#include <vector>
#include "antOptimization.h"
#include "database.h"
#include "util.h"

using namespace std;

/* Constants. */
#define INVALID INT_MIN
#define PHEROMONE_RATE 0.5
#define ALFA 1
#define BETA 4
#define MAX_ITERATIONS 500
#define POSITIVE_CONTS 1

/* Global parameters. */
int best_distance = INT_MIN;
int worse_distance = INT_MAX;
vector<int> best_path;
vector<int> worse_path;
double average = 0.0;
double variance = 0.0;
double standard_deviation = 0.0;
int greater_distance = INVALID;


/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void antOptimization(int numAnts, int maxtime, int numCities, double evaporationRate) 
{
	clock_t time_start = clock();
	
	vector<Ant> ants(numAnts);
	vector<vector<double>> pheromone(numCities, vector<double>(numCities));
	vector<vector<double>> adjacencies (numCities, vector<double>(numCities));
	int count = 0;
	
	srand(time(0));
	//srand(1834);	
	
	MakeGraphDistances(adjacencies);
	
	// Configurando as concetrações iniciais de feromônio
	seed_initial_pheromone(pheromone, adjacencies, false);
	// Pegando a maior distância somado com 1
	get_greater_distance(adjacencies);

	while (count < maxtime) 
	{
		build_solution(ants, adjacencies, pheromone, numCities);
		check_best_distance(ants);
		pheromone_evaporates(pheromone, evaporationRate);
		update_pheromone(ants, pheromone);
		
		
		//cout<<best_distance<<endl;
		//for (int i = 0; i < ants.size(); i++) 
			//print_path(i, ants[i].path, ants[i].distance);
		
		count++;
	}

	print_result(ants);

	cout << "\nExecution Time: " << calculate_time(time_start, clock()) << endl;
	
}

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void MakeGraphDistances(vector<vector<double>> &adjacencies)
{
    for (int i = 0; i < adjacencies.size(); i++)
        for (int j = 0; j < adjacencies.size();  j++)
				adjacencies[i][j] =  INVALID;
				
	for (int i = 0; i < database.nEdges-1; i++)
       	adjacencies[database.points[i][0]][database.points[i][1]] = database.points[i][2];
}

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void init_ant(vector<Ant> &ants, int index)
{
	ants[index].distance = 0;
	
	if(!ants[index].path.empty())
		ants[index].path.clear();
		
	ants[index].path.push_back(database.start);
}

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void seed_initial_pheromone(vector<vector<double>> &pheromone, vector<vector<double>> &adjacencies, bool random) 
{
	for (int i = 0; i < pheromone.size(); i++) 
	{
		for (int j = 0; j < pheromone.size(); j++) 
		{
			if (adjacencies[i][j] != INVALID) 
			{
				double random_pheromone = (double) get_random_number(0, 100)
						/ 100.0;
				pheromone[i][j] = (random == true) ? random_pheromone	: PHEROMONE_RATE;
			}
			else 
				pheromone[i][j] = INT_MIN;
			
		}
	}
}

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void get_greater_distance(vector<vector<double>> &adjacencies)
{
	for (int i = 0; i < adjacencies.size(); i++) 
	{
		for (int j = 0; j < adjacencies.size(); j++) 
		{
			if (adjacencies[i][j] > greater_distance) 
				greater_distance = adjacencies[i][j];
		}
	}
	greater_distance += 1;
}

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
bool check_visit(vector<Ant> &ants, int ant_index, int position)
{
	for (int i = 0; i < ants[ant_index].path.size(); i++) 
	{
		if (ants[ant_index].path[i] == position) 
			return true;
		
	}
	return false;
}

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void build_solution(vector<Ant> &ants, vector<vector<double>> &adjacencies, vector<vector<double>> &pheromone, int numCities) 
{
	// Para cada formiga
	for (int i = 0; i < ants.size(); i++) 
	{
		// inicializando a formiga
		init_ant(ants, i);
		// Enquanto não passar em todas as cidades
		do
		{
			int position = ants[i].path.back();
			
			vector<double> transition_probability(numCities);
			
			double link_rate_sum = 0;
			
			// Somando as taxas de feromonio e heuristica
			for (int j = 0; j < numCities; j++) 
			{
				// Se a cidade já visitada, não entrar na análise
				if (check_visit(ants, i, j) == false)
				{
					if (pheromone[position][j] >= 0 and adjacencies[position][j] >= 0) 
					{
						link_rate_sum += 1/(pow(pheromone[position][j], ALFA) *
										pow( greater_distance - adjacencies[position][j], BETA));
					}
				}
			}
			// Calculando a probabilidade de transição
			for (int j = 0; j < numCities; j++) 
			{
				// Se a cidade já visitada, não entrar na análise
				if (check_visit(ants,i, j) == false) 
				{
					if (pheromone[position][j] >= 0 and adjacencies[position][j] >= 0)
					{
						transition_probability[j] = 1/((pow( pheromone[position][j], ALFA) * 
													 pow(greater_distance - adjacencies[position][j], BETA)))/ link_rate_sum;
					} 
					else 
						transition_probability[j] = 0;
					
				} else
					transition_probability[j] = 0;
			}
			// fazendo a roleta
			double roulette = (double) get_random_number(0, 100) / 100.0;
			double minor = 0;
			double major = 0;

			// Selecionando o próximo nó
			for (int j = 0; j < numCities; j++) 
			{
				// Se a cidade já visitada, não entrar na análise
				if (check_visit(ants, i, j) == false) 
				{
					major += transition_probability[j];
					if (roulette >= minor and roulette <= major)
					{
						ants[i].path.push_back(j);
						ants[i].distance += adjacencies[position][j];
						break;
					} 
					else 
						minor = major;
					
				}
			}
		}while (ants[i].path.back() != database.end);
	}
}

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void check_best_distance(vector<Ant> &ants)
{
	for (int i = 0; i < ants.size(); i++) 
	{
		if (ants[i].distance > best_distance and ants[i].distance > 0) 
		{
			best_distance = ants[i].distance;
			best_path.clear();
			for (int j = 0; j < ants[i].path.size(); j++) 
				best_path.push_back((ants[i].path[j])+1);
			
		} 
		else if (ants[i].distance < worse_distance and ants[i].distance > 0)
		{
			worse_distance = ants[i].distance;
			worse_path.clear();
			for (int j = 0; j < ants[i].path.size(); j++)
				worse_path.push_back((ants[i].path[j])+1);
			
		}
	}
}

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void pheromone_evaporates(vector<vector<double>> &pheromone, double evaporationRate)
{
	for (int i = 0; i < pheromone.size(); i++) 
	{
		for (int j = 0; j < pheromone.size(); j++) 
		{
			if (pheromone[i][j] != INVALID) 
				pheromone[i][j] = (1 - evaporationRate) * pheromone[i][j];
			
		}
	}
}

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void update_pheromone(vector<Ant> &ants, vector< vector<double>> &pheromone) 
{
	for (int i = 0; i < ants.size(); i++) 
	{
		for (int j = 0; j < (ants[i].path.size() - 1); j++) 
		{
			int city = ants[i].path[j];
			int next_city = ants[i].path[j + 1];
			
			if(ants[i].distance > 0)
				if (pheromone[city][next_city] != INVALID) 
					pheromone[city][next_city] += POSITIVE_CONTS/ants[i].distance;
			
		}
	}
}

/********************************************************************************************************************
 * 																													*
 * 													 Auxiliary methods												*
 * 																													*
 *******************************************************************************************************************/

/**
 * 	@brief 
 * 
 * 	@details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
int get_random_number(int from, int to) { return (from < to) ? (rand() % to) + from : 0; }

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
double calculate_time(clock_t start, clock_t end) 
{
	return ((double) (end - start) / (double) CLOCKS_PER_SEC);
}
/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void print_path(int ant_index, vector<int> path, double distance)
{
	string temp = "Ant's path " + number_to_String(ant_index) + " : ";
	for (unsigned int i = 0; i < path.size(); i++) 
	{
		temp += number_to_String(path[i]);
		
		if ((i + 1) != path.size()) 
			temp += ", ";
		
	}
	temp += ". Length: " + number_to_String(distance);
	cout << temp << endl;
}
/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void print_pheromone(vector< vector<double>> &pheromone) 
{
	string temp = "Pheromone rate:";
	temp += "\n{";
	for (int i = 0; i < pheromone.size(); i++)
	{
		temp += "{";
		for (int j = 0; j < pheromone.size(); j++)
		{
			temp += number_to_String(pheromone[i][j]);
			if ((j + 1) != pheromone.size()) 
				temp += ", ";
			
		}
		if ((i + 1) != pheromone.size()) 
			temp += "}, ";
		else 
			temp += "}";
	}
	temp += "}\n";
	cout << temp << endl;
}

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void print_result(vector<Ant> &ants) 
{
	//cout << "Worse result:" << endl;
	//print_path(0, worse_path, worse_distance);

	//cout << "Best result:" << endl;
	print_path(0, best_path, best_distance);

	statistics(ants);
	//cout << "Mean:" << average << endl;
	//cout << "Variance:" << variance << endl;
	//cout << "Standard Deviation:" << standard_deviation << endl;
}

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
string number_to_String(double n) 
{
	stringstream out;
	out << n;
	return out.str();
}

/**
 * @brief 
 *
 * @details 
 *
 * @param 
 *
 * @returns 
 * 
 * @todo 
 */
void statistics(vector<Ant> &ants) 
{
	// Calcular a média
	double sum = 0;
	
	for (int i = 0; i < ants.size(); i++) 
	{
		if(ants[i].distance > 0)
			sum += ants[i].distance;
	}
	
	average = (double) sum / (double) ants.size();
	
	// Calcuar a variância
	sum = 0;
	for (int i = 0; i < ants.size(); i++) 
	{
		if(ants[i].distance > 0)
			sum += pow(ants[i].distance - average, 2);
	}
	
	variance = (double) sum / (double) ants.size();
	// Calculando o desvio padrão
	standard_deviation = pow(variance, 0.5);
}
