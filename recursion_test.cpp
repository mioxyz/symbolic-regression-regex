#include<iostream>
#define puts std::cout << std::endl <<

/*
   Deliberately make a non-tail-recursive function
*/
int f(int k, int actualCount) {
   puts "actualCount: " << actualCount;
   if(k % 2 == 0) {
      return f(k, ++actualCount)+1;
   }else{
      return f(k, ++actualCount)+2;
   }
}

int main() {
   puts "+++main";
   // puts ">>> 0: ";
   // for(int k = 0; k < 10; ++k) {
   //    puts "[" << k << "]: " << k % 0;
   // }
   puts ">>> 1: ";
   for(int k = 0; k < 10; ++k) {
      puts "[" << k << "]: " << k % 1;
   }

   puts ">>> 2: ";
   for(int k = 0; k < 10; ++k) {
      puts "[" << k << "]: " << k % 2;
   }

   // f(0, 0);

   puts "---main";
}