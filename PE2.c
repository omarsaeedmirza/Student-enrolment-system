

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 30
#define fieldLength 200

#define diskFile "diskFile.dat"
#define courseFile "course.txt"

struct db_type
{
  char name[fieldLength];
  int age;
  char course1[fieldLength];
  char course2[fieldLength];
  char status[fieldLength];
};

struct courseInfo
{
  char code[20]; // e.g., EECS2030
  char title[fieldLength];
  char date[20];
  char time_start[20];
  char time_end[20];
  char location[20];
};

struct courseInfo courseArr[SIZE]; // global variable  array of struc

char prompt_menu(void);
void init_list(struct db_type *pArr[]);
void clearDB(struct db_type *pArr[]);
void init_courseArr(void);
void writeDisk(struct db_type *pArr[]);
void emptyDisk(void);
void loadDisk(struct db_type *pArr[]);
void enterNew(struct db_type *pArr[SIZE]);
void displayDB(struct db_type *pArr[]);
void removeRecord(struct db_type *pArr[]);
void swap(struct db_type *pArr[]);
void sort(struct db_type *pArr[]);
void displayCourses(void);
char *timeConflict(struct db_type *pArr[], int count);
int helperStartTime(int index);
int helperEndTime(int index);

int actualSize;
int count=0;

int main(int argc, char *argv[])
{

  struct db_type *db_pArr[SIZE]; // main db storage

  init_list(db_pArr); // set to NULL

  init_courseArr(); // load course from diskfile

  char choice;
  for (;;)
  {
    choice = prompt_menu();
    switch (choice)
    {
    case 'n':
      enterNew(db_pArr);
      break;
    case 'd':
      displayDB(db_pArr);
      break;
    case 'w':
      writeDisk(db_pArr);
      break;
    case 'l':
      loadDisk(db_pArr);
      break;
    case 's':
      sort(db_pArr);
      break;

    case 'c':
      clearDB(db_pArr);
      break;
    case 'e':
      emptyDisk();
      break;

    case 'v':
      displayCourses();
      break;
    case 'p':
      swap(db_pArr);
      break;
    case 'r':
      removeRecord(db_pArr);
      break;

    case 'q':
      exit(1); // terminate the whole program
    }
  }
  return 0;
}

void init_list(struct db_type *pArr[])
{
  int t;
  for (t = 0; t < SIZE; t++)
  {
    pArr[t] = NULL;
  }
}

void clearDB(struct db_type *pArr[])
{
  char c3[3];
  printf("are you sure to clear db? (y) or (n)? ");

  fgets(c3, 3, stdin);

  if (!strcmp(c3, "y\n"))
    init_list(pArr);
  count = 0;
}

char prompt_menu(void)
{
  char s[80];
  while (1)
  {
    printf("\n-----------------------------------------------------------------\n");
    printf("|    %-20s", "(N)ew record");
    printf("%-20s", "(R)emove record");
    printf("Swa(p) records\t|\n");
    printf("|    %-20s", "(S)ort database");
    printf("%-20s", "(C)lear database");
    printf("(D)isplay db\t|\n");

    printf("|    %-20s", "(L)oad disk");
    printf("%-20s", "(W)rite disk");
    printf("(E)mpty disk\t|\n");

    printf("|    %-20s", "(V)iew courses"); //|\tSwa(p) record\t(Q)uit\t\t\t\t|\n");
    printf("%-20s", "(Q)uit");
    printf("*Case Insensitive*\t|\n");
    printf("-----------------------------------------------------------------\n");
    printf("choose one: ");

    fgets(s, 50, stdin); // \n added

    if (strlen(s) == 2 && strchr("edlsuqrcwnvpr", tolower(*s)))
      return tolower(*s); // s[0], return the first character of s
    //else
    printf("not a valid input!\n");
  }
}

