#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void printHelp(void) {
	printf("Usage of math-quiz:\n");
	printf("Ask a series of simple math questions.\n");
	printf("Usage:\n");
	printf("\t-q int\n");
	printf("\t\tSet number of questions\n");
	printf("\t-a\n");
	printf("\t\tEnable addition questions\n");
	printf("\t-s\n");
	printf("\t\tEnable subtraction questions\n");
	printf("\t-m\n");
	printf("\t\tEnable multiplication questions\n");
	printf("\t-d\n");
	printf("\t\tEnable division questions\n");
}

int main(int argc, char *argv[]) {
	int questions = 0;
	int correctAnswers = 0;
	int answer = 0;
	bool multiplication = false;
	bool division = false;
	bool addition = false;
	bool subtraction = false;
	char operator = '0';

	// Seed random number generator.
	srand(time(NULL));

	// Evaluate command line arguments.
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			printHelp();
			return 0;
		// Get number of questions from command line arguments.
		} else if (strcmp(argv[i], "-q") == 0) {
			if (argc > i + 1) {
				i++;
				questions = atoi(argv[i]);
			} else {
				printf("Error: Please pass an integer with -q.\n");
				return 1;
			}
		// Get allowed operators from command line arguments.
		} else if (strcmp(argv[i],  "-m") == 0) {
			multiplication = true;
		} else if (strcmp(argv[i],  "-d") == 0) {
			division = true;
		} else if (strcmp(argv[i],  "-a") == 0) {
			addition = true;
		} else if (strcmp(argv[i],  "-s") == 0) {
			subtraction = true;
		} else {
			printf("Error: Invalid argument passed: %s.\nTry running with '-h' for help.\n", argv[i]);
			return 1;
		}
	} 

	// Make sure we have at least one operator and a number of questions.
	if (!multiplication && !division && !addition && !subtraction) {
		printf("Error: No operators specified.\n");
		return 1;
	} else if (questions <= 0) {
		printf("Error: No number of questions specified.\n");
		return 1;
	}

	// Run while.
	int remainingQuestions = questions;
	while (remainingQuestions > 0) {
		// Pick an operator.
		char ops[4];
		int opCount = 0;
		
		if (addition) ops[opCount++] = '+';
		if (subtraction) ops[opCount++] = '-';
		if (multiplication) ops[opCount++] = '*';
		if (division) ops[opCount++] = '/';
		
		operator = ops[rand() % opCount];

		// Generate two random numbers.
		int num1 = rand() % 13;
		int num2 = rand() % 13;

		// Check that answer will be a positive int.
		if (operator == '/') {
			if(num2 == 0 || num1 <= 0 || num1 % num2 != 0) {
				continue;
			}
		} else if (operator == '+' && num1 + num2 < 0) {
			continue;
		} else if (operator == '-' && num1 - num2 < 0) {
			continue;
		} else if (operator == '*' && num1 * num2 < 0) {
			continue;
		}

		// Display problem.
		printf("%d %c %d = ", num1, operator, num2);

		// Get user input.
		char inputBuffer[5];
		if (fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL) {
			// Convert and store input as int.
			answer = atoi(inputBuffer);

			// Check for overflow
			if (strchr(inputBuffer, '\n') == NULL) {
				printf("Warning: Input too long.\n");

				// Clear stdin input buffer by discarding excess input.
				int c;
				while ((c = getchar()) != '\n' && c != EOF);
			}
		}

		// Check answer.
		if ((operator == '/' && num1 / num2 == answer) || (operator == '*' && num1 * num2 == answer) || (operator == '+' && num1 + num2 == answer) || (operator == '-' && num1 - num2 == answer)) {
			printf("Correct!\n");
			correctAnswers++;
		} else {
			printf("Incorrect!\n");
		}

		remainingQuestions--;
		operator = '0';
	}

	// Calculate score.
	printf("Score: %.2f%%\n", ((float)correctAnswers / questions) * 100);

	return 0;
}
