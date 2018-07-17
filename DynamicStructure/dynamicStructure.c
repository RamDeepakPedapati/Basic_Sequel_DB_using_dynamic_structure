#define _CRT_SECURE_NO_WARNINGS

/*
									Documentaion:

----------------------------------->[a simple mrnd database ]<--------------------------------

commands and their syntaxes with example :-
.........................................................................
	->create:

	  syntax:
		mrnd>>>create schema schema_name 
		field1:itsDatatype feild2:itsDatatype feild3:itsDatatype

		ex:
		mrnd>>>create schema s1
			rollno:int name:string

...........................................................................
	->import:

	  syntax:
		mrnd>>>imprt file_path schema_name

		ex:
		mrnd>>>import marks1.txt s1

..............................................................................

	->join:
		mrnd>>>join schema1_name schema2_name destination_schema_name
		
		ex:
		mrnd>>>join s1 s2 s3

...............................................................................
	->select
		syntax:
		mrnd>>> select field1_name,field2_name schema_name
		mrnd>>>selecct * s1

		ex:
		mrnd>>>select name,rollno s1
		mrnd>>>select * s1
.................................................................................

	->select with where
		syntax:
		mrnd>>> select <*><fieldnames> schema_name where field <operator> value
		
		ex:
		mrnd>>>select name,rollno s1 where m1 GT 50	

.................................................................................
	->exit:

	  syntax:
		mrnd>>>exit
.................................................................................
	->clearscreen:

	syntax:
		mrnd>>>cls
...................................................................................
	->flush
	syntax:

		mrnd>>>flush schema_name filepath
	ex:
		mrnd>>>flush s1 marks1.txt
..................................................................................

 */







#include "dependentFunc.h"    //contains some basic functions like stringcomparison,atoi etc...........................................

#include "structures.h"       //this conatins all info about how dynamic structures are created and stored ..............................






/*.................................to skip the index(len) of string until given delimiter comes in that that.........................*/
void stringskip(char *mstring, char *delimiter, int *len)
{
	int i = 0;
	while (mstring[(*len)] != delimiter && mstring[(*len)] != '\0' && mstring[(*len)] != '\n')
	{
		(*len)++;
	}
	(*len) = (*len) + 1;
}



/*....................returns delimiter count such that how many times that delimiter occured in the given string..................*/
int returncommacount(char *string, char c)
{
	int i = 0, count = 0;
	while (string[i] != '\0')
	{
		if (string[i] == c)
			count++;
		i++;
	}
	return count;
}


/*.....stores schema information such that "no of felids" and name of that schema etc 
 But,memory is allocated at the time of the import from a file.......................................................................*/

void createschema(mainstruct* ms, char *schemaname, char *domainstring)
{
	int variablecount = returncommacount(domainstring, ' ') + 1, stringlen = 0;
	(*ms).noOfFields = variablecount;

	//copying fieldnames and types in domainstring for further reference while allocating memory

	(*ms).domainsString = (char*)malloc(sizeof(char) * 100);
	strcpy((*ms).domainsString, domainstring);


	(*ms).name = (char *)malloc(sizeof(char) * 20);
	strcpy((*ms).name, schemaname);


}




/*imports the file information into the created schema and stores in the mainstruct.structobj and also info about that schema in mainstruct */

