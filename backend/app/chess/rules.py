from app.chess.board import apply_move, indices_to_algebraic, algebraic_to_indices
from app.chess.moves import calculate_moves, is_position_inbounds
from enum import Enum

class GameState(Enum):
    ONGOING = "ongoing"
    CHECKMATE = "checkmate"
    STALEMATE = "stalemate"
    DRAW = "draw"

def find_king(board: list[list[str]], is_white: bool) -> list[int]:
    target = "K" if is_white else "k"

    for i in range(len(board)):
        for j in range(len(board[0])):
            if board[i][j] == target:
                return [i, j]

    raise ValueError("Couldn't find the given side's king on board")


def is_square_attacked(board: list[list[str]], square: list[int], by_white: bool) -> bool:
    t_row, t_col = square

    for row in range(len(board)):
        for col in range(len(board[0])):
            contains = board[row][col]

            if (not contains
                or (not by_white and contains.upper() == contains) 
                or (by_white and contains.lower() == contains)):
                continue

            if contains.lower() == "p": # Handle pawn separately
                offsets = [(-1, 1), (-1, -1)] if by_white else [(1, 1), (1, -1)]

                valid_squares = []

                # Check if pawn can attack left and right
                for dy, dx in offsets:
                    n_row = row + dy
                    n_col = col + dx
                    if is_position_inbounds([n_row, n_col]):
                        valid_squares.append(indices_to_algebraic([n_row, n_col]))
            else:
                valid_squares = calculate_moves(board, indices_to_algebraic([row, col]))
            
            if indices_to_algebraic([t_row, t_col]) in valid_squares:
                return True
    return False


def is_in_check(board: list[list[str]], is_white: bool) -> bool:
    location = find_king(board, is_white)
    return is_square_attacked(board, location, not is_white)


def is_move_legal(board: list[list[str]], from_square: str, to_square: str, is_white: bool) -> bool:
    new_board = apply_move(board, from_square, to_square)
    return not is_in_check(new_board, is_white)


def calculate_legal_moves(board: list[list[str]], position: str, is_whites_turn: bool, en_passant: str ="-", castling_rights: str = "-") -> list[str]:
    row, col = algebraic_to_indices(position)
    piece = board[row][col]

    if not piece:  # ignore empty squares
        return []

    is_white = piece == piece.upper()

    # if it is whites turn and the piece is not white, return empty
    # and vice versa for black
    if is_whites_turn != is_white:
        return []

    # filter all illegal moves away
    all_moves = calculate_moves(board, position, en_passant, castling_rights)
    legal_moves = [move for move in all_moves if is_move_legal(board, position, move, is_white)]

    # filtering castling out of check or through a check
    # Castling into a check is already dealt with automatically
    if board[row][col].lower() == "k": # if the moving piece is a king
        castle_legal_moves = []

        for move in legal_moves:
            _, to_col = algebraic_to_indices(move)
            col_difference = abs(col - to_col)
            if col_difference == 2: # its a castling move

                # can't castle out of a check
                if is_in_check(board, is_whites_turn):
                    continue

                # can't castle through a check
                if to_col > col: # kingside
                    if is_whites_turn:
                        if is_square_attacked(board, algebraic_to_indices("f1"), False):
                            continue
                    else:
                        if is_square_attacked(board, algebraic_to_indices("f8"), True):
                            continue
                elif to_col < col: # queenside
                    if is_whites_turn:
                        if is_square_attacked(board, algebraic_to_indices("d1"), False):
                            continue
                    else:
                        if is_square_attacked(board, algebraic_to_indices("d8"), True):
                            continue
                            
                castle_legal_moves.append(move)
            else:
                castle_legal_moves.append(move)

        legal_moves = castle_legal_moves
    return legal_moves


def is_stalemate_or_checkmate(board: list[list[str]], is_whites_turn: bool) -> GameState:
    # Does the side to move have any legal moves
    for i in range(len(board)):
        for j in range(len(board[0])):
            content = board[i][j]
            if not content:
                continue

            is_white_piece = content == content.upper()

            # only consider pieces belonging to the side to move
            if is_whites_turn == is_white_piece:
                legal_moves = calculate_legal_moves(board, indices_to_algebraic([i, j]), is_whites_turn)
                if legal_moves:
                    return GameState.ONGOING

    is_check = is_in_check(board, is_whites_turn)
    if is_check:
        return GameState.CHECKMATE
    return GameState.STALEMATE