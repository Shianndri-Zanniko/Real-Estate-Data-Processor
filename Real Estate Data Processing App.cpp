#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct data
{
	char location[100];
	char city[100];
	long long price;
	long long rooms;
	long long bathrooms;
	long long carparks;
	char type[100];
	char furnish[100];
};
char l[100];
int gottendata = 0;
long long amount = 0;


data list[10000];

char ignoretitle[] = "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]%*c";

char filescanfFormat[] = "%[^,],%[^,],%lld,%lld,%lld,%lld,%[^,],%[^\n]%*c";

char printingFormat[] = "%-28s %-15s %-15lld %-15lld %-15lld %-15lld %-15s %-15s\n";
char titleprintingFormat[] = "%-28s %-15s %-15s %-15s %-15s %-15s %-15s %-15s\n";

void display(int n);
void getdata(FILE* fp);
int collum_picker(char collum[]);
void search(int col, char find[], long long findint);
void PrintSearchFound(int title_flag, int i);
int orderpicker(char sortby[]);
void getdata(FILE* fp);
int collum_picker(char collum[]);
void search(int col, char find[]);
void PrintSearchFound(int title_flag, int i);
int orderpicker(char sortby[]);
void swap(data* xp, data* yp);
void bubbleSort(data arr[], int n, int col, int order);
void exportdata(const char* filename, const char* printFname);

int main(void)
{
    int choice;
    system("cls");
    //menu
	do
    {
    	choice = 0;
        printf("What do you want to do?\n");
        printf("1. Display Data\n");
        printf("2. Search Data\n");
        printf("3. Sort Data\n");
        printf("4. Export Data?\n");
        printf("5. Exit\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        fflush(stdin);
        if(choice < 1 || choice > 5)
		{
			printf("wrong input, please enter a number between 1 to 5\n\n");
			continue;	
		}
		
		//get data from file
		if(gottendata == 0)
		{
		    FILE* fp = fopen("file.csv", "r");
		        
		    if(fp == NULL)
			{
				printf("File not found\n");
				break;	
			}
		    
	        //catches title from file
	    	fscanf(fp, ignoretitle, l,l,l,l,l,l,l,l);
		    getdata(fp);
			fclose(fp);
			gottendata = 1;
		}
        
        //display
		if(choice == 1)
	    {
	        long long n;
	        
	        printf("number of rows: ");
	        scanf("%lld", &n);
	        system("cls");
	        if(n <= 0) continue;
			display(n);
			printf("\n");
	    }
	    
	    //select row
	    else if(choice == 2)
	    {
	        int col = 0;
	        
	        char collum[50];
			long long findint;
	        printf("Choose collum: ");
	        scanf("%s%*c", collum);
	        col = collum_picker(collum);
	        if(col == 0)
			{
				printf("there is no collum with that name\n\n");
				continue;
			}
			
	        printf("What data do you want to find? ");
	        if(col >= 3 && col <= 6)
	        {
	        	scanf("%lld", &findint);
	        	char find[] = "none";
	        	search(col, find, findint);
			}

	        else
	        {
	        	char find[50];
	        	scanf("%s%*c", find);
	        	search(col, find, findint);
			}
	        
	        printf("\n");
	        fflush(stdin);
		}
	    //sort
	    else if(choice == 3)
	    {
	    	int col = 0, order = 0;
			char collum[50], sortby[50];
			
			printf("Choose collum: ");
	        scanf("%s%*c", collum);
	        col = collum_picker(collum);
	        if(col == 0)
			{
				printf("there is no collum with that name\n\n");
				continue;
			}
			
			printf("Sort ascending (asc) or descending (desc)? ");
	        scanf("%s%*c", sortby);
			order = orderpicker(sortby);

			int arr[10000] = {};
			bubbleSort(list, amount, col, order);

			display(10);
		}
	    
	    //export
	    else if(choice == 4)
	    {
	    	char file_name[100] = {"\0"};
	    	int dot_location = -1, file_extension_exist = 0;
	    	printf("File name: ");
	    	scanf("%[^\n]", file_name);
	    	//replace underscore with space and find it user alrd specify a file extension
	    	for(int i = 0; i < strlen(file_name); i++)
	    	{
	    		if(file_name[i] == '_')
					file_name[i] = ' ';
				if(file_name[i] == '.')
					dot_location = i;	    		
			}
		  	char printFname[100] = {"\0"};
		  	
		  	//if file extension exist ingnore default to .csv
			if(dot_location != -1 && file_name[dot_location+1] != '\0')
			{
				file_extension_exist = 1;
				for(int i = 0; i < dot_location; i++)
					printFname[i] = file_name[i];
			}
			
			else if(file_extension_exist == 0)
			{
				strcpy(printFname, file_name);
		    	int lenght = strlen(file_name);
		    	file_name[lenght] = '.';
		    	file_name[lenght+1] = 'c';
		    	file_name[lenght+2] = 's';
		    	file_name[lenght+3] = 'v';
		    	file_name[lenght+4] = '\0';
			}
			
			// call export funtion
	    	exportdata(file_name, printFname);
		}
	    
	    
	    
	    
    }while (choice != 5);
    
}

void exportdata(const char* filename, const char* printFname)
{
	FILE* fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Error can't open file for writing'\n");
        return;
    }
    // insert header and data
    fprintf(fp, "Location,City,Price,Rooms,Bathrooms,Carparks,Type,Furnish\n");
    
    for(long long i = 0; i < amount; i++)
    {
        fprintf(fp, "%s,%s,%lld,%lld,%lld,%lld,%s,%s\n", list[i].location, list[i].city, list[i].price, list[i].rooms, list[i].bathrooms, list[i].carparks, list[i].type, list[i].furnish);
    }
    fclose(fp);
    
    //output
    printf("Data successfully written to file %s!\n", printFname);

}

