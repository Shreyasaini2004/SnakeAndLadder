#include<stdio.h>
#include<time.h>
#include<stdbool.h>
#include<stdlib.h>
#include<limits.h>
#include<queue>

#define MAX_ROWS 10
#define MAX_COLS 10

struct Cell
{
    int row;
    int col;
    int position;
};

struct SnakeOrLadder
{
    int start;
    int end;
};

struct Pair
{
    int val;
    int step;
};

struct Player
{
    int position;
    char name[5];
};

struct Pair *newPair(int val,int step)
{
struct Pair *pair=(struct Pair *)malloc(sizeof(struct Pair));
pair->val=val;
pair->step=step;
return pair;
}

struct Queue
{
    int front,rear,size;
    unsigned capacity;
    struct Pair *array;
};

void map(struct Cell board[][MAX_COLS],int rows,int cols,int position,int *row,int *col)
{
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            if(board[i][j].position==position)
            {
                *row=i;
                *col=j;
                return;
            }
        }
    }
    // If position is not found,set row and col to -1 to indicate not found
    *row=-1;
    *col=-1;
}

//Function to create a Queue
struct Queue *createQueue(unsigned capacity)
{
    struct Queue *queue=(struct Queue *)malloc(sizeof(struct Queue));
    queue->capacity=capacity;
    queue->front=queue->size=0;
    queue->rear=capacity-1;
    queue->array=(struct Pair *)malloc(queue->capacity * sizeof(struct Pair));
    return queue;
}

//Function to check if the queue is empty
bool isEmpty(struct Queue *queue)
{
    return (queue->size==0);
}

//Function to check if the queue is full
bool isFull(struct Queue *queue)
{
    return (queue->size==queue->capacity);
}

void enqueue(struct Queue *queue,struct Pair *item)
{
    if(isFull(queue))
       return;
    queue->rear=(queue->rear+1)%queue->capacity;
    queue->array[queue->rear]=*item;
    queue->size=queue->size+1;
}

struct Pair dequeue(struct Queue *queue)
{
    struct Pair item={-1,1};
    if(isEmpty(queue))
       return item;
    item=queue->array[queue->front];
    queue->front=(queue->front+1)%queue->capacity;
    queue->size=queue->size-1;
    return item;
}

