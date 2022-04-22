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

void addToEnd(Node **head, char value) {
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

void add(Node **head, char value, Node *lastNode) {
  Node *tmp = *head;
  Node *node = (Node *)malloc(sizeof(Node));

  node->value = value;
  node->next = NULL;
  node->head = *head;
  node->tail = node;

  while (tmp != lastNode) {
    if(lastNode->next == NULL) {
      tmp->tail = node;
    }
    
    tmp = tmp->next;
  }

  if(tmp->next) {
    node->next = tmp->next;
  }
  
  tmp->next = node;
  // if(lastNode->next == NULL) {
    tmp->tail = node;
  // }
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
    if(node->next == NULL) {
      tmp->tail = node->previous;
    }
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
      addToEnd(&bracketStackHead, tmp->value);
    } else if (tmp->value == ')') {
      while (operatorStackHead->next) {
        addToEnd(&newStackHead, operatorStackHead->tail->value);
        pop(&operatorStackHead, operatorStackHead->tail);
        pop(&bracketStackHead, bracketStackHead->tail);
      }
    } else if (isdigit(tmp->value)) {
      addToEnd(&newStackHead, tmp->value);
    } else if (tmp->value == '+' || tmp->value == '-' || tmp->value == '*' ||
               tmp->value == '/') {
      addToEnd(&operatorStackHead, tmp->value);
    }
  }
  while (operatorStackHead->next) {
    addToEnd(&newStackHead, operatorStackHead->tail->value);
    pop(&operatorStackHead, operatorStackHead->tail);
  }
  
  return newStackHead;
}

int isOperator(char character) {
  if (
    character == '+' ||
    character == '-' ||
    character == '*' ||
    character == '/'
  ) return 1;

  return 0;
}

int calculate(Node *head) {
  Node *tmp = head;
  int result = 0;
  
  while(tmp->next) {
    if(isOperator(tmp->value) == 1) {
      int a = tmp->previous->previous->value;
      int b = tmp->previous->value;

      printf("\n%c %c %c\n", a, tmp->value, b);
      if(tmp->value == '+') {
        result += (a + b);
      } else if(tmp->value == '-') {
        result += (a - b);
      } else if(tmp->value == '/') {
        result += (a / b);
      } else if(tmp->value == '*') {
        result += (a * b);
      }

      pop(&tmp, tmp);
      pop(&tmp, tmp->previous);
      pop(&tmp, tmp->previous->previous);
    }

    tmp = tmp->next;
  }

  return result;
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
    addToEnd(&head, entrada[i]);
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

  Node *postfix = parseToPostFix(head);
 list(postfix);
// printf("\nresult: %d\n", calculate(postfix));

  return 0;
}