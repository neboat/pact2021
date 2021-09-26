#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <cilk/cilk.h>

unsigned long long todval(struct timeval *tp) {
    return tp->tv_sec * 1000 * 1000 + tp->tv_usec;
}

/*
 * This program computes the number of possible placements
 * of <n> queens on an <n> by <n> board with none of the
 * queens conflicting with each other.
 *
 * nqueen  4 = 2
 * nqueen  5 = 10
 * nqueen  6 = 4
 * nqueen  7 = 40
 * nqueen  8 = 92
 * nqueen  9 = 352
 * nqueen 10 = 724
 * nqueen 11 = 2680
 * nqueen 12 = 14200
 * nqueen 13 = 73712
 * nqueen 14 = 365596
 * nqueen 15 = 2279184
 */

/*
 * <a> contains array of <n> queen positions.  Returns 1
 * if none of the queens conflict, and returns 0 otherwise.
 */
int no_conflict(int n, char *board) {

  int i, j;
  char p, q;

  for (i = 0; i < n; i++) {
    p = board[i];
    for (j = i + 1; j < n; j++) {
      q = board[j];
      if (q == p || q == p - (j - i) || q == p + (j - i))
        return 0;
    }
  }

  return 1;
}

/*
 * <n> is the number of rows/columns on the board
 * <a> contains the position of queens placed thus far
 * <j> is the number of queens placed thus far
 * Returns the total number of solutions
 */
int nqueens(int n, int j, char *board) {

  char *new_board;
  int *count;
  int solNum = 0;

  if (n == j) {
    // end of the board reached; found a solution
    return 1;
  }

  count = (int *) alloca(n * sizeof(int));
  (void) memset(count, 0, n * sizeof (int));

  for (int i = 0; i < n; i++) {
    new_board = (char *) alloca((j + 1) * sizeof (char));
    memcpy(new_board, board, j * sizeof (char));
    new_board[j] = i;
    if (no_conflict(j + 1, new_board))
      count[i] = cilk_spawn nqueens(n, j + 1, new_board);
  }
  cilk_sync;

  for (int i = 0; i < n; i++) {
    solNum += count[i];
  }

  return solNum;
}


int main(int argc, char *argv[]) {

  int n = 13;
  char *board;
  int res;

  if (argc < 2) {
    fprintf (stderr, "Usage: %s [<cilk-options>] <n>\n", argv[0]);
    fprintf (stderr, "Use default board size, n = 13.\n");

  } else {
    n = atoi (argv[1]);
    fprintf (stderr, "Running %s with n = %d.\n", argv[0], n);
  }

  board = (char *) alloca (n * sizeof (char));
  res = 0;

  struct timeval t1, t2;
  gettimeofday(&t1,0);

  res = nqueens(n, 0, board);

  gettimeofday(&t2,0);
  unsigned long long runtime_ms = (todval(&t2)-todval(&t1))/1000;
  printf("%f\n", runtime_ms/1000.0);

  if (res == 0) {
    fprintf (stderr, "No solution found.\n");
  } else {
    fprintf (stderr, "Total number of solutions : %d\n", res);
  }

  return 0;
}
