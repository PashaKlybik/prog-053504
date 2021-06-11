#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LENGTH 1000

#include "list.h"

int isDigit(char c);
int compare(struct list *l1, struct list *l2);  //compares 2 numbers
void multiply(struct list *number1, int number2, struct list *result);
void subtract(struct list *minuend, struct list *subtrahend);
void freeMemory (
        char *num1,
        char *num2,
        struct list *l1,
        struct list *l2,
        struct list *result,
        struct list *incompleteDividend,
        struct list *tempList);

int main() {
    const char MEMORY_ERROR_MESSAGE[] = "Error while allocating memory. Aborting.";
    char* num1 = (char*) malloc(sizeof(char) * MAX_LENGTH);
    char* num2 = (char*) malloc(sizeof(char) * MAX_LENGTH);
    if (num1 == NULL || num2 == NULL) {
        printf(MEMORY_ERROR_MESSAGE);
        freeMemory(num1, num2, NULL, NULL, NULL, NULL, NULL);
        return 1;
    }
    // input
    gets(num1);
    gets(num2);
    unsigned int length1 = strlen(num1);
    unsigned int length2 = strlen(num2);

    for (int i = 0; i < length1; i++) {
        if (!isDigit(num1[i])) {
            printf("Error. '%c' isn't digit.", num1[i]);
            free(num1);
            free(num2);
            return 1;
        }
    }
    for (int i = 0; i < length2; i++) {
        if (!isDigit(num2[i])) {
            printf("Error. '%c' isn't digit.", num2[i]);
            free(num1);
            free(num2);
            return 1;
        }
    }

    // array of strings that represents answer in formatted style
    char **answerMatrix = (char **) malloc(sizeof(char *) * 2 * (length1 - length2 + 1));
    if (answerMatrix == NULL) {
        printf(MEMORY_ERROR_MESSAGE);
        freeMemory(num1, num2, NULL, NULL, NULL, NULL, NULL);
        free(answerMatrix);
        return 1;
    }
    for (int i = 0; i < 2 * (length1 - length2 + 1); i++) {
        answerMatrix[i] = (char*) malloc(sizeof(char) * (length1 + 1));
        if (answerMatrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(answerMatrix[i]);
            }
            printf(MEMORY_ERROR_MESSAGE);
            freeMemory(num1, num2, NULL, NULL, NULL, NULL, NULL);
            free(answerMatrix);
            return 1;
        }
        answerMatrix[i][0] = '\0';
    }

    // lists that represent long numbers
    // l1 - dividend, l2 - divider
    // tempList contains result of multiplication of divider to 0..9 in each step
    struct list *l1, *l2, *result, *incompleteDividend, *tempList;
    l1 = list();
    l2 = list();
    result = list();
    tempList = list();
    incompleteDividend = list();

    if (l1 == NULL || l2 == NULL || result == NULL || tempList == NULL || incompleteDividend == NULL) {
        printf(MEMORY_ERROR_MESSAGE);
        freeMemory(num1, num2, l1, l2, result, incompleteDividend, tempList);
        return 1;
    }
    for (int i = 0; i < length1; i++) {
        pushBack(l1, num1[i] - '0');
    }
    for (int i = 0; i < length2; i++) {
        pushBack(l2, num2[i] - '0');
    }

    printf("%s | %s\n", num1, num2);
    struct node *n = l1->head;
    while (incompleteDividend->size < l2->size - 1) {
        pushBack(incompleteDividend, n->data);
        n = n->next;
    }
    unsigned int margin_right = length1 - length2; // for formatted out. Number of spaces needed from the right
    for (int i = 0; i < length1 - length2 + 1; i++) {
        pushBack(incompleteDividend, n->data);
        n = n->next;
        for (int num = 9; num >= 0; num--) {
            multiply(l2, num, tempList);
            // delete leading zeroes
            while (incompleteDividend->size && incompleteDividend->head->data == 0) popFront(incompleteDividend);
            while (tempList->size && tempList->head->data == 0) popFront(tempList);

            if (compare(incompleteDividend, tempList) >= 0) {
                if (num != 0) {
                    struct node *it = incompleteDividend->head;
                    int j = 0;
                    while (j < length1 - margin_right - incompleteDividend->size) {
                        answerMatrix[i * 2][j] = ' ';
                        j++;
                    }
                    while (it) {
                        answerMatrix[i * 2][j] = (char) ('0' + it->data);
                        it = it->next;
                        j++;
                    }
                    answerMatrix[i * 2][j] = '\0';
                    it = tempList->head;
                    j = 0;
                    while (j < length1 - margin_right - tempList->size) {
                        // left margin //
                        answerMatrix[i * 2 + 1][j] = ' ';
                        j++;
                    }
                    while (it) {
                        answerMatrix[i * 2 + 1][j] = (char) ('0' + it->data);
                        it = it->next;
                        j++;
                    }
                    answerMatrix[i * 2 + 1][j] = '\0';
                }
                subtract(incompleteDividend, tempList);
                pushBack(result, num);
                break;
            }
        }
        margin_right--;
    }

    // output
    int answerShowed = 0;
    int firstStringSkipped = 0;
    for (int i = 0; i < 2 * (length1 - length2 + 1); i++) {
        if (strlen(answerMatrix[i]) > 0) {
            if (!firstStringSkipped) {
                firstStringSkipped = 1;
                continue;
            }
            printf("%s", answerMatrix[i]);
            if (!answerShowed) {
                for (int j = 0; j < length1 - strlen(answerMatrix[i]); j++) { printf(" "); }
                printf(" | ");
                while (result->head->data == 0) popFront(result);
                struct node *it = result->head;
                while (it) {
                    printf("%d", it->data);
                    it = it->next;
                }
                answerShowed = 1;
            }
            printf("\n");
        }

    }
    freeMemory(num1, num2, l1, l2, result, incompleteDividend, tempList);
    return 0;
}

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int compare(struct list *l1, struct list *l2) {
    if (l1->size < l2->size) { return -1; }
    if (l1->size > l2->size) { return 1; }
    struct node *n1, *n2;
    n1 = l1->head;
    n2 = l2->head;
    while (n1 && n2) {
        if (n1->data < n2->data) { return -1; }
        if (n1->data > n2->data) { return 1; }
        n1 = n1->next;
        n2 = n2->next;
    }
    return 0;
}

