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
        {"test",    required_argument, 0, 't'},
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
                    "");\
   }while(0)

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

bool client, server, ipid, seq, ack;

int main(int argc, char **argv){

    time_t t;
    int c;
    char dests[80], srcs[80], file[80];
    bool test,stest;
    unsigned int dest, src;
    unsigned short sport, dport;
    server = true;
    client = false;

    srand((unsigned) time(&t));

   while(1){
    int option_index = 0;
    c = getopt_long (argc, argv, "t:y:i:q:a", long_options, &option_index);
    if (c == -1){
        break;
    }
    switch(c) {
        case 'd':
            dest = atoi(optarg);
            printf("Destination Address: %s\n", dests);
            dest = inet_addr(dests);
            break;
        case 's':
            src = atoi(optarg);
            printf("Source Address: %s\n", srcs);
            src = inet_addr(srcs);
            break;
        case 'p':
            sport = atoi(optarg);
            printf("Source Port: %d\n", sport);
            break;
        case 'e':
            dport = atoi(optarg);
            printf("Destination Port: %d\n", dport);
            break;
        case 'f':
            strcpy(optarg, file);
            printf("File: %s\n", file);
            break;
        case 'i':
            ipid = true;
            printf("Encoding: IP ID Number\n");
            break;
        case 'q':
            seq = true;
            printf("Encoding: Sequence Number\n");
            break;
        case 'a':
            ack = true;
            printf("Encoding: Acknowledgement Number\n");
            break;
        case 'c':
                client = true;
                server = false;
                printf("client: true\n");
            break;
        case 't':
            test = true;
            break;
        case 'y':
            stest = true;
            break;
        case '?':

        default:
            print_usage();
            return EXIT_SUCCESS;
    }
   }

   //check if the user has root privilages
   //check for encoding type
   //check for file
   //check if server or client
   if(geteuid() !=0){
        printf("User must be root to run this program\n");
        exit(0);
   }
   if(test) {
        strcpy(dests, "127.0.0.1");
        dest = inet_addr(dests);
        strcpy(srcs, "127.0.0.1");
        src = inet_addr(srcs);
        strcpy(file, "secret");
        sport = 7005;
        dport = 7005;
        seq = true;
        server = false;
        client = true;
   } else if(stest){
        strcpy(dests, "127.0.0.1");
        dest = inet_addr(dests);
        strcpy(srcs, "127.0.0.1");
        src = inet_addr(srcs);
        strcpy(file, "secret");
        sport = 7005;
        dport = 7005;
        seq = true;
        server = true;
        client = false;
   } else {
    if (seq == false && ipid == false && ack == false){
        printf("No encoding was selected, defaulted to IPID encoding\n");
        ipid = true;
   } else if(seq == true && ipid == true && ack == true){
        printf("Only one encoding type can be chosen");
   } else if(seq == true && ipid == true){
        printf("Only one encoding type can be chosen");
   } else if(ipid == true && ack == true){
        printf("Only one encoding type can be chosen");
   }

    if(client){
        if(dest == 0){
            printf("Please select a destination address");
            print_usage();
            exit(0);
        }
        if(src == 0){
            printf("Please select a source address");
            print_usage();
            exit(0);
        }
        if(dport == 0){
            printf("Please select a destination port");
            print_usage();
            exit(0);
        }
       if(sport == 0){
            sport = 1+(int)(10000.0*rand()/(RAND_MAX+1.0));
            printf("Source Port: Random");
        }
        printf("\nClient selected: Sending data. \n\n");
    } else {
       if(dest == 0){
            strcpy(dests,"Any Host");
       }
        if(src == 0){
            strcpy(srcs,"Any Host");
        }
        if(sport == 0){
            printf("Source Port: Any Port\n");
        }
            printf("\nServer Mode: Listening for data \n\n");
    }
   }
   //check if there is a file
   //check if server or client
    if(client){
    SendPacket(src, dest, sport, dport, file, ipid, seq, ack);
    } else {
    RecvPacket(src, sport, dport, file, ipid, seq, ack);
    }

    exit(0);
}

