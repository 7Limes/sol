#include <iostream>
#include <sys/stat.h>
#include <string>
#include <string.h>
#include "sol.hpp"

using namespace std;
struct stat buffer;   

bool file_exists(string &name) {
  return (stat(name.c_str(), &buffer) == 0); 
}

// ./main [path] (-d)
int main(int argc, char* argv[]) {
  if (argc > 1) {
    string path(argv[1]);
    if (file_exists(path)) {
      bool debug = false;
      bool timed = false;
      bool step = false;
      for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0)
          debug = true;
        else if (strcmp(argv[i], "-t") == 0)
          timed = true;
        else if (strcmp(argv[i], "-s") == 0)
          step = true;
        else {
          cout << "Flag `" << argv[i] << "` not recognized." << endl;
          return 3;
        }
      }
      sol_interpret(path, debug, timed, step);
      return 0;
    }
    cout << "File `" << argv[1] << "` not found." << endl;
    return 2;
  }
  cout << "Expected path at argument 1." << endl;
  return 1;
}
