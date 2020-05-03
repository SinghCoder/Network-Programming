int
main ()
{
  execl ("/bin/ls", "ls", "-l", (char *) 0);
  printf ("hello");
}
