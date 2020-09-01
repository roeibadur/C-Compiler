#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#define TABLE_SIZE 1021
#include <stdio.h>

typedef enum EntryType {
	Void,
	Integer,
	Float,
	Exception
}EntryType;

typedef enum EntryRole {
	Function,
	Variable,
	Array
}EntryRole;



/*an entry to store the variable name, its datatype, and its line of declaration */
typedef struct TableEntry {
	EntryRole entry_role;
	int isUsed;
	EntryType entry_type;
	int arrayDimnestions;
	char *name;
	int line;
	int isFunctionImplemented;
	struct TableEntry* next;
	struct TableEntry* nextParam;
}TableEntry;

/* HashTable is a hash table built from pointers to table enteries,
	each table entry is a one way linked list */
typedef TableEntry* HashTable[TABLE_SIZE];

/* a table node represent a single Symbol Table in a stack,
	one single table node is dedicated to a single scope
	each node is linked with its "father" node */
typedef struct TableNode {
	HashTable* hash_table;
	struct TableNode* father;
}TableNode;



void make_table ();

void pop_table (FILE* yyoutS);

TableEntry* insert (char* id_name);

TableEntry* lookup (HashTable* curr_table, char* id_name);

TableEntry* find (char* id_name);



void initHashTable(HashTable* newHashTable);
void deleteHashTable(HashTable* hash_table, FILE* yyoutS);
int hash_function(char *lexeme);

#endif