void swap(data* xp, data* yp)
{
    data temp = *xp;

    *xp = *yp;
    *yp = temp;
}
 
//sorry pak merge sortnya tidak bisa sudah di coba tapi error terus
void bubbleSort(data arr[], int n, int col, int order)
{
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++) {
    	int temparr[10000] = {};
        swapped = false;
        for (j = 0; j < n - i - 1; j++) 
		{
			//sort by specify collum
			if(col == 1)
				temparr[j] = strcmp(arr[j].location, arr[j+1].location);
			else if(col == 2)
				temparr[j] = strcmp(arr[j].city, arr[j+1].city);
			else if(col == 3)
			{
				if (arr[j].price > arr[j + 1].price)
					temparr[j] = 1;
				else
					temparr[j] = -1;
			}
			else if(col == 4)
			{
				if (arr[j].rooms > arr[j + 1].rooms)
					temparr[j] = 1;
				else
					temparr[j] = -1;
			}
			else if(col == 5)
			{
				if (arr[j].bathrooms > arr[j + 1].bathrooms)
					temparr[j] = 1;
				else
					temparr[j] = -1;
			}
			else if(col == 6)
			{
				if (arr[j].carparks > arr[j + 1].carparks)
					temparr[j] = 1;
				else
					temparr[j] = -1;
			}
			else if(col == 7)
				temparr[j] = strcmp(arr[j].type, arr[j+1].type);
			else if(col == 8)
				temparr[j] = strcmp(arr[j].furnish, arr[j+1].furnish);
			
			//sort by ascending or descending
			if(order == 1)
			{
	            if (temparr[j] > 0) 
				{
	                swap(&arr[j], &arr[j + 1]);
	                swapped = true;
	            }	
			}
			else
			{
				if (temparr[j] < 0)
				{
	                swap(&arr[j], &arr[j + 1]);
	                swapped = true;
	            }
			}
        }
 
        // If no two elements were swapped by inner loop,
        // then break
        if (swapped == false)
            break;
    }
}

int orderpicker(char sortby[])
{
	//picks order by asc or desc
	for(int i = 0; i < strlen(sortby); i++) 
	if(sortby[i] >= 'A' && sortby[i] <= 'Z') 
		sortby[i] += 32;
	
    if(strcmp(sortby, "ascending") == 0 || strcmp(sortby, "asc") == 0) return 1;
    else if(strcmp(sortby, "descending") == 0 || strcmp(sortby, "desc") == 0) return 2;
	else
	{
		printf("Wrong Input Format\n\n");
		return 0;
	}
}

