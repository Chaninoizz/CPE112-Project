
# CPE112-Project


Customer Management and Ranking System using:

- Hash Table + Linked List

- AVL Tree (Self-balancing BST)

## Functions
- Search by UUID/Name

- Add Customer

- Delete Customer

- Edit Customer

- Customer Ranking

- CSV File Storage

- Benchmark / Performance Testing

# Data Structure

## Hash Table

- Fast average lookup: O(1)

- Uses djb2 hashing

- Collision handling with linked list chaining

- Efficient duplicate-name handling

## AVL Tree

- Self-balancing Binary Search Tree

- Guaranteed O(log n) operations

- Ordered UUID indexing

- Stable performance on large datasets

# Algorithms Implemented

## Hash Table

- djb2 Hash Function

- Chaining Collision Resolution

- Linked List Traversal

## AVL Tree

- AVL Rotations

  - Left Rotation

  - Right Rotation

  - Left-Right Rotation

  - Right-Left Rotation

- Recursive Insertion

- Recursive Deletion

- Tree Balancing

## Ranking System

- Merge Sort

- Linked List Collection


## Execution

macOS / Linux

```bash
  ./main
```

Windows (MinGW)

```bash
  main.exe
```
## Compliation

macOS / Linux

```bash
  gcc main.c hashtable.c bst.c -o main
```

Windows (MinGW)

```bash
  gcc main.c hashtable.c bst.c -o main.exe
```