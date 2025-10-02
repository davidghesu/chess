#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800
#define BOARD_SIZE 8
#define SQUARE_SIZE (SCREEN_WIDTH / BOARD_SIZE)

Texture2D whitePawnTexture, blackPawnTexture, 
whiteRookTexture, blackRookTexture,
whiteKnightTexture, blackKnightTexture,
whiteBishopTexture, blackBishopTexture, 
whiteQueenTexture, blackQueenTexture,
blackKingTexture, whiteKingTexture,
whiteCheckedKingTexture, blackCheckedKingTexture;

Color MYLIGHTBROWN = (Color){124, 76, 62, 255};  
Color MYDARKBROWN = (Color){88, 44, 44, 255};    

char board[BOARD_SIZE][BOARD_SIZE];
char selectedPiece = ' ';
int selectedRow = -1, selectedCol = -1;

int white_king_Row = 7, white_king_Col = 4;
int black_king_Row = 0, black_king_Col = 4;
int black_is_checked = 0, white_is_checked = 0;
float timeSinceCheck = 0.0f;
int turn = 0; //0 - alb, 1 - negru

int whiteKing_red = 0, blackKing_red = 0;


void initBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    char initialRow[] = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};

    // Piesele negre
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[0][i] = initialRow[i];  // Primul rând (majuscule)
        board[1][i] = 'P';            // Pionii (majuscule)
    }

    // Piesele albe (litere mici)
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[7][i] = initialRow[i] + 32; // Convertim la litere mici
        board[6][i] = 'p';                // Pionii negri
    }

    // Pătratele goale
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = ' ';  // Gol
        }
    }
}

void initBoardCustom(char board[BOARD_SIZE][BOARD_SIZE], char customPieces[BOARD_SIZE][BOARD_SIZE]) {
    // Iterăm prin fiecare rând și coloană pentru a pune piesele dorite
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            // Plasăm piesele pe baza tabelului customPieces
            board[i][j] = customPieces[i][j];
        }
    }
}


int white_pawn_move_check(int row, int col) {
    int ok = 0;

    // Mutarea normala
    if (col == selectedCol && row == selectedRow - 1 && board[row][col] == ' ') {
        ok = 1;
    }
    // Mutarea dubla
    if (selectedRow == 6 && row == selectedRow - 2 && col == selectedCol && board[row][col] == ' ' && board[selectedRow - 1][selectedCol] == ' ') {
        ok = 1;
    }
    // Capturarea pe diagonala
    if ((row == selectedRow - 1) && (abs(col - selectedCol) == 1) && board[row][col] != ' ' && board[row][col] != selectedPiece) {
        ok = 1;
    }
    if (ok == 0) {
        return 0; 
    }

    return 1; 
}

int black_pawn_move_check(int row, int col) {
    int ok = 0;

    // Mutarea normala
    if (col == selectedCol && row == selectedRow + 1 && board[row][col] == ' ') {
        ok = 1;
    }
    // Mutarea dubla
    if (selectedRow == 1 && row == selectedRow + 2 && col == selectedCol && board[row][col] == ' ' && board[selectedRow + 1][selectedCol] == ' ') {
        ok = 1;
    }
    // Capturarea pe diagonala
    if ((row == selectedRow + 1) && (abs(col - selectedCol) == 1) && board[row][col] != ' ' && board[row][col] != selectedPiece) {
        ok = 1;
    }
    if (ok == 0) {
        return 0; 
    }

    return 1; 
}

int white_pawn_attacks_for_check(int row, int col) {
    return (row == selectedRow - 1 && abs(col - selectedCol) == 1);
}

int black_pawn_attacks_for_check(int row, int col) {
    return (row == selectedRow + 1 && abs(col - selectedCol) == 1);
}

int white_rook_move_check(int row, int col) {
    int ok = 1;
    if (col == selectedCol) { // Mutarea pe verticala
        if (row > selectedRow) {
            for (int i = selectedRow + 1; i < row; i++)
                if (board[i][col] != ' ') {
                    ok = 0;
                    break;
                }
        } else {
            for (int i = selectedRow - 1; i > row; i--)
                if (board[i][col] != ' ') {
                    ok = 0;
                    break;
                }
                    
        }   
    } else if (row == selectedRow) { // Mutarea pe orizontala
        if (col > selectedCol) {
            for (int i = selectedCol + 1; i < col; i++)
                if (board[row][i] != ' ') {
                    ok = 0;
                    break;
                }
        } else {
            for (int i = selectedCol - 1; i > col; i--)
                if (board[row][i] != ' ') {
                    ok = 0;
                    break;
                }
           
        }
    } else { 
        ok = 0;
    }

    if (board[row][col] >= 'b' && board[row][col] <= 'r' && ok == 1) { // Capturarea
        return 1; 
    }

    if (ok == 0) {
        return 0; 
    }

    return 1; 
}