void search(int col, char find[], long long findint)
{
	// find specify data from given collum
	int title_flag = 0;
	int found_flag = 0;
	if(col == 1)
	{
		for(long long i = 0; i < amount; i++)
		{
			//find data needed (case sesitive)
			if(strcmp(list[i].location, find) == 0)
			{
				PrintSearchFound(title_flag, i);
				title_flag = 1;
				found_flag = 1;
			}
		}
	}
	else if(col == 2)
	{
		for(long long i = 0; i < amount; i++)
		{
			//find data needed (case sesitive)
			if(strcmp(list[i].city, find) == 0)
			{
				PrintSearchFound(title_flag, i);
				title_flag = 1;
				found_flag = 1;
			}
		}
	}
	else if(col == 3)
	{
		for(long long i = 0; i < amount; i++)
		{
			//find data needed (case sesitive)
			if(list[i].price == findint)
			{
				PrintSearchFound(title_flag, i);
				title_flag = 1;
				found_flag = 1;
			}
		}
	}
	else if(col == 4)
	{
		for(long long i = 0; i < amount; i++)
		{
			//find data needed (case sesitive)
			if (list[i].rooms == findint)

			{
				PrintSearchFound(title_flag, i);
				title_flag = 1;
				found_flag = 1;
			}
		}
	}
	else if(col == 5)
	{
		for(long long i = 0; i < amount; i++)
		{
			//find data needed (case sesitive)
			if(list[i].bathrooms == findint)
			{
				PrintSearchFound(title_flag, i);
				title_flag = 1;
				found_flag = 1;
			}
		}
	}
	else if(col == 6)
	{
		for(long long i = 0; i < amount; i++)
		{
			//find data needed (case sesitive)
			if(list[i].carparks == findint)
			{
				PrintSearchFound(title_flag, i);
				title_flag = 1;
				found_flag = 1;
			}
		}
	}
	else if(col == 7)
	{
		for(long long i = 0; i < amount; i++)
		{
			//find data needed (case sesitive)
			if(strcmp(list[i].type, find) == 0)
			{
				PrintSearchFound(title_flag, i);
				title_flag = 1;
				found_flag = 1;
			}
		}
	}
	else if(col == 8)
	{
		for(long long i = 0; i < amount; i++)
		{
			//find data needed (case sesitive)
			if(strcmp(list[i].furnish, find) == 0)
			{
				PrintSearchFound(title_flag, i);
				title_flag = 1;
				found_flag = 1;
			}
		}
	}
	
	if(found_flag == 0)
		printf("Data not found!\n\n");
}

void PrintSearchFound(int title_flag, int i)
{
	//prints the data from search function
	if(title_flag == 0)
	{
		printf(titleprintingFormat, "Location", "City", "Price", "Rooms","Bathrooms","Carparks", "Type", "Furnish");
	}
	
	printf(printingFormat, list[i].location, list[i].city, list[i].price, list[i].rooms, list[i].bathrooms, list[i].carparks, list[i].type, list[i].furnish);
}

int collum_picker(char collum[])
{
	//convert collum to uncapitalize
	for(int i = 0; i < strlen(collum); i++) 
		if(collum[i] >= 'A' && collum[i] <= 'Z') 
			collum[i] += 32;
	
	//return title number 
	if(strcmp(collum, "location") == 0) return 1;
	else if (strcmp(collum, "city") == 0) return 2;
	else if (strcmp(collum, "price") == 0) return 3;
	else if (strcmp(collum, "rooms") == 0) return 4;
	else if (strcmp(collum, "bathrooms") == 0) return 5;
	else if (strcmp(collum, "carparks") == 0) return 6;
	else if (strcmp(collum, "type") == 0) return 7;
	else if (strcmp(collum, "furnish") == 0) return 8;
	else return 0;
}

void display(int n)
{
	printf(titleprintingFormat, "Location", "City", "Price", "Rooms","Bathrooms","Carparks", "Type", "Furnish");
	long long i = 0;
	while(i < n && i<amount)
	{
		//item
		printf(printingFormat, list[i].location, list[i].city, list[i].price, list[i].rooms, list[i].bathrooms, list[i].carparks, list[i].type, list[i].furnish);
        i++;
	}
}

void getdata(FILE* fp)
{
	long long i = 0;
	while(true)
	{
        //scan for item and detect eof
        int itemsRead = fscanf(fp, filescanfFormat, list[i].location, list[i].city, &list[i].price, &list[i].rooms, &list[i].bathrooms, &list[i].carparks, list[i].type, list[i].furnish);
	        if (itemsRead != 8)
			{
				break;
			}
		amount++;
		i++;
	}
}
