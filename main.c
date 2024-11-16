#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 2048 // change this later according to use case
#define MAX_FIELDS 100       // change this later according to use case

#define MAX_TASK_LENGTH 100
#define MAX_TASKS 1024

#define MAX_EVENT_HEAD_SIZE 256
#define MAX_EVENT_DESC_SIZE 1024

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
void print_csv(const char *filename)
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

        for (int i = 0; i < field_count; i++)
        {
            printf("|_________________");
        }
        printf("|\n");

        for (int i = 0; i < field_count; i++)
        {
            // fields[i][strcspn(fields[i], "\n")] = '\0';
            printf("| %-15s ", fields[i]);
        }
        printf("|\n");
    }

    fclose(file);
}
void clear_buffer()
{
    int c;
    /* discard all characters up to and including newline */
    while ((c = getchar()) != '\n' && c != EOF)
        ;
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

void show_main_menu()
{
    clear_console();
    printf("------------------------MENU------------------------\n\n");
    printf("1. Calendar\n2. Notes\n3. Events\n4. Tasks\n5. Exit\n");
    printf("----------------------------------------------------\n\n");

    printf("Enter your choice: ");
}

void show_calendar_menu()
{
    printf("------------------------MENU------------------------\n\n");
    printf("1. To look at another month\n");
    // printf("2. To look at all your tasks on a particular date\n");
    // printf("3. To add new tasks at in current month\n");
    // printf("4. To delete a particualr task in current month\n");
    printf("2. To go back to previous menu\n\n");
    printf("----------------------------------------------------\n\n");
    printf("Enter your choice: ");
}

void show_events_menu()
{
    printf("------------------------MENU------------------------\n\n");
    printf("1. Show all events\n");
    printf("2. Add new event\n");
    printf("3. Delete event\n");
    printf("----------------------------------------------------\n\n");
    printf("Enter your choice: ");
}

void wait() // make it better by making it take a string or char instead
{
    int temp;
    printf("Enter any number to proceed\n");
    scanf("%d", &temp);
    clear_buffer();
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
    wait();

    clear_console();
    show_calendar_menu();

    int choice;
    scanf("%d", &choice);
    clear_buffer();

    if (choice == 1)
    {
        clear_console();
        printf("Enter month and year: ");
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
    printf("Enter 1 to see all notes\n");
    printf("Enter 2 to go back\n");
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
void tasks()
{
    clear_console();
    print_csv("tasks.csv");
    wait();
}

int main()
{
    clear_console();
    print_csv("data.csv");
    clear_console();

    while (1)
    {
        int8_t choice;
        fflush(stdin);

        show_main_menu();

        scanf("%hhd", &choice);
        clear_buffer();

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
