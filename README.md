
# CPE112 Project

Customer Management and Ranking System implemented using:

- Hash Table + Linked List
- AVL Tree (Self-balancing BST)
- Dynamic Hybrid Architecture

---

# Features

- Search by Name / UUID
- Add Customer
- Delete Customer
- Edit Customer
- Customer Ranking
- CSV File Storage
- Large Dataset Generator
- Benchmark / Performance Testing
- Duplicate Name Handling

---

# Data Structures

## Hash Table + Linked List

Used for:
- Fast customer lookup
- Duplicate name handling
- Average O(1) searching

Features:
- djb2 hashing
- Separate chaining collision handling
- Linked-list traversal

---

## AVL Tree (Self-balancing BST)

Used for:
- Ordered UUID indexing
- Stable performance on large datasets
- Guaranteed balanced operations

Complexities:

- Search: O(log n)
- Insert: O(log n)
- Delete: O(log n)

Features:
- Left Rotation
- Right Rotation
- Left-Right Rotation
- Right-Left Rotation
- Recursive balancing

---

## Dynamic Hybrid Architecture

Dynamic mode combines:
- Hash table indexing
- AVL tree synchronization

Behavior:
- Hash table handles fast search and duplicate-name selection
- AVL tree maintains balanced ordered storage
- UUID used for synchronization between structures

---

# Algorithms Implemented

## Hash Table

- djb2 Hash Function
- Chaining Collision Resolution
- Linked List Traversal

## AVL Tree

- Recursive Insertion
- Recursive Deletion
- Tree Rebalancing
- AVL Rotations

## Ranking System

- Merge Sort
- Linked List Collection
- Top-100 Ranking Display

---

# Benchmarking

The project benchmarks:
- CSV loading time
- Search time
- Ranking time
- Insert/Delete/Edit performance

Supports large datasets:
- 10K
- 100K
- 1M+ (experimental)

---

# Dataset Generator

The project includes a dataset generator capable of:
- Random customer generation
- Random pseudo-UUID generation
- Large CSV dataset creation

---

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
  gcc main.c bst.c ranking.c hashtable.c generate.c -o main
```

Windows (MinGW)

```bash
  gcc main.c bst.c ranking.c hashtable.c generate.c -o main.exe
```

# File Structure

```text
main.c
hashtable.c
hashtable.h
bst.c
bst.h
ranking.c
ranking.h
generate.c
README.md
```