void shortestPath(struct Cell board[][MAX_COLS],int rows,int cols,int matrix[][MAX_COLS],int start,int end)
{
    int distance[rows][cols];
    int parent[rows][cols];
    int diceNumbers[rows][cols]; //Array to store dice numbers

    //Initialize distance,parent and diceNumbers arrays
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            distance[i][j]=1e9;
            parent[i][j]=-1;
            diceNumbers[i][j]=-1; //Initialize with an invalid value
        }
    }

 //Set distance to 0 for the starting position 

 int startRow,startCol;
 map(board,rows,cols,start, &startRow, &startCol);
 distance[startRow][startCol]=0;

 //Create a queue for BFS
 struct Queue *queue=createQueue(rows * cols);
 struct Pair *startPair=newPair(start,0);
 enqueue(queue,startPair);

 while(!isEmpty(queue))
 {
    struct Pair current=dequeue(queue);
    int num=current.val;
    int d=current.step;

    for(int i=1;i<=6;i++)
    {
        if(num+i<=rows*cols)
        {
            int newNum=num+i;
            int newRow,newCol;
            map(board,rows,cols,newNum,&newRow,&newCol);

            if(d+1<distance[newRow][newCol])
            {
                parent[newRow][newCol]=num;
                distance[newRow][newCol]=d+1;
                diceNumbers[newRow][newCol]=i; //Record the dice number

                if(matrix[newRow][newCol]==-1){
                    struct Pair *newPair1=newPair(newNum,d+1);
                    enqueue(queue,newPair1);
                }
                else
                {
                    int newVal=matrix[newRow][newCol];
                    int newR,newC;
                    map(board,rows,cols,newVal,&newR,&newC);
                    parent[newR][newC]=newNum;
                    distance[newR][newC]=d+1;
                    struct Pair *newPair2=newPair(newVal,d+1);
                    enqueue(queue,newPair2);
                }
            }
        }
    }
 }

 int r,c;
 map(board,rows,cols,rows*cols,&r,&c);
 printf("%d",rows*cols);

 //Create an array to store the dice numbers in the path
 int dicePath[rows*cols];
 int pathLength=0;


 while(parent[r][c]!=-1)
 {
    printf("<-");
    // if(matrix[r][c]!=-1){
        dicePath[pathLength++]=diceNumbers[r][c];
        //} // Record the dice Number
        printf("%d",parent[r][c]);
        printf("<-");
        map(board,rows,cols,parent[r][c],&r,&c);
    }  

    printf("\n");
    printf("MINIMUM NUMBER OF DICE THROW %d\n",distance[0][0]);
    printf("Dice Number Used: ");

    // Print the dice numbers in reverse order to print the path from start to end
    for(int i=pathLength-1;i>=0;i--)
    {
        if(dicePath[i]!=-1)
        {
            printf("%d\t",dicePath[i]);
        }
    }
    printf("\n");
    }

    void printBoardWithPlayers(struct Cell board[][MAX_COLS],int rows,int cols,struct Player player1,struct Player player2)
    {
        for(int i=rows-1;i>=0;i--)
        {
            for(int j=0;j<cols;j++)
            {
                if(player1.position==board[i][j].position)
                {
                    printf("(%d) P1\t",board[i][j].position);
                }
                else if(player2.position==board[i][j].position)
                {
                    printf("(%d) P2\t",board[i][j].position);
                }
                else
                {
                    printf("(%d)\t",board[i][j].position);
                }
        }
        printf("\n");
        }
    }

    //Function to create the board 
    //the board will follow zigzag pattern start remains the same
    //switching the direction after each loop the max it goes till the number of cols then switch the position

    void createBoard(struct Cell board[][MAX_COLS],int rows,int cols)
    {
        int num=1;
        int direction=1;

        //Initialize the board and store the cell position
        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<cols;j++)
            {
                board[i][j].row=i;
                board[i][j].col=j;
                board[i][j].position=0;
            }
        }

        //Fill the board with numbers in a zigzag pattern and update positions
        for(int i=rows-1;i>=0;i--)
        {
            if(direction==1)
            {
                for(int j=0;j<cols;j++)
                {
                    board[i][j].position=num++;
                }
            }
            else
            {
                for(int j=cols-1;j>=0;j--)
                {
                    board[i][j].position=num++;
                }
            }
            direction=-direction;// direction change after every row 
        }
    }

void createMatrix(struct Cell board[][MAX_COLS],int matrix[][MAX_COLS],int rows,int cols,struct SnakeOrLadder *snakesAndLadders,int numSnakesAndLadders)
{
    // Initialize the matrix with -1 in beginning
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            matrix[i][j]=-1;
        }
    }

    for(int i=0;i<numSnakesAndLadders;i++)
    {
        int position=snakesAndLadders[i].start;
        int value=snakesAndLadders[i].end;
        int row,col;
        map(board,rows,cols,position,&row,&col);
        matrix[row][col]=value;
    }
}

void printBoard(struct Cell board[][MAX_COLS],int rows,int cols)
{
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            printf("(%d,%d)-%d\t",board[i][j].row,board[i][j].col,board[i][j].position);
        }
        printf("\n");
    }
}

//Function to create random snake and ladder positions
void createRandomSnakesAndLadders(struct SnakeOrLadder *snakesAndLadders,int numSnakesAndLadders,int totalcells)
{
    srand(time(NULL));
    for(int i=0;i<numSnakesAndLadders;i++)
    {
        int start,end;

        start=rand()%(totalcells-1)+1;
        end=rand()%(totalcells-start)+(start+1);

        //Create the snake and ladder entry
        snakesAndLadders[i].start=start;
        snakesAndLadders[i].end=end;
    }
}

