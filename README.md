
# c-hash_table
### A generic hash table in C

Example:
```c
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

   // Frees the table
   hash_table_delete(&ht);
}
```

By default string hash and compare functions are used, if you wish to use integer instead do the following:
```c
hash_table ht;

// Adjust the table to use integer keys
ht.compare_function = compare_integer;
ht.hash_function = hash_integer;
```
Alternatively you can provide your own functions for custom data types.