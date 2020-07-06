/*
Name: Chase Lewis
Date: 11/14/19
Description: A game of Poker
*/

typedef struct card
{
	int suit;
	int face;
} Card;

typedef struct hand
{
	Card cards[5];
} Hand;

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

// displays menu and takes in user option
int menu_select(void);

// print game rules
void print_rules(void);

// starts game, conducts game, prints outcome
void play_game(void);

// shows user their hand
void display_hand(Hand current_hand, const char* suit[4], const char* face[13]);

// shows dealer hand at the end
void display_dealer_hand(Hand current_hand, const char* suit[4], const char* face[13]);

// puts 1-52 randomly in 4x13 array. position indicates face and suit, number in that position indicates order
void shuffle(int wDeck[][13]);

// deal up to 5 cards to dealer or player
void deal(const int wDeck[][13], int* card_ptr, Hand *current_hand, int keep_hand[]);

// player chooses what cards to keep
void player_determine_keep_cards(int keep_cards[]);

// dealer chooses what cards to keep based on algorithm
void dealer_determine_keep_cards(int dealer_keep_cards[], Hand current_hand, int face_frequency[]);

// converts hand to frequency index
void convert_hand(Hand current_hand, int face_frequency[]);

// checks for flush. all five cards same suit
int flush(Hand current_hand);

// checks for straight. 5 cards in a row
int straight(int face_frequency[]);

// checks for four of a kind
int four_of_a_kind(int face_frequency[]);

// checks for three of a kind and pair
int full_house(int face_frequency[]);

// checks for three of a kind
int three_of_a_kind(int face_frequency[]);

// checks for two pairs
int two_pair(int face_frequency[]);

// checks for pair
int pair(int face_frequency[]);

// assigns value of 0-8 based on hand. 0 is nothing, 8 is straight flush
int evaluate_hand(Hand current_hand, int face_frequency[]);

// determines winner or tie based on hand values
void determine_winner(int player_score, int dealer_score, char outcomes[][16]);

// clears frequency index for dealer. dealer's hand must be evaluated before exchanging cards so the computer can determine what cards to swap out. not resetting index would cause issues
void clear_face_frequency(int face_frequency[]);

// asks user to keep playing or not
char continue_playing(void);