#include <stdio.h>
#include <stdbool.h>

#define MAX_STATES 20
#define MAX_TRANS 50

typedef struct {
    int from;
    char sym;
    int to;
} Transition;

int state_count, trans_count;
Transition trans[MAX_TRANS];

// DFS to find epsilon closure of a state
void dfs_epsilon(int state, bool visited[]) {
    visited[state] = true;

    // For each transition from 'state'
    for (int i = 0; i < trans_count; i++) {
        if (trans[i].from == state && trans[i].sym == 'e') {
            if (!visited[trans[i].to]) {
                dfs_epsilon(trans[i].to, visited);
            }
        }
    }
}

int main() {
    printf("Enter number of states: ");
    scanf("%d", &state_count);

    printf("Enter number of transitions: ");
    scanf("%d", &trans_count);

    printf("Enter transitions (from sym to), 'e' for epsilon:\n");
    for (int i = 0; i < trans_count; i++) {
        scanf("%d %c %d", &trans[i].from, &trans[i].sym, &trans[i].to);
    }

    printf("\nEpsilon closures:\n");
    for (int s = 0; s < state_count; s++) {
        bool visited[MAX_STATES] = {false};

        dfs_epsilon(s, visited);

        printf("ε-closure(%d): { ", s);
        for (int i = 0; i < state_count; i++) {
            if (visited[i]) {
                printf("%d ", i);
            }
        }
        printf("}\n");
    }

    return 0;
}