# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <assert.h>

# include <containers/staticstacks.h>

int main(void)
{
  int val[] = {1, 2, 3, 4,};

  sstacks_t *s = NULL;
  sstacks_new(s, sizeof(val[0]), 4);

  for ( size_t i = 0; i < 4; i++ ) sstacks_push(s, &val[i]);

  while ( !sstacks_empty(s) ) {
    int x = *sstacks_pop(s);
    printf("%d\n", x);
  }

  sstacks_free(s);

  exit(0);
}