void playGame(struct Cell board[][MAX_COLS],int rows,int cols,int matrix[][MAX_COLS])
{
    struct Player player1={1,"P1"};
    struct Player player2={1,"P2"};
    int goal=rows*cols;
    int player1Turn=1;

    while(player1.position!=goal && player2.position!=goal)
    {
        int dice=rand()%6+1;
        if(player1Turn)
        {
            //Player1's turn
            printf("Player 1's Turn \n");
            printf("Current Player: %s\n",player1.name);
            printf("From Position: %d\n",player1.position);
            player1.position+=dice;
            if(player1.position>goal)
            {
                player1.position=goal;
            }

            int row1,col1;
            map(board,rows,cols,player1.position,&row1,&col1);
            if(matrix[row1][col1]!=-1)
            {
                int newPosition=matrix[row1][col1];
                map(board,rows,cols,newPosition,&row1,&col1);
                player1.position=board[row1][col1].position;
            }
            printf("Dice Move: %d\n",dice);
            printf("To Position: %d\n",player1.position);
        }
        else
        {
            //Player2's Turn
            printf("Player 2's Turn \n");
            printf("Current Player: %s\n",player2.name);
            printf("From Position: %d\n",player2.position);
            player2.position+=dice;
            if(player2.position>goal)
            {
                player2.position=goal;
            }

            int row1,col1;
            map(board,rows,cols,player2.position,&row1,&col1);
            if(matrix[row1][col1]!=-1)
            {
                int newPosition=matrix[row1][col1];
                map(board,rows,cols,newPosition,&row1,&col1);
                player2.position=board[row1][col1].position;
            }
            printf("Dice Move: %d\n",dice);
            printf("To Position: %d\n",player2.position);
        }

        //Print the board with player position
        printBoardWithPlayers(board,rows,cols,player1,player2);
       
       //Switch to the other player's turn
       player1Turn=!player1Turn;
    }

    if(player1.position==goal)
    {
        printf("Player 1 is the winner\n");
    }
    else
    {
        printf("Player 2 is the winner\n");
    }
    }

int main()
{
    int rows=10;
    int cols=10;

    struct Cell board[rows][10];

    //Create the board
    createBoard(board, rows, cols);

    //Create an array to store snake and ladder positions
    int numSnakesAndLadders=10;
    struct SnakeOrLadder snakesAndLadders[numSnakesAndLadders];

    //Create random snake and ladder positions
    createRandomSnakesAndLadders(snakesAndLadders,numSnakesAndLadders,rows*cols);

    //Print the board
    printBoard(board,rows,cols);

    //Map position 1 to (4,0)
    int row,col;
    int positionToFind=25;
    map(board,rows,cols,positionToFind,&row,&col);

    //Print the snake and ladder positions
    for(int i=0;i<numSnakesAndLadders;i++)
    {
        printf("(%d,%d)",snakesAndLadders[i].start,snakesAndLadders[i].end);
    }
    printf("\n");

    //Create a mapping of the board such that in which there is no snake or ladder we write -1 at that position else we write the position of the snake or ladder
    int matrix[rows][10];
    createMatrix(board,matrix,rows,cols,snakesAndLadders,numSnakesAndLadders);

   //Print the matrix
   printf("\n Matrix with snake and ladder values:\n");
   for(int i=0;i<rows;i++)
   {
    for(int j=0;j<cols;j++)
    {
        printf("%d\t",matrix[i][j]);
    }
    printf("\n");
   }

   //--------------TWO PLAYER CODE-----------
   
   while(1)
   {
    printf("\nChoose an option:\n");
    printf("1.Find the shortest path\n");
    printf("2.Play the game\n");
    printf("3.Generate the random position of snake and ladder\n");
    printf("4.QUIT\n");
    printf("\n");

    int choice;
    printf("Your Choice is");
    scanf("%d",&choice);
    
    switch(choice)
    {
        case 1:
        {
		printf("\nFinding the shortest path:\n");
        int start=1;
        int end=rows*cols;
        shortestPath(board,rows,cols,matrix,start,end);
        break;
    }
        case 2:
        	{
        printf("\nPlaying the game\n");
        playGame(board,rows,cols,matrix);
        break;
}
        case 3:
        	{
       createRandomSnakesAndLadders(snakesAndLadders,numSnakesAndLadders,rows*cols);
       createMatrix(board,matrix,rows,cols,snakesAndLadders,numSnakesAndLadders);
       //Print the matrix
       printf("\nMatrix with snake and ladder values:\n");
       for(int i=0;i<rows;i++)
       {
        for(int j=0;j<cols;j++)
        {
            printf("%d\t",matrix[i][j]);
        }
        printf("\n");
      }
       break;
}
       case 4:
        return 0;
        
       default:
        printf("Invalid choice.Please select a valid option.\n");
        break;
    }
   }
}

