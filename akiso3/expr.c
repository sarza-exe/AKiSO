#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        //from printf.c
        printf(2, "Usage: expr <num1> <operator> <num2>\n");
        exit();
    }

    // Konwersja argumentów
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[3]);
    char operator = argv[2][0];
    int result;

    // Wykonywanie operacji
    switch (operator) {
    case '+':
        result = num1 + num2;
        break;
    case '-':
        result = num1 - num2;
        break;
    case '*':
        result = num1 * num2;
        break;
    case '/':
        if (num2 == 0) {
            // 2 - standard error
            printf(2, "Error: Division by zero\n");
            exit();
        }
        result = num1 / num2;
        break;
    default:
        printf(2, "Error: Unknown operator '%c'\n", operator);
        exit();
    }

    // Wyświetlanie wyniku
    // 1 - standard output
    printf(1, "%d\n", result);
    exit();
}
