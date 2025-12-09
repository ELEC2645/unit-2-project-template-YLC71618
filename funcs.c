#include <stdio.h>
#include "funcs.h"
#include <string.h>
#include <time.h> //for mktime()
#include <math.h> //for ceil (round up numbers)

/*Global Variables*/
Module modules[20];  //Set 20 modules storage
int moduleCount = 0; //Initialise module count
Task tasks[100];     //Set 100 modules storage
int taskCount = 0;   //Initialise task count
FreeSlot freeTime[100];
int freeCount = 0;
AllocatedSlot allocated[200];
int allocatedCount = 0;
DailyEvent dayEvents[200];
int eventCount;
Week weekRecord;     //Stores weekly record

/*Settings*/
void settingsMenu(void) {
    printf("\n========= Menu 1: Settings =========\n");
    
    //Show already entered modules
    if(moduleCount>0) {
        printf("You have already entered %d module(s):\n", moduleCount);
        printModules();
    }else{
        printf("You haven't entered any modules\n");
    }
    
    inputModules();
    printModules();
}

int convertDayToInt(const char *s) {
    const char *days[] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
    for(int i=0; i<7; i++){
        //Compare user input with day array and return 0-6
        if(strncmp(s, days[i], 3)==0) return i; 
    }
}

void parseSlot(const char *slot, int *day, int *start, int *end) {
    char dayStr[4]; //Store Mon, Tue, Wed, Thu, Fri, Sat, Sun
    char hourStr[15]; //Store start+end hour 
    strncpy(dayStr, slot, 3); //Copies the first 3 characters (Mon) from slot into dayStr[]
    dayStr[3]='\0'; //termination
    strcpy(hourStr, slot+3); //Copy from the fourth character of slot (11-15)
    *day = convertDayToInt(dayStr);
    //Use sscanf to extract the two numbers from array
    sscanf(hourStr, "%d-%d", start, end); //First %d saves in start, Second %d saves in end
    
}

