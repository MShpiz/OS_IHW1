#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

const size_t size = 5000;
char readingBuffer[size + 1];
char writingBuffer[size + 1];
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

void ParentLogic() {
  bool read_f = readFromFile();
  if (!read_f) {
    exit(1);
  }

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

  int fd2;
  if ((fd2 = open(child1_child2_fifo, O_RDONLY)) < 0) {
    printf("Can\'t open FIFO for writting\n");
    exit(-1);
  }
  size_t messize = read(fd2, writingBuffer, size);


  if (messize < 0) {
    printf("parent: Can\'t read string from pipe2\n");
    exit(-1);
  }
  if (close(fd2) < 0) {
    printf("parent: Can\'t close reading side of pipe2\n");
    exit(-1);
  }

  writeToFile(messize);

  printf("Parent exit\n");

}

int main(int argc, char *argv[]) {

  inFile = argv[1];
  outFile = argv[2];

  mknod(parent_child1_fifo, S_IFIFO | 0666, 0);
  mknod(child1_child2_fifo, S_IFIFO | 0666, 0);

  ParentLogic();
  
  return 0;
}
