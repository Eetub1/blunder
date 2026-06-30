from app.chess.board import indices_to_algebraic, algebraic_to_indices
from enum import Enum

class CellContentType(Enum):
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


def calculate_moves(board: list[list[str]], position: str) -> list[str]:
    """Return the pseudo-legal moves for whatever piece sits on `position`.

    Dispatches to a per-piece generator based on the piece type. Does not
    consider checks. Returns an empty list if the square is empty.

    Args:
        board: The current board.
        position: The square to generate moves from, in algebraic notation.

    Returns:
        A list of destination squares in algebraic notation, e.g. ["c3", "e4"].
    """
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
            possibilities = calculate_stepping_moves(board, [row, col], is_white, KING_QUEEN_DIRECTIONS)
        case "p":
            possibilities = calculate_pawn_moves(board, [row, col], is_white)
    return possibilities


def square_state(board: list[list[str]], position: list[int], is_white: bool) -> CellContentType:
    """Classify a target square relative to the moving piece's color.

    Args:
        board: The current board.
        position: The [row, col] of the square to classify.
        is_white: True if the moving piece is white.

    Returns:
        EMPTY, FRIEND, or ENEMY depending on the square's contents.
    """
    row, col = position
    cell_content = board[row][col]

    if cell_content == "":
        return CellContentType.EMPTY

    if is_white:
        if cell_content == cell_content.lower():
            return CellContentType.ENEMY
        else:
            return CellContentType.FRIEND
    else:
        if cell_content == cell_content.upper():
            return CellContentType.ENEMY
        else:
            return CellContentType.FRIEND


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


def calculate_pawn_moves(board: list[list[str]], position: list[int], is_white: bool) -> list[str]:
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

    return valid_squares