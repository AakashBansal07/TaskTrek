#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024 // change this later according to use case
#define MAX_FIELDS 100       // change this later according to use case

#define MAX_TASK_LENGTH 100
#define MAX_TASKS 1024

/*
    THINGS IMPROVED:
        1. Made the code platform independent
        2. Wrote better code by:
            i) Removing Nested conditions
            ii) Better Error Handling
        3. Increased Space efficiency like reducing space of date[] from 20 chars to 8 chars
*/

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
    Notes_choice,
    Events_choice,
    Tasks_choice,
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
void throw_error()
{
    perror("");
    printf("Error Code: %d\n", errno);
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
void read_csv(const char *filename)
{
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

            // Print the separator line
            for (int i = 0; i < field_count; i++)
            {
                printf("|_________________");
            }
            printf("|\n");
        // }

        for (int i = 0; i < field_count; i++)
        {
            // fields[i][strcspn(fields[i], "\n")] = '\0';
            printf("| %-15s ", fields[i]);
        }
        printf("|\n");
    }

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

void show_calendar_menu()
{
    // clear_console();
    printf("------------------------MENU------------------------\n\n");
    printf("1. To look at another month\n");
    printf("2. To look at all your tasks on a particular date\n");
    printf("3. To add new tasks\n");
    printf("4. To delete a particualr task\n");
    printf("5. To go back to previous menu\n\n");
    printf("----------------------------------------------------\n\n");
    printf("Enter your choice: ");
}

void show_main_menu()
{
    // clear_console();
    printf("------------------------MENU------------------------\n\n");
    printf("1. Calendar\n2. Notes\n3. Events\n4. Tasks\n5. Exit\n");
    printf("----------------------------------------------------\n\n");

    printf("Enter your choice: ");
}

void wait() {
    char temp[2];
    printf("Press Enter to proceed\n");
    fgets(temp, 2, stdin);
}

void calendar()
{
    clear_console();
    printf("\nCalendar\n");

    time_t t = time(NULL); // number of seconds from 1 Jan 1970
    struct tm *tm_local = localtime(&t);

    int current_month = tm_local->tm_mon;
    int current_year = (tm_local->tm_year) + 1900; // for some reason tm_year is 1900 less than actual year :/

    print_calendar(current_month, current_year);
    show_calendar_menu();

    int choice;
    scanf("%d", &choice);

    if (choice == 1)
    {
        clear_console();
        printf("Enter month and year: ");
        int mm, yyyy;
        scanf("%d%d", &mm, &yyyy);
        print_calendar(mm, yyyy);
        wait();
    }

    if (choice == 2)
    {   
        clear_console();
        read_csv("tasks.csv");
        getchar();
    }

    // char date[8]; // in dd/mm/yy format (1 byte each char)
    // FILE *tasks_file_ptr = fopen("tasks.csv", "r");
    // FILE *events_file_ptr = fopen("events.csv", "r");

    // printf("Enter date(dd/mm/yy): ");
    // fflush(stdin);
    // fgets(date, 8, stdin);

    // if (tasks_file_ptr == NULL)
    //     throw_error();
    // if (events_file_ptr == NULL)
    //     throw_error();

    // if (is_file_empty("tasks.csv"))
    //     printf("No tasks");
}
void notes()
{
    clear_console();
    int choice;
    printf("Enter 1 to see all notes\n");
    printf("Enter 2 to go back\n");
    if (choice == 1)
    {
    }
}
void events()
{
    return;
}
void tasks()
{
    return;
}

int main()
{
    clear_console();
    read_csv("data.csv");
    clear_console();

    while (1)
    {
        int8_t choice;
        fflush(stdin);

        show_main_menu();

        scanf("%hhd", &choice);

        printf("%d\n", choice);

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

        case Notes_choice:
            notes();
            break;

        case Events_choice:
            events();
            break;

        case Tasks_choice:
            tasks();
            break;

        case Exit_choice:
            return 0;

        default:
            printf("Invalid Input\n");
            break;
        }
        // }
    }
}
