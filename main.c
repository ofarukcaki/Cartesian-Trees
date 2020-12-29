/* 
CSE4094 Advanced Data Structures Project #1
Using Cartesian Tree to Solve All Nearest Smaller Values Problem
1.12.2020 Marmara University

150315029 - Havva Karaçam
150116072 - Buğra Akdeniz
150117821 - Ömer Faruk Çakı


[usage]
gcc main.c -o Cartesian.exe
Cartesian.exe <inputfile name>

[example]
Cartesian.exe input.txt

[input format]
space seperated numbers

*/

#include <stdio.h>
#include <stdlib.h>

/*
    For every Node, maintain two variables,
        a. `Node.index` which is set to the Node’s index in the sequence corresponding
        to the in-order traversal of the Cartesian tree and never changed, and\
        
        b. `Node.inherited`, which is initialized to null.
*/
struct Node {
    int index; // index value corresponding to the in-order traversal
    int inherited;
    int inheritedRight; // similar to node.inherited but to be used for finding nearest right smaller
    int value;          // value of the Node
    struct Node *left;  // left child
    struct Node *right; // right child
};

// function to create and return a new Node
struct Node *newNode(int value, int index, int inherited, int inheritedRight) {
    // allocate memory for new node
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    // set value and index
    node->value = value;
    node->index = index + 1;    // index numbers are starting from 1 as we did on class
    // set left and right null
    node->left = NULL;
    node->right = NULL;
    // set inherited
    node->inherited = inherited;
    node->inheritedRight = inheritedRight;
    return node;
}

// // Inorder Traversal
// // to debug our tree
// void inorder(struct Node *root) {
//     if (root != NULL) {
//         // Traverse left
//         inorder(root->left);

//         // Traverse root
//         printf("%d -> ", root->value);
//         // printf("{[%d]%d  %d - %d - %d } -> ", root->index, root->value, root->index, root->inherited, root->inheritedRight);

//         // Traverse right
//         inorder(root->right);
//     }
// }

void NLS(int arr[], struct Node *root) {
    if (root != NULL) {
        // Traverse left
        NLS(arr, root->left);

        // Traverse root
        if (root->inherited == -1) {
            printf("-, ");
        } else {
            printf("%d, ", arr[root->inherited - 1]); // we substract 1 because indexes start from 1
        }

        // Traverse right
        NLS(arr, root->right);
    }
}

void NRS(int arr[], struct Node *root) {
    if (root != NULL) {
        // Traverse left
        NRS(arr, root->left);

        // Traverse root
        if (root->inheritedRight == -1) {
            printf("-, ");
        } else {
            printf("%d, ", arr[root->inheritedRight - 1]); // we substract 1 because indexes start from 1
        }

        // Traverse right
        NRS(arr, root->right);
    }
}

// Find the minimum element in the array and return it's index
int findMin(int arr[], int start, int end) {
    int min = arr[start], index = start;

    for (int i = start; i <= end; i++) {
        if (arr[i] < min) {
            min = arr[i];
            index = i;
        }
    }
    // printf("\nMin element is: %d\n", arr[index]);

    return index;
}

// Create cartesian tree recursively from given array and return the root
struct Node *createCartesianTree(int arr[], int start, int end, int inherited, int inheritedRight) {
    // stop case
    if (start > end)
        return NULL;

    // find the minimum element's index within given interval
    int minIndex = findMin(arr, start, end);

    // this is the minimum element in the array, hence it's the root
    struct Node *root = newNode(arr[minIndex], minIndex, inherited, inheritedRight);

    // cosntruct the left subtree recursively
    root->left = createCartesianTree(arr, start, minIndex - 1, root->inherited, root->index);

    // cosntruct the right subtree recursively
    root->right = createCartesianTree(arr, minIndex + 1, end, root->index, root->inheritedRight);

    return root;
}

int main(int argc, char *argv[]) {
    // 1st argument is the input file

    // file handler
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");

    // catch file errors
    if (fp == NULL) {
        printf("Error while opening the file\n");
        return -1;
    }

    // read all numbers on the file
    int arr[1000];
    int size;
    {
        int num, i = 0;
        while (fscanf(fp, "%d", &num) == 1) {
            arr[i] = num;
            i++;
        }
        arr[i] = '\0'; // end of the array
        size = i;      // at the end, i is the size of the array
    }

    // construct the tree
    // node.inherited and node.inheritedRight value for head node is -1, represents NULL
    struct Node *root = createCartesianTree(arr, 0, size - 1, -1, -1);

    printf("Nearest left smaller values:  ");
    NLS(arr, root);
    printf("\n");

    printf("Nearest right smaller values: ");
    NRS(arr, root);
    return 0;
}