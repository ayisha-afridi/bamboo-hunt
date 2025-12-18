# Mochi's Bamboo Hunt 

## Project Overview

Mochi is on a quest to find bamboo shoots hidden in a forest grid. The forest is represented as an N×N 2D array filled with random digits (0-9), where each digit represents a different type of bamboo. The program allows users to search for specific bamboo types and then track Mochi's unique discoveries across multiple searches.

## Features

1. **Dynamic Forest Generation**: Creates N×N grid with random bamboo types (0-9)
2. **Search Options**: 
   - Single bamboo type search
   - Dual bamboo type search to get combined results
3. **Unique Bamboo Tracking**: Prevents counting the same bamboo more than once
4. **Logging**: Records all activities in `mochi_search_log.txt`
5. **Search Statistics**: Calculates coverage percentages and totals accross all searches 
6. **Coordinate Tracking**: Displays the exact location of all found bamboos


## Compilation and Execution

### Command Line Compilation:
```bash
gcc Assignment1.c -o Assignment1
```

### Running the Program:
```bash
./Assignment1 <grid_size>
```

**Examples:**
```bash
./Assignment1 3    # creates a 3x3 forest grid
./Assignment1 9    # creates a 9x9 forest grid
```

## Implementation Details

### Data Structures

**Forest Grid:**
```c
int **forest  // 2D dynamic array storing bamboo types (0-9) in the forest
```

**Discovery Tracking:**
```c
int **discovered  // 2D boolean array for tracking all discovered positions
```

**Coordinate Storage:**
```c
int coordinates[][2]  // Array of [row, col] pairs for holding search results
```

### Memory Management
- **Dynamic Allocation**: Uses `malloc()` for scalable grid sizes and `calloc()` to initialize memory to 0
- **Memory Cleanup**: Deallocation of dynamic memory
- **Error Checks**: Verifies allocations and prints a message if an error exists

## Algorithm Analysis

### Search Algorithm: **Linear Search**

**Implementation:**
```c
int searchBambooType(int **forest, int size, int bambooType, int coordinates[][2])
```

**Algorithm Details:**
- **Type**: Linear Search
- **Method**:  Enter each row first and traverse the columns within it, before moving to the next row.
- **Time Complexity**: **O(n²)** 
- **Space Complexity**: **O(n²)** 

**Search Process:**
1. Start at initial position (0,0)
2. Compare each bamboo in the grid with the target bamboo type
3. Store coordinates of matches in the coordinates[][2] array
4. Continue until entire grid has been searched through
5. Return the count of matches 

**Example:**
```c
/*  Matrix: 
    1 2
    7 8   
*/
target = 2;

// r represents the row number, and c represents the column number
for (int r = 0 ; r < matrixSize; r++){
    //we exhaustively search each row before moving to the next
    for (int c = 0; c <= matrixSize; c++){
        if (matrix[r][c] == target)
            count++;
    }
}
```
**Traversal Order:** matrix[0][0], matrix[0][1], matrix[1][0], matrix[1][1]
- the target matches the element matrix[0][1]


### Unique Bamboo Findings 

**Implementation:**
```c
int countUniqueBamboos(int count, int coordinates[][2], int **discovered)
```

**Purpose**: Prevents counting same grid position multiple times across different searches.

**Process:**
1. Check each found coordinate against the `discovered` matrix
2. If the bamboos at a certain position, was not previously found, mark it as discovered
3. Increment unique counter
4. Return count of the discovered positions 

## Percentage Calculation

### Formula Implementation:

- To obtain the coverage percentage we need to take the count (number of found bamboos) and divide it by the total bamboos in the grid
- This will result in a decimal so we multiple by 100 to obtain the percent value

```c
float calculatePercentage(int count, int gridSize) {
    return ((float)count / gridSize) * 100.0;
}
```

### Calculation Types:

1. **Individual Search Percentage:**
   ```
   percentage = (count / gridSize) × 100
   ```

2. **Dual (Combined) Search Percentage:**
   ```
   percentage = (combinedUniqueCount / gridSize) × 100
   ```

3. **Unique Discovery Percentage:**
Calculated if two or more searches were made
   ```
   percentage = (totalUniqueCount / gridSize) × 100
   ```

## Input/Output Examples

### Sample Run (2x2 Grid):

**Forest Grid:**
```
Forest Grid (2x2):
2 7 
4 9
```

**Selecting Menu Option and Giving Input:**
```
1. Search for a single bamboo type (0-9)
2. Search for two bamboo types (0-9)
3. Quit (Mochi needs a nap!)

Enter your choice: 1
Enter bamboo type to search for (0-9): 7
```

**Search for Bamboo Type 7:**
```
Searching for bamboo type 7
Found 1 times!
1. (0,1)
Percentage of forest covered: 25.00%
```

**Prompt The User to Search Again or Quit:**
```
1. Search for a single bamboo type (0-9)
2. Search for two bamboo types (0-9)
3. Quit (Mochi needs a nap!)

Enter your choice: 3

Mochi's bamboo hunt is complete. Thanks for joining!
```

## Log File Format

The program generates `mochi_search_log.txt` with:

```
Mochi's Bamboo Hunt!
Forest Grid (2x2):
2 7 
4 9 

====== Mochi will search for one bamboo type! ======
Searching for bamboo type 7
Found 1 times!
1. (0,1)
Percentage of forest covered: 25.00%

[If program quits here...]
Mochi's bamboo hunt is complete. Thanks for joining!

[In case of additional searches being made...]
Total unique bamboos found: 2
Total percentage of forest covered: 50.00%
Total searches performed: 2
```

## Error Handling

### Input Validation:
- **Grid size**: Must be positive integer
- **Menu choices**: Must be 1, 2, or 3
- **Bamboo types**: Must be 0-9

### Memory Management:
- **Allocation failures**: Detected and handled 
- **Memory leaks**: Free dynamic memory
- **File errors**: Log file creation verified

## Complexity Analysis

### Time Complexity:
- **Forest initialization**: O(n²)
- **Singular linear search**: O(n²)
- **Dual linear search**: O(n²) × 2 = O(n²)
- **Overall program**: O(numOfSearches × n²) 

### Space Complexity:
- **Forest grid**: O(n²)
- **Discovery matrix**: O(n²)
- **Coordinate storage**: O(n²) 
- **Total space**: O(n²)

