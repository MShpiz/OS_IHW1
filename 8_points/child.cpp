#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

const size_t size = 5000;
char middleBuffer[size + 1];
ssize_t resultSize = 0;

char parent_child1_fifo[] = "text.fifo";
char child1_child2_fifo[] = "text2.fifo";




std::string countLetters(char buff[]) {
  int count[26] = {0};
  for (int i = 0; i < size; i++) {
    if (buff[i] >= 'A' && buff[i] <= 'Z') {
      count[buff[i] - 'A']++;
    }
  }

  std::string s = std::string();
  for (int i = 0; i < 26; i++) {
    char c = 'A' + i;
    s += c;
    s += ": " + std::to_string(count[i]) + '\n';
  }
  return s;
}


void ChildLogic() {

  int fd;
  if ((fd = open(parent_child1_fifo, O_RDONLY)) < 0) {
    printf("Can\'t open FIFO for writting\n");
    exit(-1);
  }

  size_t messize = read(fd, middleBuffer, size);
  if (messize < 0) {
    printf("Can\'t read string from pipe\n");
    exit(-1);
  }
  std::string res = countLetters(middleBuffer);
  resultSize = res.length();

  if (close(fd) < 0) {
    printf("child: Can\'t close writing side of pipe\n");
    exit(-1);
  }


  int fd2;
  if ((fd2 = open(child1_child2_fifo, O_WRONLY)) < 0) {
    printf("Can\'t open FIFO for writting\n");
    exit(-1);
  }

  size_t msgsize = write(fd2, res.c_str(), resultSize);
  // printf("child got result %s", res.c_str());

  if (msgsize != resultSize) {
    printf("Can\'t write all string to pipe2\n");
    exit(-1);
  }
  if (close(fd2) < 0) {
    printf("parent: Can\'t close writing side of pipe\n");
    exit(-1);
  }

  printf("Child exit\n");
}

int main() {

  ChildLogic();

  return 0;
}
