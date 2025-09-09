#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100

typedef struct Node {
    char name[MAX_NAME_LEN];
    int isFile;
    struct Node *child;
    struct Node *sibling;
    struct Node *parent;
} Node;

Node* createNode(const char* name, int isFile, Node* parent) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->name, name);
    newNode->isFile = isFile;
    newNode->child = NULL;
    newNode->sibling = NULL;
    newNode->parent = parent;
    return newNode;
}

void insert(Node* parent, const char* name, int isFile) {
    Node* newNode = createNode(name, isFile, parent);
    if (!parent->child) {
        parent->child = newNode;
    } else {
        Node* temp = parent->child;
        while (temp->sibling)
            temp = temp->sibling;
        temp->sibling = newNode;
    }
}

Node* findChild(Node* parent, const char* name) {
    Node* temp = parent->child;
    while (temp) {
        if (!strcmp(temp->name, name) && temp->isFile == 0)
            return temp;
        temp = temp->sibling;
    }
    return NULL;
}

void printTree(Node* node, int depth) {
    for (int i = 0; i < depth; i++)
        printf("   ");
    if (node->isFile)
        printf("|-- %s\n", node->name);
    else {
        printf("|-- %s/\n", node->name);
        Node* temp = node->child;
        while (temp) {
            printTree(temp, depth + 1);
            temp = temp->sibling;
        }
    }
}

int main() {
    char command[100], name[MAX_NAME_LEN];
    Node* root = createNode("root", 0, NULL);
    Node* current = root;

    while (1) {
        printf(">> ");
        fgets(command, sizeof(command), stdin);

        if (strncmp(command, "mkdir ", 6) == 0) {
            sscanf(command + 6, "%s", name);
            insert(current, name, 0);
        } else if (strncmp(command, "create ", 7) == 0) {
            sscanf(command + 7, "%s", name);
            insert(current, name, 1);
        } else if (strncmp(command, "cd ", 3) == 0) {
            sscanf(command + 3, "%s", name);
            if (strcmp(name, "..") == 0 && current->parent) {
                current = current->parent;
            } else {
                Node* next = findChild(current, name);
                if (next)
                    current = next;
                else
                    printf("Directory not found.\n");
            }
        } else if (strncmp(command, "ls", 2) == 0) {
            printTree(current,0);
        } else if (strncmp(command, "tree", 4) == 0) {
            printTree(root,0);
        } else if (strncmp(command, "exit", 4) == 0) {
            break;
        } else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}