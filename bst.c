

#include "bst.h"
#include "ranking.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

// ================= CREATE NODE =================
BSTNode* bst_create_node(
    const char* name,
    const char* uuid,
    int orders,
    float purchase
) {

    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));

    strcpy(node->name, name);
    strcpy(node->uuid_str, uuid);

    node->total_orders = orders;
    node->total_purchase = purchase;

    node->score = (orders * 1.5) + (purchase * 0.01);

    node->height = 1;

    node->left = NULL;
    node->right = NULL;

    return node;
}

// ================= HEIGHT =================
int bst_height(BSTNode* node) {

    if (!node)
        return 0;

    return node->height;
}

// ================= MAX =================
int bst_max(int a, int b) {

    return (a > b) ? a : b;
}

// ================= BALANCE =================
int bst_get_balance(BSTNode* node) {

    if (!node)
        return 0;

    return bst_height(node->left)
         - bst_height(node->right);
}

// ================= RIGHT ROTATE =================
BSTNode* bst_right_rotate(BSTNode* y) {

    BSTNode* x = y->left;
    BSTNode* t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = bst_max(
        bst_height(y->left),
        bst_height(y->right)
    ) + 1;

    x->height = bst_max(
        bst_height(x->left),
        bst_height(x->right)
    ) + 1;

    return x;
}

// ================= LEFT ROTATE =================
BSTNode* bst_left_rotate(BSTNode* x) {

    BSTNode* y = x->right;
    BSTNode* t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = bst_max(
        bst_height(x->left),
        bst_height(x->right)
    ) + 1;

    y->height = bst_max(
        bst_height(y->left),
        bst_height(y->right)
    ) + 1;

    return y;
}

// ================= INSERT =================
BSTNode* bst_insert(
    BSTNode* root,
    const char* name,
    const char* uuid,
    int orders,
    float purchase
) {

    if (!root)
        return bst_create_node(
            name,
            uuid,
            orders,
            purchase
        );

    if (strcmp(uuid, root->uuid_str) < 0) {

        root->left = bst_insert(
            root->left,
            name,
            uuid,
            orders,
            purchase
        );
    }

    else if (strcmp(uuid, root->uuid_str) > 0) {

        root->right = bst_insert(
            root->right,
            name,
            uuid,
            orders,
            purchase
        );
    }

    else {
        return root;
    }

    root->height = 1 + bst_max(
        bst_height(root->left),
        bst_height(root->right)
    );

    int balance = bst_get_balance(root);

    // LEFT LEFT
    if (
        balance > 1 &&
        strcmp(uuid, root->left->uuid_str) < 0
    ) {
        return bst_right_rotate(root);
    }

    // RIGHT RIGHT
    if (
        balance < -1 &&
        strcmp(uuid, root->right->uuid_str) > 0
    ) {
        return bst_left_rotate(root);
    }

    // LEFT RIGHT
    if (
        balance > 1 &&
        strcmp(uuid, root->left->uuid_str) > 0
    ) {

        root->left = bst_left_rotate(root->left);

        return bst_right_rotate(root);
    }

    // RIGHT LEFT
    if (
        balance < -1 &&
        strcmp(uuid, root->right->uuid_str) < 0
    ) {

        root->right = bst_right_rotate(root->right);

        return bst_left_rotate(root);
    }

    return root;
}

// ================= SEARCH UUID =================
BSTNode* bst_search_uuid(
    BSTNode* root,
    const char* uuid
) {

    if (!root)
        return NULL;

    int cmp = strcmp(uuid, root->uuid_str);

    if (cmp == 0)
        return root;

    if (cmp < 0)
        return bst_search_uuid(root->left, uuid);

    return bst_search_uuid(root->right, uuid);
}

// ================= SEARCH NAME =================
void bst_search_all_by_name(
    BSTNode* root,
    const char* name,
    BSTNode* results[],
    int* count,
    int max_results
) {

    if (!root || *count >= max_results)
        return;

    bst_search_all_by_name(
        root->left,
        name,
        results,
        count,
        max_results
    );

    if (strcmp(root->name, name) == 0) {

        results[*count] = root;
        (*count)++;
    }

    bst_search_all_by_name(
        root->right,
        name,
        results,
        count,
        max_results
    );
}

// ================= MIN NODE =================
BSTNode* bst_min_value_node(BSTNode* node) {

    BSTNode* current = node;

    while (current->left)
        current = current->left;

    return current;
}

