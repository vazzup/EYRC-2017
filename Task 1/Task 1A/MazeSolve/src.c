#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.c"


#define X 25
#define Y 5

#define NORTH(x) (((x) >> 0) & 1)
#define EAST(x) (((x) >> 1) & 1)
#define SOUTH(x) (((x) >> 2) & 1)
#define WEST(x) (((x) >> 3) & 1)

/** Structure Name: Maze
 ** Purpose: Used to store maze information retrieved from file.
 **          The maze is represented in form of a 2D array wrapped in a structure.
 **/
struct Maze
{
    int maze_array[Y][Y];
};

/** Structure Name: Graph
 ** Purpose: Used to store the Graph constructed by the buildGraph() function.
 **          The graph is represented in form of a 2D array wrapped in a structure.
 **/
struct Graph
{
    _Bool graph_array[X][X];
};

/** Structure Name: Path_Array
 ** Purpose: Used to store the final path Array constructed by the findPath() function.
 **          The path is represented in form of a 1D array wrapped in a structure.
 **/
struct Path_Array
{
	int path[X];
};

/** Function Name: init_path()
 ** Input: None
 ** Output: Returns a Path_Array structure in which all elements of array are initialized to -1
 ** Example Call: struct Path_Array path = init_path();
 **/
struct Path_Array init_path()
{
	struct Path_Array path;
	for(int i = 0; i < X; i++)
		path.path[i] = -1;
	return path;
}
/** Function Name: buildGraph()
 ** Input: Accepts a Maze structure as input
 ** Output: Returns a Graph structure as output
 ** Purpose: Constructs a adjacency matrix graph using the maze information passed as argument
 ** Example Call: struct Graph matrix = buildGraph(maze);
 **/
struct Graph buildGraph(struct Maze maze)
{
    struct Graph adj_matrix;
     // Initialize maze_array to all 0s
    for(int i = 0; i < X; i++) {
        for(int j = 0; j < X; j++) {
            adj_matrix.graph_array[i][j] = 0;
        }
    }
    // Create edges after checking for walls
    for(int i = 0; i < Y; i++) {
        for(int j = 0; j < Y; j++) {
            int index = Y * i + j;
            if(!NORTH(maze.maze_array[i][j])) {
                if(i - 1 >= 0) {
                    adj_matrix.graph_array[index][index - Y] = 1;
                    adj_matrix.graph_array[index - Y][index] = 1;
                }
            }

            if(!EAST(maze.maze_array[i][j])) {
                if(j + 1 < Y) {
                    adj_matrix.graph_array[index][index + 1] = 1;
                    adj_matrix.graph_array[index + 1][index] = 1;
                }
            }

            if(!SOUTH(maze.maze_array[i][j])) {
                if(i + 1 < Y) {
                    adj_matrix.graph_array[index][index + Y] = 1;
                    adj_matrix.graph_array[index + Y][index] = 1;
                }
            }

            if(!WEST(maze.maze_array[i][j])) {
                if(j - 1 >= 0) {
                    adj_matrix.graph_array[index][index - 1] = 1;
                    adj_matrix.graph_array[index - 1][index] = 1;
                }
            }
        }
    }

    // Uncomment to print the adjacency matrix
    /*for(int i = 0; i < X; i++) {
        for(int j = 0; j < X; j++) {
            printf("%d ", adj_matrix.graph_array[i][j]);
        }
        printf("\n");
    }*/
    return adj_matrix;
}
/** Function Name: findPath
 ** Input: graph - Graph structure
 **        src - start point of path
 **        dst - end point of path
 ** Output: Returns a Path_Array structure which contains the path in the maze from src to dst.
 ** Example Call: struct Path_Array path = findPath(graph, 0, 24)
 **/
struct Path_Array findPath(struct Graph graph, int start, int finish)
{
	struct Path_Array path = init_path();
    // We will use BFS
    Queue *q = (Queue *)malloc(sizeof(Queue)); // Queue for BFS
    init_queue(q);
    int visited[X] = {0};
    int parent[X] = {-1};
    int depth[X] = {0};
    enqueue(q, start);
    visited[start] = 1;
    // Actual BFS
    while(!is_empty(q)) {
        // Uncomment the line below to debug queue
        // print_queue(q);
        int curr = peek(q);
        dequeue(q);
        visited[curr] = 1;
        for(int i = 0; i < X; i++) {
            if(graph.graph_array[curr][i] == 1 && visited[i] == 0) {
                visited[i] = 1;
                enqueue(q, i);
                parent[i] = curr;
                depth[i] = depth[curr] + 1;
            }
        }
    }
    int index = depth[finish];
    int temp = finish;
    for(; index >=0; index--) {
        path.path[index] = temp;
        temp = parent[temp];
    }
    // Uncomment to print path
    /* for(int i=0; i<=depth[finish]; i++) {
        printf("%d ", path.path[i]);
    }
    printf("\n"); */
    free(q);
    return path;
}

/** Function Name: main_function
 ** Input: maze - Maze structure
 **        start - start point of path
 **        finish - end point of path
 ** Output: Returns a Path_Array structure which contains the path in the maze from start to finish.
 ** Logic: This function first constructs a graph from the maze passed as argument and then finds out
 **        path from start to finish. The path is returned as a Path_Array structure.
 ** Example Call: struct Path_Array path = main_function(maze,0,24);
 **/
struct Path_Array main_function(struct Maze maze, int start, int finish)
{
    struct Graph adjacency_matrix = buildGraph(maze);
    struct Path_Array path = findPath(adjacency_matrix, start, finish);
	return path;
}

/** Function Name: parseFile
 ** Input: fileName -  a string containing the relative path of the test case
 ** Output: Returns a Maze structure.
 ** Logic: This function reads the the specified txt file in the TestCases folder and constructs
           a Maze structure from the information.
 ** Example Call: struct Maze maze = parseFile("..\\..\\Task 1A\\TestCases\\maze_4.txt");
 **/
struct Maze parseFile(char fileName[70])
{
    FILE *fp;
    char buff[100];
    int i = 0, j = 0;
    struct Maze maze;
    fp = fopen(fileName,"r");
    fgets(buff, 100, (FILE*)fp);
    char *token = strtok(buff," ");
    while(token != NULL)
    {
        int temp = atoi(token);
        maze.maze_array[i][j] = temp;
        j++;
        if(j == 5)
        {
            i++;
            j = 0;
        }
        token = strtok(NULL, " ");
    }
    return maze;
}

/**************************************** Utility Functions *********************************/

/** Function Name: coord_to_flat
 ** Input: Accepts coordinates x and y of maze cell as input.
 ** Logic: The maze structure is represented as a 2D array. The cells of the maze can be numbered
 **        in a sequential form (0,1,2,3,4,5....23,24) so that it is easier to construct a graph.
 **        This function accepts the maze cell coordinates and returns an integer.
 ** Output: Returns an integer representing the position of maze cell relative to the start position.
 ** Example Call: coord_to_flat()
 **/
int coord_to_flat(int x, int y)
{
    return x*Y + y;
}

/**************************************** Utility Functions *********************************/
