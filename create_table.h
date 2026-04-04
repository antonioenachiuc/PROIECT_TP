#define SIZE_col 50
#define SIZE_lin 25 
#define WALL '#'
#define DOT '.' 
#define EMPTY ' '
#define CHERRY 'C' 

void free_table(char **table) ;
void print_table(char **table) ; 
char **create_table() ; 
void create_maze(char **table) ; 