// ================= DELETE =================
BSTNode* bst_delete(
    BSTNode* root,
    const char* uuid
) {

    if (!root)
        return root;

    if (strcmp(uuid, root->uuid_str) < 0) {

        root->left = bst_delete(
            root->left,
            uuid
        );
    }

    else if (strcmp(uuid, root->uuid_str) > 0) {

        root->right = bst_delete(
            root->right,
            uuid
        );
    }

    else {

        if (
            !root->left ||
            !root->right
        ) {

            BSTNode* temp;

            if (root->left)
                temp = root->left;
            else
                temp = root->right;

            if (!temp) {

                temp = root;
                root = NULL;
            }

            else {
                *root = *temp;
            }

            free(temp);
        }

        else {

            BSTNode* temp =
                bst_min_value_node(root->right);

            strcpy(root->name, temp->name);
            strcpy(root->uuid_str, temp->uuid_str);

            root->total_orders =
                temp->total_orders;

            root->total_purchase =
                temp->total_purchase;

            root->score =
                temp->score;

            root->right = bst_delete(
                root->right,
                temp->uuid_str
            );
        }
    }

    if (!root)
        return root;

    root->height = 1 + bst_max(
        bst_height(root->left),
        bst_height(root->right)
    );

    int balance = bst_get_balance(root);

    // LEFT LEFT
    if (
        balance > 1 &&
        bst_get_balance(root->left) >= 0
    ) {
        return bst_right_rotate(root);
    }

    // LEFT RIGHT
    if (
        balance > 1 &&
        bst_get_balance(root->left) < 0
    ) {

        root->left =
            bst_left_rotate(root->left);

        return bst_right_rotate(root);
    }

    // RIGHT RIGHT
    if (
        balance < -1 &&
        bst_get_balance(root->right) <= 0
    ) {
        return bst_left_rotate(root);
    }

    // RIGHT LEFT
    if (
        balance < -1 &&
        bst_get_balance(root->right) > 0
    ) {

        root->right =
            bst_right_rotate(root->right);

        return bst_left_rotate(root);
    }

    return root;
}

// ================= UPDATE =================
void bst_update_customer(
    BSTNode* root,
    const char* uuid,
    int new_orders,
    float new_purchase
) {

    BSTNode* node =
        bst_search_uuid(root, uuid);

    if (!node)
        return;

    node->total_orders = new_orders;

    node->total_purchase = new_purchase;

    node->score =
        (new_orders * 1.5)
        + (new_purchase * 0.01);
}

// ================= CSV =================
BSTNode* bst_read_csv(
    const char* filename,
    BSTNode* root
) {

    FILE* file = fopen(filename, "r");

    if (!file) {
        perror("Error opening CSV");
        return root;
    }

    char line[1024];

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {

        char name[MAX_NAME] = {0};
        char uuid[MAX_UUID_STR] = {0};

        int orders;
        float purchase;

        if (
            sscanf(
                line,
                " %255[^,],%36[^,],%d,%f",
                name,
                uuid,
                &orders,
                &purchase
            ) == 4
        ) {

            root = bst_insert(
                root,
                name,
                uuid,
                orders,
                purchase
            );
        }
    }

    fclose(file);

    return root;
}

// ================= SAVE CSV =================
void bst_save_csv_helper(
    FILE* file,
    BSTNode* root
) {

    if (!root)
        return;

    bst_save_csv_helper(file, root->left);

    fprintf(
        file,
        "%s,%s,%d,%.2f\n",
        root->name,
        root->uuid_str,
        root->total_orders,
        root->total_purchase
    );

    bst_save_csv_helper(file, root->right);
}

void bst_save_to_csv(
    const char* filename,
    BSTNode* root
) {

    FILE* file = fopen(filename, "w");

    if (!file) {
        perror("Error writing CSV");
        return;
    }

    fprintf(
        file,
        "name,uuid,orders,purchase\n"
    );

    bst_save_csv_helper(file, root);

    fclose(file);
}

// ================= RANKING =================

Node* create_rank_node(BSTNode* node) {

    Node* r = (Node*)malloc(sizeof(Node));

    strcpy(r->name, node->name);
    strcpy(r->uuid_str, node->uuid_str);

    r->total_orders = node->total_orders;
    r->total_purchase = node->total_purchase;

    r->score = node->score;

    r->next = NULL;

    return r;
}

void collect_bst_nodes(BSTNode* root, Node** head) {

    if (!root)
        return;

    collect_bst_nodes(root->left, head);

    Node* n = create_rank_node(root);
    n->next = *head;
    *head = n;

    collect_bst_nodes(root->right, head);
}


void bst_print_ranking(BSTNode* root) {

    Node* head = NULL;

    collect_bst_nodes(root, &head);

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    head = iterative_merge_sort(head);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;

    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

    print_top_100(head);

    printf("\nMerge Sort took %f seconds\n\n", time_taken);

}

// ================= INORDER =================
void bst_inorder(BSTNode* root) {

    if (!root)
        return;

    bst_inorder(root->left);

    printf(
        "%s | %s\n",
        root->name,
        root->uuid_str
    );

    bst_inorder(root->right);
}

// ================= FREE =================
void bst_free(BSTNode* root) {

    if (!root)
        return;

    bst_free(root->left);
    bst_free(root->right);

    free(root);
}