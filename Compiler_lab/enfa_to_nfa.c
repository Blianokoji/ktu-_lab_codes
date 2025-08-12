#include <stdio.h>
#include <stdbool.h>

#define MAX_STATES 20
#define MAX_TRANS 50
#define MAX_SYMBOLS 20

typedef struct {
    int from;
    char sym;
    int to;
} Transition;

int state_count, trans_count;
Transition trans[MAX_TRANS];
bool closure[MAX_STATES][MAX_STATES];
char symbols[MAX_SYMBOLS];
int sym_count;

void dfs_epsilon(int state, bool visited[]) {
    visited[state] = true;
    for (int i = 0; i < trans_count; i++)
        if (trans[i].from == state && trans[i].sym == 'e' && !visited[trans[i].to])
            dfs_epsilon(trans[i].to, visited);
}

void get_symbols() {
    bool seen[256] = {false};
    sym_count = 0;
    for (int i = 0; i < trans_count; i++) {
        char s = trans[i].sym;
        if (s != 'e' && !seen[(unsigned char)s]) {
            symbols[sym_count++] = s;
            seen[(unsigned char)s] = true;
        }
    }
}

int main() {
    scanf("%d", &state_count);
    scanf("%d", &trans_count);
    for (int i = 0; i < trans_count; i++)
        scanf("%d %c %d", &trans[i].from, &trans[i].sym, &trans[i].to);

    for (int s = 0; s < state_count; s++) {
        bool visited[MAX_STATES] = {false};
        dfs_epsilon(s, visited);
        for (int i = 0; i < state_count; i++) closure[s][i] = visited[i];
    }

    get_symbols();

    for (int s = 0; s < state_count; s++) {
        for (int si = 0; si < sym_count; si++) {
            bool result[MAX_STATES] = {false};
            for (int c = 0; c < state_count; c++) {
                if (closure[s][c]) {
                    for (int t = 0; t < trans_count; t++) {
                        if (trans[t].from == c && trans[t].sym == symbols[si]) {
                            for (int r = 0; r < state_count; r++)
                                if (closure[trans[t].to][r]) result[r] = true;
                        }
                    }
                }
            }
            printf("%d --%c--> {", s, symbols[si]);
            for (int r = 0; r < state_count; r++) if (result[r]) printf(" %d", r);
            printf(" }\n");
        }
    }
}