void importintoschema(char *filePath, mainstruct *ms)
{
	FILE *fp;
	int stringlen = 0;
	char *temp = (char*)malloc(sizeof(char) * 10);
	char *buffer = (char*)malloc(sizeof(char) * 100);
	char *temp1 = (char*)malloc(sizeof(char) * 10);


	(*ms).noOfRows = noOfLine(filePath) - 1;

	//allocating memory based on the no of lines in given file and no of feilds in the given schema...........................................

	(*ms).structobj = (structobj**)malloc(sizeof(structobj*) *((*ms).noOfRows));
	for (int iter = 0; iter < (*ms).noOfRows; iter++)
		(*ms).structobj[iter] = (structobj*)malloc(sizeof(structobj) * ((*ms).noOfFields));


	//creating an 2D array of structobj and allocating memory to if, so that it holds all the data from the file...............................

	for (int iter = 0; iter < (*ms).noOfFields; iter++)
	{
		stringtok((*ms).domainsString, temp, ':', &stringlen);
		stringtok((*ms).domainsString, temp1, ' ', &stringlen);
		for (int iter1 = 0; iter1 < (*ms).noOfRows; iter1++)
		{
			(*ms).structobj[iter1][iter].dt.fieldname = (char*)malloc(sizeof(char) * 10);
			(*ms).structobj[iter1][iter].dt.typeofdata = (char*)malloc(sizeof(char) * 10);
			strcpy((*ms).structobj[iter1][iter].dt.fieldname, temp);
			strcpy((*ms).structobj[iter1][iter].dt.typeofdata, temp1);
		}
	}

	
	//getting information line by line from the file and storing in the structobj 2D array and storing it into main struct..................................
	//each field is seperated here by a delimiter comma.....................................................................................................

	fp = fopen(filePath, "r");
	if (fp != NULL)
	{
		fgets(buffer, 200, fp);
		int iter = 0, i, stringlen;
		while (fgets(buffer, 200, fp))
		{
			i = 0, stringlen = 0;
			while (i < (*ms).noOfFields)
			{
				char *str = (char*)malloc(sizeof(char) * 20);
				stringtok(buffer, str, ',', &stringlen);
				if (!mystrcmp((*ms).structobj[iter][i].dt.typeofdata, "int"))
					(*ms).structobj[iter][i].type.value = useratoi(str);
				else if (!mystrcmp((*ms).structobj[iter][i].dt.typeofdata, "string"))
				{
					(*ms).structobj[iter][i].type.string = (char*)malloc(sizeof(char) * 20);
					strcpy((*ms).structobj[iter][i].type.string, str);
				}
				i++;
			}
			iter++;
		}
	}
	free(temp);
	free(temp1);
	free(buffer);
	fclose(fp);
}


/*...........................dumps the information from the mainstruct to the file specified by the filePath................................*/

