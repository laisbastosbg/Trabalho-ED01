#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  char value;
  struct Node *next;
  struct Node *previous;
  struct Node *head;
  struct Node *tail;
} Node;

void add(Node **head, char value) {
  Node *tmp = *head;
  Node *node = (Node *)malloc(sizeof(Node));

  node->value = value;
  node->next = NULL;
  node->head = *head;
  node->tail = node;

  while (tmp->next != NULL) {
    tmp->tail = node;
    tmp = tmp->next;
  }

  tmp->next = node;
  tmp->tail = node;
  node->previous = tmp;
}

void pop(Node **head, Node *node) {

  if (node->previous) {
    node->previous->next = node->next;
  }

  if (node->next) {
    node->next->previous = node->previous;
  }

  Node *tmp = *head;
  while (tmp->next) {
    tmp->tail = node->previous;
    tmp = tmp->next;
  }

  // free(node); // essa linha faz o loop de stripSpaces quebrar (?)
}

void list(Node *head) {
  Node *tmp = head;

  while (tmp->next) {
    tmp = tmp->next;
    printf("%c", tmp->value);
  }
  printf("\n");
}

void validateBrackets(Node *head) {
  int openingBracketCounter = 0;
  int closingBracketCounter = 0;
  Node *tmp = head;

  while (tmp->next) {
    tmp = tmp->next;
    if (tmp->value == '(')
      openingBracketCounter++;
    if (tmp->value == ')')
      closingBracketCounter++;

    if (closingBracketCounter > openingBracketCounter) {
      printf("Error: closing brackets must come after opening brackets\n");
      exit(0);
    }
  }

  if (closingBracketCounter != openingBracketCounter){
    
    printf("Error: brackets count not matched\n");
    exit(0);
  }
}

void validateOperators(Node *head) {
  Node *tmp = head->next;

  char currentElement = tmp->value;
  char nextElement = tmp->next->value;

  if (currentElement == '+' || currentElement == '-' || currentElement == '*' ||
      currentElement == '/') {
    printf("Error: Invalid operation.\n");
    exit(0);
  }

  while (tmp->next) {
    currentElement = tmp->value;
    nextElement = tmp->next->value;

    if (tmp->next == NULL && (tmp->value == '+' || tmp->value == '-' ||
                              tmp->value == '*' || tmp->value == '/')) {
      printf("Error: Invalid operation.\n");
      exit(0);
    }

    if ((currentElement == '+' || currentElement == '-' ||
         currentElement == '*' || currentElement == '/' ||
         currentElement == '(') &&

        (nextElement == '+' || nextElement == '-' || nextElement == '*' ||
         nextElement == '/' || nextElement == ')')) {
      printf("Error: Invalid operation\n");
      exit(0);
    }
    tmp = tmp->next;
  }

  if (tmp->value == '+' || tmp->value == '-' || tmp->value == '*' ||
      tmp->value == '/') {
    printf("Error: Invalid operation.\n");
    exit(0);
    }
}

void stripSpaces(Node *head) {
  Node *tmp = head;

  while (tmp->next) {
    tmp = tmp->next;
    if (tmp->value == ' ') {
      pop(&head, tmp);
    }
  }
}

Node *parseToPostFix(Node *head) {
  Node *tmp = head;

  Node *bracketStackHead = (Node *)malloc(sizeof(Node));
  bracketStackHead->tail = bracketStackHead;
  bracketStackHead->head = bracketStackHead;
  bracketStackHead->next = NULL;
  bracketStackHead->previous = NULL;

  Node *operatorStackHead = (Node *)malloc(sizeof(Node));
  operatorStackHead->tail = operatorStackHead;
  operatorStackHead->head = operatorStackHead;
  operatorStackHead->next = NULL;
  operatorStackHead->previous = NULL;

  Node *newStackHead = (Node *)malloc(sizeof(Node));
  newStackHead->tail = newStackHead;
  newStackHead->head = newStackHead;
  newStackHead->next = NULL;
  newStackHead->previous = NULL;

  while (tmp->next) {
    tmp = tmp->next;
    if (tmp->value == '(') {
      add(&bracketStackHead, tmp->value);
    } else if (tmp->value == ')') {
      while (operatorStackHead->next) {
        add(&newStackHead, operatorStackHead->tail->value);
        pop(&operatorStackHead, operatorStackHead->tail);
        pop(&bracketStackHead, bracketStackHead->tail);
      }
    } else if (isdigit(tmp->value)) {
      add(&newStackHead, tmp->value);
    } else if (tmp->value == '+' || tmp->value == '-' || tmp->value == '*' ||
               tmp->value == '/') {
      add(&operatorStackHead, tmp->value);
    }
  }
  while (operatorStackHead->next) {
    add(&newStackHead, operatorStackHead->tail->value);
    pop(&operatorStackHead, operatorStackHead->tail);
  }
  
  return newStackHead;
}

int main() {
  Node *head = (Node *)malloc(sizeof(Node));
  head->next = NULL;
  head->previous = NULL;
  head->head = head;
  head->tail = head;

  // lê entrada
  char *entrada;
  size_t length = 0;
  ssize_t read;

  printf("enter infix expression: ");
  read = getline(&entrada, &length, stdin);

  // coloca a string de entrada em uma lista encadeada
  int i;
  for (i = 0; i < strlen(entrada); i++) {
    add(&head, entrada[i]);
  }

  validateBrackets(head);

  validateOperators(head);

  stripSpaces(head);

  Node *postfix = parseToPostFix(head);
  printf("postfix: ");
  list(postfix);

  return 0;
}