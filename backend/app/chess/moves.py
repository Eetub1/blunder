from app.chess.board import indices_to_algebraic, algebraic_to_indices
from enum import Enum

class CellContentType(Enum):
    """How a target square relates to the piece being moved."""
    EMPTY = "empty"
    FRIEND = "friend"
    ENEMY = "enemy"

KNIGHT_DIRECTIONS = [(-2, -1), (-2, 1), (-1, -2), (-1, 2), (1, -2),  (1, 2), (2, -1),  (2, 1)]

ROOK_DIRECTIONS = [(1, 0), (-1, 0), (0, 1), (0, -1)]

BISHOP_DIRECTIONS = [(1, 1), (-1, -1), (1, -1), (-1, 1)]

# King and Queen share same directions but the Queen can move farther in every direction
KING_QUEEN_DIRECTIONS = [(1, 1), (-1, -1), (1, -1), (-1, 1), (1, 0), (-1, 0), (0, 1), (0, -1)]

def is_position_inbounds(position: list[int]) -> bool:
    """Return True if [row, col] falls within the 8x8 board."""
    row, col = position
    if row < 0 or row >= 8 or col < 0 or col >= 8:
        return False
    return True


def calculate_moves(board: list[list[str]], position: str, en_passant: str = "-", castling_rights: str = "-") -> list[str]:
    row, col = algebraic_to_indices(position)
    piece = board[row][col]

    if not piece:  # ignore empty squares
        return []

    is_white = piece == piece.upper()

    possibilities = []
    match piece.lower():
        case "r":
            possibilities = calculate_sliding_moves(board, [row, col], is_white, ROOK_DIRECTIONS)
        case "n":
            possibilities = calculate_stepping_moves(board, [row, col], is_white, KNIGHT_DIRECTIONS)
        case "b":
            possibilities = calculate_sliding_moves(board, [row, col], is_white, BISHOP_DIRECTIONS)
        case "q":
            possibilities = calculate_sliding_moves(board, [row, col], is_white, KING_QUEEN_DIRECTIONS)
        case "k":
            possibilities = calculate_king_moves(board, [row, col], is_white, castling_rights)
        case "p":
            possibilities = calculate_pawn_moves(board, [row, col], is_white, en_passant)
    return possibilities


def square_state(board: list[list[str]], position: list[int], is_white: bool) -> CellContentType:
    row, col = position
    cell_content = board[row][col]

    if cell_content == "":
        return CellContentType.EMPTY

    if (is_white and cell_content == cell_content.lower() or 
        not is_white and cell_content == cell_content.upper()):
        return CellContentType.ENEMY
    return CellContentType.FRIEND


def calculate_king_moves(board: list[list[str]], position: list[int], is_white: bool, castling_rights: str) -> list[str]:
    # calculate where the king can move normally
    valid_squares = calculate_stepping_moves(board, position, is_white, KING_QUEEN_DIRECTIONS)

    if castling_rights == "-":
        return valid_squares

    if is_white:
        # kingside castling for white
        if "K" in castling_rights:
            f1 = square_state(board, algebraic_to_indices("f1"), is_white)
            g1 = square_state(board, algebraic_to_indices("g1"), is_white)
            if f1 == g1 and g1 == CellContentType.EMPTY:
                valid_squares.append("g1")

        # queenside
        if "Q" in castling_rights:
            b1 = square_state(board, algebraic_to_indices("b1"), is_white)
            c1 = square_state(board, algebraic_to_indices("c1"), is_white)
            d1 = square_state(board, algebraic_to_indices("d1"), is_white)
            if b1 == c1 and c1 == d1 and d1 == CellContentType.EMPTY:
                valid_squares.append("c1")

    else:
        # kingside castling for black
        if "k" in castling_rights:
            f8 = square_state(board, algebraic_to_indices("f8"), is_white)
            g8 = square_state(board, algebraic_to_indices("g8"), is_white)
            if f8 == g8 and g8 == CellContentType.EMPTY:
                valid_squares.append("g8")

        # queenside
        if "q" in castling_rights:
            b8 = square_state(board, algebraic_to_indices("b8"), is_white)
            c8 = square_state(board, algebraic_to_indices("c8"), is_white)
            d8 = square_state(board, algebraic_to_indices("d8"), is_white)
            if b8 == c8 and c8 == d8 and d8 == CellContentType.EMPTY:
                valid_squares.append("c8")
    return valid_squares