void RecvPacket(unsigned int src, unsigned short sport, unsigned short dport, char *filename, int ipid, int seq, int ack){

    //if server open file and read it
    int recv_fd;
    FILE *output;

    srand((getpid())*(dport));

        if((output=fopen(filename,"wb"))==NULL){
            printf("Cannot open %s for writing\n", filename);
            exit(1);
        }
        while(1){
            recv_fd = socket(AF_INET, SOCK_RAW, 6);
            if(recv_fd < 0){
                perror("receive socket cannot be opened");
                exit(1);
            }

            read(recv_fd, (struct recv_header *)&recv_header, 9999);

            if(sport == 0){
                //check if packet is a syn and the source address is correct
                if((recv_header.tcp.syn == 1) && (recv_header.ip.saddr == src)){
                    if(ipid==1){
                        //decoding ipid
                        printf("Receiving Data: %c\n", recv_header.ip.id);
                        fprintf(output, "%c", recv_header.ip.id);
                        fflush(output);
                    } else if(seq==1){
                        //decoding sequence
                        printf("Receiving Data: %c \n", recv_header.tcp.seq);
                        fprintf(output, "%c", recv_header.tcp.seq);
                        fflush(output);
                    } else if(ack==1){
                        //decoding ack numbers
                        printf("Receiving Data: %c\n", recv_header.tcp.ack_seq);
                        fprintf(output, "%c", recv_header.tcp.ack_seq);
                        fflush(output);
                    }
                }
            } else {
                if((recv_header.tcp.syn==1) && (ntohs(recv_header.tcp.dest) == sport)){
                    if(ipid==1){
                        printf("Receiving Data: %c\n", recv_header.ip.id);
                        fprintf(output, "%c", recv_header.ip.id);
                        fflush(output);
                    } else if(seq==1){
                        printf("Receiving Data: %c\n", recv_header.tcp.seq);
                        fprintf(output, "%c", recv_header.tcp.seq);
                        fflush(output);
                    } else if(ack==1){
                        printf("Receiving Data: %c\n", recv_header.tcp.ack_seq);
                        fprintf(output,"%c", recv_header.tcp.ack_seq);
                        fflush(output);
                    }
                }
            }
            //close socket
            close(recv_fd);
        }
        //close file
        fclose(output);
    }

void SendPacket(unsigned int src, unsigned int dst, unsigned short sport, unsigned short dport, char *filename, int ipid, int seq, int ack){

    //if server open file and read it
    int ch;
    int send_fd;
    struct sockaddr_in sin;
    FILE *input;

    srand((getpid())*(dport));

        if((input=fopen(filename, "rb"))== NULL){
            printf("File: %s could not be opened \n", filename);
        } else {
        while((ch=fgetc(input)) != EOF){

            //without sleep the characters are read in the wrong order
            sleep(1);

           //IP HEADER
           //VERSION | IHL | TOS | TOTAL LENGTH
           //IDENTIFICATION | FLAGS | FRAGMENT OFFSET
           //TTL | PROTOCOL | HEADER CHECKSUM
           //SOURCE ADDRESS
           //DESTINATION ADDRESS
           //OPTIONS
           send_header.ip.ihl = 5;
           send_header.ip.version = 4;
           send_header.ip.tos = 0;
           send_header.ip.tot_len = htons(40);
           if(ipid == 0){
                send_header.ip.id = (int)(255.0*rand()/(RAND_MAX+1.0));
           } else {
                send_header.ip.id = ch;
           }
           send_header.ip.frag_off = 0;
           send_header.ip.ttl = 64;
           send_header.ip.protocol = IPPROTO_TCP;
           send_header.ip.check = 0;
           send_header.ip.saddr = src;
           send_header.ip.daddr = dst;
           // change host byte order to network byte order
           send_header.tcp.source = htons(sport);
           if(seq == 0){
                send_header.tcp.seq = 1+(int)(10000.0*rand()/(RAND_MAX+1.0));
           } else {
                send_header.tcp.seq = ch;
           }
           //host byte order to network byte order
           send_header.tcp.dest = htons(dport);

           send_header.tcp.ack_seq = 0;
           send_header.tcp.res1 = 0;
           send_header.tcp.doff = 5;
           send_header.tcp.fin = 0;
           send_header.tcp.syn = 1;
           send_header.tcp.rst = 0;
           send_header.tcp.psh = 0;
           send_header.tcp.ack = 0;
           send_header.tcp.urg = 0;
           send_header.tcp.res2 = 0;
           send_header.tcp.window = htons(512);
           send_header.tcp.check = 0;
           send_header.tcp.urg_ptr = 0;


           //put packet into the socket struct
          sin.sin_family = AF_INET;
          sin.sin_port = send_header.tcp.source;
          sin.sin_addr.s_addr = send_header.ip.daddr;

          //open raw socket
          send_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
          if(send_fd < 0){
                perror("Cannot open raw socket");
                exit(1);
          }

          send_header.ip.check = in_cksum((unsigned short *)&send_header.ip, 20);

          pseudo_header.source_address = send_header.ip.saddr;
          pseudo_header.destination_address = send_header.ip.daddr;
          pseudo_header.placeholder = 0;
          pseudo_header.protocol = IPPROTO_TCP;
          pseudo_header.tcp_length = htons(20);

          bcopy((char *)&send_header.tcp, (char *)&pseudo_header.tcp, 20);
          send_header.tcp.check = in_cksum((unsigned short *)&pseudo_header, 32);
          sendto(send_fd, &send_header, 40, 0, (struct sockaddr *)&sin, sizeof(sin));
          printf("Sending Data: %c\n", ch);

          close(send_fd); //close the socket
        }
        fclose(input); //close the file
        }
    }


unsigned short in_cksum(unsigned short *ptr, int nbytes){
    register long sum;
    u_short oddbyte;
    register u_short answer;

    sum = 0;
    while(nbytes > 1){
        sum += *ptr++;
        nbytes -= 2;
    }

    if(nbytes == 1){
        oddbyte = 0;
        *((u_char *) &oddbyte) = *(u_char *)ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return(answer);
}