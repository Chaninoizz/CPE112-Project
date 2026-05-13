#include "ranking.h"
#include <stdio.h>
#include <stdlib.h>

// ================= SPLIT =================
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

// ================= ITERATIVE MERGE =================
Node* merge(Node* a, Node* b) {

    Node dummy;
    Node* tail = &dummy;

    dummy.next = NULL;

    while (a && b) {

        if (a->score >= b->score) {
            tail->next = a;
            a = a->next;
        }
        else {
            tail->next = b;
            b = b->next;
        }

        tail = tail->next;
    }

    if (a)
        tail->next = a;
    else
        tail->next = b;

    return dummy.next;
}

// ================= MERGE SORT =================
Node* iterative_merge_sort(Node* head) {

    if (!head || !head->next)
        return head;

    Node* mid = split(head);

    Node* left = iterative_merge_sort(head);
    Node* right = iterative_merge_sort(mid);

    return merge(left, right);
}

// ================= PRINT TOP 100 =================
void print_top_100(Node* head) {

    int rank = 1;

    while (head && rank <= 100) {

        printf(
            "%-3d %-20s %-36s Orders:%-3d Purchase:%-10.2f Score:%.2f\n",
            rank++,
            head->name,
            head->uuid_str,
            head->total_orders,
            head->total_purchase,
            head->score
        );

        Node* temp = head;
        head = head->next;
        free(temp);
    }

    // free remaining nodes
    while (head) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}