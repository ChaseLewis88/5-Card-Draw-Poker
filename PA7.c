#include "PA7.h"

// displays menu and takes in user option
int menu_select(void)
{
	int option = 0;

	printf("Please enter an option:\n\n");
	printf("1. Print Game Rules\n");
	printf("2. Play Game\n");
	printf("3. Exit\n");

	scanf("%d", &option);

	return option;
}

// print game rules
void print_rules(void)
{
	printf("This is a game of 5-card poker. You play against the dealer. You are each randomly dealt 5 cards. You cannot see the dealer's cards. The winner is the one with the best combination of cards. \n"
		"The combinations are explained here, with the lower combinations being better: \n\n"

		"Nothing - The cards have no pattern\n"
		"Pair - Two cards with the same face\n"
		"Two-Pair - Two sets of two cards with the same face\n"
		"Three-of-a-kind - Three cards with the same face\n"
		"Straight - All five card faces are in sequential order\n"
		"Flush - All five cards have the same suit\n"
		"Full House - Three-of-a-kind and Two-pair\n"
		"Four-of-a-kind - Four cards with the same face\n"
		"Straight Flush - All five cards have the same suit and are in sequential order\n\n"

		"You each have the option to exchange up to 3 of your cards for new cards. If you and the dealer have the same combination in the end, there is a tie.\n\n");
}

// starts game, conducts game, prints outcome
void play_game(void)
{
	/* initialize suit array */
	const char* suit[4] = { "Hearts", "Diamonds", "Clubs", "Spades" };

	/* initialize face array */
	const char* face[13] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight",
		"Nine", "Ten", "Jack", "Queen", "King" };

	char outcomes[][16] = { "Nothing", "Pair", "Two-Pair", "Three-of-a-kind", "Straight", "Flush", "Full House", "Four-of-a-kind", "Straight Flush" };

	char keep_playing = 'y';

	// initialize hands for user and dealer
	Hand player_hand, dealer_hand;

	// breaks when user doesn't want to play anymore
	while (keep_playing == 'y')
	{
		/* initalize deck array */
		int deck[4][13] = { 0 };

		shuffle(deck);
		int current_card = 1;

		// array of 1's and 0's to store what cards user wants to keep. start as 0 so that a full hand is dealt initially
		int player_keep_hand[5] = { 0 };
		int dealer_keep_hand[5] = { 0 };

		// frequency index so that combinations are easier to analyze
		int player_face_frequency[14] = { 0 };
		int dealer_face_frequency[14] = { 0 };

		int player_score = 0, dealer_score = 0;

		// deal out 5 cards to user and dealer
		deal(deck, &current_card, &player_hand, player_keep_hand);
		deal(deck, &current_card, &dealer_hand, dealer_keep_hand);

		// determines best play for dealer
		convert_hand(dealer_hand, dealer_face_frequency);
		dealer_determine_keep_cards(dealer_keep_hand, dealer_hand, dealer_face_frequency);

		// shows user hand
		display_hand(player_hand, suit, face);

		// prompts user to choose cards to keep
		player_determine_keep_cards(player_keep_hand);

		// deals 3 cards or less to dealer and player, depending on what cards are kept
		deal(deck, &current_card, &player_hand, player_keep_hand);
		deal(deck, &current_card, &dealer_hand, dealer_keep_hand);

		// clear old dealer freq array, converts both dealer and player hands to freq array
		clear_face_frequency(dealer_face_frequency);
		convert_hand(player_hand, player_face_frequency);
		convert_hand(dealer_hand, dealer_face_frequency);

		// stores score for dealer and player
		player_score = evaluate_hand(player_hand, player_face_frequency);
		dealer_score = evaluate_hand(dealer_hand, dealer_face_frequency);

		// clear screen and display final hands
		system("cls");
		display_hand(player_hand, suit, face);
		display_dealer_hand(dealer_hand, suit, face);

		// displays winner
		determine_winner(player_score, dealer_score, outcomes);
		
		keep_playing = continue_playing();
		system("cls");
	}
}

// puts 1-52 randomly in 4x13 array. position indicates face and suit, number in that position indicates order
void shuffle(int wDeck[][13])
{
	int row = 0;    /* row number */
	int column = 0; /*column number */
	int card = 0;   /* card counter */

	/* for each of the 52 cards, choose slot of deck randomly */
	for (card = 1; card <= 52; card++)
	{
		/* choose new random location until unoccupied slot found */
		do
		{
			row = rand() % 4;
			column = rand() % 13;
		} while (wDeck[row][column] != 0);

		/* place card number in chosen slot of deck */
		wDeck[row][column] = card;
	}
}

