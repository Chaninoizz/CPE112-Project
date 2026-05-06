#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

// ================= HASH =================
unsigned int hash(const char* str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % TABLE_SIZE;
}

// ================= CREATE =================
Node* create_node(const char* name, const char* uuid_str, int orders, float purchase) {
    Node* new_node = (Node*)malloc(sizeof(Node));

    strncpy(new_node->name, name, MAX_NAME);
    strncpy(new_node->uuid_str, uuid_str, MAX_UUID_STR);

    new_node->total_orders = orders;
    new_node->total_purchase = purchase;
    new_node->score = (orders * 1.5) + (purchase * 0.01);

    new_node->next = NULL;
    return new_node;
}

// ================= INSERT =================
void insert(HashTable* ht, const char* key, const char* name, const char* uuid_str, int orders, float purchase) {
    unsigned int index = hash(key);
    Node* new_node = create_node(name, uuid_str, orders, purchase);

    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
}

// ================= SEARCH =================
Node* search_by_name(HashTable* ht, const char* name) {
    unsigned int index = hash(name);
    Node* current = ht->buckets[index];

    while (current) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

Node* search_by_uuid(HashTable* ht, const char* uuid) {
    unsigned int index = hash(uuid);
    Node* current = ht->buckets[index];

    while (current) {
        if (strcmp(current->uuid_str, uuid) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

// ================= DELETE =================
void delete_by_name(HashTable* ht, const char* name) {
    unsigned int index = hash(name);
    Node* current = ht->buckets[index];
    Node* prev = NULL;

    while (current) {
        if (strcmp(current->name, name) == 0) {
            if (prev) prev->next = current->next;
            else ht->buckets[index] = current->next;

            free(current);
            printf("Deleted (name)\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Not found\n");
}

void delete_by_uuid(HashTable* ht, const char* uuid) {
    unsigned int index = hash(uuid);
    Node* current = ht->buckets[index];
    Node* prev = NULL;

    while (current) {
        if (strcmp(current->uuid_str, uuid) == 0) {
            if (prev) prev->next = current->next;
            else ht->buckets[index] = current->next;

            free(current);
            printf("Deleted (uuid)\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Not found\n");
}

// ================= CSV =================
void read_csv(const char* filename, HashTable* name_ht, HashTable* uuid_ht) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening CSV");
        return;
    }

    char line[1024];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char name[MAX_NAME] = {0};
        char uuid_str[MAX_UUID_STR] = {0};
        int orders;
        float purchase;

        if (sscanf(line, " %255[^,],%36[^,],%d,%f",
                   name, uuid_str, &orders, &purchase) == 4) {

            name[strcspn(name, "\r\n")] = 0;
            uuid_str[strcspn(uuid_str, "\r\n")] = 0;

            insert(name_ht, name, name, uuid_str, orders, purchase);
            insert(uuid_ht, uuid_str, name, uuid_str, orders, purchase);
        }
    }

    fclose(file);
}

// ================= SORT =================
Node* merge(Node* a, Node* b) {
    if (!a) return b;
    if (!b) return a;

    if (a->score >= b->score) {
        a->next = merge(a->next, b);
        return a;
    } else {
        b->next = merge(a, b->next);
        return b;
    }
}

Node* split(Node* head) {
    Node* fast = head->next;
    Node* slow = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    Node* mid = slow->next;
    slow->next = NULL;
    return mid;
}

Node* merge_sort(Node* head) {
    if (!head || !head->next) return head;

    Node* mid = split(head);
    head = merge_sort(head);
    mid = merge_sort(mid);

    return merge(head, mid);
}

Node* collect(HashTable* ht) {
    Node* head = NULL;

    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = ht->buckets[i];
        while (current) {
            Node* n = create_node(
                current->name,
                current->uuid_str,
                current->total_orders,
                current->total_purchase
            );
            n->next = head;
            head = n;
            current = current->next;
        }
    }
    return head;
}

// ================= PRINT =================
void print_ranking(HashTable* ht) {
    Node* list = collect(ht);
    
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    list = merge_sort(list);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;

    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

    int rank = 1;
    while (list && rank <= 100) {
        printf("%-3d %-20s %-36s Orders:%-3d Purchase:%-10.2f Score:%.2f\n",
        rank++,
        list->name,
        list->uuid_str,
        list->total_orders,
        list->total_purchase,
        list->score);

        Node* temp = list;
        list = list->next;
        free(temp);
    }

    printf("\nMerge Sort took %f seconds\n\n", time_taken);
}

// ================= FREE =================
void free_hashtable(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = ht->buckets[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
}

void save_to_csv(const char* filename, HashTable* ht) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error writing file");
        return;
    }

    fprintf(file, "name,uuid,orders,purchase\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = ht->buckets[i];
        while (current) {
            fprintf(file, "%s,%s,%d,%.2f\n",
                    current->name,
                    current->uuid_str,
                    current->total_orders,
                    current->total_purchase);

            current = current->next;
        }
    }

    fclose(file);
    printf("CSV updated!\n");
}

void update_customer(HashTable* name_ht, HashTable* uuid_ht,
                     const char* name, const char* uuid,
                     int new_orders, float new_purchase) {

    Node* n1 = search_by_name(name_ht, name);
    Node* n2 = search_by_uuid(uuid_ht, uuid);

    if (n1) {
        n1->total_orders = new_orders;
        n1->total_purchase = new_purchase;
        n1->score = (new_orders * 1.5) + (new_purchase * 0.01);
    }

    if (n2) {
        n2->total_orders = new_orders;
        n2->total_purchase = new_purchase;
        n2->score = (new_orders * 1.5) + (new_purchase * 0.01);
    }
}

int search_all_by_name(HashTable* ht, const char* name, Node* results[], int max_results) {
    unsigned int index = hash(name);
    Node* current = ht->buckets[index];

    int count = 0;

    while (current && count < max_results) {
        if (strcmp(current->name, name) == 0) {
            results[count++] = current;
        }
        current = current->next;
    }

    return count;
}