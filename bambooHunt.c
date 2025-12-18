#include <stdio.h>
#include <stdlib.h>         
#include <time.h>
#include <string.h>

// fill the forest grid with random numbers
void initializeForest(int **forest, int size){

    // using a nested for loop to get every element in 2D array
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){

            // setting each element to be a random number
            forest[i][j] = (rand() % 10);
        }
    }
}

// display the forest grid
void printForest(int **forest, int size){
    printf("\nForest Grid (%dx%d):\n", size, size);

    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            printf("%d ", forest[i][j]);
        }
        printf("\n");
    }
}

// using linear search to find a given bamboo type
int searchBambooType(int **forest, int size, int bambooType, int coordinates[][2]){
    // using coordinates[][2] so each row in the 2D array contains the row and column coordinate for every found bamboo

    int count = 0;

    for (int i = 0; i < size; i++){
        for (int j = 0; j<size; j++){

            // check if the value of the current element is the correct type of bamboo
            if (forest[i][j] == bambooType){
                // store the row number 
                coordinates[count][0] = i;
                // store the column number
                coordinates[count][1] = j;

                // update count value because another bamboo of the given type was found
                count++;
            }
        }
    }
    return count;
}

// count how many unique bamboos were found in a search
int countUniqueBamboos(int count, int coordinates[][2], int **discovered){
    int uniqueCount = 0;

    for (int i = 0; i < count; i++){
        int row = coordinates[i][0];
        int col = coordinates[i][1];

        // if the bamboo was not previously discovered, now mark it as discovered
        if (!discovered[row][col]){
            discovered[row][col] = 1;
            uniqueCount++;
        }
    }
    return uniqueCount;
}

float calculatePercentage(int count, int gridSize){
    return ((float)count / gridSize) * 100.0;
}

void printSearchResults(int bambooType, int count, int coordinates[][2], float percentage){
    printf("\nSearching for bamboo type %d\n", bambooType);

    // if no bamboos of the specified type are found, print a message and exit the function
    if (count == 0){
        printf("Oh no! Mochi didn't find any bamboos.\n");
        return;
    }

    // display how many times the given bamboo type was found
    printf("Found %d times!\n", count);

    // display the coordinates of all found bamboos
    for (int i = 0; i < count; i++){
        printf("%d. (%d,%d)\n", i+1, coordinates[i][0], coordinates[i][1]);
    }

    // display the percentage of the forest covered by this bamboo type
    printf("Percentage of forest covered: %.2f%%\n\n", percentage);   
}

void writeGridToLog(int **forest, int size, FILE *logfile){
    fprintf(logfile, "Mochi's Bamboo Hunt!\n");
    fprintf(logfile, "Forest Grid (%dx%d):\n", size, size);

    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            fprintf(logfile, "%d ", forest[i][j]);
        }
        // after each row is printed, move to the next line
        fprintf(logfile, "\n");
    }
    fprintf(logfile, "\n");
}

void writeSearchToLog(int bambooType, int count, int coordinates[][2], float percentage, FILE *logfile){
    fprintf(logfile, "\nSearching for bamboo type %d\n", bambooType);

    if (count == 0){
        fprintf(logfile, "Oh no! Mochi didn't find any bamboos.\n");
        return;
    }

    fprintf(logfile, "Found %d times!\n", count);
    for (int i = 0; i < count; i++){
        fprintf(logfile, "%d. (%d,%d)\n", i+1, coordinates[i][0], coordinates[i][1]);
    }

    fprintf(logfile, "Percentage of forest covered: %.2f%%\n\n", percentage);
}

void calculateSearchStats(int foundBamboo, int gridSize, FILE *logfile){
    fprintf(logfile, "Total unique bamboos found: %d\n", foundBamboo);

    float totalPercentage = calculatePercentage(foundBamboo, gridSize);
    fprintf(logfile, "Total percentage of forest covered: %.2f%%\n", totalPercentage);
}


