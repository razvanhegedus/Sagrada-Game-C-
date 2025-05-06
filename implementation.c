#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define max_players 6
#define MAX_STRING_SIZE 100 // Maximum size of the string
#define MAX_NR_PBLGLS 11 // Corrected the constant name here
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"


typedef struct 
{
    int number;
    char string[MAX_STRING_SIZE];
    int value;
} publicgoals;
publicgoals pblgoals[MAX_NR_PBLGLS];


typedef struct 
{
    int window[4][5]; //
    int helper_gems;
    int privategoals; //
    int publicgoals;
    int nr_dices_placed;
    int game_score;
}playerwindow;
playerwindow players[max_players];

typedef struct 
{
    int **diceExtraction; // 2D array to hold color and value of extractions
    int color[5]; // Assuming 5 colors
} dice;

dice DICE;
void initializeGlobalDice() 
{
    DICE.color[0] = 18; //red
    DICE.color[1] = 18; //yellow
    DICE.color[2] = 18; //green
    DICE.color[3] = 18; //blue 
    DICE.color[4] = 18; //purple
}
typedef struct
{
    int number[10];
}Round_marker;
Round_marker RRound;

void initializeroundmarker()
{
    for(int i = 0;i<10;i++)
    {
        RRound.number[i] = 0;
    }
                
}
void initializeDiceExtraction(dice *D, int extractions) {
    D->diceExtraction = (int **)malloc(extractions * sizeof(int *));
    for(int i = 0; i < extractions; i++) {
        D->diceExtraction[i] = (int *)malloc(2 * sizeof(int)); // Each row has 2 ints: color and value
    }
}

void freeDiceExtraction(dice *D, int extractions) {
    for(int i = 0; i < extractions; i++) {
        free(D->diceExtraction[i]);
    }
    free(D->diceExtraction);
}

void dice_extraction(dice *D, int extractions)
{
    for (int i = 0; i < extractions; i++) 
    {
        int color1 = rand() % 5;
        while(D->color[color1] == 0)
        {
            color1 = rand() % 5;
        }
        int value = (rand() % 6) + 1;
        //D->color[color1] = D->color[color1] - 1;
        D->diceExtraction[i][0] = color1; // Store color
        D->diceExtraction[i][1] = value; // Store value
        printf("Extraction %d: ", i+1);
        if(color1 == 0)
            printf(RED "%d\n" RESET,value);

        if(color1 == 1)
            printf(YEL "%d\n" RESET,value);

        if(color1 == 2)
            printf(GRN "%d\n" RESET,value);

        if(color1== 3)
            printf(BLU "%d\n" RESET,value);

        if(color1== 4)
            printf(MAG "%d\n" RESET,value);
    }
}


void privategoalsselector(int nr_of_players)
{
    for(int i = 1;i<= nr_of_players;i++)
    {
        players[i].privategoals = rand() % 5;
        for(int j = 1;j<i;j++)
        {
            while(players[i].privategoals == players[j].privategoals)
                players[i].privategoals = rand() % 5;
        }
        //printf("secret goal of the player %d is %d\n",i,players[i].privategoals);
    }
    printf("the secret goals have been assigned\n");
}


