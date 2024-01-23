#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <cstdlib>

using namespace std;


void get_random_directions(int **a, int x, int length, int index, int coordinate, int place) {
    srand(time(NULL));
    int diff1, diff2, new_directions;
    if (x < 1) {
        if (coordinate > 1) {
            diff1 = coordinate - 1;
            diff2 = length - coordinate;
            new_directions = (rand() % diff2) - diff1;
        } else if (coordinate == 1) {
            diff1 = length - coordinate;
            new_directions = (rand() % (length - 1)) + 1;
        }
    } else if (x > length) {
        if (coordinate < length) {
            diff1 = length - coordinate;
            diff2 = coordinate - 1;
            new_directions = (rand() % diff1) - diff2;
        } else if (coordinate == length) {
            new_directions = (rand() % (length - 2)) - (length - 1);
        }
    }
    a[index][place] = new_directions;
}

void print(char **board, int x) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < x; j++) {
            if (board[i][j] == 'C') cout << "\x1b[91mC\x1b[0m ";
            else if (board[i][j] == 'H') cout << "\x1b[32mH\x1b[0m ";
            else if (board[i][j] == 'V') cout << "\x1b[34mV\x1b[0m ";
            else if (board[i][j] == 'E') cout << "\x1b[33mE\x1b[0m ";
            else if (board[i][j] == 'S') cout << "\x1b[31mS\x1b[0m ";
            else cout << board[i][j] << ' ';
        }
        cout << '\n';
    }
}

void distance(int **coordinate, char *status, int n, int c) {
    int distance1, x1, x2, y1, y2;
    int cnt = 0, cnt1 = 0;
    int violation[n], special[n];
    for (int i = 0; i < n; i++) violation[i] = 0;
    for (int i = 0; i < n; i++) special[i] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j != i) {
                x1 = coordinate[i][0], x2 = coordinate[j][0];
                y1 = coordinate[i][1], y2 = coordinate[j][1];
                distance1 = abs(x1 - x2) + abs(y1 - y2);
                if (((status[i] == 'H' || status[i] == 'E') && status[j] == 'C') || ((status[j] == 'H' || status[j] == 'E') && status[i] == 'C')) {
                    if (distance1 <= c) {
                        if (violation[i] == 0) {
                            violation[i] = 1;
                            cnt++;
                        }
                        if (violation[j] == 0) {
                            violation[j] = 1;
                            cnt++;
                        }
                        if (status[i] == 'H') status[i] = 'C';
                        else if (status[i] == 'E') status[i] = 'S';
                        if (status[j] == 'H') status[j] = 'C';
                        else if (status[j] == 'E') status[j] = 'S';
                    }
                }
                else if (((status[i] == 'H' || status[i] == 'E') && status[j] == 'S') || ((status[j] == 'H' || status[j] == 'E') && status[i] == 'S')) {
                    if (distance1 <= c + 2) {
                        if (special[i] == 0) {
                            special[i] = 1;
                            cnt1++;
                        }
                        if (special[j] == 0) {
                            special[j] = 1;
                            cnt1++;
                        }
                        if (status[i] == 'H') status[i] = 'C';
                        else if (status[i] == 'E') status[i] = 'S';
                        if (status[j] == 'H') status[j] = 'C';
                        else if (status[j] == 'E') status[j] = 'S';
                    }
                }
            }
        }
    }
    if (!cnt) cout << "No healthy people violate the social distance in this phase\n";
    else if (cnt == n) cout << "All users violate the social distance\n";
    else {
        cout << "Users ";
        for (int i = 0; i < n; i++) {
            if (violation[i] == 1) cout << i + 1 << ' ';
        }
        cout << "violate the social distance" << '\n';
    }
    if (!cnt1) cout << "No healthy people walk too close with a sick elderly or people under medical illness" << '\n';
    else {
        cout << "Users ";
        for (int i = 0; i < n; i++) {
            if (special[i] == 1) cout << i + 1 << ' ';
        }
        cout << "walk too close with a sick elderly or people under medical illness" << '\n';
    }
}

void update(char **board, int n, int **coordinate, char *status) {
    for (int i = 0; i < n; i ++) {
        int x = coordinate[i][0];
        int y = coordinate[i][1];
        board[x][y] = status[i];
    }
}