int main(int argc, char *argv[]){

    // if there are not 2 command line arguments, return an error
    if (argc != 2){
        return 1;
    }

    // store the user's command line argument as an integer
    int size = atoi(argv[1]);

    // you cannot create a grid with negative or 0 dimensions
    if (size <= 0){
        printf("Invalid input. Please enter a positive integer.\n");
        return 1;
    }

    // seed the random number generator 
    // use time(NULL) to make the random numbers different for every program run
    srand(time(NULL));

    // allocate memory for the forest grid (2D array)
    // create an array of pointers that will point to each row
    int **forest = (int **)malloc(size * sizeof(int*));
    if (forest == NULL){
        printf("Memory allocation failed (for forest grid)!\n");
        return 1;
    }

    for (int i = 0; i < size; i++){
        // create an array of integers for the current row, to hold values
        forest[i] = (int *)malloc(size * sizeof(int));

        if (forest[i] == NULL){
            printf("Memory allocation failed (for forest row)!\n");
            for (int j = 0; j < i; j++){
                // free any pre-allocated rows
                free(forest[j]);
            }
            free(forest);
            return 1;
        }
    }

    // initialize the forest grid, then print it
    initializeForest(forest, size);
    printf("Mochi's Bamboo Forest: \n");
    printForest(forest, size);

    // open a log file to record Mochi's search 
    FILE *logfile = fopen("mochi_search_log.txt", "w");
    if (logfile == NULL){
        printf("Could not create log file.\n");
        return 1;
    }

    // write the forest grid to the log file
    writeGridToLog(forest, size, logfile);

    int gridSize = size * size;
    int totalUniqueCount = 0;
    int searchCount = 0;

    // create an array of pointers to track all found bamboos
    int **discovered = (int **)malloc(size * sizeof(int *));
    if (discovered == NULL){
        printf("Memory allocation failed (for discovered grid)!\n");

        // forest and discovered are both required for the program
        // free the forest grid
        for (int i = 0; i < size; i++){
            free(forest[i]);
        }
        free(forest);
        return 1;
    }

    for (int i = 0; i < size; i++){
        // use calloc to initialize memory to 0
        discovered[i] = (int *)calloc(size, sizeof(int));

        if (discovered[i] == NULL){
            printf("Memory allocation failed (for discovered row)!\n");

            //free any pre-allocated discovered rows
            for (int j = 0; j < i; j++){
                free(discovered[j]);
            }
            free(discovered);

            for (int j = 0; j < size; j++){
                free(forest[j]);
            }
            free(forest);
            return 1;
        }
    }

    // the main search loop (ends when the user chooses to exit)
    while (1){
        int userChoice;
        printf("\n\n1. Search for a single bamboo type (0-9)\n");
        printf("2. Search for two bamboo types (0-9)\n");
        printf("3. Quit (Mochi needs a nap!)\n");
        printf("\nEnter your choice: ");
        scanf("%d", &userChoice);

        if (userChoice == 3){
            break;
        }

        if (userChoice == 1){
            fprintf(logfile, "\n====== Mochi will search for one bamboo type! ======");
            int bambooType;
            printf("Enter bamboo type to search for (0-9): ");
            scanf("%d", &bambooType);

            //if user input isn't within range, prompt them to search again
            if (bambooType < 0 || bambooType > 9){
                printf("Invalid input. Please enter a bamboo type between 0-9.\n");
                continue;
            }

            // initialize an array to store the coordinates of bamboos after searching
            int coordinates[gridSize][2];
            int count = searchBambooType(forest, size, bambooType, coordinates);

            float percentage = calculatePercentage(count, gridSize);

            printSearchResults(bambooType, count, coordinates, percentage);
            writeSearchToLog(bambooType, count, coordinates, percentage, logfile);
            
            totalUniqueCount += countUniqueBamboos(count, coordinates, discovered);
            searchCount++;
        }

        else if (userChoice == 2){
            fprintf(logfile, "\n====== Mochi will search for two bamboo types at the same time! ======");
            int bambooType1, bambooType2;
            printf("\nEnter first bamboo type (0-9): ");
            scanf("%d", &bambooType1);
            printf("Enter second bamboo type (0-9): ");
            scanf("%d", &bambooType2);
            printf("\n");

            if (bambooType1 < 0 || bambooType1 > 9 || bambooType2 < 0 || bambooType2 > 9){
                printf("Invalid input. Please enter a bamboo type between 0-9.\n");
                continue;
            }

            // search for both bamboo types and collect results 
            int coordinates1[gridSize][2];
            int count1 = searchBambooType(forest, size, bambooType1, coordinates1);

            int coordinates2[gridSize][2];
            int count2 = searchBambooType(forest, size, bambooType2, coordinates2);

            // create temporary discovered grid for this dual search
            int **tempDiscovered = (int **)malloc(size * sizeof(int *));
            for (int i = 0; i < size; i++) {
                tempDiscovered[i] = (int *)calloc(size, sizeof(int));
            }

            // count the unique bamboos for this dual search 
            int uniqueCount1 = countUniqueBamboos(count1, coordinates1, tempDiscovered);
            int uniqueCount2 = countUniqueBamboos(count2, coordinates2, tempDiscovered);
            
            // handle same-type searches to correctly print combined results
            int combinedTotalCount;
            int combinedUniqueCount;
            
            if (bambooType1 == bambooType2) {
                // if searching for the same type, total count is just count1 or count 2
                combinedTotalCount = count1;
                // uniqueCount1 and uniqueCount2 would also be the same
                combinedUniqueCount = uniqueCount1; 
            } else {
                // if searching for different types, add both individual counts
                combinedTotalCount = count1 + count2;
                combinedUniqueCount = uniqueCount1 + uniqueCount2;
            }

            // calculate percentages
            float percentage1 = calculatePercentage(count1, gridSize);
            float percentage2 = calculatePercentage(count2, gridSize);
            float combinedUniquePercentage = calculatePercentage(combinedUniqueCount, gridSize);

            // display individual search results
            printSearchResults(bambooType1, count1, coordinates1, percentage1);
            writeSearchToLog(bambooType1, count1, coordinates1, percentage1, logfile);

            printSearchResults(bambooType2, count2, coordinates2, percentage2);
            writeSearchToLog(bambooType2, count2, coordinates2, percentage2, logfile);
            
            // display the combined results
            printf("Together, bamboo types %d and %d were found %d times!\n", bambooType1, bambooType2, combinedTotalCount);
            printf("Combined forest coverage: %.2f%%\n\n", combinedUniquePercentage);

            // write combined results to log
            fprintf(logfile, "Together, bamboo types %d and %d were found %d times!\n", bambooType1, bambooType2, combinedTotalCount);
            fprintf(logfile, "Combined forest coverage: %.2f%%\n\n", combinedUniquePercentage);

            // update total statistics 
            totalUniqueCount += countUniqueBamboos(count1, coordinates1, discovered);
            if (bambooType1 != bambooType2){
                totalUniqueCount += countUniqueBamboos(count2, coordinates2, discovered);
            }

            searchCount++;

            // free temporary discovered grid
            for (int i = 0; i < size; i++) {
                free(tempDiscovered[i]);
            }
            free(tempDiscovered);
        }

        // user did not select any given option
        else {
            printf("Invalid choice. Please try again!\n");
            fprintf(logfile, "\nUser entered invalid menu choice: %d\n", userChoice);
        }
    }

    // calculate the final search statistics
    if (searchCount >= 2){
        calculateSearchStats(totalUniqueCount, gridSize, logfile);
        fprintf(logfile, "\n\nTotal searches performed: %d\n", searchCount);
    }
    else{
        fprintf(logfile, "Mochi needs to make at least two searches to get total statistics!\n");
    }
        
    // free allocated memory
    for (int i = 0; i < size; i++){
        // first free the integer array for each row
        free(forest[i]);
        free(discovered[i]);
    }
    // free the pointer array
    free(forest);
    free(discovered);

    // close the log file
    fclose(logfile);

    printf("\nMochi's bamboo hunt is complete. Thanks for joining!");

    return 0;
}