import sys
import chess

board = chess.Board()

def is_valid_move(move_str):
    try:
        if board.is_checkmate():
            print("LOSS", flush=True)
            sys.exit(0)

        move = chess.Move.from_uci(move_str)
        # if move in board.pseudo_legal_moves and board.is_check:
        #     temp_board1 = board.copy()
        #     temp_board1.push(move)
        #     if temp_board1.is_check() and temp_board1.turn == board.turn:
        #         print("LOSS", flush=True)
        #         sys.exit(0)
        #     return True



        if move in board.legal_moves:

            # Сделаем ход на копии доски
            temp_board = board.copy()
            temp_board.push(move)


            if temp_board.is_check() and temp_board.turn == board.turn:
                print("LOSS", flush=True)
                sys.exit(0)

            board.push(move)
            return True
    except:
        pass
    return False

def handle_command(command):
    if command.startswith('load_fen'):
        fen_string = command.split(' ', 1)[1]
        board.set_fen(fen_string)
        print("OK", flush=True)

for line in sys.stdin:
    line = line.strip()
    if line == "quit":
        break

    if line.startswith("load_fen"):
        handle_command(line)
    elif is_valid_move(line):
        print("OK", flush=True)
    else:
        print("INVALID", flush=True)