/* display all or specified course */
void displayCourses(void)
{ // the provide PE2.out uses "%s\t%-40s%-5s %s-%s   %s\n" as formatting string for printing each course info
  char code[10];
  char all = 'a';
  printf("course code (or 'a')?: ");
  fgets(code, 10, stdin);
  code[strlen(code) - 1] = '\0';
  //scanf("%s", &code);
  if (code[0] == all)
  {
    int i;
    for (i = 0; i < actualSize; i++)
    {
      printf("%s\t%-40s%-5s %s-%s   %s\n", courseArr[i].code,
             courseArr[i].title, courseArr[i].date,
             courseArr[i].time_start, courseArr[i].time_end,
             courseArr[i].location);
    }
  }
  else
  {
    int check=0;
    int i;
    for (int i = 0; i < actualSize; i++)
    {
      int test = strcmp(code, courseArr[i].code);
      if (test == 0)
      {
        printf("%s\t%-40s%-5s %s-%s   %s\n", courseArr[i].code,
               courseArr[i].title, courseArr[i].date,
               courseArr[i].time_start, courseArr[i].time_end,
               courseArr[i].location);
        check = 1;
      }
    }
    for (int i = 0; i < actualSize; i++)
    {
      char *ptr = courseArr[i].code + 4;
      int test = strcmp(code, ptr);
      if (test == 0)
      {
        printf("%s\t%-40s%-5s %s-%s   %s\n", courseArr[i].code,
               courseArr[i].title, courseArr[i].date,
               courseArr[i].time_start, courseArr[i].time_end,
               courseArr[i].location);
        check = 1;
      }
    }
    if (check == 0)
    {
      printf("Course not found :(");
    }
  }
}

/* input items into the list */
void enterNew(struct db_type *pArr[SIZE])
{
  char choice;
  char temp[20];
  char temp2[20];

  struct db_type *ptr;

  printf("name: ");
  pArr[count] = (struct db_type *)malloc(sizeof(struct db_type));
  fgets(pArr[count]->name, fieldLength, stdin);

  printf("age: ");
  scanf("%d", &pArr[count]->age);
  char f = getchar();

  printf("course-1: ");

  fgets(temp, fieldLength, stdin);
  temp[strlen(temp) - 1] = '\0';

  int tester = 0;
  while (tester == 0)
  {
    int i;
    for (i = 0; i < actualSize; i++)
    {
      int test = strcmp(temp, courseArr[i].code);
      if (test == 0)
      {
        strcpy(pArr[count]->course1, temp);
        tester = 1;
        break;
      }
    }

    for (i = 0; i < actualSize; i++)
    {
      char *ptrr = courseArr[i].code + 4;
      int test = strcmp(temp, ptrr);
      if (test == 0)
      {
        strcpy(pArr[count]->course1, courseArr[i].code);
        tester = 1;
        break;
      }
    }

    if (tester == 0)
    {
      printf("course does not exist, enter again: ");
      fgets(temp, fieldLength, stdin);
      temp[strlen(temp) - 1] = '\0';
    }
  }

  printf("course-2: ");

  fgets(temp2, fieldLength, stdin);
  temp2[strlen(temp2) - 1] = '\0';

  int tester2 = 0;
  while (tester2 == 0)
  {
    int i;
    for (i = 0; i < actualSize; i++)
    {
      int test = strcmp(temp2, courseArr[i].code);
      if (test == 0)
      {
        strcpy(pArr[count]->course2, temp);
        tester2 = 1;
        break;
      }
    }

    for (i = 0; i < actualSize; i++)
    {
      char *ptrr = courseArr[i].code + 4;
      int test = strcmp(temp2, ptrr);
      if (test == 0)
      {
        strcpy(pArr[count]->course2, courseArr[i].code);
        tester2 = 1;
        break;
      }
    }

    if (tester2 == 0)
    {
      printf("course does not exist, enter again: ");
      fgets(temp2, fieldLength, stdin);
      temp2[strlen(temp2) - 1] = '\0';
    }
  }
  //printf("%s\n",pArr[count]->course1 );
  //printf("%s\n",pArr[count]->course2 );

  printf("%s", timeConflict(pArr, count));
  strcpy(pArr[count]->status, timeConflict(pArr, count));
  // printf("%s\n",pArr[count]->status );

  count++;
}

