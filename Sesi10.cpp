#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
	char id[6];
	char name[100];
	int price;
	struct Node *left, *right;
	int height;
} Node;

Node *root = NULL;

bool isUppercase(char a) {
	return (a >= 'A' && a <= 'Z');
}

Node *createNode(char name[], int price) {
	Node *newNode = (Node*) malloc(sizeof(Node));
	sprintf(newNode->id, "%c%c%d%d%d",
		isUppercase(name[0]) ? name[0] : name[0] - 32,
		isUppercase(name[1]) ? name[1] : name[1] - 32,
		rand() % 10,
		rand() % 10,
		rand() % 10
	);
	strcpy(newNode->name, name);
	newNode->price = price;
	newNode->left = newNode->right = NULL;
	newNode->height = 1;
	return newNode;
}

int findMax(int a, int b) {
	return (a > b) ? a : b;
}

int getHeight(Node *curr) {
	return (curr) ? curr->height : 0;
}

int getBf(Node *curr) {
	return getHeight(curr->left) - getHeight(curr->right);
}

void updateHeight(Node *curr) {
	curr->height = findMax(getHeight(curr->left), getHeight(curr->right)) + 1;
}

Node *leftRotate(Node *curr) {
	Node *right = curr->right;
	Node *rightLeft = right->left;
	
	curr->right = rightLeft;
	right->left = curr;
	
	updateHeight(curr);
	updateHeight(right);
	
	return right;
}

Node *rightRotate(Node *curr) {
	Node *left = curr->left;
	Node *leftRight = left->right;
	
	curr->left = leftRight;
	left->right = curr;
	
	updateHeight(curr);
	updateHeight(left);
	
	return left;
}

Node *balancing(Node *curr) {
	if (!curr) return NULL;
	
	updateHeight(curr);
	int bf = getBf(curr);
	
	if (bf > 1) {
		if (getBf(curr->left) <= 0)
			curr->left = leftRotate(curr->left);
		curr = rightRotate(curr);
	} else if (bf < -1) {
		if (getBf(curr->right) >= 0)
			curr->right = rightRotate(curr->right);
		curr = leftRotate(curr);
	}
	
	return curr;
}

Node *insertion(Node *curr, Node *newNode) {
	if (!curr) return newNode;
	else if (strcmp(newNode->id, curr->id) < 0)
		curr->left = insertion(curr->left, newNode);
	else if (strcmp(newNode->id, curr->id) > 0)
		curr->right = insertion(curr->right, newNode);
	return balancing(curr);
}

void inOrder(Node *curr) {
	if (curr) {
		inOrder(curr->left);
		printf("ID: %s\nName: %s\nPrice: %d\nHeight: %d\n\n",
		curr->id, curr->name, curr->price, curr->height);
		inOrder(curr->right);
	}
}

Node *searching(Node *curr, char id[]) {
	if (!curr) return NULL;
	else if (strcmp(id, curr->id) < 0)
		return searching(curr->left, id);
	else if (strcmp(id, curr->id) > 0)
		return searching(curr->right, id);
	return curr;
}

Node *deletion(Node *curr, char id[]) {
	if (!curr) {
		puts("Movie not found.");
		return NULL;	
	}
	else if (strcmp(id, curr->id) < 0)
		curr->left = deletion(curr->left, id);
	else if (strcmp(id, curr->id) > 0)
		curr->right = deletion(curr->right, id);
	else {
		if (!curr->left && !curr->right) {
			free(curr);
			curr = NULL;
		} else if (!curr->left || !curr->right) {
			Node *temp = (curr->left) ? curr->left : curr->right;
			*curr = *temp;
			free(temp);
			temp = NULL;
		} else {
			Node *successor = curr->right;
			while (successor->left) {
				successor = successor->left;
			}
			
			strcpy(curr->id, successor->id);
			strcpy(curr->name, successor->name);
			curr->price = successor->price;
			
			curr->right = deletion(curr->right, successor->id);
		}
	}
	
	return balancing(curr);
}

Node *popAll(Node *curr) {
	if (curr) {
		curr->left = popAll(curr->left);
		curr->right = popAll(curr->right);
		free(curr);
		return NULL;
	}
}

int main() {
	srand(time(NULL));
	root = insertion(root, createNode("Up", 1231231));
	root = insertion(root, createNode("Naruto", 9123321));
	root = insertion(root, createNode("Exhuma", 12390123));
	root = insertion(root, createNode("Kimi no Nawa", 12389));

	inOrder(root);
	
//	printf("Input ID to be deleted: ");
//	char id[6];
//	scanf("%[^\n]", id); getchar();
	
//	Node *result = searching(root, id);
//	
//	if (result == NULL) {
//		puts("Not found.");
//	} else {
//		printf("Movie name: %s\n", result->name);
//	}

//	root = deletion(root, id);

	printf("Press Enter to Pop All");
	getchar();
	
	root = popAll(root);
	
	inOrder(root);

	return 0;
}
