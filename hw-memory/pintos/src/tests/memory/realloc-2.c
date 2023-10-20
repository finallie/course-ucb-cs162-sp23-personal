/* Test realloc. */

#include <stdlib.h>
#include <string.h>
#include "tests/lib.h"
#include "tests/main.h"

#define GIBI (1 << 30)
#define NUM_INTS 3997

#define P_CONST 0x05158E57
#define Q_CONST 0xfa112019
#define R_CONST 0x1abe1162
#define S_CONST 0xba5eba11

static void check_zero(int* buffer, int len) {
  for (int i = 0; i != len; i++) {
    ASSERT(buffer[i] == 0);
  }
}

static void fill(int* buffer, int len, int with) {
  for (int i = 0; i != len; i++) {
    buffer[i] = with;
  }
}

static void check_filled(int* buffer, int len, int with) {
  for (int i = 0; i != len; i++) {
    ASSERT(buffer[i] == with);
  }
}

static void* test_alloc(int num_ints, int with) {
  int* ptr = calloc(num_ints, sizeof(int));
  check_zero(ptr, num_ints);
  fill(ptr, num_ints, with);
  return ptr;
}

void test_main(void) {
  int* p = test_alloc(2 * NUM_INTS, P_CONST);
  int* q = test_alloc(2 * NUM_INTS, Q_CONST);
  int* r = realloc(p, 4 * NUM_INTS * sizeof(int));
  check_filled(r, 2 * NUM_INTS, P_CONST);
  fill(r + 2 * NUM_INTS, 2 * NUM_INTS, R_CONST);
  ASSERT(r != p);

  int* s = test_alloc(2 * NUM_INTS, S_CONST);
  ASSERT(s == p);

  int* t = realloc(r, GIBI);
  ASSERT(t == NULL);

  check_filled(r, 2 * NUM_INTS, P_CONST);
  check_filled(r + 2 * NUM_INTS, 2 * NUM_INTS, R_CONST);
  check_filled(q, 2 * NUM_INTS, Q_CONST);
  check_filled(s, 2 * NUM_INTS, S_CONST);

  free(q);
  free(r);
  free(s);
}

int main(int argc UNUSED, char* argv[] UNUSED) {
  test_name = "realloc-2";
  msg("begin");
  test_main();
  msg("end");
  return 0;
}