void inputModules(void) {
    int no_module;//user input number of module
    int day, start, end;
    char buf[128];
    char slot[50];
    char choice[20];
    while(1) {
        printf("\nHow many modules would you like to enter? ");
        if(scanf("%d", &no_module)!=1) {
            printf("Invalid input! Please enter an integer\n");
            while(getchar() != '\n'); //clear all until reach '\n'
            continue;
        }
        if(no_module<=0 || no_module>20){
            printf("Please enter a number between 1-20.\n");
            continue;
        }
        while(getchar() != '\n'); //clear all until reach '\n'
        break;

    }

    for(int i=0; i<no_module; i++) {
        
        printf("Please enter module %d name: ", moduleCount+1); //User enter module name
        fgets(modules[moduleCount].name, sizeof(modules[moduleCount].name), stdin); //Store in module struct array
        //replace '\n' with '\0' to avoid unnecessary line change
        modules[moduleCount].name[strcspn(modules[moduleCount].name,"\n")] = '\0'; 

        while(1) {
            printf("Please enter module %d credits: ", moduleCount+1);
            if(scanf("%d", &modules[moduleCount].credits)!=1) {
                printf("Invalid input! Please enter an integer\n");
                while(getchar() != '\n'); //clear all until reach '\n'
                continue;
            }
            if(modules[moduleCount].credits<0){
                printf("Credits cannot be negative. Please enter again\n");
                continue;
            }
            if(modules[moduleCount].credits>120){
                printf("Too many credits! Please enter again\n");
                continue;
            }
            while(getchar() != '\n'); //clear all until reach '\n'
            break;

        }
        while(1) {

            printf("Enter timetable slot (format: Mon10-13): ");
            fgets(slot, sizeof(slot), stdin);
            slot[strcspn(slot,"\n")] = '\0';

            //check length
            if(strlen(slot) < 8) {
                printf("Invalid format! Please try again!\n");
                continue;
            }
            //check day
            //strncmp() compares the first 3 characters of two strings 
            //if no mismatch return 0
            if (strncmp(slot, "Mon", 3)!=0 &&
                strncmp(slot, "Tue", 3)!=0 &&
                strncmp(slot, "Wed", 3)!=0 &&
                strncmp(slot, "Thu", 3)!=0 &&
                strncmp(slot, "Fri", 3)!=0 &&
                strncmp(slot, "Sat", 3)!=0 &&
                strncmp(slot, "Sun", 3)!=0){
                
                printf("Invalid day. Please use Mon/Tue/Wed/Thu/Sat/Sun\n");
                continue;
            }
            //check HH-HH
            if(sscanf(slot+3, "%d-%d", &start, &end) != 2) {
                printf("Invalid time format. Please use Mon10-13: \n");
                continue;
            }        
            //check time
            if(start<0 || start>23 || end<0 || end>23 || start>=end) {
                printf("Invalid hour range. Example: Mon10-13: \n");
                continue;
            }                
            break;//out of loop

        }
        
        parseSlot(slot, &day, &start, &end);

        int n = modules[moduleCount].slotCount;
        modules[moduleCount].weekday[n] = day;
        modules[moduleCount].startHour[n] = start;
        modules[moduleCount].endHour[n] = end;
        modules[moduleCount].slotCount++;

        //copy slot into timetable for UI settings
        strcpy(modules[moduleCount].timetable, slot);

        // Next Slot
        while(1) {
            printf("Enter 'S' or 's' to stop, otherwise enter next time slot(format: Mon10-13): ");
            fgets(slot, sizeof(slot), stdin);
            slot[strcspn(slot,"\n")] = '\0';

            if(slot[0]=='S' || slot[0]=='s'){
                break;
            }
            // input validation
            while(1) {

                //check length
                if(strlen(slot) < 8) {
                    printf("Invalid format! Please try again!\n");
                    printf("Enter 'S' or 's' to stop, otherwise enter next time slot(format: Mon10-13): ");
                    fgets(slot, sizeof(slot), stdin);
                    slot[strcspn(slot,"\n")] = '\0';
                    continue;
                }
                //check day
                //strncmp() compares the first 3 characters of two strings 
                //if no mismatch return 0
                if (strncmp(slot, "Mon", 3)!=0 &&
                    strncmp(slot, "Tue", 3)!=0 &&
                    strncmp(slot, "Wed", 3)!=0 &&
                    strncmp(slot, "Thu", 3)!=0 &&
                    strncmp(slot, "Fri", 3)!=0 &&
                    strncmp(slot, "Sat", 3)!=0 &&
                    strncmp(slot, "Sun", 3)!=0){
                
                    printf("Invalid day. Please use Mon/Tue/Wed/Thu/Sat/Sun\n");
                    printf("Enter 'S' or 's' to stop, otherwise enter next time slot(format: Mon10-13): ");
                    fgets(slot, sizeof(slot), stdin);
                    slot[strcspn(slot,"\n")] = '\0';
                    continue;
                }
                //check HH-HH
                if(sscanf(slot+3, "%d-%d", &start, &end) != 2) {
                    printf("Invalid time format. Please use Mon10-13: \n");
                    printf("Enter 'S' or 's' to stop, otherwise enter next time slot(format: Mon10-13): ");
                    fgets(slot, sizeof(slot), stdin);
                    slot[strcspn(slot,"\n")] = '\0';
                    continue;
                }        
                //check time
                if(start<0 || start>23 || end<0 || end>23 || start>=end) {
                    printf("Invalid hour range. Example: Mon10-13: \n");
                    printf("Enter 'S' or 's' to stop, otherwise enter next time slot(format: Mon10-13): ");
                    fgets(slot, sizeof(slot), stdin);
                    slot[strcspn(slot,"\n")] = '\0';
                    continue;
                }                
                break;//out of loop

            }     
                  
            parseSlot(slot, &day, &start, &end);
            n = modules[moduleCount].slotCount;
            modules[moduleCount].weekday[n] = day;
            modules[moduleCount].startHour[n] = start;
            modules[moduleCount].endHour[n] = end;
            modules[moduleCount].slotCount++;

            //copy slot into timetable for UI settings
            //strcat() appends '|' to the end of timetable
            strcat(modules[moduleCount].timetable, "|");
            //strcat() appends slot to the end of timetable '|'
            strcat(modules[moduleCount].timetable, slot);


        }
        moduleCount++;
    }
}
void printModules(void){
    printf("\n========== Entered Details ============\n");
    
    for(int i=0; i<moduleCount; i++) {
        printf("\nModule %d: %s\n", i+1, modules[i].name);
        printf("Credits: %d\n", modules[i].credits);
        printf("Lecture/Lab Time: %s\n", modules[i].timetable);
    }
    printf("\n=======================================\n");
}

void loadModules(void);
void saveModules(void);

