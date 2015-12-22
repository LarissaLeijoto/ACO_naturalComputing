	/**
	*	Larissa Fernandes Leijôto <larissa.leijoto@gmail.com>
	*/
	#include <assert.h>
	#include <stdio.h>
	#include <string.h>
	#include <string>
	#include <stdlib.h>
	#include <cstdio>
	#include <cstdlib>
	#include <limits.h>
	#include <time.h>
	#include <vector>
	#include <fstream>  
	#include <sstream>
	#include <iostream> 

	#include "database.h"
	#include "util.h"


	#define dbg 0
	#define time_program 0

	using namespace std;

	/**
	 * @brief Parses the database.
	 *
	 * @details Parses the database so that I can count the number of lines.
	 *
	 * @param filename Name of input files.
	 *
	 * @returns The number of points.
	 * 
	 * @todo Check for bad file format.
	 */
	void database_parse(const char *filename)
	{
		/* Sanity checks. */
		assert(filename != NULL);

		ifstream inFile;  /* Working File*/
		string line;	  /* Working line*/
			
		/* Open working file. */
		inFile.open(filename, ios::in);
		if (!inFile.is_open())
			error ("cannot open input file");
			
		inFile >> database.nNodes;
		inFile >> database.start;
		inFile >> database.end;
		
		database.start--;
		database.end--;
			
		/* Get number of points. */
		while (getline (inFile,line) )
			database.nEdges++;
			
						
		inFile.close();		
				
		/* Invalid number of points. */
		if(database.nNodes == 0)
			error("invalid number of nodes");
		if(database.nEdges == 0)
			error("invalid number of edges");
	}

	/**
	 * @brief Reads the database into memory.
	 *
	 * @details Reads the database into memory so we can speedup computation.
	 *
	 * @param filenames   Name of input files.
	 *
	 * @returns The database.
	 * 
	 * @todo Check for bad file format.
	 */
	void database_read(const char *filename)	
	{
		/* Sanity check. */
		assert(filename != NULL);
		
		/* Declarations*/
		ifstream inFile;  /* Working File*/
		int line;	  /* Working line*/
		
		/* Allocate database. */
		database.points = (int **)smalloc(database.nEdges*sizeof(int *));
		for (int i = 0; i < database.nEdges-1; i++)
			database.points[i] = (int *)scalloc(3,sizeof(int));

		/* Open working file.*/
		inFile.open(filename, ios::in);
			if (!inFile.is_open())
				error ("cannot open input file");
		
		inFile >>line;
		inFile >>line;
		inFile >>line;

		
		for(int i = 0; i < database.nEdges-1; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				inFile >> database.points[i][j];
				
					if(j != 2)
						database.points[i][j] = database.points[i][j] - 1;
			}
		}
		
		inFile.close();
	}

	/**
	 * @brief Destroys the database.
	 * 
	 * @details Destroys the database, freeing underlying resources.
	 */
	void database_destroy()
	{	
		for(int i = 0; i < database.nEdges-1;i++)
			free(database.points[i]);
		
		free(database.points);	
	}

	/**
	 * @brief Print the database.
	 * 
	 * @details Prints the database, when is necessary.
	 */

	void print_base(void)
	{		
		for(int i = 0; i < database.nEdges-1;i++)
		{
			for(int j = 0; j < 3;j++)
			{
				fprintf(stderr, "%d  ", database.points[i][j]);
			}
			fprintf(stderr, "\n");
		}
		
	}
