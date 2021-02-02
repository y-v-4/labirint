#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<math.h>



#define L 24 //długość boku
#define WALL  0 //ściana
#define ROUTE 1 //droga
#define PLAYER 2//gracz

// Kontrolujemy trudność labiryntu, im większa wartość, tym niższa trudność, minimalna wartość wynosi 0
int Rank = 6;

void menu();   //menu
void start();  //rozpocznij grę
void degree(); //trudność gry
void explain();//Opis gry
int init(int** Maze);  //Zainicjujemy labirynt
void print(int** Maze);//Narysujemy labirynt
void CreateMaze(int **maze, int x, int y);    //Tworzenie labiryntów
void move(int** Maze, char t, int *x, int *y);//ruch postaci

int main() {
    menu();
    return 0;
}

void menu() { 
    while(1) {
        system("cls"); //Wyczyść ekran
        char t;
        printf("*******(Pass the maze)*******");
        printf("\n==========================\n");
        printf("\n||   1. start the game    ||\n");
        printf("\n||   2. game description  ||\n");
        printf("\n||   3. difficulty levels ||\n");
        printf("\n||   4. close the game    ||\n");
        printf("============================\n");
        t=getch();   
        switch(t) {
           case '1':
                start();
                break;  //rozpocznij grę
            case '2':
                explain();
                break;  //opis gry
            case '3':
               degree();
                break;  //wybierz trudność gry
            case '4':
                printf("Good-bye!");
                Sleep(1500);
                exit(0);
                break;  //koniec gry
            default :
                break;
        }
    }
}

void CreateMaze(int **maze, int x, int y) {
    maze[x][y] = ROUTE;

    int direction[4][2] = { { 1,0 },{ -1,0 },{ 0,-1 },{ 0,1 } };
    int i, j;
    for (i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp = direction[0][0];
        direction[0][0] = direction[r][0];
        direction[r][0] = temp;
        temp = direction[0][1];
        direction[0][1] = direction[r][1];
        direction[r][1] = temp;
    } 
    
    for (i = 0; i < 4; i++) {
        int dx = x;
        int dy = y;
        
        int range = 1 + (Rank == 0 ? 0 : rand() % Rank);
        while (range > 0) {
            
            dx += direction[i][0];
            dy += direction[i][1];
            
            if (maze[dx][dy] == ROUTE) {
               break;
           }
            
            int count = 0, k;
            for (j = dx - 1; j < dx + 2; j++) {
                for (k = dy - 1; k < dy + 2; k++) {
                   
                    if (abs(j - dx) + abs(k - dy) == 1 && maze[j][k] == ROUTE) {
                        count++;
                    }
                }
            }
            
            if (count > 1)
               break;
            
            range -= 1;
            maze[dx][dy] = ROUTE;
        }
       
       if (range <= 0) {
           CreateMaze(maze, dx, dy);
        }
    }
}

int init(int** Maze) {
    int i;
   
     for (i = 0; i < L; i++) {
        Maze[i][0] = ROUTE;
        Maze[0][i] = ROUTE;
        Maze[i][L - 1] = ROUTE;
        Maze[L - 1][i] = ROUTE;
    }
    
    CreateMaze(Maze, 2, 2);
    // Narysuj wejście i wyjście z labiryntu, nadając graczowi pozycję wyjściową
    Maze[2][1] = PLAYER;
    //Ze względu na losowość algorytmu istnieje pewne prawdopodobieństwo, że nie ma wyjścia (L-3, L-2) w tym czasie trzeba znaleźć wyjście
   for (i = L - 3; i >= 0; i--) {
        if (Maze[i][L - 3] == ROUTE) {
            Maze[i][L - 2] = ROUTE;
           // Zwraca rzędną, w której znajduje się wyjście
            return i;
        }
    }
}

