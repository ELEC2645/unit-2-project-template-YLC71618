#ifndef FUNCS_H
#define FUNCS_H

/*Struct Definition*/
#define MAX_SLOTS 10

typedef struct {
    char name[50];
    int credits;
    int slotCount; //Count number of lecture slots
    int weekday[MAX_SLOTS]; //Store data in corresponding day
    int startHour[MAX_SLOTS]; //Store lecture start time
    int endHour[MAX_SLOTS]; //Store lecture end time
    char timetable[200]; //Use for printing timetable in UI settings 
} Module; //Settings

typedef struct {
    char moduleName[50];
    char name[50];
    char type[30];
    int deadlineYear;
    int deadlineMonth;
    int deadlineDay;
    double studyHours;
} Task; //Add Task

typedef struct {
    int day;
    int startHour;
    int endHour;
} FreeSlot; //For saving data of calculating free time

typedef struct {
    int day;
    int startHour;
    int endHour;
    char taskName[50];
} AllocatedSlot;

typedef struct {
    int day;
    int startHour;
    int endHour;
    char label[100];
} DailyEvent;

typedef struct {
    double duration[7];
    int mood[7];
} Week; //Time&Record, Mood checker

/*Global Variables
  extern allows other source files to access*/

extern Module modules[]; //List of all modules
extern int moduleCount;  //Store current number of modules
extern Task tasks[];     //List of all tasks
extern int taskCount;    //Store current number of tasks
extern FreeSlot freetime[];
extern int freeCount; 
extern AllocatedSlot allocated[];
extern int allocatedCount;
extern DailyEvent dayEvents[];
extern int eventCount;
extern Week weekRecord;  //Week


/*Menu Functions*/
void settingsMenu(void);
void addTaskMenu(void);
void timetableMenu(void);
void timerMenu(void);
void moodMenu(void);

/*Settings Functions*/
void inputModules(void);
void printModules(void);
void loadModules(void);
void saveModules(void);
int convertDayToInt(const char *s);
void parseSlot(const char *slot, int *day, int *start, int *end);

/*Add Tasks Functions*/
void inputTasks(void);
void printTasks(void);
void loadTasks(void);
void saveTasks(void);

/*Timetable Functions*/
void calculateFreeTime(void);
void sortTasks(void);
void allocateTasks(void);
void printTimetable(void);
int compareDeadline(Task a, Task b);
int dateDiffInDays(int y1, int m1, int d1, 
                    int y2, int m2, int d2);
void buildEventList(void);
void sortEvents(void);
char* dayName(int day);

/*Timer & Record Functions*/
void startTimer(void);
void manualRecord(void);
void printStudyGraph(void);
void loadWeek(void);
void saveWeek(void);
int getToday(void);
double getWeeklyTotal(void);
double calculateEfficiency(void);
void printReward(double efficiency);


/*Mood Functions*/
void saveMood(void);
void printMoodGraph(void);


#endif