bool is_here(int x, int y, int overlap[][2], int length) {
    for (int i = 0; i < length; i++) {
        if (overlap[i][0] == x && overlap[i][1] == y) return true;
    }
    return false;
}

void check_same_coordinates(int **coordinate, char *status, int n) {
    bool same = false;
    int overlap[n][2];
    for (int i = 0; i < n; i++) {
        overlap[i][0] = 0;
        overlap[i][1] = 0;
    }
    int start = 0;
    int x, y;
    for (int i = 0; i < n; i++) {
        if (!is_here(coordinate[i][0], coordinate[i][1], overlap, start)) {
            x = coordinate[i][0], y = coordinate[i][1];
            for (int j = 0; j < n; j++) {
                if (coordinate[j][0] == x && coordinate[j][1] == y && j != i) {
                    same = true;
                    overlap[start][0] = x;
                    overlap[start][1] = y;
                    start++;
                    break;
                }
            }
        }
    }
    if (!same) cout << "No one stands on the same coordinates" << '\n';
    else {
        for (int i = 0; i < start; i++) {
            cout << '(' << overlap[i][0] << ',' << overlap[i][1] << ')' << ' ' << "overlap" << '\n';
        }
    }
}

void clear(char **board, int length) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (board[i][j] == 'V' || board[i][j] == 'C' || board[i][j] == 'H' || board[i][j] == 'E' || board[i][j] == 'S') board[i][j] = ' ';
        }
    }
}

void count_status(char *status, int n) {
    int sick = 0, healthy = 0, vaccine = 0, elderly = 0, special = 0;
    for (int i = 0; i < n; i++) {
        if (status[i] == 'H') healthy++;
        else if (status[i] == 'C') sick++;
        else if (status[i] == 'V') vaccine++;
        else if (status[i] == 'E') elderly ++;
        else special++;
    }
    cout << sick << ' ' << "people are sick" << '\n';
    cout << healthy << ' ' << "people are healthy" << '\n';
    cout << vaccine << ' ' << "people are vaccinated" << '\n';
    cout << elderly << ' ' << "people are elderly" << '\n';
    cout << special << ' ' << "people are sick elderly or have medical illness" << '\n';
}


