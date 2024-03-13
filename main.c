#include <stdio.h>
#include <stdlib.h>

typedef struct person {
    int ID;
    char fname[26];
    char surname[26];
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
        free(new_node); //there cannot be duplicit values
        return root;
    }
    int leftH;
    int rightH;
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

void create(NODE **root, int ID, char *fname, char *surname, char *dateOfBirth) {
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

void search(NODE *root, int ID, int ID2, int *was_printed) {
    if (ID2 != -1) {
        if (root == NULL) {
            return;
        }
        if (root->ID > ID) {
            search(root->left, ID, ID2, was_printed);
        }
        if (ID <= root->ID && root->ID <= ID2) {
            if (*was_printed == 1) {
                printf("%d %s %s %s", root->ID, root->fname, root->surname, root->dateOfBirth);
                *was_printed = 0;
            } else {
                printf("\n%d %s %s %s", root->ID, root->fname, root->surname, root->dateOfBirth);
            }
        }
        if (root->ID < ID2) {
            search(root->right, ID, ID2, was_printed);
        }
        if (root->ID > ID2) {
            return;
        }
    } else {
        if (root == NULL) {
            return;
        }
        if (ID == root->ID) {
            if (*was_printed == 1) {
                printf("%d %s %s %s", root->ID, root->fname, root->surname, root->dateOfBirth);
                *was_printed = 0;
            } else {
                printf("\n%d %s %s %s", root->ID, root->fname, root->surname, root->dateOfBirth);
            }
            return;
        }
        if (root->ID > ID) {
            search(root->left, ID, ID2, was_printed);
        } else {
            search(root->right, ID, ID2, was_printed);
        }
    }
}

NODE *rightMost(NODE *node) {
    if (node == NULL)
        return NULL;
    else if (node->right == NULL)
        return node;
    else
        return rightMost(node->right);
}

NODE *delete(NODE *root, int ID) {

    if (root == NULL) {
        return NULL;
    } else if (ID < root->ID) {
        root->left = delete(root->left, ID);  //searching for the value we want to delete
    } else if (ID > root->ID) {
        root->right = delete(root->right, ID);
    } else {
        NODE *node = NULL;
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        } else if (root->right == NULL) {
            node = root;
            root = root->left;
            free(node);
        } else if (root->left == NULL) {
            node = root;
            root = root->right;
            free(node);
        } else {
            //dolava doprava aj nezajdem na null
            if (rightMost(root->left) == NULL) {
                node = root;
                root = root->left;
                free(node);
            } else {
                node = rightMost(root->left);
                root->ID = node->ID;
                toString(root->fname, node->fname);
                toString(root->surname, node->surname);
                toString(root->dateOfBirth, node->dateOfBirth);
                root->left = delete(root->left, node->ID);
            }
        }
    }
    if (root == NULL) {
        return root;
    }
    int leftH;
    int rightH;
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
    if (balance > 1 && height(root->left->left) >= height(root->left->right))
        return rightrotation(root);

    if (balance > 1 && height(root->left->left) < height(root->left->right)) {
        root->left = leftrotation(root->left);
        return rightrotation(root);
    }

    if (balance < -1 && height(root->right->left) <= height(root->right->right))
        return leftrotation(root);

    if (balance < -1 && height(root->right->left) > height(root->right->right)) {
        root->right = rightrotation(root->right);
        return leftrotation(root);
    }

    return root;
}

int main() {
    char input;
    int ID;
    int ID2 = -1;
    char fname[26];
    char surname[26];
    char dateOfBirth[12];
    int was_printed = 1;
    NODE *root = NULL;
    while (scanf(" %c", &input) == 1) {
        switch (input) {
            case 's':
                scanf(" %d", &ID);
                if (getchar() == ' ') {
                    scanf("%d", &ID2); //pozriet ci ma byt medzera
                    search(root, ID, ID2, &was_printed);
                } else {
                    ID2 = -1;
                    search(root, ID, ID2, &was_printed);
                }
                break;
            case 'd':
                scanf(" %d", &ID);
                if (root != NULL)
                    root = delete(root, ID);
                break;
            case 'i':
                scanf(" %d %s %s %s", &ID, fname, surname, dateOfBirth);
                create(&root, ID, fname, surname, dateOfBirth);
                break;
            default:
                break;
        }
    }
    freeAll(root);
    return 0;
}