void printplayerwindow(int p)
{
    for (int i = 0; i < 4; i++) 
        {
            for (int j = 0; j < 5; j++) 
            {
                
                if(players[p].window[i][j] == 9)
                    printf("EMP ");

                if(players[p].window[i][j] == 10)
                    printf("RE1 ");

                if(players[p].window[i][j] == 20)
                    printf("RE2 ");

                if(players[p].window[i][j] == 30)
                    printf("RE3 ");

                if(players[p].window[i][j] == 40)
                    printf("RE4 ");

                if(players[p].window[i][j] == 50)
                    printf("RE5 ");

                if(players[p].window[i][j] == 60)
                    printf("RE6 ");

                if(players[p].window[i][j] == 0)
                    printf(RED "RED " RESET);

                if(players[p].window[i][j] == 1)
                    printf(YEL "YEL " RESET);

                if(players[p].window[i][j] == 2)
                    printf(GRN "GRN " RESET);

                if(players[p].window[i][j] == 3)
                    printf(BLU "BLU " RESET);

                if(players[p].window[i][j] == 4)
                    printf(MAG "PUR " RESET);
                if(players[p].window[i][j] >= 100)
                {
                    if(players[p].window[i][j]%10 == 0)
                        printf(RED "%d   " RESET,players[p].window[i][j]/100);

                    if(players[p].window[i][j]%10 == 1)
                        printf(YEL "%d   " RESET,players[p].window[i][j]/100);

                    if(players[p].window[i][j]%10 == 2)
                        printf(GRN "%d   " RESET,players[p].window[i][j]/100);

                    if(players[p].window[i][j]%10 == 3)
                        printf(BLU "%d   " RESET,players[p].window[i][j]/100);

                    if(players[p].window[i][j]%10 == 4)
                        printf(MAG "%d   " RESET,players[p].window[i][j]/100);
                }

            }
            printf("\n");
        }
        printf("\n");
}


void swap_window(int a[][5], int b[][5]) 
{
    int temp[4][5];
    memcpy(temp, a, sizeof(temp));
    memcpy(a, b, sizeof(temp));
    memcpy(b, temp, sizeof(temp));
}

// Function to randomly swap window arrays within playerwindow structures
void random_swap(playerwindow arr[], int n) 
{
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < n; i++) 
    {
        int j = rand() % n; // Generate a random index
        swap_window(arr[i].window, arr[j].window); // Swap window arrays
    }
}


int init_window(const char* filename, int nr_of_players)
{
    FILE *file = fopen(filename, "r");
    if(file == NULL) 
    {
        printf("Error opening file\n");
        return 1;
    }
    
    char line[1024]; // Buffer to hold each line of the file
    const char *delimiter = " \t\n"; // Delimiters include space, tab, and newline
    
    for (int p = 1; p <= max_players; p++) 
    {
        players[p].game_score = 0;
        for (int i = 0; i < 4; i++) 
        {
            if (fgets(line, sizeof(line), file) == NULL)
            {
                fprintf(stderr, "Error or end of file reached unexpectedly while reading.\n");
                fclose(file);
                return 1;
            }
            
            char *token = strtok(line, delimiter);
            for (int j = 0; j < 5 && token != NULL; j++) 
            {
                players[p].window[i][j] = atoi(token);
                token = strtok(NULL, delimiter);
            }
        }
    }
    
    fclose(file);
    random_swap(players,max_players);
    // Optional: For verification, print the matrices
    for (int p = 1; p <= nr_of_players; p++) 
    {
        players[p].helper_gems = players[p].window[0][0]/100;
        players[p].window[0][0] = players[p].window[0][0] - players[p].helper_gems*100;
        printf("helper_gems of player %d is %d:\n",p,players[p].helper_gems);
        printf("Player window %d:\n", p);
        printplayerwindow(p);
        players[p].nr_dices_placed = 0;
    }
    int initial_values[4][5] = {
        {103, 204, 300, 402, 501},
        {204, 203, 301, 402, 501},
        {302, 201, 303, 402, 501},
        {401, 203, 300, 402, 601}
    };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            players[0].window[i][j] = initial_values[i][j];
        }
    }
    players[0].game_score = 0;
    players[0].privategoals = 1;
    //printplayerwindow(0);
    return 0;
}


void readpublicgoals()
{
    FILE *file;
    const char *filename = "publicgoals.txt";

    // Open the file for reading
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open the file");
        exit(1);
    }
    int i = 1;
    while (fscanf(file, "%d.%[^,],%d\n", &pblgoals[i].number, pblgoals[i].string, &pblgoals[i].value) == 3) 
    {
     //printf("Number: %d, String: %s, Value: %d\n", pblgoals[i].number, pblgoals[i].string, pblgoals[i].value);
        i++;
    }

    fclose(file);
}
int first_public_goal = 0;
int second_public_goal = 0;


