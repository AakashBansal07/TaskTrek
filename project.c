#include <stdio.h>
#include<stdlib.h>
#include <string.h>

// int  system(const char *command);

int main();
void calendar();
void notes();
void events();
void tasks();

char* notynote(char* line, int num){
    char* tok;
    for (tok = strtok(line,";");
        tok && *tok;
        tok = strtok(NULL,";\n")){
            if (!--num){
                return tok;
            }
        }
        return NULL;
}

char* eventyevent(char* line, int num){
    char* tok;
    for (tok = strtok(line,";");
        tok && *tok;
        tok = strtok(NULL,";\n")){
            if (!--num){
                return tok;
            }
        }
        return NULL;
}

char* taskytask(char* line, int num){
    char* tok;
    for (tok = strtok(line,";");
        tok && *tok;
        tok = strtok(NULL,";\n")){
            if (!--num){
                return tok;
            }
        }
        return NULL;
}

struct events{
    char event[200], date[15];
};

struct notes{
    char heading[50], date[15], note[600];
};

struct tasks{
    char task[100], date[15];
};

void calendar(){
    system("cls"); 
    printf("\nCalendar\n");
    char date[50];
    int compare;
    int compare2;
    FILE* ptr1;
    ptr1 = fopen("tasks.csv","r");
    FILE* ptr2;
    ptr2 = fopen("events.csv","r");
    printf("Enter date(dd/mm/yy): ");
    fflush(stdin);
    gets(date);
    if (!ptr1){
        printf("No tasks");
        if (!ptr2){
            printf("No Events");
        }
    }
    char line1[1024];
    char line2[1024];
    while(fgets(line1, 1024, ptr1)){
        char* tmp = strdup(line1);
        compare = strcmp(date, taskytask(tmp,2));
        if (compare == 0){
            char* tmp = strdup(line1);
            printf("Task: %s\n", taskytask(tmp,3));
        }
        free(tmp);
    }
    while(fgets(line2, 1024, ptr2)){
        char* tmp = strdup(line2);
        compare2 = strcmp(date, eventyevent(tmp,2));
        if (compare2==0){
            char* tmp = strdup(line2);
            printf("Event: %s\n",eventyevent(tmp,3));
        }
    }
}

