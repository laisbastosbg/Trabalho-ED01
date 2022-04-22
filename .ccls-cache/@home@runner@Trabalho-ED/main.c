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

char *validateBrackets(Node *head) {
  int openingBracketCounter = 0;
  int closingBracketCounter = 0;
  Node *tmp = head;

  while (tmp->next) {
    tmp = tmp->next;
    if (tmp->value == '(')
      openingBracketCounter++;
    if (tmp->value == ')')
      closingBracketCounter++;

    if (closingBracketCounter > openingBracketCounter)
      return "Error: closing brackets must come after opening brackets\n";
  }

  if (closingBracketCounter != openingBracketCounter)
    return "Error: brackets count not matched\n";

  return "ok\n";
}

char *validateOperators(Node *head) {
  Node *tmp = head->next;

  char currentElement = tmp->value;
  char nextElement = tmp->next->value;

  if (currentElement == '+' || currentElement == '-' || currentElement == '*' ||
      currentElement == '/') {
    return "Error: Invalid operation.\n";
  }

  while (tmp->next) {
    currentElement = tmp->value;
    nextElement = tmp->next->value;

    if (tmp->next == NULL && (tmp->value == '+' || tmp->value == '-' ||
                              tmp->value == '*' || tmp->value == '/')) {
      return "Error: Invalid operation.\n";
    }

    if ((currentElement == '+' || currentElement == '-' ||
         currentElement == '*' || currentElement == '/' ||
         currentElement == '(') &&

        (nextElement == '+' || nextElement == '-' || nextElement == '*' ||
         nextElement == '/' || nextElement == ')')) {
      return "Error: Invalid operation\n";
    }
    tmp = tmp->next;
  }

  if (tmp->value == '+' || tmp->value == '-' || tmp->value == '*' ||
      tmp->value == '/') {
    return "Error: Invalid operation.\n";
  }

  return "ok\n";
}

char *validateCharacters(Node *head) {
  Node *tmp = head;

  while (tmp->next) {
    tmp = tmp->next;

    switch (tmp->value) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '+':
    case '-':
    case '*':
    case '/':
    case '(':
    case ')':
      break;
    default:
      return "Error: Invalid input\n";
    }
  }

  return "ok\n";
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

void parseToPostFix(Node *head) {
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
  list(bracketStackHead);
  list(operatorStackHead);
  list(newStackHead);
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

  // read = getline(&entrada, &length, stdin);
  // printf("-> %s", entrada);

  // coloca a string de entrada em uma lista encadeada
  int i;
  entrada = "((2 + 3) * (5 + 2) ) * ((3 + 8 - 2 )  * (2 + 3)  * (3 + 4) )";
  // entrada = "2+1-3*4";
  for (i = 0; i < strlen(entrada); i++) {
    // printf("%d: %c\n", i, entrada[i]);
    add(&head, entrada[i]);
  }

  char *validateBracketsResponse = validateBrackets(head);

  // printf("%s\n", validateBracketsResponse);

  char *validateOpeatorsResponse = validateOperators(head);

  // printf("%s\n", validateOpeatorsResponse);

  char *validateCharactersResponse = validateCharacters(head);

  // printf("%s\n\n\n", validateCharactersResponse);
  // list(head);
  stripSpaces(head);
  list(head);

  parseToPostFix(head);

  return 0;
}