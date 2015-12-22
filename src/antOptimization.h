	/**
	*	Larissa Fernandes Leijôto <larissa.leijoto@gmail.com>
	*/
	
	#ifndef _ANT_OPTIMIZATION_H_
	#define _ANT_OPTIMIZATION_H_
	
	struct Ant 
	{
		double distance;
		std::vector<int> path;
	};
	
	/* Forward definitions. */	
	void antOptimization(int numAnt, int maxtime, int numCities, double evaporatioRate);
	void MakeGraphDistances(std::vector<std::vector<double>> &adjacencies);
	void init_ant(std::vector<Ant> &ants, int index);
	void seed_initial_pheromone(std::vector<std::vector<double>> &pheromone, std::vector<std::vector<double>> &adjacencies, bool random);
	void get_greater_distance(std::vector<std::vector<double>> &pheromone);
	bool check_visit(std::vector<Ant> &ants, int ant_index, int position);
	void build_solution(std::vector<Ant> &ants, std::vector<std::vector<double>> &adjacencies, std::vector<std::vector<double>> &pheromone, int numCities);
	void check_best_distance(std::vector<Ant> &ants);
	void pheromone_evaporates(std::vector<std::vector<double>> &pheromone, double evaporatioRate);
	void update_pheromone(std::vector<Ant> &ants, std::vector<std::vector<double>> &pheromone);
	
	/* Auxiliary methods. */
	double calculate_time(clock_t start, clock_t end);
	int get_random_number(int from, int to);
	void print_path(int ant_index, std::vector<int> path, double distance);
	void print_pheromone(std::vector< std::vector<double>> &pheromone);
	std::string number_to_String(double n);
	void print_result(std::vector<Ant> &ants);
	void statistics(std::vector<Ant> &ants);
	
	#endif /* _ANT_OPTIMIZATION_H_ */