char *timeConflict(struct db_type *pArr[SIZE], int count)
{

  int i;
  int indexCr1;
  int indexCr2;
  for (i = 0; i < actualSize; i++)
  {
    char *ptr = pArr[count]->course1;
    int test = strcmp(ptr, courseArr[i].code);
    if (test == 0)
    {
      indexCr1 = i;
      break;
    }
  }
  for (i = 0; i < actualSize; i++)
  {
    int test = strcmp(pArr[count]->course2, courseArr[i].code);
    if (test == 0)
    {
      indexCr2 = i;
      break;
    }
  }

  size_t sizeC1 = strlen(courseArr[indexCr1].date);
  size_t sizeC2 = strlen(courseArr[indexCr2].date);
  int startTimeC1 = helperStartTime(indexCr1);
  int endTimeC1 = helperEndTime(indexCr1);
  int startTimeC2 = helperStartTime(indexCr2);
  int endTimeC2 = helperEndTime(indexCr2);

  int k;
  for (k = 0; k < sizeC1; k++)
  {
    for (int i = 0; i < sizeC1; i++)
    {
      if (courseArr[indexCr1].date[k] == courseArr[indexCr1].date[i])
      {
        if (startTimeC1 < startTimeC2)
        {
          if (startTimeC1 <= startTimeC2 && startTimeC2 < endTimeC1)
          {
            return "ATTENTION! time conflict";
          }
        }
        else
        {
          if (startTimeC2 <= startTimeC1 && startTimeC1 < endTimeC2)
          {
            return "ATTENTION! time conflict";
          }
        }
      }
    }
  }
  return "SUCCESSFUL! no time conflict";
}

int helperStartTime(int index)
{
  //printf("%d", index);
  int temp = courseArr[index].time_start[0] - '0';
  int x = temp * 1000;
  temp = courseArr[index].time_start[1] - '0';
  x += temp * 100;
  temp = courseArr[index].time_start[3] - '0';
  x += temp * 10;
  temp = courseArr[index].time_start[4] - '0';
  x += temp * 1;
  return x;
}

int helperEndTime(int index)
{
  //printf("%d", index);
  int temp = courseArr[index].time_end[0] - '0';
  int x = temp * 1000;
  temp = courseArr[index].time_end[1] - '0';
  x += temp * 100;
  temp = courseArr[index].time_end[3] - '0';
  x += temp * 10;
  temp = courseArr[index].time_end[4] - '0';
  x += temp * 1;
  return x;
}

/* display records */
void displayDB(struct db_type *pArr[])
{
  printf("===============================\n");
  for (int i = 0; i < count; i++)
  {
    printf(" \n");
    pArr[i]->name[strcspn(pArr[i]->name, "\n")] = 0;
    printf("name:    %s\n", pArr[i]->name);
    printf("age:     %d\n", pArr[i]->age);

    for (int j = 0; j < actualSize; j++)
    {
      //pArr[i]->course1[strlen(pArr[i]->course1)-1]='\0';
      char *ptr = pArr[i]->course1;
      ;

      int test = strcmp(ptr, courseArr[j].code);
      //printf("%d", test);
      if (test == 0)
      {
        printf("course1: %s\t%-40s%-5s %s-%s   %s\n", courseArr[j].code,
               courseArr[j].title, courseArr[j].date,
               courseArr[j].time_start, courseArr[j].time_end,
               courseArr[j].location);
      }
    }
    for (int k = 0; k < actualSize; k++)
    {
      char *ptr = pArr[i]->course2;
      int test = strcmp(ptr, courseArr[k].code);
      if (test == 0)
      {
        printf("course2: %s\t%-40s%-5s %s-%s   %s\n", courseArr[k].code,
               courseArr[k].title, courseArr[k].date,
               courseArr[k].time_start, courseArr[k].time_end,
               courseArr[k].location);
      }
    }

    printf("remarks: %s\n", pArr[i]->status);
  }
  printf("\n========== %d records ==========\n", count);
}

/* remove an existing item */
void removeRecord(struct db_type *pArr[])
{
  char name[20];
  printf("enter full name to remove: ");
  fgets(name, 20, stdin);
  name[strlen(name) - 1] = '\0';
  int tester = 0;
  int i;
  int index;

  for (i = 0; i < count; i++)
  {
    char *ptr = pArr[i]->name;
    int test = strcmp(ptr, name);
    if (test == 0)
    {
      index = i;
      tester = 1;
    }
  }
  if (tester == 0)
  {
    printf("record not found");
  }
  else
  {
    for (i = index; i < count - 1; i++)
    {
      pArr[i] = pArr[i + 1];
    }
    printf("record [%s] removed successfully.", name);
    count--;
  }
}

