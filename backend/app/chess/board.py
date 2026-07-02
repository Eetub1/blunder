def parse_fen(fen: str) -> list[list[str]]:
    """Parse a FEN string into a 2D board.

    Args:
        fen: A full FEN string. Only the first (piece-placement) field is used.

    Returns:
        An 8x8 grid where board[0] is rank 8 (top from White's view).
        Pieces are FEN chars ('K', 'p', ...); empty squares are "".
    """
    allowed = set("pnbrqk")

    placement = fen.split(" ")[0]
    rows = placement.split("/")

    if len(rows) != 8:
        raise ValueError(f"FEN has {len(rows)} ranks, expected 8")

    board = [[] for _ in range(8)]

    for i in range(8):
        row = rows[i]
        for c in row:
            if c.isdigit():
                for _ in range(int(c)):
                    board[i].append("")
            else:
                if c.lower() not in allowed:
                    raise ValueError(f"Character {c} not allowed in a FEN string")

                board[i].append(c)
    return board


def to_fen(board: list[list[str]]) -> str:
    """Serialize a board back into a FEN piece-placement field.

    Args:
        board: An 8x8 grid as produced by parse_fen.

    Returns:
        The piece-placement field only (the part before the first space).
    """
    if len(board) != 8:
        raise ValueError(f"Board should have 8 rows, now it has: {len(board)} rows")

    fen = []

    for row in board:
        if len(row) != 8:
            raise ValueError(f"Board row should have 8 cells, now it has {len(row)} cells")

        fen_row = ""

        empty = 0
        for cell in row:
            if cell == "":
                empty += 1
            else:
                if empty:
                    fen_row += (str(empty) + cell)
                else:
                    fen_row += cell
                empty = 0
        if empty:
            fen_row += str(empty)

        fen.append(fen_row)
    return "/".join(fen)


def flip_turn(fen: str) -> str:
    fen_parts = fen.split(" ")
    fen_parts[1] = "b" if fen_parts[1] == "w" else "w"
    return " ".join(fen_parts)


def algebraic_to_indices(position: str) -> list[int]:
    """Convert an algebraic square like "e2" into (row, col) board indices.

    Args:
        position: A two-character algebraic square — a file letter "a"-"h"
            followed by a rank digit "1"-"8", e.g. "e2".

    Returns:
        A [row, col] pair for a board where board[0] is rank 8.
        For example, "e2" -> [6, 4] (rank 2 maps to row 6 because of the flip).

    Raises:
        ValueError: If position is not exactly two characters, the file is
            not "a"-"h", or the rank is not a valid digit.
    """
    if len(position) != 2:
        raise ValueError(f"Algebraic cell notation should have length 2, now it has {len(position)}")

    lut = {"a": 0, "b": 1, "c": 2, "d": 3, "e": 4, "f": 5, "g": 6, "h": 7}

    col = position[0]
    row = position[1]

    if col not in lut:
        raise ValueError(f"Value {col} not allowed in algebraic notation")
    if not row.isdigit():
        raise ValueError(f"Value {row} is not a digit")
    if int(row) < 1 or int(row) > 8:
        raise ValueError(f"Value {row} should be between 1-8")

    return [8 - int(row), lut[col]] # row, col


def indices_to_algebraic(indices: list[int]) -> str:
    """Convert [row, col] board indices into an algebraic square like "e2".

    The inverse of algebraic_to_indices. Undoes the rank flip: row 0 is rank 8
    (top from White's view), so the rank digit is 8 - row.

    Args:
        indices: A [row, col] pair for a board where board[0] is rank 8.

    Returns:
        The square in algebraic notation, e.g. [6, 4] -> "e2".
    """
    row, col = indices
    lut = {0: "a", 1: "b", 2: "c", 3: "d", 4: "e", 5: "f", 6: "g", 7: "h"}
    return lut[col] + str(8 - row)


def apply_move(board: list[list[str]], from_square: str, to_square: str) -> list[list[str]]:
    """Move a piece from one square to another. No legality checking. Copies the board

    Args:
        board: The current board.
        from_square: Algebraic origin, e.g. "e2".
        to_square: Algebraic destination, e.g. "e4".

    Returns:
        The resulting board.
    """
    board_copy = [[] for _ in range(8)]

    for i in range(len(board)):
        row = board_copy[i]
        for j in range(len(board[0])):
            row.append(board[i][j])

    from_row, from_col = algebraic_to_indices(from_square)
    to_row, to_col = algebraic_to_indices(to_square)

    board_copy[to_row][to_col] = board_copy[from_row][from_col]
    board_copy[from_row][from_col] = ""

    return board_copy