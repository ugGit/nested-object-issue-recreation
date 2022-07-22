#include <stdio.h>

struct transformation_faker{
  int point_to_global(int local){
    return local;
  }
};

struct cell_module {
    transformation_faker placement = {};
    int range0[2];
};

int main(){
  // just a simple test to check the correctness of the transformation_faker
  transformation_faker t;
  auto x = t.point_to_global(2);
  printf("x=%d\n", x); // prints out x=2

  // problem during instantiation of the cell_module
  auto test = cell_module{};
  
  return 0;
}
