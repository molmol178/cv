#include <stdio.h>
#include <signal.h>


long alarm_hdr();
long check = 0;

void main()
{
  signal(SIGALRM,alarm_hdr);
  alarm(2);
  while (1)
    {
      if (check == 1)
	{
	  alarm(2);
	  check = 0;
	}
    }
}


long alarm_hdr()
{
  check = 1;
  fprintf(stderr,"alarm signal received\n");
}