void selectpublicgoals()
{
    srand(time(NULL));
    int first_public_goal = rand() % 10 + 1;
    int second_public_goal = rand() % 10 + 1;

    while(second_public_goal == first_public_goal && second_public_goal != 0)
        second_public_goal = rand() % 10 + 1;
    printf("The 2 public goals are:\n");
    printf("Number: %d, String: %s, Value: %d\n", pblgoals[first_public_goal].number, pblgoals[first_public_goal].string, pblgoals[first_public_goal].value); 
    printf("Number: %d, String: %s, Value: %d\n", pblgoals[second_public_goal].number, pblgoals[second_public_goal].string, pblgoals[second_public_goal].value);
}


int* playerorder(int nr_of_players, int round_number) {
    static int orderp2r1[] = {1, 2, 2, 1}; 
    static int orderp2r2[] = {2, 1, 1, 2}; 
    static int orderp3r1[] = {1, 2, 3, 1, 2, 3};
    static int orderp3r2[] = {2, 3, 1, 1, 3, 2}; 
    static int orderp3r3[] = {3, 1, 2, 2, 1, 3};
    static int orderp4r1[] = {1, 2, 3, 4, 4, 3, 2, 1}; 
    static int orderp4r2[] = {2, 3, 4, 1, 1, 4, 3, 2};
    static int orderp4r3[] = {3, 4, 1, 2, 2, 1, 4, 3}; 
    static int orderp4r4[] = {4, 1, 2, 3, 3, 2, 1, 4};

    if (nr_of_players == 2) 
    {
        if (round_number % 2 == 0)
            return orderp2r2;
        else
            return orderp2r1;
    } 
    else if (nr_of_players == 3) 
    {
        if (round_number % 3 == 0)
            return orderp3r3;
        else if (round_number % 3 == 1)
            return orderp3r1;
        else
            return orderp3r2;
    } 
    else if (nr_of_players == 4) 
    {
        if (round_number % 4 == 0)
            return orderp4r4;
        else if (round_number % 4 == 1)
            return orderp4r1;
        else if (round_number % 4 == 2)
            return orderp4r2;
        else
            return orderp4r3;
    }

    // Default return (to suppress compiler warning)
    return NULL;
}
int first_digit(int x)
{
    while(x>=10)
        x /= 10;
    return x;
}

