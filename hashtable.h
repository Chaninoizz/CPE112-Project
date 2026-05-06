#ifndef HASHTABLE_H
#define HASHTABLE_H

#define TABLE_SIZE 100003
#define MAX_NAME 256
#define MAX_UUID_STR 37

typedef struct Node {
    char name[MAX_NAME];
    char uuid_str[MAX_UUID_STR];
    int total_orders;
    float total_purchase;
    float score;
    struct Node* next;
} Node;

typedef struct {
    Node* buckets[TABLE_SIZE];
} HashTable;

unsigned int hash(const char* str);

Node* create_node(const char* name, const char* uuid_str, int orders, float purchase);

void insert(HashTable* ht, const char* key, const char* name, const char* uuid_str, int orders, float purchase);

Node* search_by_name(HashTable* ht, const char* name);
Node* search_by_uuid(HashTable* ht, const char* uuid);

void delete_by_name(HashTable* ht, const char* name);
void delete_by_uuid(HashTable* ht, const char* uuid);

void read_csv(const char* filename, HashTable* name_ht, HashTable* uuid_ht);

void print_ranking(HashTable* ht);

void free_hashtable(HashTable* ht);

void save_to_csv(const char* filename, HashTable* ht);

void update_customer(HashTable* name_ht, HashTable* uuid_ht,
                     const char* name, const char* uuid,
                     int new_orders, float new_purchase);

int search_all_by_name(HashTable* ht, const char* name, Node* results[], int max_results);

#endif