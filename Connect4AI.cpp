#include <cmath>
#include <iostream>
#include <sstream>
#include <random>
#include <ctime>
#include "Connect4AI.h"

/**
 * Eine Kombination aus 4 Feldern
 *
 * Combination::has_won(): gibt true zurueck wenn alle vier Felder die gleiche Farbe haben
 *
 * Combination::print(): Gibt die Kombination aus
 */
struct Combination{
    int slots[4] = {0};

    Combination(int one, int two, int three, int four){
        this->slots[0] = one;
        this->slots[1] = two;
        this->slots[2] = three;
        this->slots[3] = four;
    }

    Combination() = default;

    bool has_won(const int board[], int color){
        return board[this->slots[0]] == color &&
               board[this->slots[1]] == color &&
               board[this->slots[2]] == color &&
               board[this->slots[3]] == color;
    }

    std::string print(){
        std::stringstream str;
        str << "[" << this->slots[0] << ", " << this->slots[1] << ", " << this->slots[2] << ", " << this->slots[3] << "]";
        return str.str();
    }
};

/**
 * Alle moeglichen gewinnenden Kombinationen, wird in der main-Funktion festgelegt, basierend auf 'BOARD_HEIGHT' und 'BOARD_WIDTH'.
 */
Combination winning_combinations[BOARD_HEIGHT * (BOARD_WIDTH - 3) + BOARD_WIDTH * (BOARD_HEIGHT - 3) + 2 * (BOARD_WIDTH - 3) * (BOARD_HEIGHT - 3)];


/**
 * Einen Stein einwerfen
 * @param board das Spielbrett
 * @param color die einzuwerfende Farbe
 * @param column in die einzuwerfende Spalte
 * @return Position des eingeworfenen Steins
 */
int board_input(int board[], int color, int column){
    for(int i = column*BOARD_HEIGHT; i < column*BOARD_HEIGHT + BOARD_HEIGHT; i++){
        if(board[i] != 0){
            if(i == column*BOARD_HEIGHT){
                return -1;
            }
            board[i - 1] = color;
            return board[i - 1];
        }else if(i == column*BOARD_HEIGHT + BOARD_HEIGHT - 1){
            board[i] = color;
            return board[i];
        }
    }
}

/**
 * Kopiert einen Array in einen anderen Array
 * @param source Der zu kopierende Array
 * @param destination Der Array in den <code>source</code> hereinkopiert werden soll
 */
void array_copy(const int source[BOARD_WIDTH*BOARD_HEIGHT], int destination[BOARD_WIDTH*BOARD_HEIGHT]){
    for(int i = 0; i < BOARD_WIDTH*BOARD_HEIGHT; i++){
        destination[i] = source[i];
    }
}

/**
 * Berechnet Punktzahl fuer den Computerzug
 * @param board das Spielbrett
 * @param color die Farbe des Computers
 * @param depth die verbleibende Tiefe
 * @param col die Spalte in die eingeworfen wurde
 * @return Punktzahl
 */
int compute_score_computer(const int board[], int color, int depth, int col){
    int board_copy[BOARD_WIDTH*BOARD_HEIGHT];
    array_copy(board, board_copy);
    board_input(board_copy, color, col);
    int score = 0;
    if(depth <= 0){
        return 0;
    }
    for(Combination &com : winning_combinations){
        if(com.has_won(board_copy, color)){
            score += 1000;
            return score;
        }
    }
    for(int i = 0; i < BOARD_WIDTH; i++){
        score += compute_score_human(board_copy, color % 2 + 1, depth - 1, i);
    }
    return score;
}

/**
 * Berechnet Punktzahl fuer den Spielerzug
 * @param board das Spielbrett
 * @param color die Farbe des Spielers
 * @param depth die verbleibende Tiefe
 * @param col die Spalte in die eingeworfen wurde
 * @return Punktzahl
 */
int compute_score_human(const int board[], int color, int depth, int col){
    int board_copy[BOARD_WIDTH*BOARD_HEIGHT];
    array_copy(board, board_copy);
    board_input(board_copy, color, col);
    int score = 0;
    if(depth <= 0){
        return 0;
    }
    for(Combination &com : winning_combinations){
        if(com.has_won(board_copy, color)){
            score -= 1000;
            return score;
        }
    }
    for(int i = 0; i < BOARD_WIDTH; i++){
        score += compute_score_computer(board_copy, color % 2 + 1, depth - 1, i);
    }
    return score;
}

