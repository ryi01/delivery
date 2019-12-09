#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

/* 
  definition of storage cell structure ----
  members :
  int building : building number of the destination
  int room : room number of the destination
  int cnt : number of packages in the cell
  char passwd[] : password setting (4 characters)
  char *context : package context (message string)
*/
typedef struct {
	int building;
	int room;
	int cnt;
	char passwd[PASSWD_LEN+1];
	
	char *context;
} storage_t;

static storage_t** deliverySystem; 			//deliverySystem
static int storedCnt = 0;					//number of cells occupied
static int systemSize[2] = {0, 0};  		//row/column of the delivery system
static char masterPassword[PASSWD_LEN+1];	//master password



// ------- inner functions ---------------

//print the inside context of a specific cell
//int x, int y : cell to print the context
static void printStorageInside(int x, int y) {
	printf("\n------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n");
	if (deliverySystem[x][y].cnt > 0)
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
	else
		printf("<<<<<<<<<<<<<<<<<<<<<<<< empty >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		
	printf("------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n\n");
}

//initialize the storage
//set all the member variable as an initial value
//and allocate memory to the context pointer
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {
	
	storage_t p;
	int i;
	
	int** deliverySystem = (int**)malloc(sizeof(int*)*6);//column's initialize
	
	for(x=0; x<6; x++)//row's initialize
	{
		*(deliverySystem + x) = (int*) malloc(sizeof(int*)*4);
	}
	
	for(y = 0; y < 6; y ++)//initialize the storage
	{
		for(x=0; x<4; x++)
		{
			*(*(deliverySystem + y) + x)=4 *y + x;
		}
	}
	
	p.context = (char*)malloc(sizeof(char*)*15);
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {

	int passwd;
	
	if(deliverySystem[x][y].passwd==0)//input passwd == save passwd
	{
		return 0;
	}
	else//input passwd != save passwd
	{
		return -1;
	}
	
}





// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {
	
	int i;
	int x, y;
	int line =0;
	
	storage_t p;
	
	FILE *fp= fopen("storage.txt", "a");
	
	if(filepath == NULL)
	{
		return -1;
	}
	else
	{
		return 0;
	}
	
	fscanf(fp, "%d %d", systemSize[0], systemSize[1]);//N and M
	fscanf(fp, "%d", masterPassword[PASSWD_LEN+1]); //masterpassword
	
	while(fscanf(fp, "%d %d %d %d %d %s", x, y, p.building, p.room, p.passwd[i], p.context)!=EOF);
	{
		fput("%d %d %d %d %d %s", x, y, p.building, p.room, p.passwd[i], p.context);
		
		if(p.context == '\0')
		{
			line++;//next line
		}	
	}
	
	fclose(fp);
	
	return 0;
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
	
	int i;
	int x, y;
	int line =0;
	
	FILE *fp;
	fp = fopen("storage.txt", "r");
	
	if(fp == NULL)
	{
		return -1;
	}
	
	fscanf(fp, "%d %d", systemSize[0], systemSize[1]);//N and M
	fscanf(fp, "%d", masterPassword[PASSWD_LEN+1]); //masterpassword
	
	for(i=0; i<15; i++)
	{
		fscanf(fp, "%d %d %d %d %d %s", x, y, deliverySystem[x][y].building, deliverySystem[x][y].room, deliverySystem[x][y].passwd[i], deliverySystem[x][y].context);
		
		if(deliverySystem[x][y].context == '\0')
		{
			line++;//next line
			storedCnt++;
		}
		
		deliverySystem[x][y].cnt = storedCnt;
	}
	
	fclose(fp);
	
	return 0;
}
	
//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
	int y, i;
	storage_t p;
	
	for(y=0; y<6; y++)
	{
		free(*(deliverySystem+y));
	}
	
	for(i=0; i<15; i++)
	{
		free((p.context+i));
	}
}



//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1];j++)
	{
		printf(" %i\t\t",j);
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<systemSize[0];i++)
	{
		printf("%i|\t",i);
		for (j=0;j<systemSize[1];j++)
		{
			if (deliverySystem[i][j].cnt > 0)
			{
				printf("%i,%i\t|\t", deliverySystem[i][j].building, deliverySystem[i][j].room);
			}
			else
			{
				printf(" -  \t|\t");
			}
		}
		printf("\n");
	}
	printf("--------------------------------------- Delivery Storage System Status --------------------------------------------\n\n");
}



//check if the input cell (x,y) is valid and whether it is occupied or not
int str_checkStorage(int x, int y) {
	if (x < 0 || x >= systemSize[0])
	{
		return -1;
	}
	
	if (y < 0 || y >= systemSize[1])
	{
		return -1;
	}
	
	return deliverySystem[x][y].cnt;	
}


//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	
	int i;
	char *filepath;
	int line=0;
	
	str_createSystem(filepath);
	
	FILE *fp;
	fp = fopen("storage.txt", "a");
	
	i= str_createSystem(filepath);
	
	if(i==0)
	{
		
		fscanf(fp, "%d %d", systemSize[0], systemSize[1]);//N and M
		fscanf(fp, "%d", masterPassword[PASSWD_LEN+1]); //masterpassword
	
		while(fscanf(fp, "%d %d %d %d %d %s", x, y, deliverySystem[x][y].building, deliverySystem[x][y].room, deliverySystem[x][y].passwd[i], deliverySystem[x][y].context)!=EOF);
		{
			fputs("%d %d %d %d %d %s", fp);
		
			if(deliverySystem[x][y].context == '\0')
			{
				line++;//next line
			}	
		}
	
		fclose(fp);
		
		return 0;
		}
	else
	{
		return -1;
	}
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	
	int i;
	char *filepath;
	
	str_createSystem(filepath);
	
	inputPasswd(x, y);
	
	i = inputPasswd(x, y);
	
	if(i==0)
	{
		printStorageInside(x, y);
	}
	else
	{
		return -1;
	}
	
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	
	int x, y;
	int cnt=0;
	char *filepath;
	
	str_createSystem(filepath);
	
	if(nBuilding==deliverySystem[x][y].building&&nRoom==deliverySystem[x][y].room)
	{
		printf(" -----------> found a package in (%d, %d)....\n", x, y);
	}
	else
	{
		return 0;
	}
	
	cnt = deliverySystem[x][y].cnt;
	
	return cnt;
}
