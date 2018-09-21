#ifndef COVERT_H
#define COVERT_H

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <linux/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <time.h>

void SendPacket(unsigned int src, unsigned int dst, unsigned short sport, unsigned short dport, char *filename, int ipid, int seq, int ack);
void RecvPacket(unsigned int src, unsigned int dst, unsigned short sport, unsigned short dport, char *filename, int ipied, int seq, int ack);
unsigned short in_cksum(unsigned short *, int);
void CheckRoot();

bool client, server, ipid, seq, ack, tos;

#endif
