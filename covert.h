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

static struct option long_options[] = {
        {"dest",    required_argument, 0, 'd'},
        {"src",     required_argument, 0, 's'},
        {"sport",   required_argument, 0, 'p'},
        {"dport",   required_argument, 0, 'e'},
        {"file",    required_argument, 0, 'f'},
        {"ipid",    required_argument, 0, 'i'},
        {"seq",     required_argument, 0, 'q'},
        {"ack",     required_argument, 0, 'a'},
        {"client",  required_argument, 0, 'c'},
        {"tos",     required_argument, 0, 't'},
        {"test",    required_argument, 0, 'u'},
        {"servertest", required_argument,0, 'y'},
        {0,         0,                 0,  0}
};

#define print_usage() \
        do {\
        printf("usage options:\n" \
                    "\t[d]estination              - destination ip address\n"\
                    "\t[s]ource                   - source ip address\n"\
                    "\tsource [p]ort              - source port\n"\
                    "\td[e]stination port         - destination port\n"\
                    "\t[f]ile                     - file name\n"\
                    "\t[c]lient                   - 1 if client\n"\
                    "\t[i]pid                     - encoding using sequence number\n"\
                    "\tse[q]uence number          - encoding using sequence number\n"\
                    "\t[a]cknowledgement number   - encoding using ackowlegement number, 1 if chosen\n"\
                    "\t[t]ype of service          - encoding using type of service \n"\
                    "");\
   }while(0)
//
    struct send_header{
        struct iphdr ip;
        struct tcphdr tcp;
    } send_header;

    struct recv_header{
        struct iphdr ip;
        struct tcphdr tcp;
        char buffer[10000];
    } recv_header;

    struct pseudo_header{
        unsigned int source_address;
        unsigned int destination_address;
        unsigned char placeholder;
        unsigned char protocol;
        unsigned short tcp_length;
        struct tcphdr tcp;
    } pseudo_header;

void SendPacket(unsigned int src, unsigned int dst, unsigned short sport, unsigned short dport, char *filename, int ipid, int seq, int ack);
void RecvPacket(unsigned int src, unsigned short sport, unsigned short dport, char *filename, int ipied, int seq, int ack);
unsigned short in_cksum(unsigned short *, int);
void CheckRoot();

bool client, server, ipid, seq, ack, tos;

#endif
