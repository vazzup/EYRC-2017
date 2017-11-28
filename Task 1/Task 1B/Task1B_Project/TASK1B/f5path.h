#ifndef PP_EY
#define PP_EY

#include "f5queue.h"
#include "f5lcd.h"

#define X 49
#define Y 7

#define NORTH(x) (((x) >> 0) & 1)
#define EAST(x) (((x) >> 1) & 1)
#define SOUTH(x) (((x) >> 2) & 1)
#define WEST(x) (((x) >> 3) & 1)

#define FNORTH 0
#define FEAST 1
#define FSOUTH 2
#define FWEST 3

#define FWD_C 'F'
#define BK_C 'B'
#define R_C 'R'
#define L_C 'L'

int maze[Y][Y] = {
	{9, 1, 5, 5, 1, 1, 3},
	{8, 2, 15, 15, 8, 0, 2},
	{8, 0, 1, 1, 0, 0, 2},
	{8, 0, 0, 0, 0, 0, 2},
	{8, 0, 0, 4, 0, 0, 2},
	{8, 0, 2, 15, 8, 0, 2},
	{12, 4, 4, 5, 4, 4, 6}
};

int graph[X][X];

void graph_init(void) {
	int i, j;
	for(i=0; i<X; i++) {
		for(j=0; j<X; j++) {
			graph[i][j] = 0;
		}
	}
}

void create_graph(void) {
	int i, j, index;
	graph_init();
	for(i = 0; i < Y; i++) {
		for(j = 0; j < Y; j++) {
			index = (Y * i) + j;
			if(!NORTH(maze[i][j])) {
				if(i - 1 >= 0) {
					graph[index][index - Y] = 1;
					graph[index - Y][index] = 1;
				}
			}

			if(!EAST(maze[i][j])) {
				if(j + 1 < Y) {
					graph[index][index + 1] = 1;
					graph[index + 1][index] = 1;
				}
			}

			if(!SOUTH(maze[i][j])) {
				if(i + 1 < Y) {
					graph[index][index + Y] = 1;
					graph[index + Y][index] = 1;
				}
			}

			if(!WEST(maze[i][j])) {
				if(j - 1 >= 0) {
					graph[index][index - 1] = 1;
					graph[index - 1][index] = 1;
				}
			}
		}
	}
}

void get_path(int start, int finish, int *path_len, int *path) {
	int visited[X] = {0};
    int parent[X] = {-1};
    int depth[X] = {0};
	int index, temp, i, j, k, curr;
	reset_queue();
    enqueue(start);
    visited[start] = 1;
    // Actual BFS
    while(!is_empty()) {
        curr = peek();
        dequeue();
        for(i = 0; i < X; i++) {
            if(graph[curr][i] == 1 && visited[i] == 0) {
                visited[i] = 1;
                enqueue(i);
                parent[i] = curr;
                depth[i] = depth[curr] + 1;
            }
        }
    }
    index = depth[finish];
    temp = finish;
    for(; index >=0; index--) {
		path[index] = temp;
        temp = parent[temp];
		if(temp == -1) break;
    }
	*path_len = depth[finish] + 1;
}

void get_commands(int start, int finish, int init_orientation, int *final_orientation, int *path_len, char *path, int *at_home) {
	int current_orientation = init_orientation;
	int path_int[X + 10] = {-1};
	char *s = path;
	int i, diff, j = 0;
	create_graph();
	get_path(start, finish, path_len, path_int);
	if(*at_home) {
		s[j] = FWD_C;
		j++;
	}
	for(i = 0; i < (*path_len) - 1; i++, j++)
	{
		if(diff == 0) continue;
		diff = path_int[i + 1] - path_int[i];
		switch(current_orientation) {
			case FNORTH: {
				switch(diff) {
					case 1: {
						s[j] = R_C;
						current_orientation = FEAST;
						break;
					}
					case -1: {
						s[j] = L_C;
						current_orientation = FWEST;
						break;
					}
					case Y: {
						s[j] = BK_C;
						current_orientation = FNORTH;
						break;
					}
					case -Y: {
						s[j] = FWD_C;
						current_orientation = FNORTH;
						break;
					}
				};
				break;
			}
			case FEAST: {
				switch(diff) {
					case 1: {
						s[j] = FWD_C;
						current_orientation = FEAST;
						break;
					}
					case -1: {
						s[j] = BK_C;
						current_orientation = FEAST;
						break;
					}
					case Y: {
						s[j] = R_C;
						current_orientation = FSOUTH;
						break;
					}
					case -Y: {
						s[j] = L_C;
						current_orientation = FNORTH;
						break;
					}
				};
				break;
			}
			case FSOUTH: {
				switch(diff) {
					case 1: {
						s[j] = L_C;
						current_orientation = FEAST;
						break;
					}
					case -1: {
						s[j] = R_C;
						current_orientation = FWEST;
						break;
					}
					case Y: {
						s[j] = FWD_C;
						current_orientation = FSOUTH;
						break;
					}
					case -Y: {
						s[j] = BK_C;
						current_orientation = FSOUTH;
						break;
					}
				};
				break;
			}
			case FWEST: {
				switch(diff) {
					case 1: {
						s[j] = BK_C;
						current_orientation = FWEST;
						break;
					}
					case -1: {
						s[j] = FWD_C;
						current_orientation = FWEST;
						break;
					}
					case Y: {
						s[j] = L_C;
						current_orientation = FSOUTH;
						break;
					}
					case -Y: {
						s[j] = R_C;
						current_orientation = FNORTH;
						break;
					}
				};
				break;
			}
		}
		/*lcd_print(1, 1, path_int[i], 2);
		lcd_print(1, 4, path_int[i + 1], 2);
		lcd_print(1, 7, current_orientation, 1);
		lcd_cursor(2, 1);
		lcd_wr_char(s[j]);
		_delay_ms(3000);*/
	}
	if(*at_home) {
		*at_home = 0;
	} else {
		(*path_len)--;
	}
	s[j] = '\0';
	*final_orientation = current_orientation;
}
#endif