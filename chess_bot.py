import sys
import chess
import chess.engine

board = chess.Board()
engine_path = "C:/Projects/stockfish/stockfish-windows-x86-64-avx2.exe"
engine = chess.engine.SimpleEngine.popen_uci(engine_path)

def handle_command(cmd):
    global board
    cmd = cmd.strip()

    if cmd == "quit":
        return "BYE"

    if len(cmd) >= 4 and chess.Move.from_uci(cmd[:4]) in board.legal_moves:
        move = chess.Move.from_uci(cmd)
        board.push(move)

        if board.is_game_over():
            return "GAME_OVER"

        result = engine.play(board, chess.engine.Limit(time=1.0))
        bot_move = result.move
        board.push(bot_move)
        return str(bot_move)
    else:
        return "INVALID"

try:
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        response = handle_command(line)
        print(response)
        sys.stdout.flush()
        if response == "BYE":
            break
finally:
    engine.quit()
