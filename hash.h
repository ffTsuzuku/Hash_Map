struct hash_node
{
  char *keys;
  void *values;
  struct hash_node *next;
};

struct hash_table
{
  int size;
  struct hash_node** table;
};

/*
@brief: This function takes in a key, and hashes it into an array index. 
@param1:is the hash table,it belongs to
@param2:the key to hash.
 */
int hash_function(struct hash_table *h_table, char *key);


/*
@brief:This Function Allocates the table, which is needed to initialize
the table, as well as for when we need to resize the table
xo@parameter size: This is the size of the table
*/
struct hash_table* malloc_hash_table(int size);

/*
@brief:
 */
struct hash_node* malloc_hash_node(char *key,void *value);


void insert_hash_node(struct hash_table *h_table, char *key, void *values);

void * retrieve_hash_key(struct hash_table *h_table,char *key);

