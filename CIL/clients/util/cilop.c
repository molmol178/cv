/*
 * filename : op.c
 * author   : Takahiro Sugiyama
 * date     : Tuesday, May 31 1994
 * describe : binary image tools
 */



#include <stdio.h>
#include "Image.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"


static char *option[] = {
  "<:Binary Image Operator:",
  "help:-h:print this messages",
  "input1:*:1:(image) name of the first input binary-image",
  "input2:*:1:(image) name of the second input binary-image",
  "output:=:1:(image) name of the output binary-image",
  "o) Arithmetic operators:",
  "or:-o:the operater of OR",
  "and:-a:the operater of AND",
  "eor:-e:the operater of Exclusive OR",
  "not:-n:the operater of NOT",
  "o) Morphology operators:",
  "dilation:-md:the operater of Dilation",
  "erosion:-me:the operater of Erosion",
  "opening:-mo:the operater of Opening",
  "closing:-mc:the operater of Closing",
  "o) Parameters for morphology:",
  "spot:-s:2:(int) coord of the spot structuring element (center)",
};

static char *op_name[] = {
  "not", "or", "and", "eor",
  "dilation", "erosion", "opening", "closing",
};

enum {
  e_NOT, e_OR, e_AND, e_EOR,
  e_DILATION, e_EROSION, e_OPENING, e_CLOSING,
};

#define CATEGORY_NOT(op) ( op == e_NOT )
#define CATEGORY_ARITHMETIC(op) (( e_OR <= op ) && ( op <= e_EOR ))
#define CATEGORY_MORPHOLOGY(op) (( e_DILATION <= op ) && ( op <= e_CLOSING ))



static find_operator()
{
  int index_op;

  for ( index_op = 0; index_op < Number(op_name); index_op++ )
    if ( optspecified( op_name[index_op] ) ) break;

  if ( index_op == Number(op_name) ) index_op = e_OR;

  return index_op;
}



image create_load_binary_image
  _P1 (( char *, input_name ))
{
  image input;

  input = Image.createFromFilename( input_name, input_name );

  if ( input == 0 )
    {
      fprintf( stderr, "error:can't open filename %s.\n", input_name );
      exit( -1 );
    }
  /*
  if ( __TYPE( input ) != Bit1 )
    {
      fprintf( stderr, "error:The image %s is wrong.\n", input_name );
      exit( -1 );
    }
  */
  return input;
}



void do_category_not
  _P2 (( image, output ),
       ( image, input  ))
{
  image__reverse( output, input );
}

void do_category_arithmetic
  _P4 (( image, output ),
       ( image, input1 ),
       ( int  , op     ),
       ( image, input2 ))
{
  switch (op)
    {
    case e_OR : image__or ( output, input1, input2 ); break;
    case e_AND: image__and( output, input1, input2 ); break;
    case e_EOR: image__eor( output, input1, input2 ); break;
    }
}

void do_category_morphology
  _P6 (( image, output ),
       ( image, input1 ),
       ( int  , op     ),
       ( image, input2 ),
       ( int  , xspot  ),
       ( int  , yspot  ))
{
  switch (op)
    {
    case e_DILATION:
      image__dilation( output, input1, input2, xspot, yspot ); break;
    case e_EROSION:
      image__erosion( output, input1, input2, xspot, yspot ); break;
    case e_OPENING:
      image__opening( output, input1, input2, xspot, yspot ); break;
    case e_CLOSING:
      image__closing( output, input1, input2, xspot, yspot ); break;
    }
}

void do_operator
  _P3 (( image , output  ),
       ( int   , op      ),
       ( char *, comment ))
{
  char *input1_name;
  char *input2_name;
  image input1;
  image input2;
  int xspot, yspot;

  input1_name = optvalue( "input1" );
  input1 = create_load_binary_image( input1_name );
  if CATEGORY_NOT(op)
    {
      sprintf( comment, "not %s", input1_name );
      do_category_not( output, input1 );
      Image.destroy( input1 );
      return;
    }

  input2_name = optvalue( "input2" );
  input2 = create_load_binary_image( input2_name );
  if CATEGORY_ARITHMETIC(op)
    {
      sprintf( comment, "%s %s %s", input1_name, op_name[op], input2_name );
      do_category_arithmetic( output, input1, op, input2 );
      Image.destroy( input1 );
      Image.destroy( input2 );
      return;
    }

  xspot = __XSIZE( input1 ) / 2;
  yspot = __YSIZE( input1 ) / 2;
  if ( optspecified( "spot" ) )
    {
      xspot = optnvalueint( "spot", 0 );
      yspot = optnvalueint( "spot", 1 );
    }

  if CATEGORY_MORPHOLOGY(op)
    {
      sprintf( comment, "%s %s %s spot (%d,%d)",
	       input1_name, op_name[op], input2_name, xspot, yspot );
      do_category_morphology( output, input1, op, input2, xspot, yspot );
      Image.destroy( input1 );
      Image.destroy( input2 );
      return;
    }
}



int main( argc, argv )
     int argc;
     char *argv[];
{
  char *output_name;
  image output;
  int op;
  char comment[ 512 ];

  OPTION_SETUP( option );

  if ( optspecified( "help" ) ) optmanual( 1 );
  if ( argc == 1 ) optusage( 1 );

  op = find_operator();
  output = Image.create( "output" );

  do_operator( output, op, comment );

  output_name = optvalue( "output" );
  fprintf( stderr, "%s = %s\n", output_name, comment );
  Image.save( output, output_name, comment );

  return 0;
}
