#include <string.h>
#include <sys/stat.h>
#include "main.h"

#define NAME_VAL 0x00000030
#define DATA_VAL 0x00000080

void main(int argc, char** argv) {
  // Check number of arguments
  if (argc < 3) {
    printf("Not enough arguments.\n");
    return;
  }
  
  // Parse argv[1] for device name
  size_t len = strlen(argv[1]);
  char* device = malloc(len+1);
  strcpy(device, argv[1]);

  // Check device name length
  if (len != 8) {
    printf("Invalid device name\n");
    free(device);
    return;
  }
  
  // Parse argv[2] for entry number
  int entrynum = atoi(argv[2]);

  // Read parsed device
  int fd  = open(device, O_RDONLY);

  // Validate device check
  if (fd <= 0)
    printf("\tValue is less than 0\n\tValue is: %d\n\n", fd);
  else {
    // Call the function from Project 1 to get the address of MFT
    uint64_t mftLocation = getMFT(fd);
    uint16_t lba = getLBA(fd);
    uint16_t csize = getCNUM(fd);
    
    // Call the function from Project 2 to get address of entry number
    uint64_t entryLocation = getEntry(fd, entrynum, mftLocation);
    // printf("Entry Location: 0x%08lX\n",entryLocation);
    
    // Call the function from Project 3 to get the location of $DATA attribute of the entry number
    uint64_t dataLocation = traverseAttribute(fd, entryLocation, DATA_VAL);
    // printf("Data Location: 0x%08lX\n",dataLocation);
    
    // Project 4 Function
    FCLUST fileInfo = printData(fd, dataLocation);

    uint64_t fileClusterLocation = lba + (csize * fileInfo.cluster_offset);
    //   printf("File Cluster Location: 0x%08lX\n",fileClusterLocation);
    
    ssize_t a;
    int fd2; // write to new file
    uint64_t j;
    uint8_t cluster_buffer[csize];
    char* filename = "/home/ethan/Documents/recovery/recovery.jpg";
    fd2 = open(filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    for(int i = 0; i < fileInfo.cluster_size; i++) {

      if(i == fileInfo.cluster_size - 1) {
	int leftover = fileInfo.attrib_size % csize;

	j = fileClusterLocation + (csize * i);

	lseek(fd, j, SEEK_SET);
	a = read(fd, cluster_buffer, leftover);
	write(fd2, cluster_buffer, a);
	printf("File recovered to /home/ethan/Documents/recovery/recovery.jpg\n");
      }

      else {
	j = fileClusterLocation + (csize * i);

	lseek(fd, j, SEEK_SET);
	a = read(fd, cluster_buffer, csize);
	write(fd2, cluster_buffer, a);
      }      
    }
  }

  // close file descriptors
  close(fd);
  // Free up memory from copied string
  free(device);

  return;
}
