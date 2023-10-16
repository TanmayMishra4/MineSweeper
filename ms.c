#include "ms.h"

#define NUMDIRECTIONS 8
#define MAXNEIGHBOURS '8'
#define MINNEIGHBOURS '0'

typedef struct coordinate {
    int x;
    int y;
} coordinate;
// Maybe some of your own function prototypes here
coordinate convert_to_coord(int x, int y);
bool is_valid(char ch);
bool is_rule2_condition(int num_unk, int grid_val, int num_mines);
bool is_valid_coord(coordinate c, coordinate max_limit);
void fill_string_with_null(char s[MAXSQ * MAXSQ + 1]);
int count_discovered_mines(board* b);
bool apply_rule1(board* b, int mines_discovered);
bool apply_rule2(board* b);
int count_neighbour_target(board* b, coordinate current, int target);
void fill_unknowns_mines(board* b, coordinate  current);
static inline int int_to_char(int a);
static inline int char_to_int(int a);
void test_convert_to_coord(void);
void test_is_valid_coord(void);
void test_is_valid(void);
void test_rule1(void);
void test_fill_string_with_null(void);
void test_count_discovered_mines(void);
void test_count_neighbour_target(void);
void test_fill_unknowns_mines(void);
void test_rule2(void);
void test_is_rule2_condition(void);
void test_rectangle_grid(void);

const int dir_x[NUMDIRECTIONS] = {1, 1, 0, -1, -1, -1, 0, 1}; // x coord offset for 8 directions
const int dir_y[NUMDIRECTIONS] = {0, 1, 1, 1, 0, -1, -1, -1}; // y coord offset for 8 directions

board solve_board(board b) {
    bool flag = true;
    while (flag) {
        int mines_discovered = count_discovered_mines(&b);
        bool val1 = apply_rule1(&b, mines_discovered);
        bool val2 = apply_rule2(&b);
        flag = val1 || val2;
    }
    return b;
}

void board2str(char s[MAXSQ * MAXSQ + 1], board b) {
    fill_string_with_null(s);
    int width = b.w;
    int height = b.h;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pos = i * width + j;
            coordinate cur = convert_to_coord(i, j);
            s[pos] = b.grid[cur.x][cur.y];
        }
    }
}
bool syntax_check(unsigned totmines, unsigned width, unsigned height, char inp[MAXSQ * MAXSQ + 1]) {
    unsigned total_cells = width * height;
    unsigned str_len = strlen(inp);

    if (totmines > total_cells) {
        return false;
    }
    if (str_len != total_cells) {
        return false;
    }

    int i = 0;
    while (inp[i]) {
        if (is_valid(inp[i]) == false) {
            return false;
        }
        i++;
    }

    return true;
}

board make_board(int totmines, int width, int height, char inp[MAXSQ * MAXSQ + 1]) {
    board b;
    b.w = width;
    b.h = height;
    b.totmines = totmines;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pos = i * width + j;
            coordinate cur = convert_to_coord(i, j);
            b.grid[cur.x][cur.y] = inp[pos];
        }
    }

    return b;
}
// function to check if a character in a board is valid or not
bool is_valid(char ch) {
    if (ch == UNK || ch == MINE || (ch >= MINNEIGHBOURS && ch <= MAXNEIGHBOURS)) {
        return true;
    }
    return false;
}
// function to fill string with null characters
void fill_string_with_null(char s[MAXSQ * MAXSQ + 1]) {
    for (int i = 0; i < MAXSQ * MAXSQ + 1; i++) {
        s[i] = '\0';
    }
}

int count_discovered_mines(board* b) {
    int height = b->h;
    int width = b->w;
    int count = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            coordinate cur = convert_to_coord(i, j);
            if (b->grid[cur.x][cur.y] == MINE) {
                count++;
            }
        }
    }
    return count;
}

// returns false if board was not changed by the function and true otherwise
bool apply_rule1(board* b, int mines_discovered) {
    int totmines = b->totmines;
    if (totmines != mines_discovered) {
        return false;
    }
    int height = b->h, width = b->w;

    bool flag = false; // flag to check if a value has been updated
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            coordinate cur = convert_to_coord(i, j);
            if (b->grid[cur.x][cur.y] == UNK) {
                int value_to_fill = count_neighbour_target(b, cur, MINE); // number of mines around the cell
                b->grid[cur.x][cur.y] = value_to_fill;
                flag = true;
            }
        }
    }
    return flag;
}

