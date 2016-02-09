/*
 * main.c
 */



#include "Edit.h"



int main(int argc,char *argv[])
{
  struct EditRec eR;

  Edit__init(&eR,argc,argv);
  Edit__open(&eR);
  Edit__exec(&eR);
  Edit__close(&eR);
  Edit__exit(&eR);

  return 0;
}