/*Add Task*/
void addTaskMenu(void) {
    printf("\n========= Menu 2: Add Tasks =========\n");
    
    if(moduleCount == 0) {
        printf("!!!You must enter Module Detail first in Menu 1");
    }else{
        //Show already entered modules
        if(taskCount>0) {
            printf("You have already entered %d task(s)", taskCount);
            printTasks();
        }else{
            printf("You haven't entered any task");
        }      
        inputTasks();
        printTasks();  
    }

}
void inputTasks(void){

    int selectModule;
    int selectType;
    printf("\n-------------- Modules --------------\n");
    for(int i=0; i<moduleCount; i++) {
        printf("%d. %s\n", i+1, modules[i].name);
    }
    
    //input validation- choose module
    while(1) {
        printf("Please select module (Integer): ");
        if(scanf("%d", &selectModule) != 1) {
            printf("Invalid input! Please enter integer only\n");
            while(getchar() != '\n'); //remove '\n'
            continue;
        }
        if(selectModule<1 || selectModule>moduleCount) {
            printf("Invalid module. Choose 1-%d.\n", moduleCount);
            continue;
        }
        while(getchar() != '\n'); //remove '\n'
        break;

    }

    //Store module name
    //copy module name into tasks module
    
    strcpy(tasks[taskCount].moduleName, modules[selectModule-1].name);

    //Task name validation
    while(1) {
        printf("Please enter task name: ");
        fgets(tasks[taskCount].name, sizeof(tasks[taskCount].name), stdin);

        //replace '\n' with '\0' to avoid change lines
        tasks[taskCount].name[strcspn(tasks[taskCount].name,"\n")] = '\0'; 
        //name cannot be none (blank)
        if(strlen(tasks[taskCount].name) == 0) {
            printf("Task name cannot be blank.\n");
            continue;
        }
        break;

    }
    
    
    //Task type
    const char *types[] = {"None", "Coursework", "Group Project", "Practical", "Exam", "Other"};

    while(1) {
        printf("\n=======Task Types=======\n");
        printf("1. Individual Coursework\n");
        printf("2. Group Project\n");
        printf("3. Practical\n");
        printf("4. Exam\n");
        printf("5. Other\n");

        printf("Please select task types (Integer): ");
        //task type selection validaion 

        if(scanf("%d", &selectType)!=1) {
            printf("Invalid input! Can only enter integer 1-5\n"); 
            while(getchar() != '\n'); //remove '\n'
            continue;
        }

        if(selectType<1 || selectType>5) {
            printf("Invalid selection! Can only enter 1-5\n"); 
            while(getchar() != '\n'); //remove '\n'
            continue;
        }

        if(selectType == 5) {
            getchar();
            while(1) { //to deal with custom type validation
                printf("Please enter custom task type: ");
                fgets(tasks[taskCount].type, sizeof(tasks[taskCount].type), stdin);
                tasks[taskCount].type[strcspn(tasks[taskCount].type,"\n")] = '\0'; //replace '\n' with '\0'

                if(strlen(tasks[taskCount].type) == 0) {
                    printf("Task type cannot be blank.\n");
                    continue;
                }   

                break; //jump out of the loop
            }
        }else{
            //Store task type
            //copy selectType into tasks type
            strcpy(tasks[taskCount].type, types[selectType]);
        
        }
        break; //jump out of the loop
    }
    
    //Task Deadline
    //validation for year
    while(1) {
        printf("Please enter deadline year (YYYY): ");
        if(scanf("%d", &tasks[taskCount].deadlineYear)!=1){
            printf("Invalid year (Please enter integer)\n");
            while(getchar() != '\n'); //remove '\n'
            continue; //back to the loop; re-enter year
        }
        getchar();
        //restrict year from 2025 to 2026
        if(tasks[taskCount].deadlineYear<2025 || tasks[taskCount].deadlineYear>2026) {
            printf("Year must be between 2025 or 2026.\n");
            continue;
        }
        break;
        
    }
    //validation for month
    while(1) {
        printf("Please enter deadline month (1-12): ");
        if(scanf("%d", &tasks[taskCount].deadlineMonth)!=1){
            printf("Invalid month (Please enter integer)\n");
            while(getchar() != '\n'); //remove '\n'
            continue; //back to the loop; re-enter year
        }
        getchar();
        //restrict month (January to December)
        if(tasks[taskCount].deadlineMonth<1 || tasks[taskCount].deadlineMonth>12) {
            printf("Month must be 1-12.\n");
            continue;
        }
        break;
    }

    //validation for month
    while(1) {
        printf("Please enter deadline day (1-31): ");
        if(scanf("%d", &tasks[taskCount].deadlineDay)!=1){
            printf("Invalid day (Please enter integer)\n");
            while(getchar() != '\n'); //remove '\n'
            continue; //back to the loop; re-enter year
        }
        getchar();

        int maxDay;
        //february 
        if(tasks[taskCount].deadlineMonth==2){
            maxDay = 28;
        }else if(tasks[taskCount].deadlineMonth==4 || tasks[taskCount].deadlineMonth==6 || tasks[taskCount].deadlineMonth==9 || tasks[taskCount].deadlineMonth==11){
            maxDay = 30;
        }else{
            maxDay = 31;
        }
        //restrict day
        if(tasks[taskCount].deadlineDay<1 || tasks[taskCount].deadlineDay>maxDay) {
            printf("Invalid Day\n");
            continue;
        }
        break;
    }


    //Task study hours validation
    while(1) {
        printf("Please enter your estimated study hours: ");
        
        //use lf because studyHours is type double
        if(scanf("%lf", &tasks[taskCount].studyHours)!=1) {
            printf("Invalid input/ Enter a number\n");
            while(getchar() != '\n'); //remove '\n'
            continue;
        }
        getchar();
        if(tasks[taskCount].studyHours<=0 || tasks[taskCount].studyHours>=200) {
            printf("Estimated study hours must be more than 0 and less than 200.\n");
            continue;
        }
        break;
    }


    taskCount++;

}
void printTasks(void) {
    printf("\n========== Entered Details ============\n");
    
    for(int i=0; i<taskCount; i++) {
        printf("\nTask %d\n", i+1);
        printf("Task Name: %s\n", tasks[i].name);
        printf("Module: %s\n", tasks[i].moduleName);
        printf("Type: %s\n", tasks[i].type);
        printf("Deadline: %02d-%02d-%04d\n", 
                tasks[i].deadlineDay, 
                tasks[i].deadlineMonth, 
                tasks[i].deadlineYear);
        printf("Study Hours: %.1f\n", tasks[i].studyHours);
    }
    printf("\n=======================================\n");
}
void loadTasks(void);
void saveTasks(void);

