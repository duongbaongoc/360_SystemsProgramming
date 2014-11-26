#include <cmd.h>

int my_cat(int argc, char* argv[])
{
    printf("cat: not yet implemented\n");
    return 0;
}

//==========================  HOW TO cat ======================================
//cat filename:
//
//   char mybuf[1024], dummy = 0;  // a null char at end of mybuf[ ]
//   int n;
//
//1. int fd = open filename for READ;
//2. while( n = read(fd, mybuf[1024], 1024)){
//       mybuf[n] = 0;             // as a null terminated string
//       // printf("%s", mybuf);   <=== THIS works but not good
//       spit out chars from mybuf[ ] but handle \n properly;
//   } 
//
//
//close(fd);