int black_rook_move_check(int row, int col) {
    int ok = 1;
    if (col == selectedCol) { // Mutarea pe verticala
        if (row > selectedRow) {
            for (int i = selectedRow + 1; i < row; i++)
                if (board[i][col] != ' ') {
                    ok = 0;
                    break;
                }
        } else {
            for (int i = selectedRow - 1; i > row; i--)
                if (board[i][col] != ' ') {
                    ok = 0;
                    break;
                }
                    
        }   
    } else if (row == selectedRow) { // Mutarea pe orizontala
        if (col > selectedCol) {
            for (int i = selectedCol + 1; i < col; i++)
                if (board[row][i] != ' ') {
                    ok = 0;
                    break;
                }
        } else {
            for (int i = selectedCol - 1; i > col; i--)
                if (board[row][i] != ' ') {
                    ok = 0;
                    break;
                }
           
        }
    } else { 
        ok = 0;
    }
    
    if (board[row][col] >= 'B' && board[row][col] <= 'R' && ok == 1) { // Capturarea
        return 1; 
    }

    if (ok == 0) {
        return 0; 
    }

    return 1; 
}

int white_knight_move_check(int row, int col) {

    int dx = abs(row - selectedRow);
    int dy = abs(col - selectedCol);

    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
        return 1;
    }

    return 0;

}

int black_knight_move_check(int row, int col) {
    int dx = abs(row - selectedRow);
    int dy = abs(col - selectedCol);

    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
        return 1;
    }

    return 0;
}

int white_bishop_move_check(int row, int col) {
    int dx = abs(row - selectedRow);
    int dy = abs(col - selectedCol);

    if (dx == dy) { 
        if (row > selectedRow && col > selectedCol) {
            for (int i = selectedRow + 1, j = selectedCol + 1; i < row && j < col; i++, j++) {
                if (board[i][j] != ' ') {
                    return 0;
                }
            }
        } else if (row > selectedRow && col < selectedCol) {
            for (int i = selectedRow + 1, j = selectedCol - 1; i < row && j > col; i++, j--) {
                if (board[i][j] != ' ') {
                    return 0;
                }
            }
        } else if (row < selectedRow && col > selectedCol) {
            for (int i = selectedRow - 1, j = selectedCol + 1; i > row && j < col; i--, j++) {
                if (board[i][j] != ' ') {
                    return 0;
                }
            }
        } else {
            for (int i = selectedRow - 1, j = selectedCol - 1; i > row && j > col; i--, j--) {
                if (board[i][j] != ' ') {
                    return 0;
                }
            }
        }
    } else {
        return 0;
    }
    
    return 1;
}

int black_bishop_move_check(int row, int col) {
    int dx = abs(row - selectedRow);
    int dy = abs(col - selectedCol);

    if (dx == dy) { 
        if (row > selectedRow && col > selectedCol) {
            for (int i = selectedRow + 1, j = selectedCol + 1; i < row && j < col; i++, j++) {
                if (board[i][j] != ' ') {
                    return 0;
                }
            }
        } else if (row > selectedRow && col < selectedCol) {
            for (int i = selectedRow + 1, j = selectedCol - 1; i < row && j > col; i++, j--) {
                if (board[i][j] != ' ') {
                    return 0;
                }
            }
        } else if (row < selectedRow && col > selectedCol) {
            for (int i = selectedRow - 1, j = selectedCol + 1; i > row && j < col; i--, j++) {
                if (board[i][j] != ' ') {
                    return 0;
                }
            }
        } else {
            for (int i = selectedRow - 1, j = selectedCol - 1; i > row && j > col; i--, j--) {
                if (board[i][j] != ' ') {
                    return 0;
                }
            }
        }
    } else {
        return 0;
    }
    
    return 1;
}

int white_queen_move_check(int row, int col) {
    if (white_bishop_move_check(row, col) == 1 || white_rook_move_check(row, col) == 1) {
        return 1;
    } else {
        return 0;
    }
}

