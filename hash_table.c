#include "hash_table.h"

int compare_integer(void* first, void* second, size_t size)
{
   return memcmp(first, second, size);
}

int compare_string(void* first, void* second, size_t size)
{
   size_t first_length = strlen((char*)first);
   size_t second_length = strlen((char*)second);

   // Useless comparasion with size to prevent compiler warning
   return memcmp(first, second, first_length < second_length ? first_length : second_length < size ? second_length : second_length);
}

size_t hash_string(void* string)
{
   char* key = (char*)string;
   size_t hash = 5381;
   int c;

   while ((c = *key++))
      hash = ((hash << 5) + hash) + c;

   return hash;
}

size_t hash_integer(void* integer)
{
   long key = *(long*)integer;

   key = ((key >> 16) ^ key) * 0x45d9f3b;
   key = ((key >> 16) ^ key) * 0x45d9f3b;
   key = (key >> 16) ^ key;

   return key;
}

void* hash_table_create(hash_table* table, size_t key_size, size_t value_size, size_t initial_capacity)
{
   if (initial_capacity < MIN_INITIAL_CAPACITY)
      initial_capacity = MIN_INITIAL_CAPACITY;

   table->buckets = calloc(initial_capacity, sizeof(_hash_bucket*));
   if (!table->buckets)
      return NULL;

   table->capacity = initial_capacity;
   table->size = 0;
   table->key_size = key_size;
   table->value_size = value_size;
   table->hash_function = hash_string;
   table->compare_function = compare_string;

   return table->buckets;
}

_hash_bucket* hash_bucket_create(hash_table* table, void* key, void* value)
{
   _hash_bucket* bucket = malloc(sizeof(_hash_bucket));
   if (!bucket)
      return NULL;

   bucket->key = malloc(table->key_size);
   if (!bucket->key)
      return NULL;

   bucket->value = malloc(table->value_size);
   if (!bucket->value)
      return NULL;

   memcpy(bucket->key, key, table->key_size);
   memcpy(bucket->value, value, table->value_size);

   return bucket;
}

void hash_table_delete(hash_table* table)
{
   for (size_t i = 0; i < table->capacity; i++)
   {
      if (!table->buckets[i])
         continue;

      free(table->buckets[i]->key);
      free(table->buckets[i]->value);
   }

   free(table->buckets);
}

void hash_table_set_hash(hash_table* table, size_t (*hash_function)(void*))
{
   table->hash_function = hash_function;
}

void hash_table_set_compare(hash_table* table, int (*compare_function)(void*, void*, size_t))
{
   table->compare_function = compare_function;
}

void* hash_table_insert(hash_table* table, void* key, void* value)
{
   if (table->size >= table->capacity * RESIZE_THREESHOLD)
   {
      size_t new_capacity = table->capacity * 2;
      hash_table new_table;

      if (!hash_table_create(&new_table, table->key_size, table->value_size, new_capacity))
         return NULL;

      for (size_t i = 0; i < table->capacity; i++)
      {
         _hash_bucket** current = &table->buckets[i];

         while (*current)
         {
            hash_table_insert(&new_table, table->buckets[i]->key, table->buckets[i]->value);
            current = &(*current)->next;
         }
      }

      hash_table_delete(table);
      table->buckets = new_table.buckets;
      table->capacity = new_table.capacity;
      table->size = new_table.size;
   }

   size_t index = table->hash_function(key) % table->capacity;
   _hash_bucket** current = &table->buckets[index];

   while (*current && table->compare_function((*current)->key, key, table->key_size))
      current = &(*current)->next;

   // Same key, override value
   if (*current)
   {
      free((*current)->value);
      memcpy((*current)->value, value, table->value_size);
      return table;
   }

   *current = hash_bucket_create(table, key, value);
   if (!current)
      return NULL;

   table->size++;

   return table;
}

void* hash_table_lookup(hash_table* table, void* key)
{
   size_t index = table->hash_function(key) % table->capacity;

   _hash_bucket** current = &table->buckets[index];

   while (*current && table->compare_function((*current)->key, key, table->key_size))
      current = &(*current)->next;

   return (*current ? (*current)->value : NULL);
}