// deal up to 5 cards to dealer or player
void deal(const int wDeck[][13], int *card_ptr, Hand *current_hand, int keep_hand[])
{
	int row = 0;    /* row number */
	int column = 0; /*column number */
	Card current_card = { 0, 0 };

	for (int i = 0; i < 5; i++)
	{
		if (keep_hand[i] == 0)
		{
			/* loop through rows of wDeck */
			for (row = 0; row <= 3; row++)
			{
				/* loop through columns of wDeck for current row */
				for (column = 0; column <= 12; column++)
				{
					/* if slot contains current card, deal card. increases current card by 1 for next loop */
					if (wDeck[row][column] == *card_ptr)
					{
						current_card.suit = row;
						current_card.face = column;
						current_hand->cards[i] = current_card;
						*card_ptr = *card_ptr + 1;
					}
				}
			}
		}
	}
}

// shows user their hand
void display_hand(Hand current_hand, const char* suit[4], const char* face[13])
{
	printf("Your Hand: \n");
	for (int i = 0; i < 5; i++)
	{
		printf("Card %d: %s of %s\n", i + 1, face[current_hand.cards[i].face], suit[current_hand.cards[i].suit]);
	}
}

// shows dealer hand at the end
void display_dealer_hand(Hand current_hand, const char* suit[4], const char* face[13])
{
	printf("\nDealer Hand: \n");
	for (int i = 0; i < 5; i++)
	{
		printf("Card %d: %s of %s\n", i + 1, face[current_hand.cards[i].face], suit[current_hand.cards[i].suit]);
	}
}

// player chooses what cards to keep
void player_determine_keep_cards(int keep_cards[])
{
	char user_keep_card = 0;
	int cards_left = 3;

	// change all from 0 to 1. this way, if loop ends early from discarding 3 cards, remaining cards are kept instead of discarded. 
	keep_cards[0] = 1; keep_cards[1] = 1; keep_cards[2] = 1; keep_cards[3] = 1; keep_cards[4] = 1;

	for (int i = 0; i < 5 && cards_left != 0; i++)
	{
		// user input validation. updates keep_card array based on user response
		do {
			printf("\nMaximum cards left to discard = %d\n"
				"Would you like to keep card %d? Please enter 'y' for 'yes' or 'n' for 'no'.\n", cards_left, i + 1);
			scanf(" %c", &user_keep_card);

			if (user_keep_card == 'y')
			{
				keep_cards[i] = 1;
			}

			else if (user_keep_card == 'n')
			{
				keep_cards[i] = 0;
				cards_left--;
			}

			else
			{
				printf("\nThat is not an acceptable input.\n");
			}

		} while (user_keep_card != 'y' && user_keep_card != 'n');
	}
}

// dealer chooses what cards to keep based on algorithm
void dealer_determine_keep_cards(int keep_cards[], Hand current_hand, int face_frequency[])
{
	int score = evaluate_hand(current_hand, face_frequency);

	int cards_left = 3;

	keep_cards[0] = 1; keep_cards[1] = 1; keep_cards[2] = 1; keep_cards[3] = 1; keep_cards[4] = 1;

	// if dealer has three of a kind or worse, tries to get a better hand. gets rid of single cards.
	if (score < 4)
	{
		for (int i = 0; i < 13 && cards_left != 0; i++)
		{
			if (face_frequency[i] == 1)
			{
				for (int j = 0; j < 5; j++)
				{
					if (current_hand.cards[j].face == i)
					{
						keep_cards[j] = 0;
						cards_left--;
					}
				}
			}
		}
	}

	// if dealer has straight or better, can't get a better hand without risking an already good hand. so dealer keeps all cards
	else
	{
		for (int i = 0; i < 5; i++)
		{
			keep_cards[i] = 1;
		}
	}
}

// converts hand to frequency index
void convert_hand(Hand current_hand, int face_frequency[])
{
	for (int i = 0; i < 5; i++)
	{
		face_frequency[current_hand.cards[i].face]++;

		// ace can be a start of ace, 2, 3, 4, 5 straight, or end of 10, jack, queen, king, ace straight. so counted at beginning and end of array to make straight algorithm easier
		if (current_hand.cards[i].face == 0)
		{
			face_frequency[13]++;
		}
	}
}