int black_queen_move_check(int row, int col) {
    if (black_bishop_move_check(row, col) == 1 || black_rook_move_check(row, col) == 1) {
        return 1;
    } else {
        return 0;
    }
}

int white_king_move_check(int row, int col) { 
    int dx = abs(row - selectedRow);
    int dy = abs(col - selectedCol);
    
    if (dx > 1 || dy > 1) {
        return 0;
    }

    if (abs(row - black_king_Row) <= 1 && abs(col - black_king_Col) <= 1) return 0;

    return 1;
}

int black_king_move_check(int row, int col) { 
    int dx = abs(row - selectedRow);
    int dy = abs(col - selectedCol);
    
    if (dx > 1 || dy > 1) {
        return 0;
    }

    if (abs(row - white_king_Row) <= 1 && abs(col - white_king_Col) <= 1) return 0; //- pentru regi apropriati n am verif inca

    return 1;
}

int black_is_in_check() { 
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            char piece = board[i][j];

            if (piece == 'p' || piece == 'r' || piece == 'n' || piece == 'b' || piece == 'q') {
                selectedRow = i;
                selectedCol = j;

                if (piece == 'p' && white_pawn_attacks_for_check(black_king_Row, black_king_Col))
                    return 1;
                if (piece == 'r' && white_rook_move_check(black_king_Row, black_king_Col))
                    return 1;
                if (piece == 'n' && white_knight_move_check(black_king_Row, black_king_Col))
                    return 1;
                if (piece == 'b' && white_bishop_move_check(black_king_Row, black_king_Col))
                    return 1;
                if (piece == 'q' && white_queen_move_check(black_king_Row, black_king_Col))
                    return 1;
            }
        }
    }
    return 0;
}

int white_is_in_check() { 
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            char piece = board[i][j];
            
            if (piece == 'P' || piece == 'R' || piece == 'N' || piece == 'B' || piece == 'Q') {
                selectedRow = i;
                selectedCol = j;

                if (piece == 'P' && black_pawn_attacks_for_check(white_king_Row, white_king_Col))
                    return 1;
                if (piece == 'R' && black_rook_move_check(white_king_Row, white_king_Col))
                    return 1;
                if (piece == 'N' && black_knight_move_check(white_king_Row, white_king_Col))
                    return 1;
                if (piece == 'B' && black_bishop_move_check(white_king_Row, white_king_Col))
                    return 1;
                if (piece == 'Q' && black_queen_move_check(white_king_Row, white_king_Col))
                    return 1;
            }
        }
    }
    return 0;
}

int black_piece_can_move_to(char piece, int fromRow, int fromCol, int toRow, int toCol) {
    if (board[toRow][toCol] >= 'A' && board[toRow][toCol] <= 'Z') return 0;
    selectedRow = fromRow;
    selectedCol = fromCol;
    selectedPiece = piece;

    switch (piece) {
        case 'P': return black_pawn_move_check(toRow, toCol);
        case 'R': return black_rook_move_check(toRow, toCol);
        case 'B': return black_bishop_move_check(toRow, toCol);
        case 'N': return black_knight_move_check(toRow, toCol);
        case 'Q': return black_queen_move_check(toRow, toCol);
        case 'K': return black_king_move_check(toRow, toCol);
    }
    return 0;
}

int white_piece_can_move_to(char piece, int fromRow, int fromCol, int toRow, int toCol) {
    if (board[toRow][toCol] >= 'a' && board[toRow][toCol] <= 'z') return 0;
    selectedRow = fromRow;
    selectedCol = fromCol;
    selectedPiece = piece;

    switch (piece) {
        case 'p': return white_pawn_move_check(toRow, toCol);
        case 'r': return white_rook_move_check(toRow, toCol);
        case 'b': return white_bishop_move_check(toRow, toCol);
        case 'n': return white_knight_move_check(toRow, toCol);
        case 'q': return white_queen_move_check(toRow, toCol);
        case 'k': return white_king_move_check(toRow, toCol);
    }
    return 0;
}

