/*
 * filename : labelling.c
 * author   : Takahiro Sugiyama
 * date     : Tuesday, July 2 1996
 * describe : labelling main function for test
 *
 * update   : Thursday, February 6 1997 by Takahiro Sugiyama
 * describe : for fast labelling algorithm and for one of CIL clients
 */


#include "Image.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include <stdio.h>


static char *option[] = {
  "input:*:1:(image) name of the input image",
  "output:-o:1:(image) name of the output label image",
  "type:-t:1:short:(type) type of the output{uchar,short,ushort,long}",
  "connectivity:-c:1:4:(connectivity) neighborhood connection{4,8}",
  "zero-ignore:-z:zero label is ignored",
  "help:-h:print this messages",
};


/*------------------------------------------------------------------------*
 * main
 *------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  image input, output;
  char *input_name, *output_name;
  long n, type, connectivity;

  OPTION_SETUP(option);

  if (optspecified("help")) optmanual(1);
  if (argc == 1) optusage(1);

  input_name = optvalue("input");
  output_name = optvalue("output");

  input = Image.createFromFilename("input", input_name);
  output = Image.create("output");
  if (input == 0)
    {
      fprintf(stderr, "error:image %s is wrong\n");
      exit(-1);
    }

  type = typeget(optvalue("type"));
  connectivity = optvalueint("connectivity");

  if (!optspecified("zero-ignore"))
    switch (connectivity)
      {
      case  4: n = image__N4_labelling(output, input, type); break;
      case  8: n = image__N8_labelling(output, input, type); break;
      default:
	fprintf(stderr,"specified connectivity is invalid %d\n", connectivity);
	fprintf(stderr,"\tonly \"4\" or \"8\"\n");
	exit(-1);
      }
  else
    switch (connectivity)
      {
      case  4: n = image__N4_labelling_foreground(output, input, type); break;
      case  8: n = image__N8_labelling_foreground(output, input, type); break;
      default:
	fprintf(stderr,"specified connectivity is invalid %d\n", connectivity);
	fprintf(stderr,"\tonly \"4\" or \"8\"\n");
	exit(-1);
      }

  fprintf(stderr,"The number of labels = %d\n", n);
  Image.save(output, output_name, "labelling");

  return 0;
}