def calculate_sliding_moves(board: list[list[str]], position: list[int], is_white: bool, directions: list[tuple[int, int]]) -> list[str]:
    row, col = position

    valid_squares = []
    for dy, dx in directions:
        n_row, n_col = row + dy, col + dx
        while is_position_inbounds([n_row, n_col]):
            content = square_state(board, [n_row, n_col], is_white)

            if content == CellContentType.FRIEND:
                break

            valid_squares.append(indices_to_algebraic([n_row, n_col]))

            if content != CellContentType.EMPTY:
                break
            n_row, n_col = n_row + dy, n_col + dx
    return valid_squares


def calculate_stepping_moves(board: list[list[str]], position: list[int], is_white: bool, directions: list[tuple[int, int]]) -> list[str]:
    row, col = position

    valid_squares = []
    for dy, dx in directions:
        n_row, n_col = row + dy, col + dx
        if not is_position_inbounds([n_row, n_col]):
            continue

        content = square_state(board, [n_row, n_col], is_white)
        if content != CellContentType.FRIEND:
            valid_squares.append(indices_to_algebraic([n_row, n_col]))
    return valid_squares


def calculate_pawn_moves(board: list[list[str]], position: list[int], is_white: bool, en_passant: str) -> list[str]:
    row, col = position
    valid_squares = []

    #                   1         2      left      right
    white_offsets = [(-1, 0), (-2, 0), (-1, -1), (-1, 1)]
    black_offsets = [(1, 0),  (2, 0),  (1, -1),  (1, 1)]
    start_row = -1

    if is_white:
        offsets = white_offsets
        start_row = 6
    else:
        offsets = black_offsets
        start_row = 1

    n_row = row + offsets[0][0]
    if is_position_inbounds([n_row, col]):
        # can pawn move 1 square ahead
        if square_state(board, [n_row, col], is_white) == CellContentType.EMPTY:
            valid_squares.append(indices_to_algebraic([n_row, col]))

            # can pawn move 2 squares ahead
            if row == start_row:
                n_row = row + offsets[1][0]
                if square_state(board, [n_row, col], is_white) == CellContentType.EMPTY:
                    valid_squares.append(indices_to_algebraic([n_row, col]))
    
    # can pawn capture left
    n_row = row + offsets[2][0]
    n_col = col + offsets[2][1]

    if is_position_inbounds([n_row, n_col]):
        if square_state(board, [n_row, n_col], is_white) == CellContentType.ENEMY:
            valid_squares.append(indices_to_algebraic([n_row, n_col]))

    # can pawn capture right
    n_row = row + offsets[3][0]
    n_col = col + offsets[3][1]
    if is_position_inbounds([n_row, n_col]):
        if square_state(board, [n_row, n_col], is_white) == CellContentType.ENEMY:
            valid_squares.append(indices_to_algebraic([n_row, n_col]))

    # check if en passant is possible
    if en_passant != "-":
        target_row, target_col = algebraic_to_indices(en_passant)
        
        # en passant target square is always either in row 2 or 5
        if target_row == 2 and is_white:
            if (row - 1 == target_row and col - 1 == target_col or 
                row - 1 == target_row and col + 1 == target_col):
                valid_squares.append(en_passant)

        if target_row == 5 and not is_white:
            if (row + 1 == target_row and col - 1 == target_col or 
                row + 1 == target_row and col + 1 == target_col):
                valid_squares.append(en_passant)

    return valid_squares