int black_is_in_check_mate() {
    if (!black_is_in_check()) return 0;

    for (int fromRow = 0; fromRow < 8; fromRow++) {
        for (int fromCol = 0; fromCol < 8; fromCol++) {
            char piece = board[fromRow][fromCol];
            if (piece < 'A' || piece > 'Z') continue;

            for (int toRow = 0; toRow < 8; toRow++) {
                for (int toCol = 0; toCol < 8; toCol++) {
                    if (!black_piece_can_move_to(piece, fromRow, fromCol, toRow, toCol)) continue;

                    char dst = board[toRow][toCol];
                    char src = board[fromRow][fromCol];
                    int kingRow = black_king_Row, kingCol = black_king_Col;

                    board[toRow][toCol] = src;
                    board[fromRow][fromCol] = ' ';
                    if (src == 'K') {
                        black_king_Row = toRow;
                        black_king_Col = toCol;
                    }

                    int safe = !black_is_in_check();

                    board[fromRow][fromCol] = src;
                    board[toRow][toCol] = dst;
                    black_king_Row = kingRow;
                    black_king_Col = kingCol;

                    if (safe) return 0;
                }
            }
        }
    }
    return 1;
}

int white_is_in_check_mate() {
    if (!white_is_in_check()) return 0;

    for (int fromRow = 0; fromRow < 8; fromRow++) {
        for (int fromCol = 0; fromCol < 8; fromCol++) {
            char piece = board[fromRow][fromCol];
            if (piece < 'a' || piece > 'z') continue;

            for (int toRow = 0; toRow < 8; toRow++) {
                for (int toCol = 0; toCol < 8; toCol++) {
                    if (!white_piece_can_move_to(piece, fromRow, fromCol, toRow, toCol)) continue;

                    char dst = board[toRow][toCol];
                    char src = board[fromRow][fromCol];
                    int kingRow = white_king_Row, kingCol = white_king_Col;

                    board[toRow][toCol] = src;
                    board[fromRow][fromCol] = ' ';
                    if (src == 'k') {
                        white_king_Row = toRow;
                        white_king_Col = toCol;
                    }

                    int safe = !white_is_in_check();

                    board[fromRow][fromCol] = src;
                    board[toRow][toCol] = dst;
                    white_king_Row = kingRow;
                    white_king_Col = kingCol;

                    if (safe) return 0;
                }
            }
        }
    }
    return 1;
}