int validposition(int i,int j,int ndp,int selected_dice, int plnumber)
{
    int ai = i;
    int aj = j;
    int ok = 1; i++;j++;
    if(ndp == 0) //ndp nr of dice placed
    {
        if ((i == 2 || i == 3) && (j >= 2 && j <= 4)) 
        {
            ok = 0;
            printf("Wrong placement, the margin restricton is not respected \n");
        }
        // checks if the 1st dice is placed on the margin
        
    }
    if(ndp >= 0)
    {   
        if (players[plnumber].window[ai][aj]<100) //this if verifies if the condition on the window are respected by the dice
        {
            int ok1 = 0;
            if(players[plnumber].window[ai][aj] == 9)
                ok1 = 1;
            if((players[plnumber].window[ai][aj]%10 == DICE.diceExtraction[selected_dice][0]) && players[plnumber].window[ai][aj]<8)
                ok1 = 1;
            if((players[plnumber].window[ai][aj]/10 == DICE.diceExtraction[selected_dice][1]) && players[plnumber].window[ai][aj]>=10)
                ok1 = 1;
            if(ok1 == 0)
                ok = 0;
            if(ok1 == 0)
                printf("Wrong placement, the square restricton is not respected \n");
        }
        if(ndp>=1)
        {
        int plc = 0;

        for (int stepi = ai - 1; stepi <= ai + 1; stepi++) 
        { // Loop to check adjacent or diagonal positions
            for (int stepj = aj - 1; stepj <= aj + 1; stepj++) 
            {
                if (stepi != ai || stepj != aj) 
                { // Skip the current position
                    if (stepi >= 0 && stepi < 4 && stepj >= 0 && stepj < 5) 
                    { // Ensure within bounds of 4x5 matrix
                        if (players[plnumber].window[stepi][stepj] >= 100) 
                        {
                            plc = 1;
                            break; 
                        }
                    }
                }
            }   
            if (plc == 1) break; // Exit the outer loop early if a valid position is found
        }
       
        if(plc == 0)
        {
            ok = 0;
            printf("Wrong placement, the neighbour restricton is not respected \n");
        }

        }
        int col_numb_neighbour = 1;
            if(i>=2)
                if(players[plnumber].window[ai-1][aj]%10 == DICE.diceExtraction[selected_dice][0])
                {
                    if(players[plnumber].window[ai-1][aj]<10 || players[plnumber].window[ai-1][aj]>60)
                        col_numb_neighbour = 0;
                }
            if(i<=3)
                if(players[plnumber].window[ai+1][aj]%10 == DICE.diceExtraction[selected_dice][0])
                {
                    if(players[plnumber].window[ai+1][aj]<10 || players[plnumber].window[ai+1][aj]>60)
                        col_numb_neighbour = 0;
                }
            if(j>=2)
                if(players[plnumber].window[ai][aj-1]%10 == DICE.diceExtraction[selected_dice][0])
                {
                    if(players[plnumber].window[ai][aj-1]<10 || players[plnumber].window[ai][aj-1]>60)
                        col_numb_neighbour = 0;
                }
            if(j<=4)
                if(players[plnumber].window[ai][aj+1]%10 == DICE.diceExtraction[selected_dice][0])
                {
                    if(players[plnumber].window[ai][aj+1]<10 || players[plnumber].window[ai][aj+1]>60)
                        col_numb_neighbour = 0;
                }
        if(col_numb_neighbour == 0)
        {
            ok = 0;
            printf("Wrong placement, the colour neighbour restricton is not respected \n");
        }
        int value_dice_neighbour = 1;
   
            
                if((first_digit(players[plnumber].window[ai-1][aj]) == DICE.diceExtraction[selected_dice][1]) && i>=2 && players[plnumber].window[ai-1][aj]>=10)
                {
                    value_dice_neighbour = 0;
                }
                if((first_digit(players[plnumber].window[ai+1][aj]) == DICE.diceExtraction[selected_dice][1]) && i<=3 && players[plnumber].window[ai+1][aj]>=10)
                {
                    value_dice_neighbour = 0;
                }
                if((first_digit(players[plnumber].window[ai][aj-1]) == DICE.diceExtraction[selected_dice][1]) && j>=2 && players[plnumber].window[ai][aj-1]>=10)
                {
                    value_dice_neighbour = 0;
                }
                if((first_digit(players[plnumber].window[ai][aj+1]) == DICE.diceExtraction[selected_dice][1]) && j<=4 && players[plnumber].window[ai][aj+1]>=10)
                {
                    value_dice_neighbour = 0;
                }
        if(value_dice_neighbour==0)
        {
            ok = 0;
            printf("Wrong placement, the dice value neighbour restricton is not respected \n");
        }
    }
    if(players[plnumber].window[ai][aj]>100)
        ok = 0;
    if(ok == 1)
        return 1;
    else
        return 0;
}