/*Timetable*/
void timetableMenu(void) {
    printf("\n========= Menu 3: Timetable =========\n");
    calculateFreeTime();
    sortTasks();
    allocateTasks();
    printTimetable();
    
}
void calculateFreeTime(void) {
    //Whole day is from 8am to 22pm
    
    freeCount = 0; //reset free slots each time

    //loop from monday to sunday 0-6
    for(int i=0; i<7; i++) {
        //find busy time
        int busyStart[20];
        int busyEnd[20];
        int busyCount = 0; //how many busy slots today

        for(int k=0; k<moduleCount; k++) {
            for(int j=0; j<modules[k].slotCount; j++) {
                //if week day = today, then add into busy array
                if (modules[k].weekday[j] == i){

                    busyStart[busyCount]=modules[k].startHour[j];
                    busyEnd[busyCount]=modules[k].endHour[j];
                    busyCount++;
                }
            }
        }
        //if no busy slots, this mean the whole day is free
        if(busyCount == 0) {
            freeTime[freeCount].day = i;
            freeTime[freeCount].startHour = 8; //from 8am
            freeTime[freeCount].endHour = 22; //to 10pm
            freeCount++;
            continue;

        }
        
        //sort the busy time from early to late of a day 
        //Below is Selection sort (sort busyStart[] and busyEnd[])
        for(int k=0; k<busyCount-1; k++) {
            // Iterate through the unsorted portion
            for(int j=k+1; j<busyCount; j++) {
                if(busyStart[j] < busyStart[k]) {
                   //compare start
                    int temp1 = busyStart[k];
                    busyStart[k] = busyStart[j];
                    busyStart[j] = temp1;

                    //compare end
                    int temp2 = busyEnd[k];
                    busyEnd[k] = busyEnd[j];
                    busyEnd[j] = temp2; 
                }
                
            }
        }

        //Cut free slots from 8am to 22pm
        int now=8;
        for(int k=0; k<busyCount; k++) {
            if(busyStart[k]>now) {
                //find free slot
                freeTime[freeCount].day = i;
                freeTime[freeCount].startHour = now; 
                freeTime[freeCount].endHour = busyStart[k]; //to busyStart[k]
                freeCount++;
            }
            now = busyEnd[k]; //set new time
        }
        if(now<22) {
            //find the free time between busyEnd[busyCount-1] to 22pm
            freeTime[freeCount].day = i;
            freeTime[freeCount].startHour = now; 
            freeTime[freeCount].endHour = 22; 
            freeCount++;
        }

    }

}

int compareDeadline(Task a, Task b) {
    if(a.deadlineYear != b.deadlineYear) {
        //if task a - task b > 0, this means task b is earlier, therefore need swap
        //if task a - task b < 0, this means task a is earlier, no swap required
        //if task a - task b = 0, this means both same day
        return a.deadlineYear-b.deadlineYear;

    }else if(a.deadlineMonth != b.deadlineMonth) {
        
        return a.deadlineMonth-b.deadlineMonth;

    }else {
        
        return a.deadlineDay-b.deadlineDay;

    }
}

