// CODE BY-
// SARTHAK DEV RATHOR
#include <stdio.h>   // Include standard input-output library
#include <stdlib.h>  // Include standard library for memory allocation
#include <stdbool.h> // Include standard library for boolean type

/* A node is created */
struct node
{
    int key;                 // Value of the node
    int depth;               // Depth of the node in the tree
    int mindepth;            // Minimum depth of the subtree rooted at this node
    bool isRoot;             // Indicates if the edge to its parent is dashed
    struct node *leftChild;  // Pointer to the left child of the node
    struct node *rightChild; // Pointer to the right child of the node
    struct node *parent;     // Pointer to the parent of the node
};

// Function to find the minimum of 2 numbers
int min(int a, int b)
{
    return (a < b) ? a : b; // Return the smaller of the two numbers
}

// Global variable to store the root of the tree
struct node *root;

// Function to display all the information about the nodes in the tree
void display(struct node *root)
{
    if (root == NULL)
        return; // Base case: if the node is NULL, return

    // Display the value and root status of the node
    printf("Value: %d\nisRoot: %s\n", root->key, root->isRoot ? "True" : "False");
    if (root->parent == NULL)
    {
        printf("Parent: NULL\n\n"); // Display NULL if the node has no parent
    }
    else
    {
        printf("Parent: %d\n\n", root->parent->key); // Display the value of the parent node
    }

    if (root->leftChild)
    {
        display(root->leftChild); // Recursively display the left child
    }
    if (root->rightChild)
    {
        display(root->rightChild); // Recursively display the right child
    }
}

// Function to create a new node
struct node *createNode(int key, int depth, int minDepth, bool isRoot)
{
    struct node *newNode = (struct node *)malloc(sizeof(struct node)); // Allocate memory for the new node
    newNode->key = key;                                                // Set the value of the node
    newNode->depth = depth;                                            // Set the depth of the node
    newNode->mindepth = minDepth;                                      // Set the minimum depth of the subtree
    newNode->isRoot = isRoot;                                          // Set the root status
    newNode->leftChild = NULL;                                         // Initialize left child to NULL
    newNode->rightChild = NULL;                                        // Initialize right child to NULL
    newNode->parent = NULL;                                            // Initialize parent to NULL
    return newNode;                                                    // Return the newly created node
}

// Function to build the tree via recursion
struct node *buildTree(int start, int end, int depth, bool isRoot)
{
    if (start == end)
        return NULL; // Base case: if start equals end, return NULL

    int mid = (start + end) / 2;                                 // Find the middle index
    struct node *temp = createNode(mid, depth, depth, isRoot);   // Create a new node with the middle index as key
    temp->leftChild = buildTree(start, mid, depth + 1, false);   // Recursively build the left subtree
    temp->rightChild = buildTree(mid + 1, end, depth + 1, true); // Recursively build the right subtree

    if (temp->leftChild)
        temp->leftChild->parent = temp; // Set parent for the left child
    if (temp->rightChild)
        temp->rightChild->parent = temp; // Set parent for the right child

    return temp; // Return the created node
}

// Function to perform rotations in the tree
void rotate(struct node *ptr)
{
    struct node *parent = ptr->parent; // Get the parent of the node
    if (parent->isRoot)
    {                           // If the parent is a root node
        parent->isRoot = false; // Set parent's isRoot to false
        ptr->isRoot = true;     // Set current node's isRoot to true
    }
    if (root == parent)
    {
        root = ptr; // If the parent is the root, set current node as the new root
    }
    if (parent->parent)
    {
        if (parent->parent->leftChild == parent)
        {
            parent->parent->leftChild = ptr; // Update parent's parent's left child
        }
        else
        {
            parent->parent->rightChild = ptr; // Update parent's parent's right child
        }
    }
    ptr->parent = parent->parent; // Update current node's parent

    if (parent->rightChild == ptr)
    {
        parent->rightChild = ptr->leftChild; // Update parent's right child
        if (ptr->leftChild)
        {
            ptr->leftChild->parent = parent; // Update left child's parent
        }
        ptr->leftChild = parent; // Set parent's left child as current node's left child
    }
    else
    {
        parent->leftChild = ptr->rightChild; // Update parent's left child
        if (ptr->rightChild)
        {
            ptr->rightChild->parent = parent; // Update right child's parent
        }
        ptr->rightChild = parent; // Set parent's right child as current node's right child
    }
    parent->parent = ptr; // Update parent's parent

    if (parent->leftChild)
    {
        parent->mindepth = min(parent->mindepth, parent->leftChild->mindepth); // Update minimum depth
    }
    if (parent->rightChild)
    {
        parent->mindepth = min(parent->mindepth, parent->rightChild->mindepth); // Update minimum depth
    }
}

