#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "bst.h"

#include <time.h>

#define HASH_MODE 1
#define BST_MODE 2
#define DYNAMIC_MODE 3

void generate_dataset(int num_records);

void generate_uuid2(char* uuid_str) {

    sprintf(uuid_str,
        "%08x-%04x-%04x-%04x-%08x",
        rand(),
        rand() & 0xffff,
        rand() & 0xffff,
        rand() & 0xffff,
        (unsigned int)time(NULL) ^ rand()
    );
}

int main() {

    srand(time(NULL));

    int mode;
    BSTNode* bst_root = NULL;

    HashTable name_ht = {0};
    HashTable uuid_ht = {0};

    struct timespec start, end;

    printf("1. Hash Table\n");
    printf("2. AVL Tree\n");
    printf("3. Dynamic\n");
    printf("Select mode: ");
    scanf("%d", &mode);

    clock_gettime(CLOCK_MONOTONIC, &start);

    if (mode == HASH_MODE) {
        read_csv("data.csv", &name_ht, &uuid_ht);
    }
    else if (mode == BST_MODE) {
        bst_root = bst_read_csv("data.csv", bst_root);
    }
    else if (mode == DYNAMIC_MODE){
        read_csv("data.csv", &name_ht, &uuid_ht);
        bst_root = bst_read_csv("data.csv", bst_root);
    }
    else {
        printf("Invalid mode\n");
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

    printf("\nReading csv took %f seconds\n", time_taken);

    int choice;

    while (1) {
        const char* currentmode;
        if (mode == HASH_MODE)
            currentmode = "Hashtable";
        else if (mode == BST_MODE)
            currentmode = "AVL";
        else
            currentmode = "Dynamic";
        
        printf("\n=== MENU (%s) ===\n",currentmode);
        printf("1. Search (name or UUID)\n");
        printf("2. Ranking\n");
        printf("3. Add\n");
        printf("4. Delete\n");
        printf("5. Edit\n");
        printf("6. Generate Dataset\n");
        printf("0. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);
        printf("\n");

        if (choice == 0) break;

        // ================= SEARCH =================
        else if (choice == 1) {
            char input[100];
            printf("Search (name or uuid): ");
            scanf("%s", input);

            input[strcspn(input, "\r\n")] = 0;

            Node* result = NULL;
            BSTNode* bst_result = NULL;
            clock_gettime(CLOCK_MONOTONIC, &start);

            if (mode == HASH_MODE || mode == DYNAMIC_MODE) {

                // ---- UUID SEARCH ----
                if (strchr(input, '-')) {
                    result = search_by_uuid(&uuid_ht, input);

                    if (!result) {
                        printf("Customer not found\n");
                        continue;
                    }

                    clock_gettime(CLOCK_MONOTONIC, &end);
                }

                // ---- NAME SEARCH ----
                else {
                    Node* results[100];
                    int count = search_all_by_name(&name_ht, input, results, 100);
                    clock_gettime(CLOCK_MONOTONIC, &end);

                    if (count == 0) {
                        printf("Customer not found\n");
                        continue;
                    }

                    printf("\nFound %d matches:\n", count);

                    for (int i = 0; i < count; i++) {
                        printf("%d. %s | UUID: %s | Orders: %d | Purchase: %.2f | Score: %.2f\n",
                            i + 1,
                            results[i]->name,
                            results[i]->uuid_str,
                            results[i]->total_orders,
                            results[i]->total_purchase,
                            results[i]->score);
                    }

                    int select;
                    printf("Select (1-%d): ", count);
                    scanf("%d", &select);

                    if (select < 1 || select > count) {
                        printf("Invalid selection\n");
                        continue;
                    }

                    result = results[select - 1];
                }
            }
            if (mode == BST_MODE) {

                // ---- UUID SEARCH ----
                if (strchr(input, '-')) {

                    bst_result = bst_search_uuid(bst_root, input);

                    if (!bst_result) {
                        printf("Customer not found\n");
                        continue;
                    }
                    clock_gettime(CLOCK_MONOTONIC, &end);
                }

                // ---- NAME SEARCH ----
                else {

                    BSTNode* results[100];
                    int count = 0;

                    bst_search_all_by_name(
                        bst_root,
                        input,
                        results,
                        &count,
                        100
                    );
                    clock_gettime(CLOCK_MONOTONIC, &end);

                    if (count == 0) {
                        printf("Customer not found\n");
                        continue;
                    }

                    printf("\nFound %d matches:\n", count);

                    for (int i = 0; i < count; i++) {
                        printf("%d. %s | UUID: %s | Orders: %d | Purchase: %.2f | Score: %.2f\n",
                            i + 1,
                            results[i]->name,
                            results[i]->uuid_str,
                            results[i]->total_orders,
                            results[i]->total_purchase,
                            results[i]->score);
                    }

                    int select;
                    printf("Select (1-%d): ", count);
                    scanf("%d", &select);

                    if (select < 1 || select > count) {
                        printf("Invalid selection\n");
                        continue;
                    }

                    bst_result = results[select - 1];
                }
            }

            // sync AVL result automatically in dynamic mode
            if (mode == DYNAMIC_MODE && result) {
                bst_result = bst_search_uuid(bst_root, result->uuid_str);

                if (!bst_result) {
                    printf("Customer not found in AVL\n");
                    continue;
                }
            }
            double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
            time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

            // ---- DISPLAY RESULT ----
            printf("\nSelected Customer:\n");

            if (mode == HASH_MODE || mode == DYNAMIC_MODE) {
                printf("Name: %s\n", result->name);
                printf("UUID: %s\n", result->uuid_str);
                printf("Orders: %d\n", result->total_orders);
                printf("Purchase: %.2f\n", result->total_purchase);
                printf("Score: %.2f\n", result->score);
            }
            if (mode == BST_MODE) {
                printf("Name: %s\n", bst_result->name);
                printf("UUID: %s\n", bst_result->uuid_str);
                printf("Orders: %d\n", bst_result->total_orders);
                printf("Purchase: %.2f\n", bst_result->total_purchase);
                printf("Score: %.2f\n", bst_result->score);
            }

            printf("\nSearching took %f seconds\n", time_taken);
        }

        // ================= RANKING =================
        else if (choice == 2) {
            if (mode == HASH_MODE || mode == DYNAMIC_MODE) {

                print_ranking(&name_ht);
            }
            else if (mode == BST_MODE) {
                bst_print_ranking(bst_root);
            }
        }

        // ================= ADD =================
        else if (choice == 3) {
            char name[100];
            int orders;
            float purchase;

            printf("Name: ");
            scanf("%s", name);

            printf("Orders: ");
            scanf("%d", &orders);

            printf("Purchase: ");
            scanf("%f", &purchase);

            // generate UUID
            char uuid_str[50];
            generate_uuid2(uuid_str);

            if (mode == HASH_MODE || mode == DYNAMIC_MODE) {
                insert(&name_ht, name, name, uuid_str, orders, purchase);
                insert(&uuid_ht, uuid_str, name, uuid_str, orders, purchase);
            }

            if (mode == BST_MODE || mode == DYNAMIC_MODE) {
                bst_root = bst_insert(bst_root,
                                      name,
                                      uuid_str,
                                      orders,
                                      purchase);
            }


            clock_gettime(CLOCK_MONOTONIC, &start);

            if (mode == HASH_MODE || mode == DYNAMIC_MODE) {
                save_to_csv("data.csv", &name_ht);
            }
            else if (mode == BST_MODE) {
                bst_save_to_csv("data.csv", bst_root);
            }

            clock_gettime(CLOCK_MONOTONIC, &end);

            double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
            time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

            printf("\nSaving took %f seconds\n", time_taken);

            printf("Added successfully!\n");
        }

        // ================= DELETE =================
        else if (choice == 4) {
            char input[100];
            printf("Delete (name or uuid): ");
            scanf("%s", input);

            input[strcspn(input, "\r\n")] = 0;

            Node* target = NULL;
            BSTNode* bst_target = NULL;

            if (mode == HASH_MODE || mode == DYNAMIC_MODE) {

                // ---- UUID ----
                if (strchr(input, '-')) {
                    target = search_by_uuid(&uuid_ht, input);

                    if (!target) {
                        printf("Customer not found\n");
                        continue;
                    }
                }

                // ---- NAME (MULTIPLE) ----
                else {
                    Node* results[100];
                    int count = search_all_by_name(&name_ht, input, results, 100);

                    if (count == 0) {
                        printf("Customer not found\n");
                        continue;
                    }

                    printf("\nFound %d matches:\n", count);

                    for (int i = 0; i < count; i++) {
                        printf("%d. %s | UUID: %s | Orders: %d | Purchase: %.2f\n",
                            i + 1,
                            results[i]->name,
                            results[i]->uuid_str,
                            results[i]->total_orders,
                            results[i]->total_purchase);
                    }

                    int select;
                    printf("Select (1-%d): ", count);
                    scanf("%d", &select);

                    if (select < 1 || select > count) {
                        printf("Invalid selection\n");
                        continue;
                    }

                    target = results[select - 1];
                }
            }

            if (mode == BST_MODE) {

                // ---- UUID ----
                if (strchr(input, '-')) {
                    bst_target = bst_search_uuid(bst_root, input);

                    if (!bst_target) {
                        printf("Customer not found\n");
                        continue;
                    }
                }

                // ---- NAME SEARCH ----
                else {
                    BSTNode* results[100];
                    int count = 0;

                    bst_search_all_by_name(
                        bst_root,
                        input,
                        results,
                        &count,
                        100
                    );

                    if (count == 0) {
                        printf("Customer not found\n");
                        continue;
                    }

                    printf("\nFound %d matches:\n", count);

                    for (int i = 0; i < count; i++) {
                        printf("%d. %s | UUID: %s | Orders: %d | Purchase: %.2f\n",
                            i + 1,
                            results[i]->name,
                            results[i]->uuid_str,
                            results[i]->total_orders,
                            results[i]->total_purchase);
                    }

                    int select;
                    printf("Select (1-%d): ", count);
                    scanf("%d", &select);

                    if (select < 1 || select > count) {
                        printf("Invalid selection\n");
                        continue;
                    }

                    bst_target = results[select - 1];
                }
            }

            if (mode == DYNAMIC_MODE && target) {

                bst_target = bst_search_uuid(

                    bst_root,

                    target->uuid_str

                );

                if (!bst_target) {

                    printf("Customer not found in AVL\n");

                    continue;

                }

            }

            clock_gettime(CLOCK_MONOTONIC, &start);

            if (mode == HASH_MODE || mode == DYNAMIC_MODE) {

                delete_by_name_and_uuid(
                    &name_ht,
                    target->name,
                    target->uuid_str
                );

                delete_by_uuid(
                    &uuid_ht,
                    target->uuid_str
                );
            }

            if ((mode == BST_MODE || mode == DYNAMIC_MODE) && bst_target) {

                bst_root = bst_delete(bst_root,bst_target->uuid_str);
            }

            if (mode == HASH_MODE || mode == DYNAMIC_MODE) {
                save_to_csv("data.csv", &name_ht);
            }
            else if (mode == BST_MODE) {
                bst_save_to_csv("data.csv", bst_root);
            }

            clock_gettime(CLOCK_MONOTONIC, &end);

            double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
            time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

            printf("\nDeleting took %f seconds\n", time_taken);

            printf("Deleted successfully\n");
        }

        // ================= EDIT =================
        else if (choice == 5) {
            char input[100];
            printf("Edit (name or uuid): ");
            scanf("%s", input);

            input[strcspn(input, "\r\n")] = 0;

            Node* target = NULL;
            BSTNode* bst_target = NULL;

            if (mode == HASH_MODE || mode == DYNAMIC_MODE) {

                // ---- UUID ----
                if (strchr(input, '-')) {

                    target = search_by_uuid(&uuid_ht, input);

                    if (!target) {
                        printf("Customer not found\n");
                        continue;
                    }
                }

                // ---- NAME (MULTIPLE) ----
                else {
                    Node* results[100];
                    int count = search_all_by_name(&name_ht, input, results, 100);

                    if (count == 0) {
                        printf("Customer not found\n");
                        continue;
                    }

                    printf("\nFound %d matches:\n", count);

                    for (int i = 0; i < count; i++) {
                        printf("%d. %s | UUID: %s | Orders: %d | Purchase: %.2f\n",
                            i + 1,
                            results[i]->name,
                            results[i]->uuid_str,
                            results[i]->total_orders,
                            results[i]->total_purchase);
                    }

                    int select;
                    printf("Select (1-%d): ", count);
                    scanf("%d", &select);

                    if (select < 1 || select > count) {
                        printf("Invalid selection\n");
                        continue;
                    }

                    target = results[select - 1];
                }
            }

            if (mode == BST_MODE) {

                // ---- UUID ----
                if (strchr(input, '-')) {

                    bst_target = bst_search_uuid(bst_root, input);

                    if (!bst_target) {
                        printf("Customer not found\n");
                        continue;
                    }
                }

                // ---- NAME SEARCH ----
                else {
                    BSTNode* results[100];
                    int count = 0;

                    bst_search_all_by_name(
                        bst_root,
                        input,
                        results,
                        &count,
                        100
                    );

                    if (count == 0) {
                        printf("Customer not found\n");
                        continue;
                    }

                    printf("\nFound %d matches:\n", count);

                    for (int i = 0; i < count; i++) {
                        printf("%d. %s | UUID: %s | Orders: %d | Purchase: %.2f\n",
                            i + 1,
                            results[i]->name,
                            results[i]->uuid_str,
                            results[i]->total_orders,
                            results[i]->total_purchase);
                    }

                    int select;
                    printf("Select (1-%d): ", count);
                    scanf("%d", &select);

                    if (select < 1 || select > count) {
                        printf("Invalid selection\n");
                        continue;
                    }

                    bst_target = results[select - 1];
                }
            }

           
            if (mode == DYNAMIC_MODE && target) {
                bst_target = bst_search_uuid(bst_root, target->uuid_str);

                if (!bst_target) {
                    printf("Customer not found in AVL\n");
                    continue;
                }
            }

            // ---- UPDATE VALUES ----
            int new_orders;
            float new_purchase;

            printf("New orders: ");
            scanf("%d", &new_orders);

            printf("New purchase: ");
            scanf("%f", &new_purchase);

            clock_gettime(CLOCK_MONOTONIC, &start);

            if (mode == HASH_MODE || mode == DYNAMIC_MODE) {

                update_customer(&name_ht, &uuid_ht,
                                target->name,
                                target->uuid_str,
                                new_orders,
                                new_purchase);
            }

            if (mode == BST_MODE || mode == DYNAMIC_MODE) {

                bst_update_customer(
                    bst_root,
                    bst_target->uuid_str,
                    new_orders,
                    new_purchase
                );
            }

            if (mode == HASH_MODE || mode == DYNAMIC_MODE) {
                save_to_csv("data.csv", &name_ht);
            }
            else if (mode == BST_MODE) {
                bst_save_to_csv("data.csv", bst_root);
            }

            clock_gettime(CLOCK_MONOTONIC, &end);

            double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
            time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

            printf("\nUpdating took %f seconds\n", time_taken);

            printf("Updated successfully\n");
        }

        else if (choice == 6)
        {
            int dataset_size;

            printf("Dataset size: ");
            scanf("%d", &dataset_size);

            if (dataset_size <= 0)
            {
                return printf("Invalid dataset size\n");
            }
            else if (dataset_size >1000000)
            {
                printf("Experimental dataset size may cause performance\n");
            }
            

            generate_dataset(dataset_size);

            if (mode == HASH_MODE || mode == DYNAMIC_MODE) {
                free_hashtable(&name_ht);
                free_hashtable(&uuid_ht);

                memset(&name_ht, 0, sizeof(HashTable));
                memset(&uuid_ht, 0, sizeof(HashTable));
            }

            if (mode == BST_MODE || mode == DYNAMIC_MODE) {
                bst_free(bst_root);
                bst_root = NULL;
            }

            clock_gettime(CLOCK_MONOTONIC, &start);

            if (mode == HASH_MODE) {
                read_csv("data.csv", &name_ht, &uuid_ht);
            }
            else if (mode == BST_MODE) {
                bst_root = bst_read_csv("data.csv", NULL);
            }
            else if (mode == DYNAMIC_MODE) {
                read_csv("data.csv", &name_ht, &uuid_ht);
                bst_root = bst_read_csv("data.csv", NULL);
            }
            else {
                printf("Invalid mode\n");
                return 1;
            }

            clock_gettime(CLOCK_MONOTONIC, &end);

            double reload_time = (end.tv_sec - start.tv_sec) * 1e9;
            reload_time = (reload_time + (end.tv_nsec - start.tv_nsec)) * 1e-9;

            printf("\nReloading dataset took %f seconds\n", reload_time);
        }


        else {
            printf("Invalid choice\n");
        }


    }

    if (mode == HASH_MODE || mode == DYNAMIC_MODE) {
        free_hashtable(&name_ht);
        free_hashtable(&uuid_ht);
    }

    if (mode == BST_MODE || mode == DYNAMIC_MODE) {
        bst_free(bst_root);
    }

    return 0;
}