// returns false if board was not changed by the function and true otherwise
bool apply_rule2(board* b) {
    bool flag = false; // flag to check if a value has been updated
    int height = b->h, width = b->w;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            coordinate cur = convert_to_coord(i, j);
            if (b->grid[cur.x][cur.y] >= MINNEIGHBOURS && b->grid[cur.x][cur.y] <= MAXNEIGHBOURS) {
                int num_unk = count_neighbour_target(b, cur, UNK); // number of unknowns around the cell
                int num_mines = count_neighbour_target(b, cur, MINE); // number of mines around the cell

                if (is_rule2_condition(num_unk, b->grid[cur.x][cur.y], num_mines)) {
                    fill_unknowns_mines(b, cur);
                    flag = true;
                }
            }
        }
    }
    return flag;
}

bool is_rule2_condition(int num_unk, int grid_val, int num_mines) {
    num_unk = char_to_int(num_unk);
    num_mines = char_to_int(num_mines);
    grid_val = char_to_int(grid_val);
    if (num_unk > 0 && (grid_val ==  num_unk + num_mines)) {
        return true;
    }
    return false;
}
// checks if a coordinate is valid or not
bool is_valid_coord(coordinate c, coordinate max_limit) {
    int row = c.x;
    int col = c.y;
    int width_max = max_limit.x;
    int height_max = max_limit.y;
    if (row < 0 || row >= width_max || col < 0 || col >= height_max) {
        return false;
    }

    return true;
}
// counts neighbouring cells with cell value == target
int count_neighbour_target(board* b, coordinate current, int target) {
    int res = 0;
    for (int k = 0; k < NUMDIRECTIONS; k++) {
        int neigh_x = current.x + dir_x[k];
        int neigh_y = current.y + dir_y[k];
        coordinate neigh, max_limit;
        max_limit = convert_to_coord(b->h, b->w);
        neigh = convert_to_coord(neigh_x, neigh_y);
        if (is_valid_coord(neigh, max_limit) && b->grid[neigh.x][neigh.y] == target) {
            res++;
        }
    }
    return int_to_char(res); // converting to char
}
// fills neighbouring unknown cells with mines
void fill_unknowns_mines(board* b, coordinate current) {
    for (int k = 0; k < NUMDIRECTIONS; k++) {
        int new_x = current.x + dir_x[k];
        int new_y = current.y + dir_y[k];
        coordinate neigh, max_limit;
        neigh = convert_to_coord(new_x, new_y);
        max_limit = convert_to_coord(b->h, b->w);
        if (is_valid_coord(neigh, max_limit) && b->grid[neigh.x][neigh.y] == UNK) {
            b->grid[neigh.x][neigh.y] = MINE;
        }
    }
}

coordinate convert_to_coord(int x, int y){
    coordinate res;
    res.x = x;
    res.y = y;
    return res;
}

static inline int int_to_char(int a) {
    return ('0' + a);
}

static inline int char_to_int(int a) {
    return (a - '0');
}

// TEST Functions

void test(void) {
    test_convert_to_coord();
    test_is_valid();
    test_fill_string_with_null();
    test_is_valid_coord();
    test_count_discovered_mines();
    test_count_neighbour_target();
    test_fill_unknowns_mines();
    test_rule1();
    test_rule2();
    test_is_rule2_condition();
    test_rectangle_grid();

}

void test_convert_to_coord(void){
    coordinate test_coord = convert_to_coord(0, 0);
    assert(test_coord.x == 0);
    assert(test_coord.y == 0);

    test_coord = convert_to_coord(-1, -1);
    assert(test_coord.x == -1);
    assert(test_coord.y == -1);

    test_coord = convert_to_coord(1000, -90);
    assert(test_coord.x == 1000);
    assert(test_coord.y == -90);
}

void test_is_valid(void) {
    assert(is_valid(MINE));
    assert(is_valid(UNK));
    assert(is_valid('1'));
    assert(is_valid('0'));
    assert(is_valid('8'));

    assert(is_valid('9') == false);
    assert(is_valid(' ') == false);
    assert(is_valid('!') == false);
}

void test_fill_string_with_null(void) {
    char test_str[MAXSQ * MAXSQ + 1];
    strcpy(test_str, "");
    fill_string_with_null(test_str);
    assert(strlen(test_str) == 0);
    strcpy(test_str, "sdfgds");
    fill_string_with_null(test_str);
    assert(strlen(test_str) == 0);
    strcpy(test_str, "  !  dfgnkn ");
    fill_string_with_null(test_str);
    assert(strlen(test_str) == 0);
    strcpy(test_str, "sdfg\n");
    fill_string_with_null(test_str);
    assert(strlen(test_str) == 0);
}