//Notes function
void notes(){
    system("cls"); 
    printf("\n");
    int choice_note;
    int no_of_notes;
    int note_number;
    int compare;
    while(2>0){
        printf("1. Add a note\n2. View your notes\n3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice_note);
        if (choice_note > 5 || choice_note <1){
            printf("Enter valid choice");
            continue;
        }
        else if (choice_note == 1){
            printf("Add note(s)\n");
            printf("How many notes do you want to enter: ");
            scanf("%d",&no_of_notes);
            FILE* fp;
            fp = fopen("notes.csv","a+");
            char heading[50];
            char content[1000];
            if (!fp) {
                printf("Can't open file\n");
                exit(1);
            }
            for (int i = 0; i < no_of_notes; i++){
                note_number = note_number + 1;
                printf("\nEnter heading: \n");
                fflush(stdin);
                gets(heading);
                printf("\nEnter Content: \n");
                gets(content);
                fprintf(fp, "%d;%s;%s\n", note_number, heading, content);
                printf("\nNew Note added.\n");
            }
            fclose(fp);
        }
        else if (choice_note == 2){
            printf("View your notes\n");
            printf("1. All notes\n2. Only one note\n");
            printf("Enter your choice: ");
            int choice3;
            scanf("%d",&choice3);
            if (choice3 == 1){
                FILE *ptr;
                ptr = fopen("notes.csv","r");
                if (!ptr){
                    printf("Error!! File doesn't exist");
                    exit(1);
                }
                else{
                    char buffer [1024];
                    int row = 0;
                    int column = 0;
                    while(fgets(buffer,1024, ptr)){
                        column = 0;
                        row ++;
                        if (row == 0)
                        continue;
                        char* value = strtok(buffer,";");
                        while(value){
                            if (column == 0){
                                printf("Note Number: ");
                            }
                            else if (column == 1){
                                printf("\nHeading: ");
                            }
                            else if (column == 2){
                                printf("\nContent: ");
                            }
                            printf("%s", value);
                            value = strtok(NULL,";");
                            column ++;
                        }
                        printf("\n");
                    }
                    fclose(ptr);
                }
            }
            else if (choice3 == 2){
                char heading[50];
                int compare;
                printf("Enter the heading: ");
                fflush(stdin);
                gets(heading);
                FILE *ptr;
                ptr = fopen("notes.csv","r");
                char line[1024];
                while(fgets(line, 1024, ptr)){
                    char* tmp = strdup(line);
                    compare = strcmp(heading, notynote(tmp,2));
                    if (compare == 0){
                        char* tmp = strdup(line);
                        printf("Heading: %s\n", notynote(tmp,2));
                        char* tmp2 = strdup(line);
                        printf("Content: %s\n", notynote(tmp2,3));
                    }
                    free(tmp);
                }
            }
        }
        else{
            main();
            break;
        }
        printf("\n");
    }
}

//Events function
void events(){
    system("cls"); 
    printf("\n");
    int choice_event;
    int no_of_events;
    int event_number;
    int compare;
    while(2>0){
        printf("1. Add a event\n2. View your events\n3. Past Events\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice_event);
        if (choice_event > 5 || choice_event <1){
            printf("Enter valid choice");
            continue;
        }
        else if (choice_event == 1){
            printf("Add an event\n");
            printf("How many events do you want to enter: ");
            scanf("%d",&no_of_events);
            FILE* fp;
            fp = fopen("events.csv","a+");
            char date[50];
            char event[1000];
            if (!fp) {
                printf("Can't open file\n");
                exit(1);
            }
            for (int i = 0; i < no_of_events; i++){
                event_number = event_number + 1;
                printf("\nEnter date(dd/mm/yy): \n");
                fflush(stdin);
                gets(date);
                printf("\nEnter Event: \n");
                gets(event);
                fprintf(fp, "%d;%s;%s\n", event_number, date, event);
                printf("\nNew Event added.\n");
            }
            fclose (fp);
        }
        else if (choice_event == 2){
            printf("View your events\n");
            printf("1. All events\n2. Only one event\n3. Past events\n");
            printf("Enter your choice: ");
            int choice4;
            printf("Enter your choice: ");
            scanf("%d", &choice4);
            if (choice4 == 1) {
                FILE *ptr;
                ptr = fopen("events.csv","r");
                if (!ptr){
                    printf("Error!! File doesn't exist");
                    exit(1);
                }
                else{
                    char buffer [1024];
                    int row = 0;
                    int column = 0;
                    while(fgets(buffer,1024, ptr)){
                        column = 0;
                        row ++;
                        if (row == 0)
                        continue;
                        char* value = strtok(buffer,";");
                        while(value){
                            if (column == 0){
                                printf("Event Number: ");
                            }
                            else if (column == 1){
                                printf("\nDate: ");
                            }
                            else if (column == 2){
                                printf("\nEvent: ");
                            }
                            printf("%s", value);
                            value = strtok(NULL,";");
                            column ++;
                        }
                        printf("\n");
                    }
                    fclose(ptr);
                }
            }
            else if(choice4 == 2){
                char date[50];
                printf("Enter the date(d/mm/yy): ");
                fflush(stdin);
                gets(date);
                FILE *ptr;
                ptr = fopen("events.csv","r");
                char line[1024];
                while(fgets(line, 1024, ptr)){
                    char* tmp = strdup(line);
                    compare = strcmp(date, eventyevent(tmp,2));
                    if (compare == 0){
                        char* tmp = strdup(line);
                        printf("Date: %s\n", eventyevent(tmp,2));
                        char* tmp2 = strdup(line);
                        printf("Event: %s\n",eventyevent(tmp2,3));
                    }
                }
            }
            else if (choice4 == 3){
                FILE *ptr;
                ptr = fopen("events2.csv","r");
                if (!ptr){
                    printf("Error!! File doesn't exist");
                    exit(1);
                }
                else{
                    char buffer [1024];
                    int row = 0;
                    int column = 0;
                    while(fgets(buffer,1024, ptr)){
                        column = 0;
                        row ++;
                        if (row == 0)
                        continue;
                        char* value = strtok(buffer,";");
                        while(value){
                            if (column == 0){
                                printf("Event Number: ");
                            }
                            else if (column == 1){
                                printf("\nDate: ");
                            }
                            else if (column == 2){
                                printf("\nEvent: ");
                            }
                            printf("%s", value);
                            value = strtok(NULL,";");
                            column ++;
                        }
                        printf("\n");
                    }
                    fclose(ptr);
                }
            }
        }
        else if (choice_event == 3){
            printf("Mark completed\n");
            char date_check[50];
            char status[20];
            int compare;
            printf("Enter the date(dd/mm/yy): ");
            fflush(stdin);
            gets(date_check);
            FILE *ptr;
            ptr = fopen("events2.csv","a+");
            char line[1024];
            FILE *ptr2;
            ptr2 = fopen("events.csv","r");
            printf("If completed type (completed): ");
            gets(status);
            printf("%s\n",status);
            while(fgets(line, 1024, ptr2)){
                char* tmp =strdup(line);
                compare = strcmp(date_check,eventyevent(tmp,2));
                if (compare == 0){
                    char* tmp = strdup(line);
                    char* tmp1 = strdup(line);
                    char* tmp2 = strdup(line);
                    fflush(stdin);
                    int comp;
                    comp = strcmp(status, "completed");
                    if (comp == 0){
                        fprintf(ptr,"%s;%s;%s\n",eventyevent(tmp,1),eventyevent(tmp1,2),eventyevent(tmp2,3));
                    }
                }
                free (tmp);
            }
            fclose (ptr);
            fclose (ptr2);
        }
        else{
            main();
            break;
        }
    }
}

//Tasks function
void tasks(){
    system("cls"); 
    printf("\n");
    int choice_tasks;
    int no_of_tasks;
    int task_number;
    int compare;
    while(2>0){
        printf("1. Add a task\n2. View your tasks\n3. Mark completed\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice_tasks);
        if (choice_tasks > 5 || choice_tasks <1){
            printf("Enter valid choice");
            continue;
        }
        else if (choice_tasks == 1){
            printf("Add a task\n");
            printf("How many notes do you want to enter: ");
            scanf("%d",&no_of_tasks);
            FILE* fp;
            fp = fopen("tasks.csv","a+");
            char date[50];
            char tasks[1000];
            if (!fp) {
                printf("Can't open file\n");
                exit(1);
            }
            for (int i = 0; i < no_of_tasks; i++){
                task_number = task_number + 1;
                printf("\nEnter Date(dd/mm/yy): \n");
                fflush(stdin);
                gets(date);
                printf("\nEnter Tasks: \n");
                gets(tasks);
                fprintf(fp, "%d;%s;%s\n", task_number, date, tasks);
                printf("\nNew Task added.\n");
            }
            fclose(fp);
        }
        else if (choice_tasks == 2){
            printf("View your tasks\n");
            printf("1. All\n2. One at a time\n3. Completed tasks\n");
            printf("Enter your choice: ");
            int choice5;
            printf("Enter your choice: ");
            scanf("%d", &choice5);
            if (choice5 == 1){
                    FILE *ptr;
                    ptr = fopen("tasks.csv","r");
                    if (!ptr){
                        printf("Error!! File doesn't exist");
                        exit(1);
                    }
                    else{
                        char buffer [1024];
                        int row = 0;
                        int column = 0;
                        while(fgets(buffer,1024, ptr)){
                            column = 0;
                            row ++;
                            if (row == 0)
                            continue;
                            char* value = strtok(buffer,";");
                            // printf("Note: %d\n",n);
                            while(value){
                                if (column == 0){
                                    printf("Task Number: ");
                                }
                                else if (column == 1){
                                    printf("\nDate: ");
                                }
                                else if (column == 2){
                                    printf("\nTasks: ");
                                }
                                printf("%s", value);
                                value = strtok(NULL,";");
                                column ++;
                            }
                            printf("\n");
                        }
                        fclose(ptr);
                    }
            }
            else if (choice5 == 2){
                char date[50];
                int compare;
                printf("Enter the date(dd/mm/yy): ");
                fflush(stdin);
                gets(date);
                FILE *ptr;
                ptr = fopen("tasks.csv","r");
                char line[1024];
                while(fgets(line, 1024, ptr)){
                    char* tmp =strdup(line);
                    compare = strcmp(date,taskytask(tmp,2));
                    if (compare == 0){
                        char* tmp = strdup(line);
                        printf("Date: %s\n", taskytask(tmp,2));
                        char* tmp2 = strdup(line);
                        printf("Task: %s\n", taskytask(tmp,3));
                    }
                    free(tmp);
                }
            }
            else if (choice5 == 3){
                FILE *ptr;
                    ptr = fopen("tasks2.csv","r");
                    if (!ptr){
                        printf("Error!! File doesn't exist");
                        exit(1);
                    }
                    else{
                        char buffer [1024];
                        int row = 0;
                        int column = 0;
                        while(fgets(buffer,1024, ptr)){
                            column = 0;
                            row ++;
                            if (row == 0)
                            continue;
                            char* value = strtok(buffer,";");
                            while(value){
                                if (column == 0){
                                    printf("Task Number: ");
                                }
                                else if (column == 1){
                                    printf("\nDate: ");
                                }
                                else if (column == 2){
                                    printf("\nTasks: ");
                                }
                                printf("%s", value);
                                value = strtok(NULL,";");
                                column ++;
                            }
                            printf("\n");
                        }
                        fclose(ptr);
                    }
            }
        }
        else if (choice_tasks == 3){
            printf("Mark completed\n");
            char date_check[50];
            char status[20];
            int compare;
            printf("Enter the date(dd/mm/yy): ");
            fflush(stdin);
            gets(date_check);
            FILE *ptr;
            ptr = fopen("tasks2.csv","a+");
            char line[1024];
            FILE *ptr2;
            ptr2 = fopen("tasks.csv","r");
            printf("If completed type (completed): ");
            gets(status);
            printf("%s\n",status);
            while(fgets(line, 1024, ptr2)){
                char* tmp =strdup(line);
                compare = strcmp(date_check,taskytask(tmp,2));
                if (compare == 0){
                    char* tmp = strdup(line);
                    char* tmp1 = strdup(line);
                    char* tmp2 = strdup(line);
                    fflush(stdin);
                    int comp;
                    comp = strcmp(status, "completed");
                    if (comp == 0){
                        fprintf(ptr,"%s;%s;%s\n",taskytask(tmp,1),taskytask(tmp1,2),taskytask(tmp2,3));
                    }
                }
                free(tmp);
            }
            fclose(ptr);
            fclose(ptr2);
        }
        else{
            main();
            break;
        }
    }
}

//Main menu
int main(){
    // system("pwd");
    system("cls"); 
    while (2>0){
        int choice;
        fflush(stdin);
        printf("\n\tMENU\n");
        printf("1. Calendar\n2. Notes\n3. Events\n4. Tasks\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);
        if (choice<=5 && choice >= 0){
            if (choice == 0){
                printf("\nEnter valid choice");
                printf("\nExiting..");
                break;
            }
            else if (choice == 1){
                calendar();
            }
            else if (choice == 2){
                notes();
            }
            else if (choice == 3){
                events();
            }
            else if (choice == 4){
                tasks();
            }
            else{
                printf("Exiting...");
                break;
            }
        }
        else{
            printf("\nEnter a valid choice\n");
            continue;
                }
            }
    return 0;
    }
