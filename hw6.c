#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAX_ROW 500
#define MAX_COL 500 
#define MAX_CITIES 60

int main(void)
{
    printf("eenter!");
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    //global var
    int col_char_num;
    int iterations = 10 ;
    int ants_num ; //modified by read file
    int cities_num ; //modified by read file

    char * files_name[4] = {"./cities/att48_d.txt", "./cities/dantzig42_d.txt", "./cities/fri26_d.txt", "./cities/gr17_d.txt" } ;
    //take the first one for example
    fp = fopen(files_name[0], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    char *save_ptr = NULL;
    char *token;
/**************************************loop for one city*************************************/
    /******************************************************************/
    int row_count = 0 ;  //row = col 
    int col_count = 0 ;
    long int temp ; //hold temp long from strtol()
    long int city_matrix[MAX_ROW][MAX_COL] ;
    double visit_status[MAX_ROW][MAX_COL] ;
    /******************************************************************/

    while ((col_char_num = getline(&line, &len, fp)) != -1)
    {
        //printf("%d \n", col_char_num);
        //printf("%d \n", row_count);
        for (token = strtok_r(line, " \t\n", &save_ptr);
             token != NULL;
             token = strtok_r(NULL, " \t\n", &save_ptr))
        {
            temp = atoi(token); //int
            city_matrix[row_count][col_count] = temp ;

            if(temp) 
                visit_status[row_count][col_count] = 1/temp ;
            else 
                visit_status[row_count][col_count] = 0 ;

            col_count++ ;
        }
        col_count = 0 ;
        row_count++ ;
    }
    cities_num = row_count ;
    ants_num = row_count ;
    
    int i ;
    int j ;
    
    for (i = 0; i < row_count ; i++)
    {
        for (j = 0; j < row_count; j++)
        {
            printf("%li " , city_matrix[i][j]);
        }
        printf("\n");
    }
    
    /**************************************loop for one city*************************************/

    //loop var initialized
    int m = ants_num; //pheromone row
    int n = ants_num; //pheromone col
    double e = .5   ;     //evaporation rate
    double alpha = 1 ;    //pheromone factor
    double beta = 2;       //visibility factor
    
    double * pheromone = malloc((m*n) * sizeof(double));
    double * route = malloc((m*(n + 1)) * sizeof(double));
    double * pher_feature = malloc(n * sizeof(double));
    double * visi_feature = malloc(n * sizeof(double));
    double * combine_feat = malloc(n * sizeof(double));
    double * probability = malloc(n * sizeof(double));
    double feature_sum = 0;
    
    //printf("%d ", pheromone[i * n + j]);
    //row : m , col : n 
    for (i = 0; i < (m * n) ; i++){     pheromone[i] = 0.1 ;}
    for (i = 0; i < (m * (n + 1)); i++){    route[i] = 1;}


    int route_col ;
    int iter ;
    int current_location ; //ant location
    int array_ind ; 
    /*main iteration*/
    for (iter = 0; iter < iterations ; iter++)
    {
        //initial starting and ending cities of every ants 
        for (route_col = 0; route_col < m; route_col++)
            route[route_col * n + 0] = 1;
        for (i = 0; i < m ; i++)
        {
            double temp_visit_status[MAX_ROW][MAX_ROW];
            memcpy(temp_visit_status, visit_status, sizeof(visit_status)); //蠻浪費的
            for (j = 0; j < n; j++)
            {
                double combine_features[MAX_CITIES] = {0}; //只取實際cities 數量
                double cummu_prob[MAX_CITIES] = {0};

                current_location = (int)(route[i * n + j] - 1); //轉int ???????????????
                for (array_ind = 0; array_ind < m; array_ind++)
                    temp_visit_status[array_ind][current_location] = 0;

                // alpha = 1  pheromone factor
                // beta = 2   visibility factor
                for (array_ind = 0; array_ind < n; array_ind++){
                    pher_feature[array_ind] = pow(pheromone[current_location * m + array_ind] , beta);
                    visi_feature[array_ind] = pow(temp_visit_status[m][array_ind] , alpha);
                    combine_feat[array_ind] = pher_feature[array_ind] * visi_feature[array_ind] ;
                    feature_sum += combine_feat[array_ind] ;
                }
                for ( array_ind = 0; array_ind < n; array_ind++) //divide total & cumulative
                {
                    probability[array_ind] = combine_feat[array_ind] / feature_sum ;
                    if(array_ind >= 1)
                        probability[array_ind] = probability[array_ind] + probability[array_ind - 1] ;
                }
                for (array_ind = 0; array_ind < n; array_ind++)
                    printf("%d" , probability[array_ind]);

                    
                

                



            }
            
        }
        break;
    }
    


    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}