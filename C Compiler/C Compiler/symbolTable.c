#include "symbolTable.h"
#include <stdio.h>
#include <stdint.h>


TableNode* currentScopeTable = NULL;

void make_table()
{
	HashTable* newHashTable = (HashTable*)malloc(sizeof(HashTable));;
	TableNode* newTableNode = (TableNode*)malloc(sizeof(TableNode));;
	initHashTable(newHashTable);
	newTableNode->hash_table = newHashTable;
	if(currentScopeTable != NULL)
	{
		newTableNode->father = currentScopeTable;
	}
	else
	{
		newTableNode->father = NULL;
	}
	currentScopeTable = newTableNode;
}

void initHashTable(HashTable* newHashTable)
{
	for(int i=0; i<TABLE_SIZE; i++)
		(*newHashTable)[i] = NULL;
}

void pop_table (FILE* yyoutS){
	if(currentScopeTable)
	{
		TableNode* deleted = currentScopeTable;
		currentScopeTable = currentScopeTable->father;
		deleteHashTable(deleted->hash_table, yyoutS);
		free(deleted);
		printf("---- Top Symbol table node was droped ----\n");
	}
}

void deleteHashTable(HashTable* hash_table, FILE* yyoutS){
	TableEntry* current_entry = NULL;
	TableEntry* entery_for_delete = NULL;
	for(int i=0; i<TABLE_SIZE; i++)
	{
		current_entry = (*hash_table)[i];
		while(current_entry){
			entery_for_delete = current_entry;
			if (entery_for_delete->entry_role != Function && entery_for_delete->isUsed == 0)
			{
				fprintf(yyoutS, "{Line %d} - unused entry '%s' \n", entery_for_delete->line,entery_for_delete->name);

			}
			if (entery_for_delete->entry_role == Function && entery_for_delete->isFunctionImplemented == 0)
			{
				fprintf(yyoutS, "{Line %d} - unimplemnted function '%s' \n", entery_for_delete->line, entery_for_delete->name);

			}
			current_entry = current_entry->next;
			if(entery_for_delete->name){
				free(entery_for_delete->name);
			}
			free(entery_for_delete);
		}
	}

	free(hash_table);
}

TableEntry* insert (char* id_name){
	TableEntry* temp = lookup(currentScopeTable->hash_table, id_name);
	if(temp){
		return NULL;
	}
	else{		
		int hashed_key = hash_function(id_name);
		HashTable* curr_table = currentScopeTable->hash_table;
		TableEntry* hashTableEntry = (*curr_table)[hashed_key];
		TableEntry* newEntry = (TableEntry*)calloc(1,sizeof(TableEntry));
		newEntry->name = (char*)malloc((strlen(id_name) + 1) * sizeof(char));
		strcpy(newEntry->name,id_name);
		newEntry->line = 0;

		if (hashTableEntry == NULL){
			newEntry->next = NULL;
			(*curr_table)[hashed_key] = newEntry;
		}
		else{
			newEntry->next = hashTableEntry;
			(*curr_table)[hashed_key] = newEntry;
		}
		return newEntry;
	}
}

TableEntry* lookup (HashTable* curr_table, char* id_name){
	printf("\nSYMBOL TABLE: Trying to find the id_name '%s' in the current scope", id_name);
	int hashed_key = hash_function(id_name);
	TableEntry* curr_entry = (*curr_table)[hashed_key];
	while(curr_entry != NULL){
		printf("\nSYMBOL TABLE: looking in hash entry %d: '%s' == '%s' ?", hashed_key, curr_entry->name, id_name);
		if( strcmp(curr_entry->name, id_name) == 0){
			printf("\nSYMBOL TABLE: the id name '%s' was found in this scope table, in entry [%d]", id_name, hashed_key);
			return curr_entry;
		}
		curr_entry = curr_entry->next;
	}
	printf("\nSYMBOL TABLE: the id name '%s' was not found in this scope table", id_name);
	return NULL;
}

TableEntry* find (char* id_name){
	printf("\nSYMBOL TABLE: Trying to find '%s' in any of the scopes", id_name);
	TableNode* curr_node = currentScopeTable;
	HashTable* curr_hash_table;
	TableEntry* entry;
	while (curr_node != NULL){
		curr_hash_table = curr_node->hash_table;
		entry = lookup(curr_hash_table,id_name);
		if(entry != NULL){
			printf("\nSYMBOL TABLE: ID name '%s' found", id_name);
				return entry;
		}
		curr_node = curr_node->father;
	}
	printf("\nSYMBOL TABLE: ID name '%s' wasn't found in any of the scopes", id_name);
	return NULL;
}

int hash_function(char *lexeme) {
	const int k = 100;

	int i = 0;
	int n = strlen(lexeme);
	uint64_t h = 0;

	for(; i < n; i++) {
		h = k * h + lexeme[i];
	}

	h = h & 0x3FFFFFFF;
	
	return h % TABLE_SIZE;
}