void placeadice(int nr_of_players, int round_number)
{
    int extractions = 2 * nr_of_players + 1; // Calculation for the number of dice extractions
    initializeDiceExtraction(&DICE, extractions);
    dice_extraction(&DICE, extractions);
    int *Order = playerorder(nr_of_players,round_number);
    int used[extractions]; // Array to track used dices
    memset(used, 0, sizeof(used)); // Initialize all to 0 (not used)

    for(int pls =  0; pls < extractions - 1; pls++)
    {
       

        printf("\nThe player window for player %d:\n\n", Order[pls]);
        printplayerwindow(Order[pls]);

        // Loop until a valid dice selection is made
        int selected_dice;
        do {
            printf("Select the dice (enter a number from 1 to %d): ", extractions);
            scanf("%d", &selected_dice);
            selected_dice -= 1; // Convert to zero-index

            if (selected_dice < 0 || used[selected_dice]) 
            {
                printf("Invalid or already used dice selection. Please try again.\n");
            }
        } while (selected_dice < 0 || used[selected_dice]);
        int i, j;
        int condition;
        int decision = 0;
        int value = 0;
        do
        {
           /* 
            int decision;
            scanf("%d",&decision);
            if (decision!=0)
            {*/
                printf("Select the position to insert the dice (row col): ");
                scanf("%d %d", &i, &j);
                i--;j--;
                // Validate chosen position
                if (i < 0 || i > 3 || j < 0 || j > 4) 
                {
                    printf("Invalid position.\n");
                }

                if (players[Order[pls]].window[i][j] > 100) 
                {
                    value = players[Order[pls]].window[i][j];
                    printf("The selected position is not empty.\n");
                }
                int ndp = players[Order[pls]].nr_dices_placed;
                condition = validposition(i,j,ndp,selected_dice,Order[pls]);
                if(condition == 0)
                {
                    printf("Selected 0 to choose another position on the board or type 1 to skip this turn \n");
                    int decision1;
                    scanf("%d",&decision1);
                    decision = decision1;
                    if(decision == 1)
                       {
                        value = 0;
                        condition = 1;
                    }
                }
            
        }while(i < 0 || i > 3 || j < 0 || j > 4 || value > 100 || 
            condition == 0);
        if(decision == 0)
        {   
            players[Order[pls]].nr_dices_placed++; //increase the nr of dice placed by a player
            // Place the dice
            used[selected_dice] = 1; // Mark the dice as used
            int color1 = DICE.diceExtraction[selected_dice][0];
            players[Order[pls]].window[i][j] = DICE.diceExtraction[selected_dice][1] * 100 + color1; //dice value * 100 + color
            DICE.color[color1] = DICE.color[color1] - 1; //remove the dice from the dice bag
            printf("Dice placed:\n");
            printplayerwindow(Order[pls]);
        }
    }

    // Loop through all dice to accumulate the values of the unused ones
    for (int d = 0; d < extractions; d++) {
        if (!used[d]) {
            RRound.number[round_number] += DICE.diceExtraction[d][1];
        }
    }

    freeDiceExtraction(&DICE, extractions);
}

void clearScreen() 
{
    system("clear"); // For Unix/Linux
}


void adding_gem_points(int player_number)
{
    players[player_number].game_score += players[player_number].helper_gems;
}


void adding_private_mission(int nr)
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 5;j++)
        {
            if(players[nr].window[i][j] % 10 == players[nr].privategoals)
                players[nr].game_score += first_digit(players[nr].window[i][j]);
        }
}

void color_variety(int nr)
{
    int fr[5];
    memset(fr, 0, sizeof(fr));
    int min = 21;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 5;j++)
        {
            fr[players[nr].window[i][j] % 10]++;
        }
    for(int l = 0;l<5;l++)
        if(fr[l] < min)
                min = fr[l];
    players[nr].game_score += min * 6;
}

void row_color_variety(int nr)
{
    int fr[5];
    int row_numbers = 0;
    for(int i = 0; i < 4; i++)
    {
        memset(fr, 0, sizeof(fr));
        for(int j = 0; j < 5;j++)
        {
            fr[players[nr].window[i][j] % 10]++;
                
        }
        if(fr[0] == 1 && fr[1] == 1 && fr[2] == 1 && fr[3] == 1 && fr[4] == 1)
            row_numbers++;
    }
    players[nr].game_score += row_numbers * 6;
}


void column_color_variety(int nr)
{
    int fr[5];
    int col_numbers = 0;
    for(int j = 0; j < 5; j++)
    {
        memset(fr, 0, sizeof(fr));
        for(int i = 0; i < 4;i++)
        {
            fr[players[nr].window[i][j] % 10]++;
        }
        int a = 0;
        for(int l = 0;l<5;l++)
            if(fr[l] == 1)
            {
                a++;
            }
        if(a == 4)
        {
            col_numbers++;
        }
    }
    players[nr].game_score += col_numbers * 5;
}   

void shade_variety(int nr)
{
    int fr[7];
    memset(fr, 0, sizeof(fr));
    int min = 21;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 5;j++)
        {
            fr[first_digit(players[nr].window[i][j])]++;
            
        }
    for(int l = 1;l<=6;l++)
    {
        if(fr[l] < min)
       min = fr[l];        
    }
    players[nr].game_score += min * 5;
}