void handleMouseClick() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        int row = mousePos.y / SQUARE_SIZE;
        int col = mousePos.x / SQUARE_SIZE;

        if (selectedPiece == ' ') { // nu avem nimic selectat -> selectam pion
            if ((turn == 0 && (board[row][col] == 'p' || board[row][col] == 'r' || board[row][col] == 'n' || board[row][col] == 'b' || board[row][col] == 'q' || board[row][col] == 'k')) || 
                (turn == 1 && (board[row][col] == 'P' || board[row][col] == 'R' || board[row][col] == 'N' || board[row][col] == 'B' || board[row][col] == 'Q' || board[row][col] == 'K'))) { 
                selectedPiece = board[row][col];
                selectedRow = row;
                selectedCol = col;
                //aici desen cerc
            }
        } else { // daca avem pion selectat, il mutăm

            if (((board[row][col] >= 'b' && board[row][col] <= 'r') && turn == 0) || ((board[row][col] >= 'B' && board[row][col] <= 'R') && turn == 1)) {
                selectedPiece = board[row][col];
                selectedRow = row;
                selectedCol = col;
                return; // nu incercam sa facem mutarea înca
            }
            
            if (turn == 0) {
                if ((selectedPiece == 'p' && white_pawn_move_check(row, col) == 1) ||
                    (selectedPiece == 'r' && white_rook_move_check(row, col) == 1) ||
                    (selectedPiece == 'n' && white_knight_move_check(row, col) == 1) ||
                    (selectedPiece == 'b' && white_bishop_move_check(row, col) == 1) ||
                    (selectedPiece == 'q' && white_queen_move_check(row, col) == 1) ||
                    (selectedPiece == 'k' && white_king_move_check(row, col) == 1)) {

                    // salveaza in caz ca mutarea era ilegala adica e pinned
                    char capturedPiece = board[row][col];
                    char movedPiece = selectedPiece;
                    int oldRow = selectedRow;
                    int oldCol = selectedCol;
                    int oldKingRow = white_king_Row;
                    int oldKingCol = white_king_Col;

                    board[row][col] = selectedPiece; // noua pozitie
                    board[selectedRow][selectedCol] = ' '; // golim vechea pozitie
                    if (selectedPiece == 'k') { 
                        white_king_Row = row;
                        white_king_Col = col;
                    }
                    //verif daca mutarea e legala
                    if (white_is_in_check() == 0) {
                        turn = 1;
                        selectedPiece = ' '; // resetam selectia
            
                        if (black_is_in_check() == 1) {
                            blackKing_red = 1;

                            if (black_is_in_check_mate()) {
                                printf("mat");
                            }
                        }
                        if (white_is_in_check() == 0) {
                            whiteKing_red = 0;
                        }

                    } else {
                        // mutarea e ilegala - anulam mutarea
                        board[oldRow][oldCol] = movedPiece;
                        board[row][col] = capturedPiece;
                        white_king_Row = oldKingRow;
                        white_king_Col = oldKingCol;
                        selectedPiece = ' ';
                    }

                } else {
                    selectedPiece = ' ';
                }
            } else { // turn == 1
                if ((selectedPiece == 'P' && black_pawn_move_check(row, col) == 1) ||
                    (selectedPiece == 'R' && black_rook_move_check(row, col) == 1) ||
                    (selectedPiece == 'N' && black_knight_move_check(row, col) == 1) ||
                    (selectedPiece == 'B' && black_bishop_move_check(row, col) == 1) ||
                    (selectedPiece == 'Q' && black_queen_move_check(row, col) == 1) ||
                    (selectedPiece == 'K' && black_king_move_check(row, col) == 1)) {
                    
                    // salveaza in caz ca mutarea era ilegala adica e pinned
                    char capturedPiece = board[row][col];
                    char movedPiece = selectedPiece;
                    int oldRow = selectedRow;
                    int oldCol = selectedCol;
                    int oldKingRow = black_king_Row;
                    int oldKingCol = black_king_Col;

                    board[row][col] = selectedPiece; // noua pozitie
                    board[selectedRow][selectedCol] = ' '; // golim vechea pozitie
                    if (selectedPiece == 'K') { 
                        black_king_Row = row;
                        black_king_Col = col;
                    }
                    //verif daca e legala mutarea
                    if (black_is_in_check() == 0) {
                        turn = 0;
                        selectedPiece = ' '; // resetam selectia
            
                        if (white_is_in_check() == 1) {
                            whiteKing_red = 1;

                            if (white_is_in_check_mate()) {
                                printf("mat");
                            }
                        }
                        if (black_is_in_check() == 0) {
                            blackKing_red = 0;
                        }
                    } else {
                        // mutarea e ilegala - anulam mutarea
                        board[oldRow][oldCol] = movedPiece;
                        board[row][col] = capturedPiece;
                        black_king_Row = oldKingRow;
                        black_king_Col = oldKingCol;
                        selectedPiece = ' ';
                    }

                } else {
                    selectedPiece = ' ';
                }
            }
        }
    }
}


