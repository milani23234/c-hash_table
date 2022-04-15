#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
   hash_table ht;

   // Initialize the table
   if (!hash_table_create(&ht, sizeof(char*), sizeof(char*), DEFAULT_TABLE_CAPACITY))
   {
      printf("hash_table_create() failed");
      return -1;
   }

   // Insert a the value -> "value" mapped to the key -> "key"
   if (!hash_table_insert(&ht, "key", "value"))
   {
      printf("hash_table_insert() failed");
      return -1;
   }

   // Lookup a value mapped to "key"
   char* result = hash_table_lookup(&ht, "key");

   // Print the value
   printf("%s\n", result ? result : "Element not found");
}
