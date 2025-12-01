#include <iostream>
#include <fstream>
#include <string>

int main() {
  std::ifstream in("input.txt");
  long long answer = 0;
  std::string line;
  while(std::getline(in, line)) {
    long long num = std::stoll(line);
    while (true) {
      num = num / 3 - 2;
      if (num > 0) {
        answer += num;
      } else {
        break;
      }
    }
  }
  std::cout << answer << std::endl;
  return 0;
}