int main() {

    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SAH");
    SetTargetFPS(60);

    { // LoadTexture
        whitePawnTexture = LoadTexture("piese/white_pawn.png");
        blackPawnTexture = LoadTexture("piese/black_pawn.png");
        whiteRookTexture = LoadTexture("piese/white_rook.png");
        blackRookTexture = LoadTexture("piese/black_rook.png");
        whiteKnightTexture = LoadTexture("piese/white_knight.png");
        blackKnightTexture = LoadTexture("piese/black_knight.png");
        whiteBishopTexture = LoadTexture("piese/white_bishop.png");
        blackBishopTexture = LoadTexture("piese/black_bishop.png");
        whiteQueenTexture = LoadTexture("piese/white_queen.png");
        blackQueenTexture = LoadTexture("piese/black_queen.png");
        whiteKingTexture = LoadTexture("piese/white_king.png");
        blackKingTexture = LoadTexture("piese/black_king.png");
        whiteCheckedKingTexture = LoadTexture("piese/white_king_checked.png");
        blackCheckedKingTexture = LoadTexture("piese/black_king_checked.png");
    }

    char customPieces[BOARD_SIZE][BOARD_SIZE] = {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', 'K', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'k', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'q', 'q', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
    };

    initBoard(board);//, customPieces); 
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
       
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                // Alternam culorile
                Color squareColor = ((i + j) % 2 == 0) ? MYLIGHTBROWN : MYDARKBROWN;
                DrawRectangle(j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, squareColor);
                
                char piece = board[i][j];

                switch (piece) {
                    case 'P':
                        DrawTextureEx(blackPawnTexture, (Vector2){ j * SQUARE_SIZE + 22.0f, i * SQUARE_SIZE + 18.0f}, 0.0f, 0.07f, WHITE);
                        break;
            
                    case 'R':
                        DrawTextureEx(blackRookTexture, (Vector2){ j * SQUARE_SIZE + 14.0f, i * SQUARE_SIZE + 15.0f}, 0.0f, 0.08f, WHITE);
                        break;
            
                    case 'N':
                        DrawTextureEx(blackKnightTexture, (Vector2){ j * SQUARE_SIZE + 9.0f, i * SQUARE_SIZE + 10.0f}, 0.0f, 0.086f, WHITE);
                        break;
            
                    case 'B':
                        DrawTextureEx(blackBishopTexture, (Vector2){ j * SQUARE_SIZE + 9.0f, i * SQUARE_SIZE + 10.0f}, 0.0f, 0.086f, WHITE);
                        break;
            
                    case 'Q':
                        DrawTextureEx(blackQueenTexture, (Vector2){ j * SQUARE_SIZE + 4.0f, i * SQUARE_SIZE + 10.0f}, 0.0f, 0.086f, WHITE);
                        break;
            
                    case 'K':
                        if (blackKing_red == 1) {
                            DrawTextureEx(blackCheckedKingTexture, (Vector2){ j * SQUARE_SIZE + 8.0f, i * SQUARE_SIZE + 8.0f}, 0.0f, 0.09f, WHITE);
                        } else {
                            DrawTextureEx(blackKingTexture, (Vector2){ j * SQUARE_SIZE + 8.0f, i * SQUARE_SIZE + 8.0f}, 0.0f, 0.09f, WHITE);
                        }
                        break;

                    case 'p':
                        DrawTextureEx(whitePawnTexture, (Vector2){ j * SQUARE_SIZE + 22.0f, i * SQUARE_SIZE + 18.0f}, 0.0f, 0.07f, WHITE);
                        break;
            
                    case 'r':
                        DrawTextureEx(whiteRookTexture, (Vector2){ j * SQUARE_SIZE + 14.0f, i * SQUARE_SIZE + 15.0f}, 0.0f, 0.08f, WHITE);
                        break;
            
                    case 'n':
                        DrawTextureEx(whiteKnightTexture, (Vector2){ j * SQUARE_SIZE + 9.0f, i * SQUARE_SIZE + 10.0f}, 0.0f, 0.086f, WHITE);
                        break;
            
                    case 'b':
                        DrawTextureEx(whiteBishopTexture, (Vector2){ j * SQUARE_SIZE + 9.0f, i * SQUARE_SIZE + 10.0f}, 0.0f, 0.086f, WHITE);
                        break;
            
                    case 'q':
                        DrawTextureEx(whiteQueenTexture, (Vector2){ j * SQUARE_SIZE + 4.0f, i * SQUARE_SIZE + 10.0f}, 0.0f, 0.086f, WHITE);
                        break;
            
                    case 'k':
                        if (whiteKing_red == 1) { 
                            DrawTextureEx(whiteCheckedKingTexture, (Vector2){ j * SQUARE_SIZE + 8.0f, i * SQUARE_SIZE + 8.0f}, 0.0f, 0.09f, WHITE);                         
                        } else {
                            DrawTextureEx(whiteKingTexture, (Vector2){ j * SQUARE_SIZE + 8.0f, i * SQUARE_SIZE + 8.0f}, 0.0f, 0.09f, WHITE);
                        }
                        break;
            
                    default:
                        // Dacă piesa nu este validă, nu facem nimic
                        break;
                }
            
              
            }
        }
        handleMouseClick();

        EndDrawing();
    
    }

    { // UnloadTexture
        UnloadTexture(whitePawnTexture);
        UnloadTexture(blackPawnTexture);
        UnloadTexture(whiteRookTexture);
        UnloadTexture(blackRookTexture);
        UnloadTexture(whiteKnightTexture);
        UnloadTexture(blackKnightTexture);
        UnloadTexture(whiteBishopTexture);
        UnloadTexture(blackBishopTexture);
        UnloadTexture(whiteQueenTexture);
        UnloadTexture(blackQueenTexture);
        UnloadTexture(whiteKingTexture);
        UnloadTexture(blackKingTexture);
        UnloadTexture(whiteCheckedKingTexture);
        UnloadTexture(blackCheckedKingTexture);
    }

    CloseWindow();
    return 0;
}