void multiply(struct list *number1, int number2, struct list *result) {
    while (result->size) {        // clear result
        popBack(result);
    }
    struct node *currentDigit = number1->tail;
    int nextDigitAdd = 0;
    while (currentDigit) {
        int n = currentDigit->data * number2 + nextDigitAdd;
        pushFront(result, n % 10);
        nextDigitAdd = n / 10;
        currentDigit = currentDigit->prev;
    }
    if (nextDigitAdd) {
        pushFront(result, nextDigitAdd);
    }
}

void subtract(struct list *minuend, struct list *subtrahend) {
    struct node *n1, *n2;
    n1 = minuend->tail;
    n2 = subtrahend->tail;
    while (n1 && n2) {
        if (n1->data < n2->data) {
            // take 10 from senior rank
            struct node *n = n1->prev;
            while (n->data == 0) {
                n->data = 9;
                n = n->prev;
            }
            n->data--;
            n1->data += 10;
        }
        n1->data -= n2->data;
        n1 = n1->prev;
        n2 = n2->prev;
    }
}

void freeMemory (
        char *num1,
        char *num2,
        struct list *l1,
        struct list *l2,
        struct list *result,
        struct list *incompleteDividend,
        struct list *tempList) {
    free(num1);
    free(num2);
    deleteList(l1);
    deleteList(l2);
    deleteList(result);
    deleteList(incompleteDividend);
    deleteList(tempList);
}