void test_is_valid_coord(void) {
    coordinate test_coordinate, max_limit;
    max_limit = convert_to_coord(8, 9);
    test_coordinate = convert_to_coord(-1, 0);

    assert(is_valid_coord(test_coordinate, max_limit) == false);
    test_coordinate = convert_to_coord(0, -1);
    assert(is_valid_coord(test_coordinate, max_limit) == false);
    test_coordinate = convert_to_coord(100, 100);
    assert(is_valid_coord(test_coordinate, max_limit) == false);
    test_coordinate = convert_to_coord(-90, -12);
    assert(is_valid_coord(test_coordinate, max_limit) == false);
    test_coordinate = convert_to_coord(8, 9);
    assert(is_valid_coord(test_coordinate, max_limit) == false);

    test_coordinate = convert_to_coord(0, 0);
    assert(is_valid_coord(test_coordinate, max_limit));
    test_coordinate = convert_to_coord(7, 0);
    assert(is_valid_coord(test_coordinate, max_limit));
    test_coordinate = convert_to_coord(0, 8);
    assert(is_valid_coord(test_coordinate, max_limit));
    test_coordinate = convert_to_coord(4, 5);
    assert(is_valid_coord(test_coordinate, max_limit));
}

void test_count_discovered_mines(void) {
    char str[MAXSQ * MAXSQ + 1];
    strcpy(str, "XXXXXX535XX303XX535XX?XXX");
    board b = make_board(16, 5, 5, str);
    assert(count_discovered_mines(&b) == 15);
}

void test_count_neighbour_target(void) {
    char str[MAXSQ * MAXSQ + 1];
    strcpy(str, "1111?1111");
    board b = make_board(1, 3, 3, str);
    coordinate current = convert_to_coord(1, 1);
    assert(count_neighbour_target(&b, current, '1') == int_to_char(8));
    strcpy(str, "0322236XXX3XXX52X44X1X333");
    b = make_board(9, 5, 5, str);
    current = convert_to_coord(3, 2);
    assert(count_neighbour_target(&b, current, MINE) == int_to_char(5));
}

void test_fill_unknowns_mines(void) {
    char str[MAXSQ * MAXSQ + 1];
    strcpy(str, "0322236X??3XX?52X44?1X333");
    board b = make_board(9, 5, 5, str);
    coordinate coord_to_fill = convert_to_coord(2, 4);
    fill_unknowns_mines(&b, coord_to_fill);
    board2str(str, b);
    assert(strcmp(str, "0322236XXX3XXX52X44X1X333") == 0);
}

void test_rule1(void) {
    char str[MAXSQ * MAXSQ + 1];
    strcpy(str, "01?321?XXX2XXX43X?3X2X2?1");
    board b = make_board(9, 5, 5, str);
    int mines_disc = count_discovered_mines(&b);
    bool val = apply_rule1(&b, mines_disc);
    board2str(str, b);
    assert(strcmp(str, "0123213XXX2XXX43X53X2X211") == 0);
    assert(val == true);

    strcpy(str, "?????????");
    b = make_board(2, 3, 3, str);
    mines_disc = count_discovered_mines(&b);
    val = apply_rule1(&b, mines_disc);
    board2str(str, b);
    assert(val == false);
    assert(strcmp(str, "?????????") == 0);
}

void test_rule2(void) {
    char str[MAXSQ * MAXSQ + 1];
    strcpy(str, "0123213XXX2?XX43X53X2?211");
    board b = make_board(9, 5, 5, str);
    bool val = apply_rule2(&b);
    board2str(str, b);
    assert(strcmp(str, "0123213XXX2XXX43X53X2X211") == 0);
    assert(val == true);

    strcpy(str, "????1????");
    b = make_board(3, 3, 3, str);
    val = apply_rule2(&b);
    board2str(str, b);
    assert(val == false);
}

void test_is_rule2_condition(void) {
    assert(is_rule2_condition('3', '5', '2'));
    assert(is_rule2_condition('3', '5', '1') == false);
    assert(is_rule2_condition('1', '2', '1'));
}

void test_rectangle_grid(void){
    char str[MAXSQ*MAXSQ+1];
    strcpy(str, "0112?01X320112?");
    assert(syntax_check(3, 5, 3, str) == true);
    board b = make_board(3, 5, 3, str);
    b = solve_board(b);
    board2str(str, b);
    assert(strcmp(str, "0112X01X320112X") == 0);

    strcpy(str, "0?12X?1X??01??X");
    assert(syntax_check(3, 5, 3, str) == true);
    b = make_board(3, 5, 3, str);
    b = solve_board(b);
    board2str(str, b);
    assert(strcmp(str, "0112X01X320112X") == 0);
}
