// gcc main.c hashtable.c -o main
// ./main

#include <stdio.h>
#include <string.h>
#include <uuid/uuid.h>
#include "hashtable.h"

int main() {
    HashTable name_ht = {0};
    HashTable uuid_ht = {0};

    read_csv("data.csv", &name_ht, &uuid_ht);

    int choice;

    while (1) {
        printf("\n=== MENU ===\n");
        printf("1. Search (name or UUID)\n");
        printf("2. Ranking\n");
        printf("3. Add\n");
        printf("4. Delete\n");
        printf("5. Edit\n");
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

            // ---- UUID SEARCH ----
            if (strchr(input, '-')) {
                result = search_by_uuid(&uuid_ht, input);

                if (!result) {
                    printf("Customer not found\n");
                    continue;
                }
            }

            // ---- NAME SEARCH (MULTIPLE RESULTS) ----
            else {
                Node* results[100];
                int count = search_all_by_name(&name_ht, input, results, 100);

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

            // ---- DISPLAY RESULT ----
            printf("\nSelected Customer:\n");
            printf("Name: %s\n", result->name);
            printf("UUID: %s\n", result->uuid_str);
            printf("Orders: %d\n", result->total_orders);
            printf("Purchase: %.2f\n", result->total_purchase);
            printf("Score: %.2f\n", result->score);
        }

        // ================= RANKING =================
        else if (choice == 2) {
            print_ranking(&name_ht);
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
            uuid_t uuid;
            uuid_generate(uuid);
            char uuid_str[37];
            uuid_unparse_lower(uuid, uuid_str);

            // insert into both tables
            insert(&name_ht, name, name, uuid_str, orders, purchase);
            insert(&uuid_ht, uuid_str, name, uuid_str, orders, purchase);

            save_to_csv("data.csv", &name_ht);

            printf("Added successfully!\n");
        }

        // ================= DELETE =================
        else if (choice == 4) {
            char input[100];
            printf("Delete (name or uuid): ");
            scanf("%s", input);

            input[strcspn(input, "\r\n")] = 0;

            Node* target = NULL;

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

            // ---- DELETE FROM BOTH TABLES ----
            delete_by_name(&name_ht, target->name);
            delete_by_uuid(&uuid_ht, target->uuid_str);

            save_to_csv("data.csv", &name_ht);

            printf("Deleted successfully\n");
        }

        // ================= EDIT =================
        else if (choice == 5) {
            char input[100];
            printf("Edit (name or uuid): ");
            scanf("%s", input);

            input[strcspn(input, "\r\n")] = 0;

            Node* target = NULL;

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

            // ---- UPDATE VALUES ----
            int new_orders;
            float new_purchase;

            printf("New orders: ");
            scanf("%d", &new_orders);

            printf("New purchase: ");
            scanf("%f", &new_purchase);

            update_customer(&name_ht, &uuid_ht,
                            target->name,
                            target->uuid_str,
                            new_orders,
                            new_purchase);

            save_to_csv("data.csv", &name_ht);

            printf("Updated successfully\n");
        }


        else {
            printf("Invalid choice\n");
        }
    }

    free_hashtable(&name_ht);
    free_hashtable(&uuid_ht);

    return 0;
}