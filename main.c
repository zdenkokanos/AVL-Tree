#include <stdio.h>
#include <stdlib.h>

typedef struct person {
    long ID;
    char fname[100];
    char surname[100];
    char dateOfBirth[12];
    int height;
    struct person *left;
    struct person *right;
} NODE;

int height(NODE *node) {            //this function is mostly due to checking if the pointer is not NULL
    if (node == NULL) {
        return 0;
    }
    return (node->height);
}

int max(int num1, int num2) {
    if (num1 > num2) {
        return num1;
    } else {
        return num2;
    }
}

void freeAll(NODE *root) {
    if (root == NULL) {
        return;
    }
    freeAll(root->left);
    freeAll(root->right);
    free(root);
}

void toString(char *N_string, char *string) {
    int i;
    for (i = 0; string[i] != '\0'; i++) {
        N_string[i] = string[i];
    }
    N_string[i] = '\0';
}

NODE *leftrotation(NODE *A) {
    NODE *B = A->right;
    NODE *temp = B->left;
    B->left = A;
    A->right = temp;
    A->height = max(height(A->left), height(A->right)) + 1;
    B->height = max(height(B->left), height(B->right)) + 1;
    return B;
}

NODE *rightrotation(NODE *A) {
    NODE *B = A->left;
    NODE *temp = B->right;
    B->right = A;
    A->left = temp;
    A->height = max(height(A->left), height(A->right)) + 1;
    B->height = max(height(B->left), height(B->right)) + 1;
    return B;
}

NODE *insert(NODE *root, NODE *new_node) {
    if (root == NULL) {
        return new_node;
    }
    if (root->ID > new_node->ID) {
        root->left = insert(root->left, new_node);               //basic BST insertion //lower to left //higher to right
    } else if (root->ID < new_node->ID) {
        root->right = insert(root->right, new_node);
    } else {
        return root; //there cannot be duplicit values
    }
    int leftH = 0;
    int rightH = 0;
    if (root->left != NULL) {
        leftH = root->left->height;
    } else {
        leftH = 0;                 //if the parent does not have a child assign -1, because we add + 1
        //                           // when assigning height to parent, so the final height is 0
    }
    if (root->right != NULL) {
        rightH = root->right->height;
    } else {
        rightH = 0;
    }

    root->height = max(leftH, rightH) + 1;          //assigning height to parent

    int balance = height(root->left) - height(root->right);
    if (balance > 1) {
        if (new_node->ID < root->left->ID) {  //the child is on the left ready to perform
            return rightrotation(root);
        } else {                                        //it means that the child is on the right
            root->left = leftrotation(root->left);
            return rightrotation(root);             //now it is in the right place to perform RR
        }
    } else if (balance < -1) {
        if (new_node->ID > root->right->ID) { //the child is on the right ready to perform
            return leftrotation(root);
        } else {                        //it means the child is on the left
            root->right = rightrotation(root->right);
            return leftrotation(root);     //now it is in the right place to perform LR
        }
    }
    return root;
}

void create(NODE **root, long ID, char *fname, char *surname, char *dateOfBirth) {
    NODE *new_node = (NODE *) malloc(sizeof(NODE));
    new_node->ID = ID;
    toString(new_node->fname, fname);           //implementation of own function to assign string
    toString(new_node->surname, surname);
    toString(new_node->dateOfBirth, dateOfBirth);
    new_node->height = 0;   //leaf and root has always height 0 at start
    new_node->left = NULL;
    new_node->right = NULL;
    if ((*root) == NULL) {
        *root = new_node;
    } else {
        *root = insert(*root, new_node); //perform binary search tree insertion
    }
}

void search(NODE *root, long ID, long ID2) {
    if (ID2 != -1) {
        if (root == NULL) {
            return;
        }
        if (ID <= root->ID && root->ID <= ID2) {
            printf("%ld %s %s %s\n", root->ID, root->fname, root->surname, root->dateOfBirth);
        }
        if (root->ID <= ID2) {
            search(root->right, ID, ID2);
        }
        if (root->ID >= ID) {
            search(root->left, ID, ID2);
        }

    } else {
        if (root == NULL) {
            return;
        }
        if (ID == root->ID) {
            printf("%ld %s %s %s\n", root->ID, root->fname, root->surname, root->dateOfBirth);
            return;
        }
        if (root->ID < ID) {
            search(root->left, ID, ID2);
        } else {
            search(root->right, ID, ID2);
        }

    }
}

void delete(NODE *root, long ID) {

}

int main() {
    char input;
    long ID;
    long ID2 = -1;
    char fname[100];
    char surname[100];
    char dateOfBirth[12];
    NODE *root = NULL;
    while (scanf(" %c", &input) == 1) {
        switch (input) {
            case 's':
                scanf(" %ld", &ID);
                if (getchar() == ' ') {
                    scanf(" %ld", &ID2);
                    search(root, ID, ID2);
                } else {
                    ID2 = -1;
                    search(root, ID, ID2);
                }
                break;
            case 'd':
                scanf(" %ld", &ID);
                delete(root, ID);
                break;
            case 'i':
                scanf(" %ld %s %s %s", &ID, fname, surname, dateOfBirth);
                create(&root, ID, fname, surname, dateOfBirth);
                break;
            default:
                break;
        }
    }
    freeAll(root);
    return 0;
}
