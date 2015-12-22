	/**
	*	Larissa Fernandes Leijôto <larissa.leijoto@gmail.com>
	*/
	#include <string.h>
	#include <string>


	#ifndef DATABASE_H_
	#define DATABASE_H_

	struct database
	{
		int **points;   /**<  Nodes to construct the graph.	*/
		int nNodes;     /**<  Number of nodes.	  				*/
		int nEdges;     /**<  Number of edges.					*/
		int start;      /**<  Beginning node.					*/
		int end;	    /**<  Endding node.					*/
	};
	
	
	/* Forward definitions. */
	void database_parse(const char* filename);
	void database_read(const char* filename);
	void database_destroy(void);
	void print_base(void);
	
	/* Forward definitions. */
	extern struct database database;
		

		
	#endif /* DATABASE_H_ */
