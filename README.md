# Basic_Sequel_DB_using_dynamic_structure
###This is a sequel database clone that performs basic operations give below. This is done done by the concept of creating dynamic datastructures

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