void row_shade_variety(int nr)
{
    int fr[7];
    int row_numbers = 0;
    for(int i = 0; i < 4; i++)
    {
        memset(fr, 0, sizeof(fr));
        for(int j = 0; j < 5;j++)
        {
            fr[first_digit(players[nr].window[i][j])]++;
                
        }
        int a = 0;
        for(int l = 1;l<= 6;l++)
            if(fr[l] == 1)
            {
                a++;
            }
        if(a == 5)
            row_numbers++;
    }
    players[nr].game_score += row_numbers * 6;
}

void column_shade_variety(int nr)
{
    int fr[7];  // Adjust size to 7 to use indices from 1 to 6
    int col_numbers = 0;
    for(int j = 0; j < 5; j++)
    {
        memset(fr, 0, sizeof(fr));
        for(int i = 0; i < 4; i++)
        {
            fr[first_digit(players[nr].window[i][j])]++;
        }

        int a = 0;  // Move this inside the loop to reset for each column
        for(int l = 1; l <= 6; l++)
        {
            if(fr[l] == 1)
            {
                a++;
            }
        }

        if(a == 4)
        {
            col_numbers++;
        }
    }
    players[nr].game_score += col_numbers * 4;
}
void low_shade_variety(int nr)
{
    int fr[7];
    memset(fr, 0, sizeof(fr));
    int min = 21;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 5;j++)
        {
            fr[first_digit(players[nr].window[i][j]) % 10]++;
        }
    for(int l = 1;l<=2;l++)
        if(fr[l] < min)
                min = fr[l];
    players[nr].game_score += min*2;
}

void medium_shade_variety(int nr)
{
    int fr[7];
    memset(fr, 0, sizeof(fr));
    int min = 21;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 5;j++)
        {
            fr[first_digit(players[nr].window[i][j]) % 10]++;
        }
    for(int l = 3;l<=4;l++)
        if(fr[l] < min)
                min = fr[l];
    players[nr].game_score += min*2;
}

void deep_shade_variety(int nr)
{
    int fr[7];
    memset(fr, 0, sizeof(fr));
    int min = 21;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 5;j++)
        {
            fr[first_digit(players[nr].window[i][j]) % 10]++;
        }
    for(int l = 5;l<=6;l++)
        if(fr[l] < min)
                min = fr[l];
    players[nr].game_score += min*2;
}



// Function to get the last digit of a number
int last_digit(int num) 
{
    return num % 10;
}

// Recursive function to mark connected dice
void mark_diagonal_chain(int nr, int marked[4][5], int row, int col, int color, int *chain_length) 
{
    // Check for out of bounds and if already marked or different color
    if (row < 0 || row >= 4 || col < 0 || col >= 5 || marked[row][col] || last_digit(players[nr].window[row][col]) != color) 
    {
        return;
    }

    // Mark the current die
    marked[row][col] = 1;
    (*chain_length)++;

    // Check all four possible diagonal directions
    mark_diagonal_chain(nr, marked, row - 1, col - 1, color, chain_length);
    mark_diagonal_chain(nr, marked, row - 1, col + 1, color, chain_length);
    mark_diagonal_chain(nr, marked, row + 1, col - 1, color, chain_length);
    mark_diagonal_chain(nr, marked, row + 1, col + 1, color, chain_length);
}

// Function to calculate the diagonal chain points for a player
void color_diagonaly(int nr) 
{
    int marked[4][5];
    memset(marked, 0, sizeof(marked));  // Initialize the marked array

    for (int row = 0; row < 4; row++) 
    {
        for (int col = 0; col < 5; col++) 
        {
            if (!marked[row][col]) 
            {
                int color = last_digit(players[nr].window[row][col]);
                int chain_length = 0;

                // Mark the diagonal chain starting from this die
                mark_diagonal_chain(nr, marked, row, col, color, &chain_length);

                // Add the chain length to the score
                if (chain_length > 1) 
                {  // Only consider chains of length > 1
                    players[nr].game_score += chain_length;
                }
            }
        }
    }
}