int main() {

    // Input from the users
    int n, x, c, r, v, t, m, k, e, s;
    cout << "Length of the square: ";
    cin >> x;

    cout << "No. of users: ";
    cin >> n;
    while (n > (x * x)) {
        cout << "No. of user exceed the grid space!: ";
        cin >> n;
    }

    cout << "Soical distance: ";
    cin >> c;
    while (c > (2 * (x - 1))) {
        cout << "Social distance exceed maximum distance! ";
        cin >> c;
    }

    char *status = new char[n];
    cout << "Coose mode 1 for random status for users, 2 for input status for user youself: ";
    cin >> k;
    while (k != 1 && k != 2) {
        cout << "Enter 1 or 2 only: ";
        cin >> k;
    }
    if (k == 2) {
        char s;
        cout << "Healthy: " << "Enter H" << '\n';
        cout << "Contagious: " << "Enter C" << '\n';
        cout << "Vaccinated: " << "Enter V" << '\n';
        cout << "Healthy elderly: " << "Enter E" << '\n'; 
        cout << "Contagious elderly: " << "Enter S" << '\n';
        for (int i = 0; i < n; i++) {
            cout << "Enter the status of user " << i + 1 << ": ";
            cin >> s;
            while (s != 'V' && s != 'C' && s != 'H' && s != 'E' && s != 'S') {
                cout << "Please enter according the instructions: ";
                cin >> s;
            }
            status[i] = s;
        }
    }
    if (k == 1) {
        cout << "No. of contagious users (aged under 65): ";
        cin >> r;
        while (r > n) {
            cout << "No. of contagious exceed total users: ";
            cin >> r;
        }

        cout << "No. of vaccinated users: ";
        cin >> v;
        while (n < (r + v)) {
            cout << "Total people exceed users: ";
            cin >> v;
        } 

        cout << "No. of healthy elderly: ";
        cin >> e;
        while (n < (r + v + e)) {
            cout << "Total people exceed users: ";
            cin >> e;
        }

        cout << "No. of contagious elderly: ";
        cin >> s;
        while (n < (r + v + e + s)) {
            cout << "Total people exceed users: ";
            cin >> s;
        }
    }

    cout << "No. of minutes of simulation: ";
    cin >> t;
    while (t < 0) {
        cout << "Please enter a positive integer: ";
        cin >> t;
    }

    cout << "Choose mode. 1 for random coordinate, 2 for input coordinate youself: ";
    cin >> m;
    while (m != 1 && m != 2) {
        cout << "Enter 1 or 2 only: ";
        cin >> m;
    }


    // Creating the board for the environment
    char **board = new char*[x + 2];
    for (int i = 0; i < x + 2; i ++) board[i] = new char[x + 2];
    for (int i = 0; i < x + 2; i ++) {
        for (int j = 0; j < x + 2; j++) {
            if (i == 0 || i == x + 1) board[i][j] = '-' ;
            else {
                if (j == 0 || j == x + 1) board[i][j] = '|';
                else board[i][j] = ' ';
            }
        }
    }

    // creating the arrays needed 
    int **coordinates = new int*[n];
    int healthy = n - (r + v + s + e);
    for (int i = 0; i < n; i++) coordinates[i] = new int[2];
    int **directions = new int *[n];
    for (int i = 0; i < n; i++) directions[i] = new int[2];

    //Generate some random coordonates to each person (mode 1)
    if (m == 1) {
        srand(time(NULL));
        for (int i = 0; i < n; i++) {
            coordinates[i][0] = 1 + (rand() % x);
            coordinates[i][1] = 1 + (rand() % x);
        }
    }

    else if (m == 2) {
        int a, b;
        for (int i = 0; i < n; i++) {
            cout << "Enter x coordinate and y coordinate with a blank for user " << i + 1 << ": ";
            cin >> a >> b;
            while (a < 1 || a > x || b < 1 || b > x) {
                cout << "Coordinate out of bound, please enter again: ";
                cin >> a >> b;
            }
            coordinates[i][0] = a;
            coordinates[i][1] = b;
        }
    }
    
    //Randomly assign status for every person
    if (k == 1) {
        int track[n];
        for (int i = 0; i < n; i++) track[i] = 0;
        for (int i = 0; i < r; i++) {
            int people = rand() % n;
            while (track[people] == 1) people = rand() % n;
            status[people] = 'C';
            track[people] = 1;
        }
        for (int i = 0; i < v; i++) {
            int vaccine = rand() % n;
            while (track[vaccine] == 1) vaccine = rand() % n;
            status[vaccine] = 'V';
            track[vaccine] = 1;
        }
        for (int i = 0; i < e; i++) {
            int normal = rand() % n;
            while (track[normal] == 1) normal = rand() % n;
            status[normal] = 'E';
            track[normal] = 1;;
        }
        for (int i = 0; i < s; i++) {
            int normal = rand() % n;
            while (track[normal] == 1) normal = rand() % n;
            status[normal] = 'S';
            track[normal] = 1;;
        }
        for (int i = 0; i < healthy; i++) {
            int normal = rand() % n;
            while (track[normal] == 1) normal = rand() % n;
            status[normal] = 'H';
            track[normal] = 1;;
        }
    }
    

    // Generating random directions for each person
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int x_move = (rand() % (2 * x)) - x;
        int y_move = (rand() % (2 * x)) - x;
        if (!x_move) x_move++;
        if (!y_move) y_move--;
        directions[i][0] = x_move;
        directions[i][1] = y_move;
    }

    update(board, n, coordinates, status);
    print(board, x + 2);
    count_status(status, n);

    // notify users that some people are on the same coordinate
    check_same_coordinates(coordinates, status, n);
    // Simulation part
    int time = 0;
    while (time < t) {
        clear(board, x + 2);
        for (int i = 0; i < n; i++) {
            int x_check = coordinates[i][0] + directions[i][0];
            if (x_check < 1 || x_check > x) {
                get_random_directions(directions, x_check, x, i, coordinates[i][0], 0);
            }
            coordinates[i][0] += directions[i][0];
            int y_check = coordinates[i][1] + directions[i][1];
            if (y_check < 1 || y_check > x) {
                get_random_directions(directions, y_check, x, i, coordinates[i][1], 1);
            }
            coordinates[i][1] += directions[i][1];
        }
        update(board, n, coordinates, status);
        print(board, x + 2);
        distance(coordinates, status, n, c);
        count_status(status, n);
        check_same_coordinates(coordinates, status, n);
        time++;
    }
    return 0;
}