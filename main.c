#include <stdio.h>
#include <stdlib.h> 
#define SIZE_col 50
#define SIZE_lin 25 
#define WALL '#'
#define DOT '.' 
#define PACMAN 3 
#define EMPTY 0 

void create_maze(char **table){
for (int i=0 ; i<SIZE_lin ;i++){
     for(int j=0 ; j<SIZE_col ; j++) {
        if (i==0 || j==0 || j==SIZE_col-1 || i==SIZE_lin-1) 
         table[i][j]=WALL ; 
         else 
         table[i][j]=DOT ; 
        if (i>1 && i<5 && ((j>2 && j<20) || (j>30 && j<48))) 
          table[i][j]=WALL ;  
        if (j>23 && j<27 && i>1 && i<8)
          table[i][j]=WALL ; 
        if (i>5 && i<8 && j>1 && j<48)
          table[i][j]=WALL ;   
        if (i>8 && i<11 && j>1 && j<48)
          table[i][j]=WALL ; 
        if (i>10 && i<13 && ((j>1 && j<4) || (j<48 && j>45)))
          table[i][j]=WALL ; 
        if (i==12 && ((j>3 && j<20) || (j<46 && j>29)))
          table[i][j]=WALL ; 
        if (i==15 && ((j>7 && j<20) || (j>29 && j<42) || (j>43 && j<47) || (j<6 && j>2))) 
          table[i][j]=WALL ; 
        if ((i>15 && i<19) && ((j>7 && j<10)||(j>39 && j<42)|| (j>43 && j<47) || (j<6 && j>2)))
          table[i][j]=WALL ;   
        if(i==18 && (j>9 && j<40))
          table[i][j]=WALL ; 
        if((i>18 && i<23) && ((j>43 && j<47) || (j<6 && j>2)))
          table[i][j]=WALL ;   
        if((i>20 && i<23) && (j>7 && j<42)) 
          table[i][j]=WALL ; 

     }
  }
}

char **create_table() {
  char **table=NULL ; 
  if ((table=malloc(SIZE_lin*sizeof(char*)))==NULL) {
    printf("TABLA NU A PUTUT FI GENERATA , JOCUL NU POATE PORNI \n") ; 
    return NULL ; 
  }
  
  for (int i=0 ; i<SIZE_lin;i++) {
      if((table[i]=malloc(SIZE_col*sizeof(char)))==NULL) {
       printf("TABLA NU A PUTUT FI GENERATA , JOCUL NU POATE PORNI \n");
       for (int j=0 ; j<i ; j++) 
        free(table[j]) ; 
       free(table) ; 
       return NULL ; 
      }      
  }
    create_maze(table) ; 
 return table ; 
}


void print_table(char **table){
   for(int i=0 ; i<SIZE_lin ; i++){
     for(int j=0 ; j<SIZE_col; j++) 
      printf("%c" , table[i][j]) ; 
    printf("\n") ; 
} 

} 

void free_table(char **table) {
  for(int i=0 ; i<SIZE_lin ; i++) 
    free(table[i]) ; 
    free(table) ; 
}

int main (int argc , char **argv){
 char **table=NULL ; 
 table=create_table() ; 
 print_table(table); 
 free_table(table) ; 
    return 0 ; 
}