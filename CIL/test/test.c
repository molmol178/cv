#define NUM 100000



pointer_call(a)
     char *a;
{
  int i;

  for (i = 0; i < NUM; i++)
    *(a + i) = 100;
}

array_call(a)
     char *a;
{
  int i;

  for (i = 0; i < NUM; i++)
    a[i] = 100;
}

main()
{
  char a[NUM];
  pointer_call(a);
  array_call(a);
}
