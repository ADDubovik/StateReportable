#include <iostream>


int main(int argc, char *argv[])
{
  if ( argc != 2 )
  {
    std::cout << "Please specify raw statistics file name as the first command line argument." << std::endl;
    return 1;
  }

  return 0;
}
