#ifndef UTILS_H
#define UTILS_H

void print_board(Piece *board, int length);
int algebraic_to_index(char *position);
void index_to_algebraic(int index, char *out);

#endif