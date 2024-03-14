#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

const size_t size = 5000;
char readingBuffer[size + 1];
char middleBuffer[size + 1];
char writingBuffer[size + 1];
ssize_t resultSize = 0;
std::string inFile;
std::string outFile;
char parent_child1_fifo[] = "text.fifo";
char child1_child2_fifo[] = "text2.fifo";

bool readFromFile() {
  ssize_t read_bytes;
  int fd;
  if ((fd = open(inFile.c_str(), O_RDONLY)) < 0) {
    printf("Can\'t open file\n");
    return false;
  }

  read_bytes = read(fd, readingBuffer, size);
  if (read_bytes == -1) {
    printf("Can\'t read this file\n");
    return false;
  }
  // читаем часть первого файла

  readingBuffer[read_bytes] = '\0';
  if (close(fd) < 0) {
    printf("Can\'t close reading file\n");
  }
  return true;
}

bool writeToFile(int byte_count) {
  int fd;
  if ((fd = open(outFile.c_str(), O_WRONLY, 0666)) < 0) {
    printf("Can\'t open file\n");
    return false;
  }

  ssize_t write_bytes = write(fd, writingBuffer, byte_count);

  if (write_bytes == -1) {
    printf("Can\'t write this file\n");
    return false;
  }
  if (close(fd) < 0) {
    printf("Can\'t close reading file\n");
  }
  return true;
}

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

void ParentLogic() {
  int fd;
  if ((fd = open(parent_child1_fifo, O_WRONLY)) < 0) {
    printf("Can\'t open FIFO for writting\n");
    exit(-1);
  }
  int msgsize = write(fd, readingBuffer, size);
  if (msgsize != size) {
    printf("Can\'t write all string to pipe\n");
    exit(-1);
  }
  if (close(fd) < 0) {
    printf("parent: Can\'t close writing side of pipe\n");
    exit(-1);
  }
  printf("Parent exit\n");
}

void SecondChildLogic() {
  printf("Second Child here\n");
  int fd;
  if ((fd = open(parent_child1_fifo, O_RDONLY)) < 0) {
    printf("Can\'t open FIFO for reading\n");
    exit(-1);
  }
  size_t messize = read(fd, writingBuffer, resultSize);

  if (messize < 0) {
    printf("Child2: Can\'t read string from pipe\n");
    exit(-1);
  }
  bool res = writeToFile(resultSize);

  if (close(fd) < 0) {
    printf("child2: Can\'t close reading side of pipe\n");

    exit(-1);
  }

  printf("child2 exit");
}

void ChildLogic() {
  int fd;
  if ((fd = open(parent_child1_fifo, O_RDONLY)) < 0) {
    printf("Can\'t open FIFO for reading\n");
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
    printf("child: Can\'t close reading side of pipe\n");
    exit(-1);
  }

  mknod(child1_child2_fifo, S_IFIFO | 0666, 0);

  int child = 0;
  if ((child = fork()) < 0) {
    printf("Can\t fork second child\n");
    exit(-1);
  } else if (child == 0) {
    SecondChildLogic();
  } else {
    int fd2;
    if ((fd2 = open(parent_child1_fifo, O_WRONLY)) < 0) {
      printf("Can\'t open FIFO for reading\n");
      exit(-1);
    }

    size_t msgsize = write(fd2, res.c_str(), resultSize);

    if (msgsize != resultSize) {
      printf("Can\'t write all string to pipe\n");
      exit(-1);
    }
    if (close(fd2) < 0) {
      printf("child: Can\'t close writing side of pipe\n");
      exit(-1);
    }
    printf("Child exit\n");
  }
}

int main(int argc, char *argv[]) {
  inFile = argv[1];
  outFile = argv[2];
  mknod(parent_child1_fifo, S_IFIFO | 0666, 0);
  bool read = readFromFile();
  if (!read) {
    exit(1);
  }
  int result;
  if ((result = fork()) < 0) {
    printf("Can\t fork child\n");
    exit(-1);
  } else if (result > 0) { /* Parent process */
    ParentLogic();
  } else { /* Child process */
    ChildLogic();
  }
  return 0;
}
