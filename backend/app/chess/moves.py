"""Pseudo-legal move generation for chess pieces.

"Pseudo-legal" means these functions return all moves a piece can geometrically
make, WITHOUT checking whether the move leaves its own king in check. Filtering
those down to fully-legal moves happens in rules.py, which builds on this module.

Board representation: an 8x8 list of lists where board[0] is rank 8 (top from
White's view). Pieces are FEN characters ('K', 'p', ...); empty squares are "".
Functions take squares as [row, col] index pairs internally and return them to
callers as algebraic strings (e.g. "e4").
"""

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
    """Generate pseudo-legal moves for a sliding piece (rook, bishop, queen).

    Steps repeatedly along each direction until blocked: it glides over empty
    squares, captures the first enemy it meets (then stops), and stops before a
    friendly piece. The piece type is determined entirely by `directions`.

    Args:
        board: The current board.
        position: The piece's square as [row, col].
        is_white: True if the piece is white.
        directions: (row_delta, col_delta) pairs to slide along, e.g.
            ROOK_DIRECTIONS for a rook.

    Returns:
        Destination squares in algebraic notation.
    """
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
    """Generate pseudo-legal moves for a single-step piece (knight, king).

    Tries each direction exactly once (no sliding). A target is valid unless it
    is off-board or holds a friendly piece. The piece type is determined
    entirely by `directions`.

    Args:
        board: The current board.
        position: The piece's square as [row, col].
        is_white: True if the piece is white.
        directions: (row_delta, col_delta) offsets to try once each, e.g.
            KNIGHT_DIRECTIONS for a knight.

    Returns:
        Destination squares in algebraic notation.
    """
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
    """Generate pseudo-legal pawn moves from `position`.

    Pawns are the one piece whose movement and capture rules differ, and which
    depend on color for direction:
      - move one square straight forward, only if that square is EMPTY;
      - move two squares forward, only from the starting rank and only if BOTH
        squares ahead are empty;
      - capture one square diagonally forward, only if that square holds an ENEMY.
    White moves toward row 0 (up); black moves toward row 7 (down).

    Does NOT yet handle promotion or en passant (both deferred: promotion belongs
    to move application, en passant needs move history).

    Args:
        board: The current board.
        position: The pawn's square as [row, col].
        is_white: True if the pawn is white.

    Returns:
        Destination squares in algebraic notation.
    """
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