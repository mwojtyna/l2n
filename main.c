#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void DumpHex(const void *data, size_t size) {
  char ascii[17];
  size_t i, j;
  ascii[16] = '\0';
  for (i = 0; i < size; ++i) {
    printf("%02X ", ((unsigned char *)data)[i]);
    if (((unsigned char *)data)[i] >= ' ' &&
        ((unsigned char *)data)[i] <= '~') {
      ascii[i % 16] = ((unsigned char *)data)[i];
    } else {
      ascii[i % 16] = '.';
    }
    if ((i + 1) % 8 == 0 || i + 1 == size) {
      printf(" ");
      if ((i + 1) % 16 == 0) {
        printf("|  %s \n", ascii);
      } else if (i + 1 == size) {
        ascii[(i + 1) % 16] = '\0';
        if ((i + 1) % 16 <= 8) {
          printf(" ");
        }
        for (j = (i + 1) % 16; j < 16; ++j) {
          printf("   ");
        }
        printf("|  %s \n", ascii);
      }
    }
  }
}

int main() {
  int sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

  if (sock_raw < 0) {
    perror("Socket creation failed (Are you root?)");
    return 1;
  }

  unsigned char buffer[1500];
  while (1) {
    ssize_t data_size = recv(sock_raw, buffer, sizeof(buffer), 0);

    if (data_size < 0) {
      perror("Recv failed");
      return 1;
    }

    // The first 14 bytes are the Ethernet header
    struct ethhdr *eth_header = (struct ethhdr *)buffer;
    printf("Captured %ld bytes | Src MAC: %02x:%02x:%02x:%02x:%02x:%02x | Dest "
           "MAC: %02x:%02x:%02x:%02x:%02x:%02x | "
           "Proto: 0x%04x\n",
           data_size, eth_header->h_source[0], eth_header->h_source[1],
           eth_header->h_source[2], eth_header->h_source[3],
           eth_header->h_source[4], eth_header->h_source[5],
           eth_header->h_dest[0], eth_header->h_dest[1], eth_header->h_dest[2],
           eth_header->h_dest[3], eth_header->h_dest[4], eth_header->h_dest[5],
           ntohs(eth_header->h_proto));
    printf("data:\n");
    DumpHex(buffer + sizeof(struct ethhdr), data_size - sizeof(struct ethhdr));
    printf("\n\n");
  }

  close(sock_raw);
  return 0;
}
