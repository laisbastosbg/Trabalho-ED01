#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  char value;
  struct Node *next;
} Node;

void add (Node **head, char value) {
  Node *tmp = *head;
  Node *node = (Node *) malloc(sizeof(Node));
  node->value = value;
  node->next = NULL;

  while (tmp->next != NULL) {
    tmp = tmp->next;
  }

  tmp->next = node;
}

void list (Node *head) {
  Node *tmp = head;

  while(tmp->next) {
    tmp = tmp->next;
    printf("%c", tmp->value);
  }
}

char *validateBrackets (Node *head) {
  int openingBracketCounter = 0;
  int closingBracketCounter = 0;
  Node *tmp = head;

  while (tmp->next) {
    tmp = tmp->next;
    if (tmp->value == '(') openingBracketCounter++;
    if (tmp->value == ')') closingBracketCounter++;

    if (closingBracketCounter > openingBracketCounter)
      return "Error: closing brackets must come after opening brackets\n";
  }

  if (closingBracketCounter != openingBracketCounter)
    return "Error: brackets count not matched\n";
  
  return "ok\n";
}

char *validateOperators (Node *head) {
  Node *tmp = head;

  char currentElement;
  char nextElement;
  while (tmp->next) {
    tmp = tmp->next;
    currentElement = tmp->value;
    nextElement = tmp->next->value;

    if (
      (currentElement == '+' ||
      currentElement == '-' ||
      currentElement == '*' ||
      currentElement == '/') &&

      (nextElement == '+' ||
      nextElement == '-' ||
      nextElement == '*' ||
      nextElement == '/' ||
      nextElement == ')')
    ) {
      return "Error: Invalid operation\n";
      
    }
  }

  return "ok\n";
}

char *validateCharacters (Node *head) {
  Node *tmp = head;

  while(tmp->next) {
    tmp = tmp->next;

    switch(tmp->value) {
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

int main () {
  Node *head = (Node *) malloc(sizeof(Node));
  head->next = NULL;


  // lê entrada
  char *entrada;
  size_t length = 0;
  ssize_t read;

  //read = getline(&entrada, &length, stdin);
  //printf("-> %s", entrada);


  // coloca a string de entrada em uma lista encadeada
  int i;
  entrada = "((2 + 3 a) * (5 + 2) ) * ((3 + 8 - 2 )  * (2 + 3)  * (3 + 4) )";
  for (i = 0; i < strlen(entrada); i++) {
    // printf("%d: %c\n", i, entrada[i]);
    add(&head, entrada[i]);
  }

  char *validateBracketsResponse = validateBrackets(head);

  printf("%s", validateBracketsResponse);

  char *validateOpeatorsResponse = validateOperators(head);

  printf("%s", validateOpeatorsResponse);

  char *validateCharactersResponse = validateCharacters(head);

  printf("%s", validateCharactersResponse);
  list(head);

  return 0;
}