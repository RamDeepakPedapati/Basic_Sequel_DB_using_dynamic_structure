/*...this structure is to store fieldname and type of that field
like name:string so that "name" is stored in dataset.fieldname   and "string" is stored in dataset.typeof data......................*/
typedef struct{
	char *fieldname;
	char *typeofdata;
}dataset;


/*...to store the values based on its type in dataset.typeofdata
if its a int it is stored in datatype.value
if its a string it is stored in datatype.string etc..................................................................................*/
typedef struct
{
	int value;
	char *string;
	float f1;
}datatype;

/*.........to store the above two structures to make a node so that it contains a feildname,datatype and its value...................*/
typedef struct{
	dataset dt;
	datatype type;
}structobj;


/*...........array of above structure gives info about a single structure and that is pointed to mainstruct.structobj
->array of this mainstruct gives us several structures each mainstructure is itself a structure containg info about a schema
->it contains name to represent a schema name............*/
typedef struct{
	int noOfFields;
	int noOfRows;
	char *name;
	char *domainsString;
	structobj **structobj;
}mainstruct;