void sortTasks(void) {
    //Compare deadline 
    //If deadline is same then compare study hours
    //Use Selection sort to arrange the arrays
    for(int i=0; i<taskCount-1; i++) {
        for(int j=i+1; j<taskCount; j++) {
            
            int swap = 0; //see if swap is needed
            if(compareDeadline(tasks[i], tasks[j]) > 0) {
                //task j is earlier, need to swap i and j positions
                Task temp = tasks[i];
                tasks[i] = tasks[j];
                tasks[j] = temp;
            }else if(compareDeadline(tasks[i], tasks[j]) == 0) {
                //compare study hours since deadline are the same for both tasks
                if(tasks[i].studyHours < tasks[j].studyHours) {
                    Task temp = tasks[i];
                    tasks[i] = tasks[j];
                    tasks[j] = temp;
                }
            }
        }
    }
}

int dateDiffInDays(int y1, int m1, int d1, int y2, int m2, int d2) {
    //returns the days between date1 and date2
    struct tm a={0};
    struct tm b={0};

    a.tm_year = y1-1900; 
    a.tm_mon = m1 - 1; // January is 0, This only has 0-11
    a.tm_mday = d1;
    b.tm_year = y2-1900; 
    b.tm_mon = m2 - 1; // January is 0, This only has 0-11
    b.tm_mday = d2;

    //convert to time_t value
    //this is in seconds
    time_t time1=mktime(&a);
    time_t time2=mktime(&b);

    //60*60*24=86400 (total seconds in a day)
    return (int)((time2-time1)/86400);

}

void allocateTasks(void){

    allocatedCount = 0;
    if(freeCount==0){
        printf("No available free time this week\n");
        printf("Tasks cannot be allocated\n");
        return;

    }

    //get todays system time
    time_t now = time(NULL);  //Get current time
    struct tm *t = localtime(&now);  //Convert to local time structure

    //get separate year month day
    int todayDay = t->tm_mday;
    int todayMonth = t->tm_mon + 1; //Add 1 to get month 1-12
    int todayYear = t->tm_year + 1900; //Add 1900 to get the actual year

    //set maximum study hour of a task per day
    int maxDaily = 2;  //2 hours maximum

    //allocate each task
    for(int i=0; i<taskCount; i++) {

        Task *task = &tasks[i]; //pointer points to tasks[i]

        //Calculate how many days left until deadline
        int daysLeft = dateDiffInDays(todayYear, todayMonth, todayDay, 
                        task->deadlineYear, task->deadlineMonth, task->deadlineDay);

        if(daysLeft <= 0) continue;  //deadline already passed so skip

        //Calculate how many weeks left until deadline
        //1–7 days=1 week, 
        //8–14 days=2 weeks ...
        int weeksLeft = (daysLeft + 6) / 7;

        //divide studyHours over weeksLeft
        int weeklyHours = (int)ceil(task->studyHours / (double)weeksLeft);

        //weekly maximum capacity
        int weeklyCap=maxDaily * 7;

        //limit weeklyHours to weeklyCap
        if (weeklyHours > weeklyCap)
            weeklyHours=weeklyCap;

        //Emergency case:
        int maxPossible = weeksLeft * weeklyCap; //Total time possible before deadline
        int shortage = task->studyHours - maxPossible;

        if (shortage > 0) {
            //Add the shortage
            weeklyHours += shortage;

            //Doesn't exceed total required hours
            if (weeklyHours > task->studyHours) weeklyHours = task->studyHours;
        }

        //If weeklyHours becomes 0 then skip
        if (weeklyHours <= 0) continue;

        int remaining = weeklyHours; //this week's hours that still need to be allocated

        for(int k=0; k<freeCount && remaining>0; k++) {

            //get free time 
            int slotStart = freeTime[k].startHour;
            int slotEnd   = freeTime[k].endHour;
            int slotTime  = slotEnd - slotStart;

            //If there is no more free time left, jump to next iteration
            if(slotTime <= 0) continue;

            int put = slotTime; //max possible in this slot

            if (put > maxDaily)
                put = maxDaily;

            //Emergency: If total time is insufficient, can fill up the full slot
            if (shortage > 0) {
                put = slotTime;//use entire slot
            }

            //Don't exceed remaining hours needed
            if (put > remaining)
                put = remaining;

            // Save allocation to allocated[]
            allocated[allocatedCount].day       = freeTime[k].day;
            allocated[allocatedCount].startHour = slotStart;
            allocated[allocatedCount].endHour   = slotStart + put;
            strcpy(allocated[allocatedCount].taskName, task->name);
            allocatedCount++;

            //update free time slot
            freeTime[k].startHour += put;

            //update remaining hours for this task
            remaining -= put;
        }
    }

}

