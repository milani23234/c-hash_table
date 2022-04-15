#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define RESIZE_THREESHOLD .65
#define MIN_INITIAL_CAPACITY 7
#define DEFAULT_TABLE_CAPACITY 7

/**
 * struct _hash_bucket - A node of the hash table
 * 
 * @brief Implemeneted as a singly linked list.
 * 
 * When a node must be created at a specific index of the
 * table and the index already contains a node, the first
 * available node from the linked list will be used.
 */
typedef struct _hash_bucket
{
   struct _hash_bucket* next;
   void* key;
   void* value;

} _hash_bucket;

/**
 * struct hash_table - The hash table object
 * 
 * @brief Contains the stored nodes and the required
 * data to perform the table operations.
 * 
 * Allows to provide custom hash and compare
 * functions for custom data types.
 */
typedef struct hash_table
{
   _hash_bucket** buckets;

   size_t size;
   size_t capacity;
   size_t key_size;
   size_t value_size;

   size_t (*hash_function)(void*);
   int (*compare_function)(void*, void*, size_t);

} hash_table;

/**
 * @brief Default compare functions for integers
 * 
 * @param first Pointer to the first integer
 * @param second Pointer to the second integer
 * @param size Size of the integer type
 */
int compare_integer(void* first, void* second, size_t size);

/**
 * @brief Default compare functions for strings
 * 
 * @param first Pointer to the first string
 * @param second Pointer to the second string
 * @param size Unused paramter for strings
 */
int compare_string(void* first, void* second, size_t size);

/**
 * @brief Default hash function for strings
 * 
 * @return size_t The generated hash value
 */
size_t hash_string(void* string);

/**
 * @brief Default hash function for integers
 * 
 * @return size_t The generated hash value
 */
size_t hash_integer(void* integer);

/**
 * @brief Inizialize an hash_table object
 * 
 * @param key_size The size of the type of the key
 * @param value_size The size of the type of the value
 * @return void* If the table is created successfully, a pointer to it's buckets, otherwise NULL.
 */
void* hash_table_create(hash_table* table, size_t key_size, size_t value_size, size_t initial_capacity);

/**
 * @brief Frees the table buckets and their keys/values
 */
void hash_table_delete(hash_table* table);

/**
 * @brief Set the function used when hashing
 */
void hash_table_set_hash(hash_table* table, size_t (*hash_function)(void*));

/**
 * @brief Set the function used when comparing keys
 */
void hash_table_set_compare(hash_table* table, int (*compare_function)(void*, void*, size_t));

/**
 * @brief Insert a value into the table mapped to the key.
 * 
 * @return void* If the insertion is successfull, a pointer to the table, otherwise NULL.
 */
void* hash_table_insert(hash_table* table, void* key, void* value);

/**
 * @brief Search a value in the table mapped to the key.
 * 
 * @return void* If the there is a value mapped to key, the value, otherwise NULL.
 */
void* hash_table_lookup(hash_table* table, void* key);

#endif  // HASH_TABLE_H