void calculatepublicgoals(int nr1, int nr2, int nop)
{
    if(nr1 == 1 || nr2 == 1)
        column_color_variety(nop);
    if(nr1 == 2 || nr2 == 2)
        row_color_variety(nop);
    if(nr1 == 3 || nr2 == 3)
        color_variety(nop);
    if(nr1 == 4 || nr2 == 4)
        color_diagonaly(nop);
    if(nr1 == 5 || nr2 == 5)
        low_shade_variety(nop);
    if(nr1 == 6 || nr2 == 6)
        medium_shade_variety(nop);
    if(nr1 == 7 || nr2 == 7)
        deep_shade_variety(nop);
    if(nr1 == 8 || nr2 == 8)
        column_shade_variety(nop);
    if(nr1 == 9 || nr2 == 9)
        row_shade_variety(nop);
    if(nr1 == 10 || nr2 == 10)
        shade_variety(nop);
}

int main(int argc, char *argv[])
{
     if (argc != 2) 
    {
        printf("Usage: %s <filename.txt>\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];
    #include <stdio.h>

    int choice;
    int nr_of_players = 0;
    int is_nr_of_players_set = 0; // Flag to check if number of players has been set

    do {
        printf("\n1. Enter the number of players\n");
        printf("2. Assign player window, public goals, secret goals\n");
        printf("3. Start the game.\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) 
        {
            case 1:
                if (!is_nr_of_players_set) 
                {
                    printf("Enter the number of players: ");
                    scanf("%d", &nr_of_players);
                    is_nr_of_players_set = 1; // Set the flag since the number of players is now set
                } else {
                    printf("Number of players has already been set.\n");
                }
                break;
            case 2:
                if (nr_of_players == 0) 
                {
                    printf("Please type 1 to enter the number of players first\n");
                } else 
                {
                    init_window(filename, nr_of_players);
                    privategoalsselector(nr_of_players);
                    readpublicgoals();
                    selectpublicgoals();
                    initializeGlobalDice();
                }
                break;
            case 3:
                if (nr_of_players == 0) 
                {
                    printf("Please type 1 to enter the number of players first\n");
                }
                else 
                {
                    clearScreen();
                    printf("THE GAME HAS STARTED\n");
                    initializeroundmarker();
                    for (int j = 0; j < 10; j++) 
                    {
                        placeadice(nr_of_players, j + 1);
                        printf("updated dice bag:");
                        for (int i = 0; i < 5; i++) 
                        {
                            printf("%d ", DICE.color[i]); // 
                        }
                        printf("\n");
                        printf("The dice put on the roundmarker in round %d is %d\n", j + 1, RRound.number[j + 1]);
                    }
                    for(int p = 1;p<=nr_of_players;p++)
                    {
                        adding_private_mission(p);
                        adding_gem_points(p);
                        calculatepublicgoals(pblgoals[first_public_goal].number,pblgoals[second_public_goal].number,p);
                        printf("final player score for contestant %d  is %d\n",p,players[p].game_score);
                    }
                }
                choice = 0;
                break;
            default:
                printf("Wrong option. Please enter a valid choice.\n");
                break;
        }
    } while (choice != 0);
    adding_private_mission(0);
    printf("%d private mission\n",players[0].game_score);
    color_variety(0);
    printf("%d color_variety\n",players[0].game_score);
    row_color_variety(0);
    printf("%d row_color_variety\n",players[0].game_score);
    column_color_variety(0);
    printf("%d column_color_variety\n",players[0].game_score);
    shade_variety(0);
    printf("%d shade_variety\n",players[0].game_score);
    column_shade_variety(0);
    printf("%d column_shade_variety\n",players[0].game_score);
    row_shade_variety(0);
    printf("%d row_shade_variety\n",players[0].game_score);
    low_shade_variety(0);
    printf("%d low_shade_variety\n",players[0].game_score);
    medium_shade_variety(0);
    printf("%d medium_shade_variety\n",players[0].game_score);
    deep_shade_variety(0);
    printf("%d deep_shade_variety\n",players[0].game_score);
    color_diagonaly(0);
    printf("%d chain diagonal\n",players[0].game_score);
    return 0;
}
