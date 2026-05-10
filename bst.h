#ifndef BST_H
#define BST_H

#define MAX_NAME 256
#define MAX_UUID_STR 37

// ================= BST NODE =================
typedef struct BSTNode {

    char name[MAX_NAME];
    char uuid_str[MAX_UUID_STR];

    int total_orders;
    float total_purchase;
    float score;

    int height;

    struct BSTNode* left;
    struct BSTNode* right;

} BSTNode;


// ================= CORE =================
BSTNode* bst_create_node(
    const char* name,
    const char* uuid,
    int orders,
    float purchase
);

int bst_height(BSTNode* node);

int bst_max(int a, int b);

int bst_get_balance(BSTNode* node);


// ================= ROTATIONS =================
BSTNode* bst_right_rotate(BSTNode* y);

BSTNode* bst_left_rotate(BSTNode* x);


// ================= INSERT =================
BSTNode* bst_insert(
    BSTNode* root,
    const char* name,
    const char* uuid,
    int orders,
    float purchase
);


// ================= SEARCH =================
BSTNode* bst_search_uuid(
    BSTNode* root,
    const char* uuid
);

void bst_search_all_by_name(
    BSTNode* root,
    const char* name,
    BSTNode* results[],
    int* count,
    int max_results
);


// ================= DELETE =================
BSTNode* bst_delete(
    BSTNode* root,
    const char* uuid
);

BSTNode* bst_min_value_node(BSTNode* node);


// ================= UPDATE =================
void bst_update_customer(
    BSTNode* root,
    const char* uuid,
    int new_orders,
    float new_purchase
);


// ================= CSV =================
BSTNode* bst_read_csv(
    const char* filename,
    BSTNode* root
);

void bst_save_to_csv(
    const char* filename,
    BSTNode* root
);


// ================= RANKING =================
void bst_print_ranking(
    BSTNode* root
);

// ================= TRAVERSAL =================
void bst_inorder(
    BSTNode* root
);

// ================= FREE =================
void bst_free(
    BSTNode* root
);

#endif