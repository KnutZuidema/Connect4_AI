#include <string>
#define BOARD_WIDTH 8
#define BOARD_HEIGHT 7

struct Combination;
int compute_score_human(const int board[], int color, int depth, int col);
int compute_score_computer(const int board[], int color, int depth, int col);
int board_input(int board[], int color, int column);
void array_copy(const int source[BOARD_WIDTH*BOARD_HEIGHT], int destination[BOARD_WIDTH*BOARD_HEIGHT]);
void compute_next_move(int board[], int color, int depth = 4);
std::string print_board(const int board[]);