// checks for flush. all five cards same suit
int flush(Hand current_hand)
{
	int flush = 1;
	int first_card = current_hand.cards[0].suit;
	for (int i = 1; i < 5; i++)
	{
		// if any suits don't match, there's no flush
		if (current_hand.cards[i].suit != current_hand.cards[0].suit)
		{
			flush = 0;
		}
	}
	return flush;
}

// checks for straight. 5 cards in a row
int straight(int face_frequency[])
{
	int straight_count = 0;

	// counter goes up for consecutive numbers. resets if a freq is zero. counter of 5 means there are five 1's in a row in freq array, so it's a straight
	for (int i = 0; i < 14; i++)
	{
		if (face_frequency[i] == 1)
		{
			straight_count++;
			if (straight_count == 5)
			{
				return 1;
			}
		}
		else
		{
			straight_count = 0;
		}
	}
	return 0;
}

// checks for four of a kind
int four_of_a_kind(int face_frequency[])
{
	for (int i = 0; i < 13; i++)
	{
		if (face_frequency[i] == 4)
		{
			return 1;
		}
	}
	return 0;
}

// checks for three of a kind and pair
int full_house(int face_frequency[])
{
	// calls pair and three of a kind functions
	if (pair(face_frequency) == 1 && three_of_a_kind(face_frequency) == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// checks for three of a kind
int three_of_a_kind(int face_frequency[])
{
	for (int i = 0; i < 13; i++)
	{
		if (face_frequency[i] == 3)
		{
			return 1;
		}
	}
	return 0;
}

// checks for two pairs
int two_pair(int face_frequency[])
{
	int pair_count = 0;
	for (int i = 0; i < 13; i++)
	{
		if (face_frequency[i] == 2)
		{
			pair_count++;
		}
	}
	
	if (pair_count == 2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// checks for pair
int pair(int face_frequency[])
{
	for (int i = 0; i < 13; i++)
	{
		if (face_frequency[i] == 2)
		{
			return 1;
		}
	}
	return 0;
}

// assigns value of 0-8 based on hand. 0 is nothing, 8 is straight flush
int evaluate_hand(Hand current_hand, int face_frequency[])
{
	// calls straight and flush functions to check for straight flush
	if (flush(current_hand) == 1 && straight(face_frequency) == 1)
	{
		return 8;
	}
	if (four_of_a_kind(face_frequency) == 1)
	{
		return 7;
	}
	if (full_house(face_frequency) == 1)
	{
		return 6;
	}
	if (flush(current_hand) == 1)
	{
		return 5;
	}
	if (straight(face_frequency) == 1)
	{
		return 4;
	}
	if (three_of_a_kind(face_frequency) == 1)
	{
		return 3;
	}
	if (two_pair(face_frequency) == 1)
	{
		return 2;
	}
	if (pair(face_frequency) == 1)
	{
		return 1;
	}
	return 0;
}

// determines winner or tie based on hand values
void determine_winner(int player_score, int dealer_score, char outcomes[][16])
{
	printf("\nPlayer Outcome: %s\n"
		   "\nDealer Outcome: %s\n", outcomes[player_score], outcomes[dealer_score]);

	if (player_score > dealer_score)
	{
		printf("\nPlayer wins.\n");
	}
	if (player_score < dealer_score)
	{
		printf("\nDealer wins.\n");
	}
	if (player_score == dealer_score)
	{
		printf("\nThe result is a tie.\n");
	}
}

// clears frequency index for dealer. dealer's hand must be evaluated before exchanging cards so the computer can determine what cards to swap out. not resetting index would cause issues
void clear_face_frequency(int face_frequency[])
{
	for (int i = 0; i < 14; i++)
	{
		face_frequency[i] = 0;
	}
}

// asks user to keep playing or not
char continue_playing(void)
{
	char keep_playing = '\0';

	do {
		printf("\nWould you like to keep playing? Please enter 'y' for 'yes' or 'n' for 'no'.\n");
		scanf(" %c", &keep_playing);

		if (keep_playing != 'y' && keep_playing != 'n')
		{
			printf("\nThat is not an acceptable input.\n");
		}

	} while (keep_playing != 'y' && keep_playing != 'n');

	return keep_playing;
}