void flush(mainstruct ms, char *filepath)
{
	FILE *fp;
	fp = fopen(filepath, "w");

	//storing the column names in the first line of the file...................................................................................

	for (int iter = 0; iter < ms.noOfFields; iter++)
	{
		fprintf(fp, "%s,", ms.structobj[0][iter].dt.fieldname);
	}
	fprintf(fp, "\n");

	//storing each line with the feilds seperated by commas....................................................................................

	for (int iter1 = 0; iter1 < ms.noOfRows; iter1++)
	{
		for (int iter2 = 0; iter2 < ms.noOfFields; iter2++)
		{
			if (!mystrcmp(ms.structobj[iter1][iter2].dt.typeofdata, "int"))
				fprintf(fp, "%d,", ms.structobj[iter1][iter2].type.value);
			else if (!mystrcmp(ms.structobj[iter1][iter2].dt.typeofdata, "string"))
				fprintf(fp, "%s,", ms.structobj[iter1][iter2].type.string);

		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}




//...performs matches and contains operations between two strings and return 1 if string1  conatins or matches string2 otherwise it returns 0

int comparestring(char *val1, char *val2, char *operator)
{
	if (!mystrcmp(operator,"MATCHES"))
	{
		if (!mystrcmp(val1, val2))
			return 1;
		return 0;

	}
	else if (!mystrcmp(operator,"CONTAINS"))
	{
		if (isSubstring(val1, val2))
			return 1;
		return 0;
	}
}


//...perfroms various comparisons like '<' , '>' b/w two integers

int compareint(int value1, int value2, char*operator)
{
	if (!mystrcmp(operator,"GT"))
	{
		if (value1 > value2)
			return 1;
		return 0;
	}
	else if (!mystrcmp(operator,"GE"))
	{
		if (value1 >= value2)
			return 1;
		return 0;
	}
	else if (!mystrcmp(operator,"LE"))
	{
		if (value1 <= value2)
			return 1;
		return 0;
	}
	else if (!mystrcmp(operator,"LT"))
	{
		if (value1  < value2)
			return 1;
		return 0;
	}
	else if (!mystrcmp(operator,"EQ"))
	{
		if (value1 == value2)
			return 1;
		return 0;
	}
}



//select command performing where operation

void selectwhere(mainstruct ms, char *list, char *query)
{
	int index = 0, i = 0, k = 0, j = returncommacount(list, ','), queryindex = 0;
	char *fieldname = (char *)malloc(sizeof(char) * 15);
	char *value = (char*)malloc(sizeof(char) * 15);
	char *operator=(char*)malloc(sizeof(char) * 15);

	stringtok(query, fieldname, ' ', &index);	//stores about which feild to compare like m1 m2 in our schemas
	stringtok(query, operator,' ', &index);		//operator like GT or MATCHES
	stringtok(query, value, '\0', &index);		//stores the value with which the comparision should be made

	for (queryindex = 0; queryindex < ms.noOfFields; queryindex++)
	{
		if (!mystrcmp(ms.structobj[0][queryindex].dt.fieldname, fieldname))
			break;
	}

	int *arr = (int *)calloc(ms.noOfFields + 1, sizeof(int));

	//stores the info about what fields should be selected and maintains a flag for fields
	// if that flag contains 1 then  that correspoding feild  is selected otherwise not selected

	if (!mystrcmp(list, "*"))
		for (int iter = 0; iter < ms.noOfFields; iter++)
			arr[iter] = 1;
	else
	{
		while (k <= j)
		{
			char *item = (char*)malloc(sizeof(char) * 15);
			stringtok(list, item, ',', &i);
			for (int iter = 0; iter < ms.noOfFields; iter++)
				if (!mystrcmp(item, ms.structobj[0][iter].dt.fieldname))
				{
					arr[iter] = 1;
					break;
				}
			k++;
		}
	}


	//selecting the information about the feilds of the selected rows...................
	//row selection is based on the execution of comparision............................

	for (int iter1 = 0; iter1 < ms.noOfRows; iter1++)
	{
		if (!mystrcmp(ms.structobj[0][queryindex].dt.typeofdata, "int"))//performs comparison in where command based on its datatype so here it is int
		{
			if (compareint(ms.structobj[iter1][queryindex].type.value, useratoi(value), operator))
			{
				for (int iter2 = 0; iter2 < ms.noOfFields; iter2++)
				{
					if (arr[iter2] == 1)
					{
						if (!mystrcmp(ms.structobj[iter1][iter2].dt.typeofdata, "int"))
							printf("%-10d ", ms.structobj[iter1][iter2].type.value);
						else if (!mystrcmp(ms.structobj[iter1][iter2].dt.typeofdata, "string"))
							printf("%-25s ", ms.structobj[iter1][iter2].type.string);

					}
				}
				printf("\n");
			}

		}
		else if (!mystrcmp(ms.structobj[0][queryindex].dt.typeofdata, "string"))//performs comparison in where command based on its datatype so here it is string
		{
			if (comparestring(ms.structobj[iter1][queryindex].type.string, value, operator))
			{
				for (int iter2 = 0; iter2 < ms.noOfFields; iter2++)
				{
					if (arr[iter2] == 1)
					{
						if (!mystrcmp(ms.structobj[iter1][iter2].dt.typeofdata, "int"))
							printf("%-10d ", ms.structobj[iter1][iter2].type.value);
						else if (!mystrcmp(ms.structobj[iter1][iter2].dt.typeofdata, "string"))
							printf("%-25s ", ms.structobj[iter1][iter2].type.string);

					}
				}
				printf("\n");
			}

		}


	}

	free(fieldname);
	free(value);
	free(operator);

}


//simpe select command without where condition............

void select(mainstruct ms, char *list)
{
	int *arr = (int *)calloc(ms.noOfFields + 1, sizeof(int));
	int i = 0, k = 0, j = returncommacount(list, ',');


	// stores the info about what fields should be selected and maintains a flag for fields
	// if that flag contains 1 then it is selected otherwise not selected...........

	if (!mystrcmp(list, "*"))
		for (int iter = 0; iter < ms.noOfFields; iter++)
			arr[iter] = 1;
	else
	{
		while (k <= j)
		{
			char *item = (char*)malloc(sizeof(char) * 15);
			stringtok(list, item, ',', &i);
			for (int iter = 0; iter < ms.noOfFields; iter++)
				if (!mystrcmp(item, ms.structobj[0][iter].dt.fieldname))
				{
					arr[iter] = 1;
					break;
				}
			k++;
		}
	}


	//selecting the feilds from each row based on the flag array(arr).....................
	
	for (int iter1 = 0; iter1 < ms.noOfRows; iter1++)
	{
		for (int iter2 = 0; iter2 < ms.noOfFields; iter2++)
			if (arr[iter2] == 1)
			{
				if (!mystrcmp(ms.structobj[iter1][iter2].dt.typeofdata, "int"))
					printf("%-10d ", ms.structobj[iter1][iter2].type.value);
				else if (!mystrcmp(ms.structobj[iter1][iter2].dt.typeofdata, "string"))
					printf("%-25s ", ms.structobj[iter1][iter2].type.string);

			}
		printf("\n");
	}
	free(arr);
}



//performing join of the two structures ms1 and ms2 and stores in the mainstruct called ms........................
//schema in the parameters is the schema name.....................................................................

void join(mainstruct *ms, mainstruct ms1, mainstruct ms2, char *schema)
{

	(*ms).name = (char*)malloc(sizeof(char) * 20);
	strcpy((*ms).name, schema);
	(*ms).noOfFields = (ms2.noOfFields) + (ms1.noOfFields) - 1;
	(*ms).noOfRows = ms1.noOfRows + ms2.noOfRows;

	//allocating memory to the combined schema......................................................................

	(*ms).structobj = (structobj**)malloc(sizeof(structobj*) * (*ms).noOfRows);
	for (int iter = 0; iter < (*ms).noOfRows; iter++)
		(*ms).structobj[iter] = (structobj*)malloc(sizeof(structobj) * ((*ms).noOfFields));

	int iter2 = 0;
	for (int iter = 0; iter < (*ms).noOfFields; iter++)
	{
		for (int iter1 = 0; iter1 < (*ms).noOfRows; iter1++)
		{
			(*ms).structobj[iter1][iter].dt.fieldname = (char*)malloc(sizeof(char) * 20);
			(*ms).structobj[iter1][iter].dt.typeofdata = (char*)malloc(sizeof(char) * 20);
			if (iter < ms1.noOfFields)		//allocating memory for mainstruct from feilds of schema1........................
			{
				strcpy((*ms).structobj[iter1][iter].dt.fieldname, ms1.structobj[0][iter].dt.fieldname);
				strcpy((*ms).structobj[iter1][iter].dt.typeofdata, ms1.structobj[0][iter].dt.typeofdata);
				iter2 = 0;
			}
			else     //allocating memory for mainstruct from feilds of schema2................................................
			{
				strcpy((*ms).structobj[iter1][iter].dt.fieldname, ms2.structobj[0][iter2].dt.fieldname);
				strcpy((*ms).structobj[iter1][iter].dt.typeofdata, ms2.structobj[0][iter2].dt.typeofdata);
			}
		}
		iter2++;
	}


	//joining two schemas......................................................................................................................................

	int *checkarr = (int *)calloc(ms2.noOfRows, sizeof(int));

	datatype dt;
	dt.value = 0;
	dt.string = "N/A";

	int iter4 = 0, count;
	for (int iter1 = 0; iter1 < ms1.noOfRows; iter1++)
	{
		count = 0;
		for (int iter2 = 0; iter2 < ms2.noOfRows; iter2++)
		{
			if (ms1.structobj[iter1][0].type.value == ms2.structobj[iter2][0].type.value)	//this means the primary key of both the schemas(ms2,ms1) is matched 
			{

				for (int iter3 = 0; iter3 < ms1.noOfFields; iter3++)
				{
					(*ms).structobj[iter4][iter3].type = ms1.structobj[iter1][iter3].type;
				}
				for (int iter3 = ms1.noOfFields, iter5 = 1; iter3 < (*ms).noOfFields; iter3++, iter5++)
				{
					(*ms).structobj[iter4][iter3].type = ms2.structobj[iter1][iter5].type;
				}
				iter4++;
				count++;
				checkarr[iter2] = 1;
				break;
			}
		}
		if (count == 0)			//this means primary key(first field)of first schema is not matched with second schema		
		{						//so strings of secondstruct(ms2) are kept as "N/A" and integer fields are kept 0 in resultant mainstruct
 
			for (int iter3 = 0; iter3 < ms1.noOfFields; iter3++)
			{
				(*ms).structobj[iter4][iter3].type = ms1.structobj[iter1][iter3].type;
			}
			for (int iter3 = ms1.noOfFields, iter5 = 1; iter3 < (*ms).noOfFields; iter3++, iter5++)
			{
				(*ms).structobj[iter4][iter3].type = dt;
			}
			iter4++;
		}

	}

	//this means primary key(first field)of second schema is not matched with first schema		
	//so strings of firststruct(ms1) are kept as "N/A" and integer fields are kept 0 in resultant main struct


	for (int iter1 = 0; iter1 < ms2.noOfRows; iter1++)
	{
		if (checkarr[iter1] == 0)
		{
			(*ms).structobj[iter4][0].type = ms2.structobj[iter1][0].type;

			for (int iter3 = 1; iter3 < ms1.noOfFields; iter3++)
			{
				(*ms).structobj[iter4][iter3].type = dt;
			}
			for (int iter3 = ms1.noOfFields, iter5 = 1; iter3 < (*ms).noOfFields; iter3++, iter5++)
			{
				(*ms).structobj[iter4][iter3].type = ms2.structobj[iter1][iter5].type;
			}
			iter4++;
		}
	}

	(*ms).noOfRows = iter4;

}





//in main function we tokenize the command and calls the respected function for that command

int main()
{
	int p = 0;//nof of schemas present

	//stores upto 10 different schemas information if it exceeds we reallocte in the below code
	mainstruct *ms = (mainstruct*)malloc(sizeof(mainstruct) * 10);


	while (1)
	{
		int m = 0, stmtindex = 0;
		char *stmt = (char*)malloc(sizeof(char) * 40);
		char *token1 = (char*)malloc(sizeof(char) * 40);	//token1
		char *token3 = (char*)malloc(sizeof(char) * 25);	//token2
		char *token2 = (char*)malloc(sizeof(char) * 25);	//token3
		char *token4 = (char*)malloc(sizeof(char) * 25);	//token4
		char *token5 = (char*)malloc(sizeof(char) * 25);	//token5

		char *wheretoken = (char*)malloc(sizeof(char) * 25);
		printf("mrnd>>>");
		gets(stmt);

		stringtok(stmt, token1, ' ', &stmtindex);
		if (!mystrcmp(token1, "cls"))		//command to clear the screen
		{
			system("cls");
		}
		else if (!mystrcmp(token1, "exit"))		/*command to stop the mrnd sql i.e, to close the program*/
		{
			break;
		}
		else
		{
			stringtok(stmt, token2, ' ', &stmtindex);
			stringtok(stmt, token3, ' ', &stmtindex);

			if (!mystrcmp(token1, "select"))		//............select command execution without where condition 
			{
				if (stmt[stmtindex - 1] == '\0')
				{
					for (int iter = 0; iter < p; iter++)
					{
						if (!mystrcmp(ms[iter].name, token3))
						{
							select(ms[iter], token2);
							break;
						}

					}
				}
				else
				{
					stringtok(stmt, token4, ' ', &stmtindex);
					if (!mystrcmp(token4, "where"))		//.....select command execution with where condition 
					{
						stringtok(stmt, token5, '\0', &stmtindex);
						for (int iter = 0; iter < p; iter++)
						{
							if (!mystrcmp(ms[iter].name, token3))
							{
								selectwhere(ms[iter], token2, token5);
								break;
							}

						}
					}

				}

			}
			else if (!mystrcmp(token1, "flush"))	//...........flush the schema given into the file specified
			{
				for (int iter = 0; iter < p; iter++)
				{
					if (!mystrcmp(ms[iter].name, token2))
					{
						flush(ms[iter], token3);
						break;
					}
				}
			}
			else if (!mystrcmp(token1, "join"))		//..........joins the two schemas 
			{
				int count = 0;
				char *j2schema = (char*)malloc(sizeof(char) * 20);
				stringtok(stmt, j2schema, '\0', &stmtindex);
				for (int iter1 = 0; iter1 < p; iter1++)
				{
					for (int iter2 = 0; iter2 < p; iter2++)
					{
						if (!mystrcmp(ms[iter1].name, token2) && !mystrcmp(ms[iter2].name, token3))
						{
							join(&ms[p], ms[iter1], ms[iter2], j2schema);
							p++;
							count++;
							break;
						}
						if (count > 0)
							break;
					}
				}
			}
			else if (!mystrcmp(token1, "create"))	//............create command execution
			{

				char *dstring = (char*)malloc(sizeof(char) * 100);
				gets(dstring);
				createschema(&ms[p], token3, dstring);
				p++;

			}
			else if (!mystrcmp(token1, "import"))	//.............imports the file specified into the specified schema
			{

				for (int iter = 0; iter < p; iter++)
				{
					if (!mystrcmp(ms[iter].name, token3))
					{
						importintoschema(token2, &ms[iter]);
						break;
					}

				}
			}
		}


		free(token1);
		free(token2);
		free(token3);
		free(token4);
		free(token5);
	}

	return 0;
}