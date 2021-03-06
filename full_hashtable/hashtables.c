#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Hash table key/value pair with linked list pointer
 ****/
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/****
  Hash table with linked pairs
 ****/
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/****
  Create a key/value linked pair to be stored in the hash table.
 ****/
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/****
  Fill this in.

  All values in storage should be initialized to NULL
 ****/
HashTable *create_hash_table(int capacity)
{
  HashTable *ht =malloc(sizeof(HashTable));

  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(char *));

  return ht;
}

/****
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 ****/
void hash_table_insert(HashTable *ht, char *key, char *value)
{
   //create new node using LinkedPair struct from above
  LinkedPair *new_node = create_pair(key, value); 

  // create array index to insert new node into 
  unsigned int new_index = hash(key, ht->capacity); 

  if (ht->storage[new_index] == 0) { // if current index is empty, insert new_node
    ht->storage[new_index] = new_node;
  }

   else {
    while (ht->storage[new_index] != 0) { 
      // if the keys are the same, replace current value with new value
      if (strcmp(ht->storage[new_index]->key, key) == 0) { 
        ht->storage[new_index]->value = value;
        break;
      }
      // if keys are different AND there's an available "next" slot, insert in next slot
      else if (strcmp(ht->storage[new_index]->key, key) != 0 && ht->storage[new_index]->next == NULL) {
        ht->storage[new_index]->next = new_node;
        break;
      }
      //attach linked list node
      ht->storage[new_index] = ht->storage[new_index]->next;
    }
  }


}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
  // hash key to find index
  unsigned int hash_key = hash(key, ht->capacity); 
  // for readability, new variable
  LinkedPair *current_node = ht->storage[hash_key]; 

  while (current_node != 0) { // while there's a current node
    if (strcmp(current_node->key, key) == 0 && current_node->next == 0) { 
      // are the strings the same? YES --- is there a linked node? --- NO
      destroy_pair(current_node);
      break;
    }
    else if (strcmp(current_node->key, key) == 0 && current_node->next != 0) { 
      // are the strings the same? YES ---  is there a linked node? --- YES
      current_node->key = current_node->next->key;
      current_node->value = current_node->next->value;
      current_node->next = current_node->next->next;
      break;
    }
    current_node = current_node->next; 
  }

}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key)
{
 
 // hash key to find index
  unsigned int hash_key = hash(key, ht->capacity); 
  // for readability, new variable
  LinkedPair *current_node = ht->storage[hash_key]; 

  while (current_node != 0) { 
    if (strcmp(current_node->key, key) == 0) { //...if match
      return current_node->value;
    }
    else if (strcmp(current_node->key, key) != 0 && current_node->next == 0) { //...if no match
      fprintf(stderr, "key not found");
      return NULL;
    }
    current_node = current_node->next; 
  }
  return 0;

}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  for (int i=0; i < ht->capacity; i++) {
    ht->storage[i] = NULL; 
    destroy_pair(ht->storage[i]); 
  }

  free(ht->storage);
  free(ht);

}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht;

  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
