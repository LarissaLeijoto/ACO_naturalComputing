	/**
	*	Larissa Fernandes Leijôto <larissa.leijoto@gmail.com>
	*/
	#include <stdio.h>
	#include <stdlib.h>
	#include <string>
	#include <time.h>
	#include <vector>
	#include <time.h>
	#include <iostream>
	#include <cstdlib>
	#include <cstdio>     
	#include <cstring> 
	#include "util.h"
	#include "database.h"
	#include "antOptimization.h"
	
	using namespace std;

	
	#define dbg  0
	#define time_program 0

	/* Program parameters. */
	static const char *filename = NULL; 	        /* Name of input files. */
    int numAnts = 0;
    int maxTime = 0;
    int numCities = 0;
    double evaporationRate = 0.0;

	/**
	 * @brief Database.
	 */
	struct database database;

	/**
	 * @brief Prints program usage and exits.
	 * 
	 * @details Prints program usage and exits.
	 */
	static void usage(void)
	{
		printf("Usage: Ant Optimization <nAnts><nIterations><EvaporationRate><file>\n");
		exit(EXIT_SUCCESS);
	}

	/**
	 * @brief Reads command line arguments.
	 * 
	 * @details Reads and parses command line arguments pointed to by argv.
	 * 
	 * @todo Read and parse command line arguments.
	 */
	static void readargs(int argc, char **argv)
	{		
		/* Missing arguments. */
		if (argc < 5)
			usage();
		
		numAnts = atoi(argv[1]);
		maxTime = atoi(argv[2]);
		evaporationRate = double(atoi(argv[3])) / 100;
		filename = argv[4];
		
			
		/* Assert program parameters. */
		if (numAnts <= 0)
			error("invalid number of ants");
		if (maxTime <= 0)
			error("invalid number of iteration");
		if (evaporationRate <= 0.0)
			error("invalid number of evaporation rate");
			
	}
	
	int main(int argc, char **argv)
	{
		readargs(argc, argv);
		/* Initialization. */
		srand( (unsigned)time(NULL) );

		/* Parse database. */
		#if(dbg>0)
		fprintf(stderr, "parsing database... Ok\n");
		#endif
		database_parse(filename);
			
		#if(dbg>0)
		fprintf(stderr, "info: reading database! Ok\n");
		#endif
		/* Read database. */
		database_read(filename);
		
		#if(dbg>0)
		fprintf(stderr, "info: ant colony optimization! Ok\n");
		#endif
		
		numCities = database.nNodes;
		
		#if(dbg>0)
		cout<<"Number cities in problem = "<<numCities<<endl;
        cout<<"Number ants = " << numAnts<<endl;
        cout<<"Maximum time = "<< maxTime<<endl;
        cout<<"Evaporation Rate ="<< evaporationRate<<endl;
        #endif       
        
		
		/* Symbolic Regression. */
		antOptimization(numAnts, maxTime, numCities, evaporationRate);
		
		//print_base();
		
		/* House keeping. */
		database_destroy();
		
		return (EXIT_SUCCESS);
	}