void print(int** Maze) {
   int i, j;
    for (i = 0; i < L; i++) {
        for (j = 0; j < L; j++) {
            if (Maze[i][j] == ROUTE)
                printf("  ");// Wskazuje drogę
            else if(Maze[i][j] == WALL)
                printf("aa");// Reprezentuje ścianę
            else
               printf("**");//gracz
        }
        printf("\n");
    }
}

void move(int** Maze, char t, int *x, int *y) {
    int i = *x, j = *y;//Zapisz oryginalną lokalizację
    switch(t) {
        case 'w':    //w górę
            *x -= 1;
            break;
        case 's':    //w dół
            *x += 1;
           break;
        case 'a':    //w lewo
           *y -= 1;
           break;
        case 'd':    //w prawo
           *y += 1;
           break;
        default:
            break;
    }
    if(*x>=0 && *x<L-1 && *y>=0 && *y<L-1 && Maze[*x][*y]!=WALL) {
       Maze[i][j] = 1;
        Maze[*x][*y] = 2;
    } else {
       *x = i;
        *y = j;
    }
}

void start() { 
    char t;
   
    int *p, *q;
    int x = 2, y = 1, out = 0, i = 0;
    p = &x;
    q = &y;
   // Funkcja inicjalizacji generatora liczb losowych
    srand((unsigned)time(NULL));
    // Dotyczy wyboru miejsca
   int **Maze = (int**)malloc(L * sizeof(int *));
    for (i = 0; i < L; i++) {
        Maze[i] = (int*)calloc(L, sizeof(int));
    }
    
    out = init(Maze);
    // Gra się rozpoczęła.
   system("cls");
    print(Maze);
    while(t = getch()) {
        if(t == 27)   //Jeśli wpiszesz klawisz ESC, zakończ grę z powrotem w menu głównym
            break;
        system("cls");// Wyczyść ekran
        move(Maze, t, p, q);
        print(Maze);
        if(x == out && y == L-2) {// Jesteśmy przy wyjściu, koniec gry.
            system("cls");
            printf("=============\n");
           printf("Congratulations!You've won !\n");
            printf("=============\n");
           printf("Coming back to the main menu……");
           Sleep(1500);
           break;
       }
    }
    // Koniec gry, zwolnij pamięć
    for (i = 0; i < L; i++) free(Maze[i]);
    free(Maze);
}

void explain() { 
    while(1) {
        char t;
        system("cls");
        printf("=================================================\n");
printf ("The operation of the game is as follows\n");
printf("\n1.Set the difficulty of the game or start playing immediately, initially there is an easy difficulty of the game\n");
printf("\n2. Through w,s,a, d, the four character control keys move up and down to the left and right\n");
printf("\n3. In any interface, press esc to return to the Main Menu\n");
printf ("\n4.Win condition: find the exit by moving the player through the maze \n");
printf("=================================================\n");
       t=getch();  
        switch(t) {
               
            case 27:
               
                menu();
                break;
            default :
                break;
        }
    }
}

void degree() { 
    while(1) {
        char t;
       system("cls");
        printf("========================\n");
        printf("Enter 1,2,3 in order to adjust the difficulty：\n");
        printf("\n||    1. Easy         ||\n");
        printf("\n||    2. Medium       ||\n");
        printf("\n||    3. Difficult.   ||\n");
        printf("=======================\n");
        t=getch();  
        switch(t) {
           case '1':
                Rank = 6;
                printf("\nCurrent Difficulty: Easy, going back to the Main menu……");
                Sleep(1500);
               menu();//Вернитесь в Главное меню
                break; 
            case '2':
                Rank = 3;
                printf("\nCurrent Difficulty: Medium , going back to the Main menu……");
                Sleep(1500);
                menu();//Вернитесь в Главное меню
               break;
            case '3':
                Rank = 0;
                printf("\nCurrent Difficulty: Difficult., going back to the Main menu………");
                Sleep(1500);
                menu();//Вернитесь в Главное меню
                break;
            case 27:
                menu();
                break; 
            default :
                break;
        }
    }
}
