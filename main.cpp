#include<stdio.h>
#include<windows.h>

int main()
{

  HINSTANCE ldll;
  int (*add2)(int);
  int (*mul)(int,int);

  ldll = LoadLibrary("mydll.dll");
  if(ldll>(void*)HINSTANCE_ERROR){
    add2 = GetProcAddress(ldll, "add2");
    mul = GetProcAddress(ldll, "mul");
    printf("add2(3): %d\nmul(4,5): %d", add2(3), mul(4,5));
  } else {
    printf("ERROR.");
  }

}
