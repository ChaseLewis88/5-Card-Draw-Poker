#include "PA7.h"

int main(void)
{
	// makes random values actually random
	srand(time(NULL));

	// intro
	printf("Welcome to Poker!\n\n");

	// initializations 
	int keep_playing = 1, option = 0;

	// starts as true so that first loop starts
	while (keep_playing == 1)
	{
		// input validation on menu selection
		do {
			option = menu_select();
			if (option > 3 || option < 1)
			{
				system("cls");
				printf("That is not an option. You must choose option 1, 2 or 3.\n\n");
			}
		} while (option > 3 || option < 1);

		// cleaning screen
		system("cls");

		// depending on input, either prints rules, plays game, or exits
		switch (option)
		{
		case 1:
			// prints rules, then return to menu
			print_rules();
			break;
		case 2:
			// plays game, then returns to menu afterwards
			play_game();
			//system("cls");
			break;
		case 3:
			// loop condition no longer satisfied, so program ends
			printf("Thanks for playing!\n");
			keep_playing = 0;
			break;
		}
	}
	return 0;
}