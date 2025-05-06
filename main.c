// import statements
#include <ctype.h>
#include <errno.h>

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// define statements
#define MAX_LINE_LENGTH 2048 // change this later according to use case
#define MAX_FIELDS 100       // change this later according to use case

#define MAX_TASK_LENGTH 100
#define MAX_TASKS 1024

#define MAX_EVENT_HEAD_SIZE 256
#define MAX_EVENT_DESC_SIZE 1024

// Define colors
#define RESET "\x1b[0m" // modern consoles support ansi formatting \x1b is ESC key
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
#define BOLD "\x1b[1m"

/*
    THINGS IMPROVED:
        1. Made the code platform independent
        2. Wrote better code by:
            i) Removing Nested conditions
            ii) Better Error Handling
        3. Increased Space efficiency like reducing space of date[] from 20 chars to 8 chars
        4. Added Better ways to handle and show the data
*/

/*
    TO DO:
        1. Change current month that is shown
*/

/*
    CAN BE ADDED:
        // 1. Reward section/
        1. Reminders
        2. Batter Formatting
*/

int tLength = 0;
FILE *fp;

struct Todo
{
    char title[50];
    char createdAt[50];
    _Bool isCompleted;
} todos[20];

// basic functions
void clear_buffer();
void throw_error();
void clear_console();
void wait();
void print_csv(const char *filename);

// menus
void show_main_menu();
void show_calendar_menu();
void show_events_menu();

// todo related functions
void saveToFile();
void getFileSize();
void readFromFile();
void addTodo();
void printAllTodo();
void markAsComplete();
void deleteTodo();
void ShowOptions();
void run();

// other features related functions
bool is_file_empty(const char *filename);
bool is_leap_year(int year);
int get_days_in_month(int month, int year);
int get_starting_day(int month, int year);
void print_calendar(int month, int year);
void calendar();
void notes();
void events();
void tasks();

typedef struct
{
    int date;
    int month;
    int year;
    char task[MAX_TASK_LENGTH];
} Task;

typedef struct
{
    int month;
    int year;
    Task tasks[MAX_TASKS];
    int task_count;
} Calendar;

enum Choice
{
    Calendar_choice = 1,
    Tasks_choice,
    Events_choice,
    Notes_choice,
    Exit_choice
};
enum Months
{
    Jan,
    Feb,
    Mar,
    Apr,
    May,
    Jun,
    Jul,
    Aug,
    Sept,
    Oct,
    Nov,
    Dec
};
void tasks()
{
    clear_console();
    printf("\033[32;1m");
    // isThisFirstTime();
    run();
}

int main()
{
    clear_console();
    // print_csv("data.csv");
    // clear_console();

    while (1)
    {
        int8_t choice; // 8 bits 1 byte -64 to 64
        fflush(stdin);

        show_main_menu();

        scanf("%hhd", &choice);
        clear_buffer();

        // printf("%d\n", choice);

        // if (!isdigit(choice))
        // {
        //     printf("Input MUST be an integer");
        //     break;
        // }
        // else
        // {
        switch ((int8_t)choice)
        {
        case Calendar_choice:
            calendar();
            break;

        case Tasks_choice:
            tasks();
            break;

        case Events_choice:
            events();
            break;

        case Notes_choice:
            notes();
            break;

        case Exit_choice:
            exit(0);

        default:
            throw_error();
            break;
        }
        // }
    }
}

