#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int g(int x) {
  return x+3;
}

int f(int x) {
  return g(x);
}

void fn() {
  printf("in fn\n");
}

void main(void) {
  printf("%d %d\n", f(8)+1, 13);
  // sleep(3);
  sigalarm(1, fn);
  exit(0);
}