// Function to perform splay operation
void splay(struct node *ptr, struct node *top)
{
    struct node *parent;
    struct node *grandparent;
    while (!(ptr->isRoot || ptr->parent == top))
    {
        parent = ptr->parent;
        if (!(parent->isRoot || parent->parent == top))
        {
            grandparent = parent->parent;
            if ((grandparent->leftChild == parent && parent->leftChild == ptr) ||
                (grandparent->rightChild == parent && parent->rightChild == ptr))
            {
                rotate(parent); // Rotate parent if both parent and current node are on the same side of grandparent
            }
            else
            {
                rotate(ptr); // Rotate current node otherwise
            }
        }
        rotate(ptr); // Rotate current node
    }
}

// Function to find the reference parent
struct node *refParent(struct node *ptr, bool flag)
{
    struct node *temp = flag ? ptr->rightChild : ptr->leftChild;
    while (temp)
    {
        if (flag)
        {
            if (temp->leftChild && temp->leftChild->mindepth < ptr->depth)
            {
                temp = temp->leftChild; // Move to the left child if its mindepth is less than current node's depth
            }
            else if (temp->rightChild && temp->rightChild->mindepth < ptr->depth)
            {
                temp = temp->rightChild; // Move to the right child if its mindepth is less than current node's depth
            }
            else
            {
                break; // Break if no child has mindepth less than current node's depth
            }
        }
        else
        {
            if (temp->rightChild && temp->rightChild->mindepth < ptr->depth)
            {
                temp = temp->rightChild; // Move to the right child if its mindepth is less than current node's depth
            }
            else if (temp->leftChild && temp->leftChild->mindepth < ptr->depth)
            {
                temp = temp->leftChild; // Move to the left child if its mindepth is less than current node's depth
            }
            else
            {
                break; // Break if no child has mindepth less than current node's depth
            }
        }
    }
    return temp; // Return the reference parent node
}

// Function to switch path
void switchPath(struct node *ptr)
{
    struct node *temp;
    if (ptr->leftChild)
    {
        if (ptr->leftChild->mindepth > ptr->depth)
        {
            ptr->leftChild->isRoot = !ptr->leftChild->isRoot; // Toggle isRoot if left child's mindepth is greater
        }
        else
        {
            temp = refParent(ptr, false); // Get the reference parent for left child
            splay(temp, ptr);             // Splay the reference parent
            if (temp->rightChild)
            {
                temp->rightChild->isRoot = !temp->rightChild->isRoot; // Toggle isRoot for right child
            }
        }
    }
    if (ptr->rightChild)
    {
        if (ptr->rightChild->mindepth > ptr->depth)
        {
            ptr->rightChild->isRoot = !ptr->rightChild->isRoot; // Toggle isRoot if right child's mindepth is greater
        }
        else
        {
            temp = refParent(ptr, true); // Get the reference parent for right child
            splay(temp, ptr);            // Splay the reference parent
            if (temp->leftChild)
            {
                temp->leftChild->isRoot = !temp->leftChild->isRoot; // Toggle isRoot for left child
            }
        }
    }
}

// Multi-splay algorithm function
void multiSplayAlgo(struct node *ptr)
{
    struct node *temp = ptr;
    while (temp->parent)
    {
        struct node *parent = temp->parent;
        if (temp->isRoot)
        {
            splay(parent, NULL); // Splay the parent if current node is root
            switchPath(parent);  // Switch path for the parent
        }
        temp = parent; // Move up to the parent node
    }
    splay(ptr, NULL); // Splay the current node
}

// Function to search a key in the tree
void search(int key)
{
    struct node *ptr = root;
    struct node *prev = root;

    while (ptr && ptr->key != key)
    {
        prev = ptr;
        ptr = (key < ptr->key) ? ptr->leftChild : ptr->rightChild; // Move left or right based on key comparison
    }
    if (!ptr)
    {
        multiSplayAlgo(prev); // Perform multi-splay on the previous node if key is not found
    }
    else
    {
        multiSplayAlgo(ptr); // Perform multi-splay on the found node
    }
}

int main()
{
    int n;
    printf("Enter the number of nodes you want in the tree: ");
    scanf("%d", &n); // Input the number of nodes for the tree

    printf("\nMultiSplayTree is Created !!!!\n\n");
    root = buildTree(1, n + 1, 0, true); // Build the tree with specified number of nodes

    int choice = 0;
    while (choice != 3)
    {
        printf("1 -> Display the Tree\n2 -> Search an Element\n3 -> Exit the Code\n\nEnter Your Choice: ");
        scanf("%d", &choice); // Input user's choice
        switch (choice)
        {
        case 1:
            display(root); // Display the tree
            break;
        case 2:
            printf("Enter element to Search: \n");
            int element;
            scanf("%d", &element); // Input the element to search
            search(element);       // Search the element in the tree
            break;
        case 3:
            printf("Exiting...\n"); // Exit the program
            break;
        default:
            printf("Enter a valid Number Choice\n"); // Prompt for a valid choice
            break;
        }
    }
    return 0; // Return 0 to indicate successful execution
}