void buildEventList(void){
    eventCount = 0; //initialise every time
    //combine task slots and lecture slots

    for(int i=0; i<moduleCount; i++) {
        for(int k=0; k<modules[i].slotCount; k++) {
            //loop throught every module and lecture slots

            dayEvents[eventCount].day = modules[i].weekday[k];
            dayEvents[eventCount].startHour = modules[i].startHour[k];
            dayEvents[eventCount].endHour = modules[i].endHour[k];
            //prints "Lecture: module name" -> into day events label
            sprintf(dayEvents[eventCount].label, "Lecture: %s", modules[i].name);
            eventCount++;

        }
    }

    //add task slots from allocated array to day events struct
    for(int i=0; i<allocatedCount; i++){
        dayEvents[eventCount].day = allocated[i].day;
        dayEvents[eventCount].startHour = allocated[i].startHour;
        dayEvents[eventCount].endHour = allocated[i].endHour;
        //prints "Tasks: task name" -> into day events label
        sprintf(dayEvents[eventCount].label, "Tasks: %s", allocated[i].taskName);
        eventCount++;
    }
}
void sortEvents(void) {
    //use selection sort to sort event from early to late (am-pm)
    for(int i=0; i<eventCount-1; i++) {
        for(int j=i+1; j<eventCount; j++) {
            
            //compare two events day
            if(dayEvents[j].day < dayEvents[i].day ||
            (dayEvents[j].day == dayEvents[i].day &&
             dayEvents[j].startHour < dayEvents[i].startHour)) {
                //task j is earlier, need to swap i and j positions
                DailyEvent temp = dayEvents[i];
                dayEvents[i] = dayEvents[j];
                dayEvents[j] = temp;
            }
        }
    }    
}
char* dayName(int day) {
    static char *dayNames[]={
        "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};

    return dayNames[day];
}

void printTimetable(void) {
    buildEventList();//combine lecture and tasks 
    sortEvents();//sort by day + hour
    printf("\n--------- Weekly Timetable ---------\n");
    
    int currentDay=-1;
    for(int i=0; i<eventCount; i++){
        //change to current day to next day
        if(dayEvents[i].day!=currentDay) {
            currentDay = dayEvents[i].day;
            printf("\n%s:\n", dayName(dayEvents[i].day));
        }
        printf("\t%02d:00 - %02d:00\t%s\n",
                dayEvents[i].startHour,
                dayEvents[i].endHour,
                dayEvents[i].label);
        
    }
    printf("\n------------------------------------\n");

}
/*Timer & Record Menu*/
void timerMenu(void) {
    int choice; //store user input
    while(1) {
        printf("\n======== Menu 4: Timer & Record ========\n");
        printf("1. Start Study Timer\n");
        printf("2. Add study hours manually\n");
        printf("3. View weekly study graph\n");
        printf("4. Back\n");

        printf("Select: ");
        scanf("%d", &choice); //save selection in choice
        getchar(); //remove '\n'

        switch(choice) {
            case 1:
                startTimer();
                break;
            case 2:
                manualRecord();
                break;
            case 3:
                printStudyGraph();
                break;
            case 4:
                return;
            
            default:
                printf("Invalid Option. Please only enter 1-4!\n");

        }
    }
    
}
int getToday(void) {
    //Get weekday and time
    time_t now = time(NULL); //Get current time
    struct tm *t = localtime(&now);  //Convert to local time structure

    int today = t->tm_wday; //tm_wday: Sunday=0, Monday=1, so need to ajust
    
    if(today == 0) {
        today = 6; // change sunday=0 to sunday=6
    }else {
        today -= 1; //monday=0, tue=1, wed=2, thu=3, fri=4 sat=5
    }
    return today;

}
void startTimer(void) {
    printf("\n------------ Study Timer ------------\n");
    printf("Press ENTER to start the timer...");
    //wait for enter
    getchar();

    time_t start = time(NULL);
    printf("Status: STUDYING.....\n");
    printf("Press ENTER to stop");
    //wait for enter
    getchar();

    time_t end = time(NULL);
    
    //difftime() Calculates the difference (in seconds) between two time_t values
    double seconds = difftime(end, start);
    double hours = seconds/3600.0;
    if(hours<0.01) hours=0.01; //minimum hours

    int today = getToday(); //gets today
    weekRecord.duration[today] += hours;
    double eff=calculateEfficiency();
    printf("\nStatus: Timer Stopped.....\n");
    printf("\n========== Learning Performance ============\n");
    printf("Study duration: %.2f hours \n", hours);
    printf("Today's total: %.2f hours\n", weekRecord.duration[today]);
    printf("Week's total: %.2f hours\n", getWeeklyTotal());
    printf("This Week's Efficiency: %.2f%%\n", eff);
    printReward(eff);

}
void manualRecord(void) {
    //manually record study time
    double manual;

    printf("\n------------ Manual Record ------------\n");
    
    while(1) {
        printf("\nEnter studied hours: ");
        if(scanf("%lf", &manual) != 1 || manual <= 0) {
            printf("Invalid input! Please enter hours (number > 0): ");
            while (getchar() != '\n');
            continue;
        }
        if(manual >= 24) {
            printf("Impossible to study over 24 hours a day! Please re-enter");
            continue;
        }
        break;
    }

    int today = getToday();
    weekRecord.duration[today] += manual; //add input into array
    double eff=calculateEfficiency(); //calls calculate efficiency function
    printf("Added %.2f hours \n", manual);
    printf("Today's total: %.2f hours\n", weekRecord.duration[today]);
    printf("Week's total: %.2f hours\n", getWeeklyTotal());
    printf("This Week's Efficiency: %.2f%%\n", eff);
    printReward(eff);

}
void printStudyGraph(void) {

    printf("\n----------- Weekly Study Graph -----------\n\n");
    printf("Hours\n\n");
    
    //example: use for testing
    weekRecord.duration[0] = 2.0; //Mon
    weekRecord.duration[1] = 1.5; //Tue
    weekRecord.duration[3] = 2.0; //Thu
    weekRecord.duration[5] = 3.0; //Sat
    weekRecord.duration[6] = 1.0; //Sun

    //loop from monday to sunday
    for(int i=0; i<7; i++) {
        printf("%-10s | ", dayName(i));

        double h = weekRecord.duration[i];
        if(h<0.01) {
            printf("\n");
            continue;
        }

        //1 bar = 1 hour
        int bars=(int)(h+0.5); //round to the nearest integer
        
        for(int k=0; k<bars; k++) {
            printf("#");
        }
        printf(" (%.2f hr)\n", h);
    }
    double eff=calculateEfficiency();
    printf("This Week's Efficiency: %.2f%%\n", eff);
    printReward(eff);
    printf("\n--------------------------------------\n");
    
}
double getWeeklyTotal(void) {
    //calculate total weekly study hours
    double sum = 0;
    for(int i=0; i<7; i++) {
        sum += weekRecord.duration[i];
    }
    return sum;
}

double calculateEfficiency(void) {
    double total=getWeeklyTotal();
    double target=0;

    for(int i=0; i<taskCount; i++) {
        double maxWeek=14; //maximum task hour for a week = 2*7
        if(tasks[i].studyHours < maxWeek) {
            //if required study hours is less than 14, set target to study hours
            target += tasks[i].studyHours; 
        }else{
            target += maxWeek; //if required study hours is more than 14, set target to 14
        }
    }
    if(target<=0) return 0.0; //no task hour
    
    //calculate efficiency
    //efficiency = total hours studied this week/required studied hours
    double efficiency = (total/target)*100.0; 
    //if exceed the required study hours, then set efficiency to 100
    if(efficiency>100) efficiency=100.0; 

    return efficiency;
}

void printReward(double efficiency) {
    printf("\nYour Reward:\n");
    
    if(efficiency >= 90) {
        printf("Well Done! You received a Gold Medal!🏅\n");
    }else if(efficiency >=80) {
        printf("🥈Silver Medal! Great effort!\n");
    }else if(efficiency >=70) {
        printf("🥉Bronze Medal! Keep improving!\n");
    }else if(efficiency >=50) {
        printf("Keep working hard! You're have way through!\n");
    }else if(efficiency >=30) {
        printf("No medal yet~ Please work harder!\n");
    }else {
        printf("!!!!STUDY!!!!\n");
    }
}


/*Mood Menu*/
void moodMenu(void) {
    while(1) {
        printf("\n======== Menu 5: Mood Checker ========\n");
        printf("1. Save today's mood\n");
        printf("2. View weekly mood graph\n");
        printf("3. Back\n");

        int choice;
        while(1) {
            printf("Select (1-3): ");
            if(scanf("%d", &choice)!=1){
                printf("Invalid Input! Please enter (1-3) integer\n");
                while(getchar() != '\n');
                continue;
            }
            getchar();
            
            if(choice < 1 || choice > 3) {
                printf("Invalid option! Please enter 1-3.\n");
                continue;
            }  
            break;          
        }

        if(choice==1) {
            saveMood();
            printMoodGraph();
        }else if(choice==2) {
            printMoodGraph();
        }else {
            return;
        }
    }
    
}

void saveMood(void) {
    
    //Get weekday and time
    int today = getToday();

    int mood;
    printf("How do you feel today? (1=Bad, 5=Great): ");
    
    while(scanf("%d", &mood) != 1 || mood < 1 || mood > 5) {
        printf("Invalid Input. Please enter integer 1-5: ");
        //ask for re-enter
        while (getchar() != '\n');
    }

    weekRecord.mood[today] = mood; //save mood in array
    printf("Mood saved for %s: %d/5\n", dayName(today), mood); //print to the terminal

}
void printMoodGraph(void) {
    printf("\n--------- Weekly Mood Graph ---------\n");
    
    //loop from monday to sunday
    for(int i=0; i<7; i++) {
        printf("%-10s | ", dayName(i));

        if(weekRecord.mood[i]==0) {
            printf("No day yet\n");
            continue;
        }

        //print stars
        for(int k=0; k<weekRecord.mood[i]; k++) {
            printf("*");
        }
        printf(" (%d)\n", weekRecord.mood[i]);

    }
    printf("\n--------------------------------------\n");

}

/*Files Save and Load*/
void saveData(void)
{
    //open file
    FILE *fp = fopen("data.bin", "wb"); //wb -> binary mode 
    if (!fp) {
        printf("Error saving data!\n"); //file validation
        return;
    }

    //Save moduleCount
    //The fwrite() writes data from a block of memory into a file.
    fwrite(&moduleCount, sizeof(int), 1, fp);
    // Save module array
    fwrite(modules, sizeof(Module), moduleCount, fp);
    // Save taskCount
    fwrite(&taskCount, sizeof(int), 1, fp);
    // Save task array
    fwrite(tasks, sizeof(Task), taskCount, fp);
    // Save weekRecord (study hours + mood)
    fwrite(&weekRecord, sizeof(Week), 1, fp);

    fclose(fp); //close file
    printf("Data saved successfully!\n");
}

void loadData(void)
{
    FILE *fp = fopen("data.bin", "rb"); //open file
    if (!fp) {
        // File not found → first run → start with empty data
        printf("No saved data before. Start fresh.\n");
        return;
    }
    
    //fread() reads data from a file and writes into a block of memory.
    //Read moduleCount
    fread(&moduleCount, sizeof(int), 1, fp);//destination, size, amount, file
    //Read modules struct array
    fread(modules, sizeof(Module), moduleCount, fp);
    // Read taskCount
    fread(&taskCount, sizeof(int), 1, fp);
    // Read tasks struct array
    fread(tasks, sizeof(Task), taskCount, fp);
    // Read whole week's study & mood record
    fread(&weekRecord, sizeof(Week), 1, fp);

    fclose(fp);//close file
    printf("Data loaded successfully!\n");
}

/*Menu 6: Reset Data*/
void resetAllData(void) {
    char confirm;
    printf("\n======== Menu 6: Reset All Data ========\n");
    while(1){
        printf("Are you sure you want to reset everything? (y/n): ");
        if(scanf(" %c", &confirm)!=1){
            printf("Invalid Input! Please enter (y/n)\n");
            while(getchar() != '\n');
            continue;
        }
        if(confirm=='y' || confirm=='Y') {
            break;  //reset
        }
        else if(confirm =='n' || confirm =='N') {
            printf("\nCANCEL RESET: No data was deleted.\n\n");
            return;//jump out of function
        }
        else{
            printf("Invalid input! Please enter only 'y' or 'n'.\n\n");
        }
    }
    
    

    while (getchar() != '\n');  //clean '\n'

    //Reset Counters
    moduleCount=0;
    taskCount=0;
    freeCount=0;
    allocatedCount=0;
    eventCount=0;

    //Clear arrays
    //use memsets to set all of the bytes in a block of memory to zero
    memset(modules, 0, sizeof(modules));
    memset(tasks, 0, sizeof(tasks));
    memset(freeTime, 0, sizeof(freeTime));
    memset(allocated, 0, sizeof(allocated));
    memset(dayEvents, 0, sizeof(dayEvents));
    memset(&weekRecord, 0, sizeof(weekRecord));

    //Remove saved files
    remove("modules.dat");
    remove("tasks.dat");
    remove("week.dat");
    printf("   All DATA has been RESET!   \n");

}