/* swap records */
void swap(struct db_type *pArr[])
{
  int i;
  struct db_type *temp;
  if (count % 2 == 0)
  {
    for (i = 0; i < count; i += 2)
    {
      temp = pArr[i];
      pArr[i] = pArr[i + 1];
      pArr[i + 1] = temp;
    }
  }
  else
  {
    for (i = 0; i < count; i += 2)
    {
      if (i == count - 1)
      {
        break;
      }

      temp = pArr[i];
      pArr[i] = pArr[i + 1];
      pArr[i + 1] = temp;
    }
  }
}

/* load from course.txt, store into (global) array of courses  */
void init_courseArr(void)
{

  FILE *fr; //file pointer
  fr = fopen(courseFile, "r");
  char arr[50];
  int count = 0;
  while ((fgets(arr, 100, fr)) != NULL)
  {
    strncpy(courseArr[count].code, arr, strlen(arr) - 1); // remove \n
    courseArr[count].code[strlen(arr) - 1] = '\0';        //to be safe

    fgets(arr, 100, fr);
    strncpy(courseArr[count].title, arr, strlen(arr) - 1);
    courseArr[count].title[strlen(arr) - 1] = '\0'; //to be safe

    fgets(arr, 100, fr);
    strncpy(courseArr[count].date, arr, strlen(arr) - 1);
    courseArr[count].date[strlen(arr) - 1] = '\0'; //to be safe

    // other ways, remove \n before
    fgets(arr, 100, fr);
    arr[strlen(arr) - 1] = '\0';
    strcpy(courseArr[count].time_start, arr);

    fgets(arr, 100, fr);
    arr[strlen(arr) - 1] = '\0';
    strcpy(courseArr[count].time_end, arr);

    fgets(arr, 100, fr);
    arr[strlen(arr) - 1] = '\0';
    strcpy(courseArr[count].location, arr);

    count++;

    fgets(arr, 100, fr); // read the empty line
  }

  strcpy(courseArr[count].code, "EECS0000"); // a terminator token structure

  fclose(fr);
  actualSize = count;
}

/************************ DISK IO *****************************************************************/

void writeDisk(struct db_type *pArr[])
{
  FILE *fp;
  int i;

  if ((fp = fopen(diskFile, "ab")) == NULL)
  {
    printf("cannot open file\n");
    return;
  }

  for (i = 0; i < SIZE; i++)
  {
    if (pArr[i] != NULL)
    {
      if (fwrite(pArr[i], sizeof(struct db_type), 1, fp) != 1)
      {
        printf("file write error\n");
      }
    }
  }
  fclose(fp);
}

void emptyDisk(void)
{

  FILE *fp;

  char c3[3];
  printf("are you sure to empty disk? (y) or (n)? ");
  fgets(c3, 3, stdin);
  if (strcmp(c3, "y\n"))
    return;

  //else
  if ((fp = fopen(diskFile, "w")) == NULL)
  {
    printf("no file to open\n");
    return;
  }
  fclose(fp); // open close, will empty the file
}

void loadDisk(struct db_type *pArr[])
{
  FILE *fp;
  int i;
  char c3[3];
  printf("will overwrite current records. are you sure to load disk? (y) or (n)? ");
  fgets(c3, 3, stdin);
  if (strcmp(c3, "y\n"))
    return;

  struct db_type *tmp;

  if ((fp = fopen(diskFile, "r")) == NULL)
  {
    printf("cannot open file\n");
    return;
  }

  init_list(pArr);
  for (i = 0; i < SIZE; i++)
  {

    tmp = (struct db_type *)malloc(sizeof(struct db_type));

    if (fread(tmp, sizeof(struct db_type), 1, fp) != 1)
    {
      if (feof(fp))
      {
        fclose(fp);
        return;
      }
      printf("file read error\n");
    }
    else
      pArr[i] = tmp;
  }
}

/****bonus*************************************************************************************/

/* sort the records by ages */
void sort(struct db_type *pArr[])
{
  int i, j, minAge;
  struct db_type *temp;
  for (i = 0; i < count - 1; i++)
  {
    minAge = pArr[i]->age;
    for (j = i + 1; j < count; j++)
    {
      if (pArr[j]->age < minAge)
      {
        minAge = pArr[j]->age;
        temp = pArr[i];
        pArr[i] = pArr[j];
        pArr[j] = temp;
      }
    }
  }
}
