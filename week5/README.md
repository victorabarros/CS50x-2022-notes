# Week 5 - Data Structure

## linked list

each node with its value and a pointer to the next node. easier to manage the memory than the array, especially in updating any node.

Complexity
search -> O(n)
insert -> O(n)

```c
typedef struct node
{
    int number;
    struct node *next;
}
node;
```

## binary search tree

each node with its value and two pointers to each child. the left must have a value lower than itself and the right must be bigger.

Complexity
search -> O(log n)
insert -> O(log n)

```c
typedef struct node
{
    int number;
    struct node *left;
    struct node *right;
}
node;
```

## hash table

it is an index of linked lists. which means the first element of the list has O(1) to find, but the followings has the same as a linked list. usualy use a hash function that return the index que the table where the element parameted should be.

Complexity
search -> O(n)
insert -> O(n)

## tries

```c
typedef struct node
{
    bool is_word;
    struct node *children[SIZE_OF_ALPHABET];
}
node;
```

Complexity
O(1)
