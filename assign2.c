/*
 * 18/03/2023
 * Computer Programming II - Assignment 2
 * Made by Elvin Jiby in VSCode
 *
 * MegaMillions Lottery Simulator in C!
 * - User specifies the amount of tickets they wish to generate
 * - Program simulates a MegaMillions lottery and gives statistics
 *   based on their winnings as well as sales and profit of the lottery
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ticket structure definition
struct ticket
{
    int identifier;
    int whiteBalls[5];
    int goldBall;
    int megaplier;
};

void shuffle(int array[], int n);
void sortWhiteBalls(int *array);
int generate_Megaplier();

int main(void)
{
    // Variables //
    int amntTicket; //Amount of Tickets that are generated
    int amntMegaplier = 0;
    double amntWon = 0; //The amount of money won
    int winners = 0; //The amount of winners
    double sales = 0; //The total sales amount
    double payout = 0; //The total payout amount
    double avg_won = 0; //The average amount won
    double profit = 0; //The total profit
    double annuityPay = 1500000;
    int winningTicket[6]; //Array with the white ball numbers of the winning ticket

                    /* Part 1 - Generate Tickets */
    // Welcome //
    printf("\n*** Welcome to the MegaMillions Lottery! ***\n");
    printf("_______________________________________________________________\n\n");
    // User Input //
    printf("Please input the amount of tickets you would like to generate:\n");
    scanf("%d", &amntTicket);

    // Invalid amount of tickets //
    if (amntTicket < 0)
    {
        printf("Invalid amount of tickets.\n");
        return 0;
    }

    struct ticket tickets[amntTicket]; //Defining array of ticket structs

    // Initialise random number generator //
    srand(time(NULL));
    rand();

    // Ticket Variables //
    int sortedWhite[5]; //Array of white ball numbers that will be sorted
    int goldBall; //Gold ball number
    int megaplier; // Megaplier number
    int whiteBallCount[70]; //Array of white ball numbers (1-70) used for shuffling

    // Fill whiteBallCount array with consecutive numbers from 1-70
    for (int i = 0; i < 70; i++) 
    {
        whiteBallCount[i] = i + 1; 
    }
    
    // Writing to file //
    FILE *cfPtr;

    // fopen opens the file; exits if file cannot be opened
    if ((cfPtr = fopen("tickets.txt", "w")) == NULL)
    {
        printf("File could not be opened.");
    }

    else
    {
        for (int i = 0; i<amntTicket; i++)
        {
            shuffle(whiteBallCount, 70);

            fprintf(cfPtr, "%d | ", i + 1); // Identifier number
            for (int j = 0; j<5; j++) 
            {
                sortedWhite[j] = whiteBallCount[j]; // white ball numbers
            }

            sortWhiteBalls(sortedWhite); // Sorts white ball numbers in order
            for (int j = 0; j<5; j++)
            {
                fprintf(cfPtr, "%d ", sortedWhite[j]); // Writes white ball numbers to file
            }
            
            goldBall = rand()% 25+1; // gold ball number
            fprintf(cfPtr, "| %d |", goldBall); // Writes gold ball number to file

            megaplier = rand()% 2; // Binary megaplier option
            fprintf(cfPtr, " %d\n", megaplier); // Writes binary megaplier number to file
        }

        fclose (cfPtr); // Close file
    }

                    /* Part 2 - Conduct the Draw */
    printf("\nYou have requested %d ticket(s)\n", amntTicket);

    // Generate winning numbers //
    shuffle(whiteBallCount, 70);

    for (int i = 0; i<5; i++)
    {
        winningTicket[i] = whiteBallCount[i];
    }
    sortWhiteBalls(winningTicket); // Sort white ball numbers of winning ticket

    winningTicket[5] = rand()% 25+1;

    printf("Winning numbers: ");
    for (int j=0; j<5; j++)
    {
        printf("%d ", winningTicket[j]);
    }
    printf("| %d", winningTicket[5]);
    printf("\n\n");
    
    printf("_______________________________________________________________\n\n");

    // fopen opens the file; exits if file cannot be opened
    if ((cfPtr = fopen("tickets.txt", "r")) == NULL)
    {
        printf("File could not be opened.");
    }

    else
    {
        // Copy all the tickets to the tickets structure array
        for (int i = 0; i<amntTicket; i++)
        {
            fscanf(cfPtr, "%d | ", &tickets[i].identifier);

            for (int j = 0; j<5; j++)
            {
                fscanf(cfPtr, "%d ", &tickets[i].whiteBalls[j]);
            }

            fscanf(cfPtr, "| %d | ", &tickets[i].goldBall);
            fscanf(cfPtr, "%d\n", &tickets[i].megaplier);
        }
        fclose(cfPtr);
    }   

    // Generate Megaplier
    int multiplier = generate_Megaplier(); // Generate random multiplier of either 2x,3x,4x or 5x

    // Outcome Variables
    int prize_outcomes[] = {2, 4, 6, 8, 10, 12, 16, 20, 30, 40, 50, 200}; // payout amounts for each outcome   
    int outcomes_noMega[12] = {0}; // array that stores the amount of times each outcome has occurred
    int outcomes_Mega[12] = {0}; // same as array above but with the consideration of a megaplier
    int prize_index = 0; // Used to access correct element to increment
    int big_Winner = 0;
    int small_Winner = 0;

    // Winning Tickets Display //
    printf("Winning Tickets:");

    // Check the tickets for matching numbers and determine payouts
    for (int i = 0; i < amntTicket; i++) 
    {
        int white_balls_matched = 0;
        int gold_ball_matched = 0;
        int prize = 0;

        // Check white balls
        for (int j = 0; j < 5; j++) 
        {
            for(int k = 0; k < 5; k++)
            {
                if (tickets[i].whiteBalls[k] == winningTicket[j]) 
                {
                    // If a white ball number matches any of the numbers in the winning ticket numbers, increment no. of matches
                    white_balls_matched++; 
                }
            }
        }

        // Check gold ball
        if (tickets[i].goldBall == winningTicket[5]) 
        {
            gold_ball_matched = 1;
        }

        // Determine payout based on number of matches and megaplier value
        if (white_balls_matched == 0 && gold_ball_matched == 0) 
        {
            prize = 0;
        } 
        else if (white_balls_matched == 0 && gold_ball_matched == 1) 
        {
            if(tickets[i].megaplier == 1)
            {
                prize = 2 * multiplier;
            }
            else
            {
                prize = 2;
            }
        } 
        else if (white_balls_matched == 1 && gold_ball_matched == 1) 
        {
            if(tickets[i].megaplier == 1)
            {
                prize = 4 * multiplier;
            }
            else
            {
                prize = 4;
            }
        } 
        else if (white_balls_matched == 2 && gold_ball_matched == 1) 
        {
            if(tickets[i].megaplier == 1)
            {
                prize = 10 * multiplier;
            }
            else
            {
                prize = 10;
            }
        }
        else if (white_balls_matched == 3 && gold_ball_matched == 0) 
        {
            if(tickets[i].megaplier == 1)
            {
                prize = 10 * multiplier;
            }
            else
            {
                prize = 10;
            }
        }  
        else if (white_balls_matched == 3 && gold_ball_matched == 1) 
        {
            if(tickets[i].megaplier == 1)
            {
                prize = 200 * multiplier;
            }
            else
            {
                prize = 200;
            }
        } 
        else if (white_balls_matched == 4 && gold_ball_matched == 0) 
        {
            if(tickets[i].megaplier == 1)
            {
                prize = 500 * multiplier;
            }
            else
            {
                prize = 500;
            }
        } 
        else if (white_balls_matched == 4 && gold_ball_matched == 1) 
        {
            if(tickets[i].megaplier == 1)
            {
                prize = 10000 * multiplier;
            }
            else
            {
                prize = 10000;
            }
        } 
        else if (white_balls_matched == 5 && gold_ball_matched == 0) 
        {
            if(tickets[i].megaplier == 1)
            {
                prize = 1000000 * multiplier;
            }
            else
            {
                prize = 1000000;
            }
        } 
        else if (white_balls_matched == 5 && gold_ball_matched == 1) 
        {
            prize = 100000000;
        } 

        // Amount of megaplier numbers //
        if (tickets[i].megaplier == 1)
        {
            amntMegaplier++;
        }

        // Print Winning tickets
        if (prize != 0)
        {
            winners++; // Increment the number of total winners
            payout += prize; //Add the prize amount to the total payout amount

            if (prize > 400) // Payout for amounts more than $400
            {
                big_Winner++;

                printf("\nTicket Number %d\n", tickets[i].identifier);
                for (int j = 0; j<5; j++)
                {
                    printf("%d ", tickets[i].whiteBalls[j]);
                }
                printf("| %d \n", tickets[i].goldBall);
                printf("Won: $%d\n", prize);
            }

            else // Payout for amounts less than $400
            {
                small_Winner++;

                for (int i = 0; i<12; i++) 
                {
                    if (prize_outcomes[i] == prize) 
                    {
                        prize_index = i;  // Use prize_index to get the correct position of the outcome arrays
                        i = 12;
                    }
                }

                if (tickets[i].megaplier == 1) 
                {
                    outcomes_Mega[prize_index]++; //If megaplier is applied, increment
                } 
                else 
                {
                    outcomes_noMega[prize_index]++; // If not, increment other array
                }
            }
        }
    }

    if (big_Winner == 0 && small_Winner == 0)
    {
        printf("No winners this time!\n");
    }
    else if (big_Winner == 0)
    {
        printf("\nNo big winners this time!\n");
    }

    printf("\n_______________________________________________________________\n");

    if (small_Winner != 0)    
    {
        // Payout Info for Amounts less than $400
        printf("\nIn addition, without a Megaplier:\n");
        for (int i = 0; i<12; i++)
        {
            if(outcomes_noMega[i] != 0)
            {
                printf("%d won $%d\n", outcomes_noMega[i], prize_outcomes[i]);
            }
        }

        printf("With a Megaplier:\n");
        for (int i = 0; i<12; i++)
        {
            if(outcomes_Mega[i] != 0)
            {
                printf("%d won $%d\n", outcomes_Mega[i], prize_outcomes[i]);
            }
        }
    }

    else
    {
        printf("No winners this time!\n");
    }

    // Statistical Summary //
    printf("\n_______________________________________________________________\n");
    printf("\n\nThere were %d winner(s) in total!\n", winners);
    printf("There were %d megaplier(s) applied!\n", amntMegaplier);

    sales = (2 * amntTicket) + amntMegaplier;
    printf("The total sales were: %.2lf \n", sales);
    printf("The total payout was: %.2lf \n", payout);

    if (winners == 0)
    {
        avg_won == 0; //If there are no winners, avg_won is 0
    }
    else
    {
        avg_won = payout / winners; // Otherwise get average
    }

    printf("The average amount won was $%.2lf \n", avg_won);

    profit = sales - payout;
    printf("The total profit was: $%.2lf\n", profit);

    // Jackpot/Annuity Options //
    printf("\n_______________________________________________________________\n");
    printf("\nIf the Jackpot is won:\n");
    printf("Cash option: $100,000,000 \n\n");
   
    printf("Annuity option: Initial Payment: $1500000\n");
    for (int i = 0; i<29; i++)
    {
        annuityPay *= 1.05;
        printf("Year %d: payment is $%.2lf\n", i+1, annuityPay);
    }

    // End Program //
    printf("\nThank you for playing MegaMillions!\n");
    return 0;
}

void sortWhiteBalls(int *array) //Sort white ball numbers in order function
{
	for (int i = 1 ; i < 5; i++) 
    {
	    int j= i;

	    while ( j > 0 && array[j] < array[j-1]) 
        {
	      int swap          = array[j];
	      array[j]   = array[j-1];
	      array[j-1] = swap;

	      j--;
	    }
	}
}

void shuffle(int array[], int n) //Fisher-Yates shuffle algorithm
{
    // read array from the highest index to lowest
    for (int i = n-1; i >= 1; i--)
    {
        // generate a random number 'j' such that '0 <= j <= i'
        int j = rand() % (i + 1);
 
        // swap the current element with the randomly generated index
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int generate_Megaplier() // Function that generates a multiplier for lottery
{
    int multipliers[] = {2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 5}; // Array that contains multiplier number
    int index = rand() % 15; // randomly select index from multipliers array
    return multipliers[index]; //returns random multiplier for results
}