/**
 * Berechnet den naechsten Zug des Computers
 * @param board das Spielbrett
 * @param color die Farbe des Computers
 * @param depth wie tief gesucht werden soll
 */
void compute_next_move(int board[], int color, int depth){
    int col_score[BOARD_WIDTH] = {0};
    for (int i = 0; i < BOARD_WIDTH; i++) {
        if(board[i*BOARD_HEIGHT] == 0) {
            col_score[i] = compute_score_computer(board, color, depth, i);
        }else{
            col_score[i] = -1;
        }
    }
    int max = col_score[BOARD_WIDTH/2];
    int col = BOARD_WIDTH/2;
    for(int i = 0; i < BOARD_WIDTH; i++){
        if(col_score[i] > max && col_score[i] != -1 || max == -1){
            max = col_score[i];
            col = i;
        }
    }
    board_input(board, color, col);
    std::cout << "Chose column " << col << ".\n";
}

/**
 * gibt das Spielbrett aus
 * @param board das Spielbrett
 * @return ein String der das Spielbrett repraesentiert
 */
std::string print_board(const int board[]){
    std::stringstream sstr;
    for(int y = 0; y < BOARD_HEIGHT; y++){
        for(int x = 0; x < BOARD_WIDTH; x++){
            sstr << board[x*BOARD_HEIGHT + y] << "  ";
        }
        sstr << "\n";
    }

    return sstr.str();
}


int main(){

    int board[BOARD_WIDTH * BOARD_HEIGHT] = {0};


    int count = 0;
    /// horizontale Kombinationen       ///
    for(int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH - 3; x++) {
            int col = x * BOARD_HEIGHT;
            winning_combinations[count] = Combination(col + y, col + y + BOARD_HEIGHT, col + y + 2*BOARD_HEIGHT, col + y + 3*BOARD_HEIGHT);
            count++;
        }
    }
    /// vertikale Kombinationen         ///
    for(int x = 0; x < BOARD_WIDTH; x++){
        for(int y = 0; y < BOARD_HEIGHT - 3; y++){
            int col = x * BOARD_HEIGHT;
            winning_combinations[count] = Combination(col + y, col + y + 1, col + y + 2, col + y + 3);
            count++;
        }
    }
    /// diagonale Kombinationen /       ///
    for(int x = 0; x < BOARD_WIDTH - 3; x++){
        for(int y = BOARD_HEIGHT - 1; y > 2; y--){
            int col = x * BOARD_HEIGHT;
            winning_combinations[count] = Combination(col + y, col + y + BOARD_HEIGHT - 1, col + y + 2*(BOARD_HEIGHT - 1), col + y + 3*(BOARD_HEIGHT - 1));
            count++;
        }
    }
    /// anti-diagonale Kombinationen \  ///
    for(int x = 0; x < BOARD_WIDTH - 3; x++){
        for(int y = 0; y < BOARD_HEIGHT - 3; y++){
            int col = x * BOARD_HEIGHT;
            winning_combinations[count] = Combination(col + y, col + y + BOARD_HEIGHT + 1, col + y + 2*(BOARD_HEIGHT + 1), col + y + 3*(BOARD_HEIGHT + 1));
            count++;
        }
    }

    bool is_finished = false;

    /// Das Spiel an sich. Schleife laeuft bis es einen Gewinner gibt.

    while(!is_finished){
        int col;
        std::cout << print_board(board);
        std::cout << "\nEnter column: ";
        std::cin >> col;

        board_input(board, 1, col);

        std::cout << "\ncomputing next move...\n\n";

        compute_next_move(board, 2, 6);

        for(Combination combi : winning_combinations){
            if(combi.has_won(board, 1)){
                std::cout << combi.print() << " : 1" << std::endl;
                is_finished = true;
            }else if(combi.has_won(board, 2)){
                std::cout << combi.print() << " : 2" << std::endl;
                is_finished = true;
            }
        }
    }
    std::cout << std::endl << print_board(board);
    system("pause");
}