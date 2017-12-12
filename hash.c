/*******************************************************************
This Hash Program Uses a Hash Function which is inspired by Dan Berstein
His Hash Function is called djb2, and the one in this program is only
slightly modified from his
********************************************************************* */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

int hash_function(struct hash_table *h_table, char *key)
{

  unsigned long hash = 5381;
  int c;

  while((c = *key++))
    hash = ((hash << 5) + hash) + c;

  return hash % 5381;

}

struct hash_table* malloc_hash_table(int size)
{
  struct hash_table *h_table;
  //Allocate the Table Struct
  if((h_table = malloc(sizeof(struct hash_table))) == NULL){
    return NULL;
  }
 
  //Allocate Size Number of Index's for the table  
  //Initialize them to 0s, And set the table size
  h_table->table = calloc(size,sizeof(struct hash_node));
  h_table->size = size;
  
  return h_table;
}

struct hash_node* malloc_hash_node(char *key,void *value)
{
  struct hash_node *new_node;
  //Error check to make sure we allocate space
  if((new_node = malloc(sizeof(struct hash_node))) == NULL){return NULL;}

  //Error check to make sure key is not empty
  if((new_node->keys = key) == NULL){return NULL;}

  //Error check to make sure value is not empty
  if((new_node->values = strdup(value)) == NULL){return NULL;}

  //Set the next node to null, and return it
  new_node->next = NULL;
  return new_node;
}

void insert_hash_node(struct hash_table *h_table,char *key,void *value)
{
  int hash_index;
  struct hash_node *search_node, *new_node, *last_node;
  
  //Calc the HashValue to get Array Index.
  hash_index = hash_function(h_table,key);
  search_node = h_table->table[hash_index];
  
  //printf("About to insert a node at  HashIndex %d\n",hash_index);
  //printf("The Table is of Size %d\n",h_table->size);
  
  //We need to check if something is already hashed to that index
  //If there is something there, and its not the same key
  //Then keep traversing the list.
  while(search_node != NULL && strcmp(search_node->keys,key) != 0){
    last_node = search_node;
    search_node = search_node->next;
  }

  //Did we exit while because a duplicate was found?
  //If yes, we need to change value in case user wants to update
  if(search_node != NULL && strcmp(search_node->keys,key) == 0){
    free(search_node->values);
    //Since we free previous memory we need need to malloc new memory
    printf("Inserting an Existing Key, Updating Key Value\n");
    search_node->values = strdup(value);
  }
  
  //We've Exited the While and No Duplicate was found.
  //So now we insert a new node
  else{
    new_node = malloc_hash_node(key,value);
    
    //We need to figure out if we're inserting in the begining, middle,or end of the list
    if(search_node == h_table->table[hash_index]){
      //We are at the start of the list
      h_table->table[hash_index] = new_node;
      new_node->next = search_node;
    }

    else if(!search_node){
      //We are at the end of the list
      last_node->next = new_node;
    }
    
    else{
      //We must be somewhere in the middle of the list
      new_node->next = search_node;
      last_node->next = new_node;
    }
    
  }
}

void * retrieve_hash_key(struct hash_table *h_table,char *key)
{
  int hash_value;
  struct hash_node* node;
  hash_value = hash_function(h_table,key);
  printf("Just Looked up a Node at HashIndex %d\n",hash_value);
  //printf("The Table is of Size %d\n",h_table->size);
  
  node = h_table->table[hash_value];
  while(node){
    if(strcmp(key,node->keys) == 0){
      //Key found
      return node->values;
    }
    else{
      node = node->next;
    }
  }
  return NULL;
}

int main(int argc, char **argv)
{ 
  struct hash_table* mytable = malloc_hash_table(10);

  insert_hash_node(mytable,"Key1","Value1");
  insert_hash_node(mytable,"Key2","Value2");
  insert_hash_node(mytable,"Key12","Value12");
  insert_hash_node(mytable,"Key1","Value12");

  printf("Values: %s\n", (char*)retrieve_hash_key(mytable,"Key1"));
  printf("Values: %s\n", (char*)retrieve_hash_key(mytable,"Key2"));
  printf("Values: %s\n", (char*)retrieve_hash_key(mytable,"Key12"));
  printf("Values: %s\n", (char*)retrieve_hash_key(mytable,"Key1"));
}