void clear_buffer()
{
    /*
     * Sometimes scanf stores new lines characters in buffer
     * which skips the next scanf or fgets statement
     * Hence this function must be called after every input statement
     * to clear the buffer.
     */
    int c;
    /* discard all characters up to and including newline */
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
void throw_error()
{
    /*
     * Function to make error handling easy and standardized
     */
    perror("");
    printf("Error Code: %d\n", errno);
}
void saveToFile()
{
    fp = fopen("todos.bin", "w");
    if (!fp)
        throw_error();
    else
    {
        for (size_t i = 0; i < tLength; i++)
        {
            fwrite(&todos[i], sizeof(struct Todo), 1, fp);
        }
        fclose(fp);
    }
}
void getFileSize()
{
    fseek(fp, 0L, SEEK_END);
    unsigned int long size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    tLength = size / sizeof(struct Todo);
}
void readFromFile()
{
    fp = fopen("todos.bin", "r");
    if (!fp)
        throw_error();
    else
    {
        getFileSize();
        for (size_t i = 0; i < tLength; i++)
        {
            fread(&todos[i], sizeof(struct Todo), 1, fp);
        }
        fclose(fp);
    }
}
void addTodo()
{
    // for todo title
    char userInput[50];
    printf("Type your todo \n>> ");
    scanf("%[^\n]s", userInput);
    // fgets(userInput, 50, stdin);
    // clear_buffer();
    strncpy(todos[tLength].title, userInput, 50);

    // add time
    char todoTime[50];
    struct tm cTime;
    time_t timeS = time(NULL);
    localtime_r(&timeS, &cTime);
    // 2/12 1:21
    snprintf(todoTime, 50, "%i/%i %i:%i", cTime.tm_mday, cTime.tm_mon + 1, cTime.tm_hour, cTime.tm_min);
    strcpy(todos[tLength].createdAt, todoTime);

    // set boolean to false
    todos[tLength].isCompleted = false;
    tLength++;
}
void printAllTodo()
{
    clear_console();
    printf("+----+-------------------------------------+--------------+-------------+\n");
    printf("| ID |            Todo Title               |  Created at  |  Completed  |\n");
    printf("+----+-------------------------------------+--------------+-------------+\n");

    for (int i = 0; i < tLength; i++)
    {
        if (todos[i].isCompleted)
        {
            printf("\033[10m");
        }
        else
        {
            printf("\033[1m");
        }

        printf("|%3d | %-35s | %-12s | %-13s |\n", i + 1, todos[i].title, todos[i].createdAt, (!todos[i].isCompleted) ? " ❌  No  " : " ✅  Yes ");
        printf("+----+-------------------------------------+--------------+-------------+\n");
    }
    // printf("\x1b[0m");
}
void markAsComplete()
{
    int todoId;
    printf("\n====== Mark Todo as Complete ======\n");

    printf("Enter the ID of the todo:\n>> ");
    if (scanf("%d", &todoId) != 1) {
        // Clear the input buffer
        while (getchar() != '\n');
        printf("Invalid input. Please enter a number.\n");
        return;
    }

    // Adjust index (if IDs are shown as 1-based to user)
    todoId--;

    if (todoId < 0 || todoId >= tLength)
    {
        printf("Invalid todo ID 😑\n");
        return;
    }

    if (todos[todoId].isCompleted)
    {
        printf("Todo \"%s\" is already completed ✅\n", todos[todoId].title);
    }
    else
    {
        todos[todoId].isCompleted = true;
        printf("Todo \"%s\" marked as completed ✅\n", todos[todoId].title);
    }
}

void deleteTodo()
{
    int todoId;
    printf("Enter the ID of todo \n>>");
    scanf("%d", &todoId);
    if (todoId < 0 || todoId > tLength)
    {
        printf("Invalid todo id 😑\n");
    }
    else
    {
        todoId--;
        memmove(todos + todoId, todos + todoId + 1, (tLength - todoId - 1) * sizeof(*todos));
        tLength--;
        printf("Your todo has been deleted 😵\n");
    }
}
void ShowOptions()
{
    char userChoice;

    printf(RESET);
    printf(RED);
    printf("Type 'A' to add, 'D' to delete & 'C' to mark complete or 'Q' to quit\n>>");
    userChoice = getchar();
    userChoice = toupper(userChoice);
    getchar();
    switch (userChoice)
    {
    case 'A':
        addTodo();
        break;
    case 'D':
        deleteTodo();
        break;
    case 'C':
        markAsComplete();
        break;
    case 'Q':
        // exit(0);
        main();
        break;
    default:
        printf("Command not found 😓\n");
        ShowOptions();
        break;
    }
    printf(RESET);
    saveToFile();
    printAllTodo();
    getchar();
    ShowOptions();
}
// void isThisFirstTime()
// {
//     if (access("todos.bin", F_OK) != -1)
//     {
//         readFromFile();
//         printAllTodo();
//         ShowOptions();
//     }
//     else
//     {
//         printf("Welcome to the Great Todo App\n");
//         addTodo();
//         saveToFile();
//         printAllTodo();
//         ShowOptions();
//     }
// }
void run()
{
    // FILE *file = ("todos")
    readFromFile();
    printAllTodo();
    ShowOptions();
}
bool is_file_empty(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        throw_error();

    fseek(file, 0, SEEK_END);
    __uint128_t size = ftell(file);

    fclose(file);

    return size == 0;
}
void clear_console()
{
#ifdef WIN32
    system("cls");
#else
    system("clear");
#endif
}
void print_csv(const char *filename)
{
    printf(RESET);
    printf(BOLD);
    printf(CYAN);
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        throw_error();

    char line[MAX_LINE_LENGTH];
    int line_count = 0;

    while (fgets(line, sizeof(line), file))
    {
        line_count++;
        char *token = strtok(line, ",");
        int field_count = 0;
        char *fields[MAX_FIELDS];

        while (token != NULL)
        {
            fields[field_count++] = token;
            token = strtok(NULL, ",");
        }

        for (int i = 0; i < field_count; i++)
        {
            printf("|_________________________");
        }
        printf("|\n");

        for (int i = 0; i < field_count; i++)
        {
            fields[i][strcspn(fields[i], "\n")] = '\0';
            printf("| %-23s ", fields[i]);
        }
        printf("|\n");
    }
    printf(RESET);
    fclose(file);
}

bool is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
int get_days_in_month(int month, int year)
{
    int days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap_year(year))
    {
        return 29;
    }
    return days_in_months[month - 1];
}
int get_starting_day(int month, int year)
{
    if (month < 3)
    {
        month += 12;
        year--;
    }
    int k = year % 100;
    int j = year / 100;
    int day = (1 + (13 * (month + 1)) / 5 + k + (k / 4) + (j / 4) - 2 * j) % 7;
    return (day + 6) % 7;
}
void print_calendar(int month, int year)
{
    const char *months[] = {"January", "February", "March", "April", "May", "June",
                            "July", "August", "September", "October", "November", "December"};
    const char *days_of_week[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    printf(BOLD);
    printf(CYAN);
    printf("\n    %s %d\n", months[month - 1], year);
    for (int i = 0; i < 7; i++)
    {
        printf("%s ", days_of_week[i]);
    }
    printf("\n");

    int days_in_month = get_days_in_month(month, year);
    int start_day = get_starting_day(month, year);

    for (int i = 0; i < start_day; i++)
    {
        printf("    ");
    }

    for (int day = 1; day <= days_in_month; day++)
    {
        printf("%3d ", day);
        if ((day + start_day) % 7 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}
void show_main_menu()
{
    printf(RESET);
    clear_console();
    printf(BOLD);
    printf(CYAN);
    printf("------------------------MENU------------------------\n\n");
    printf("\t\t  1. Calendar\n\t\t  2. Tasks\n\t\t  3. Events\n\t\t  4. Notes (Coming Soon)\n\t\t  5. Exit\n");
    printf("----------------------------------------------------\n\n");
    printf(RESET);

    printf(RED);
    printf("Enter your choice: ");
    printf(RESET);
}
void show_calendar_menu()
{
    printf(RESET);
    printf(BOLD);
    printf(CYAN);

    printf("------------------------MENU------------------------\n\n");
    printf("1. To look at another month\n");
    // printf("2. To look at all your tasks on a particular date\n");
    // printf("3. To add new tasks at in current month\n");
    // printf("4. To delete a particualr task in current month\n");
    printf("2. To go back to previous menu\n\n");
    printf("----------------------------------------------------\n\n");
    printf(RESET);

    printf(RED);
    printf("Enter your choice: ");
    printf(RESET);
}
void show_events_menu()
{
    printf(RESET);
    printf(BOLD);
    printf(CYAN);
    printf("------------------------MENU------------------------\n\n");
    printf("1. Show all events\n");
    printf("2. Add new event\n");
    printf("3. Delete event\n");
    printf("----------------------------------------------------\n\n");
    printf(RESET);

    printf(RED);
    printf("Enter your choice: ");
    printf(RESET);
}
void wait()
{
    // char temp;
    // printf("Enter any number to proceed\n");
    printf(RESET);
    printf(RED);
    printf("Press any key to continue");
    printf(RESET);
    getc(stdin);
    clear_buffer();
}

void calendar()
{
    clear_console();

    printf(BOLD);
    printf(CYAN);
    printf("\nCalendar\n");
    printf(RESET);

    time_t t = time(NULL); // number of seconds from 1 Jan 1970
    struct tm *tm_local = localtime(&t);

    int current_month = (tm_local->tm_mon) + 1;
    int current_year = (tm_local->tm_year) + 1900; // for some reason tm_year is 1900 less than actual year :/

    print_calendar(current_month, current_year);
    wait();

    clear_console();
    show_calendar_menu();

    int choice;
    scanf("%d", &choice);
    clear_buffer();

    if (choice == 1)
    {
        clear_console();

        printf(RESET);
        printf(RED);
        printf("Enter month and year in (MM space YYYY format): ");
        printf(RESET);

        int mm, yyyy;
        scanf("%d%d", &mm, &yyyy);
        clear_buffer();
        print_calendar(mm, yyyy);
        wait();
    }
}

void notes()
{
    clear_console();
    int choice;
    printf("1. See all notes\n");
    printf("2. To add a note\n");
    printf("3. To go back\n");
    wait();
}

void events()
{
    clear_console();
    show_events_menu();
    int choice;
    scanf("%d", &choice);
    clear_buffer();
    if (choice == 1)
    {
        clear_console();
        print_csv("events.csv");
        wait();
    }
    if (choice == 2)
    {
        char event_date[11];
        printf("Enter Date: ");
        if (fgets(event_date, 11, stdin) == NULL)
        {
            throw_error();
        }
        else
        {
            char *ptr = strchr(event_date, '\n');
            if (ptr) // ptr != NULL
            {
                *ptr = '\0';
            }
        }
        fflush(stdin);
        clear_buffer();
        puts(event_date);

        char event_heading[MAX_EVENT_HEAD_SIZE];
        printf("Enter Event Heading: ");
        if (fgets(event_heading, MAX_EVENT_HEAD_SIZE, stdin) == NULL)
        {
            throw_error();
        }
        else
        {
            char *ptr = strchr(event_heading, '\n');
            if (ptr)
            {
                *ptr = '\0';
            }
        }
        fflush(stdin);
        clear_buffer();
        puts(event_heading);

        char event_desc[MAX_EVENT_DESC_SIZE];
        printf("Enter Event Description: ");
        if (fgets(event_desc, MAX_EVENT_DESC_SIZE, stdin) == NULL)
        {
            throw_error();
        }
        else
        {
            char *ptr = strchr(event_desc, MAX_EVENT_DESC_SIZE);
            if (ptr)
            {
                *ptr = '\0';
            }
        }
        fflush(stdin);
        clear_buffer();
        puts(event_desc);

        FILE *events_file = fopen("events.csv", "a+");
        fprintf(events_file, "%s, %s, %s", event_date, event_heading, event_desc);
    }
    if (choice == 3)
    {
        printf("Enter Event Name to Delete ");
    }
}
// void tasks()
