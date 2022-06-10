//bob-alice Common Secret key

#include <asm-generic/fcntl.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include <inttypes.h>

#include "../../include/cryptocore_ioctl_header.h"



__u32 *result;
/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void close_physical (int);
void ReadFileContent(FILE *);


int main(void)
{	
	

	int dd = -1;
	int ret_val;
	__u32 trng_val = 0;
	__u32 i = 0;

	double seconds;
	struct timespec tstart={0,0}, tend={0,0};

	
	if ((dd = open_physical (dd)) == -1)
      return (-1);
	  
	  
// Stop TRNG and clear FIFO
	trng_val = 0x00000010;
	ret_val = ioctl(dd, IOCTL_SET_TRNG_CMD, &trng_val);
	if(ret_val != 0) {
		printf("Error occured\n");
	}

	usleep(10);

// Configure Feedback Control Polynomial
	trng_val = 0x0003ffff;
	ret_val = ioctl(dd, IOCTL_SET_TRNG_CTR, &trng_val);
	if(ret_val != 0) {
		printf("Error occured\n");
	}

// Configure Stabilisation Time
	trng_val = 0x00000050;
	ret_val = ioctl(dd, IOCTL_SET_TRNG_TSTAB, &trng_val);
	if(ret_val != 0) {
		printf("Error occured\n");
	}

// Configure Sample Time
	trng_val = 0x00000006;
	ret_val = ioctl(dd, IOCTL_SET_TRNG_TSAMPLE, &trng_val);
	if(ret_val != 0) {
		printf("Error occured\n");
	}

// Start TRNG
	trng_val = 0x00000001;
	ret_val = ioctl(dd, IOCTL_SET_TRNG_CMD, &trng_val);
	if(ret_val != 0) {
		printf("Error occured\n");
	}

	usleep(10);

		MontExp_params_t MontExp_4096_test = { 4096,
	1,
	0,
	{ 0xf4c35aa8, 0x6e931870, 0x3e6fbd23, 0x1f00470e, 
      0x3ac5a249, 0x446e003f, 0xf351cb23, 0x4a0295ad, 
      0x6a23b414, 0xf4bbc35a, 0xe1b1f853, 0xd693c03b, 
      0x843ccf6c, 0x62e119d3, 0x7803a963, 0xa29a70e3, 
      0x5ab158ed, 0xba300223, 0x21395f62, 0x0173a846, 
      0x3e7be036, 0x0ac60b85, 0x08a1bb96, 0x7d64c0fb, 
      0xe2b1a3f5, 0x86b1b891, 0x0d1c02ce, 0x0346bfc7, 
      0x00fe40b7, 0x385d8a73, 0xdfe50359, 0x16734d0d, 
      0x898bd08b, 0x83a1261f, 0xe3a4d240, 0x23c1dd34, 
      0x7ecacbad, 0x884d0d13, 0x7624176f, 0xfbf0a611, 
      0x720d5a15, 0x1017f1ac, 0xeb268c97, 0xbc2bb8af, 
      0x9e25e0d8, 0xd386631b, 0x183af1b4, 0x610aeff9, 
      0xbff21bac, 0xa8663ab1, 0xccebbb87, 0xe49db367, 
      0x368c0589, 0x2d415e13, 0x5d8a650e, 0x0d6d90ab, 
      0x14e8f47a, 0xa86b5ba8, 0x12c05073, 0x105472e6, 
      0x032758ca, 0x98ccd30b, 0x099a10c5, 0x4cdeade0, 
      0xd8b863ab, 0xd52794f6, 0x6143c417, 0x6d966543, 
      0x1cb1264f, 0xf068ab75, 0xee8f910a, 0x7fc9bc6a, 
      0xe03980a8, 0x58c6031c, 0xaecbd7f8, 0xc00b6329, 
      0xc103d76c, 0xe82872b9, 0xb13e49ac, 0xc4e4cf9d, 
      0xd6fab254, 0xeadde42f, 0x2632015c, 0xdebbfab5, 
      0x5bc734fe, 0x0622627f, 0xfd160456, 0xee17af65, 
      0x5dc94289, 0x26c68a96, 0xedc7574b, 0xfb5a1851, 
      0x4af87539, 0xbc024e36, 0xe25438a3, 0x1fdfe7eb, 
      0x83f8be24, 0x7389427b, 0x6a1f3f67, 0x515b77a0, 
      0x017709d2, 0x133bc1a5, 0xafe38ea3, 0x5b05a4f5, 
      0x7a42eeb4, 0xc9d043ba, 0xeac4d034, 0x7b6e1568, 
      0x922ff274, 0x5efea5fa, 0xa70dc43a, 0x21e4b371, 
      0x8d9e3508, 0xa42f6416, 0x69f7c8d7, 0x3f437406, 
      0x12d0f895, 0x380c9f89, 0xab23d403, 0xcc18a119, 
      0x90e77126, 0xae67fabf, 0x8a961ff2, 0x18407b20, 
      0xcbfb0167, 0x69392026, 0xcec58af2, 0x14022b4f },
	  
	{  },
	{  },
	{  },
	};

//random exponent from file

	FILE *fpReadExp = fopen("/home/bob/BobSecretExpVal.txt", "r");
    if (fpReadExp == NULL) {
        fprintf(stderr, "Error while reading file");
        return 0;
    }

    ReadFileContent(fpReadExp);


	for(i=0; i<MontExp_4096_test.prec/32; i++)
	{
	MontExp_4096_test.e[i] = result[i];
	}

	FILE *fpReadBase = fopen("/home/bob/AliceP.txt", "r");
	if (fpReadBase == NULL) {
		fprintf(stderr, "Error while reading file");
		return 0;
    }

    ReadFileContent(fpReadBase);

	for(i=0; i<MontExp_4096_test.prec/32; i++)
	{
	MontExp_4096_test.b[i] = result[i];
	}



	printf("B: 0x");
	for(i=0; i<MontExp_4096_test.prec/32; i++){
		printf("%08x", MontExp_4096_test.b[i]);
	}
	printf("\n\n");
	printf("N: 0x");
	for(i=0; i<MontExp_4096_test.prec/32; i++){
		printf("%08x", MontExp_4096_test.n[i]);
	}
	printf("\n\n");
	
	printf("E: 0x");
	for(i=0; i<MontExp_4096_test.prec/32; i++){
		printf("%08x", MontExp_4096_test.e[i]);
	}
	printf("\n\n");	

	clock_gettime(CLOCK_MONOTONIC, &tstart);
	ret_val = ioctl(dd, IOCTL_MWMAC_MONTEXP, &MontExp_4096_test);
	clock_gettime(CLOCK_MONOTONIC, &tend);

	if(ret_val != 0) {
		printf("Error\n");
	}
	
	printf("Common Secret key:  0x");
	for(i=0; i<MontExp_4096_test.prec/32; i++){
		printf("%08x", MontExp_4096_test.c[i]);
	}
	printf("\n\n");
	
	FILE *f_write = fopen("/home/bob/BobCS.txt", "w");
	
	char AliceCS_hex [128]= "";
    for(i=0 ; i< MontExp_4096_test.prec/32; i++)
	{
        sprintf(AliceCS_hex, "%08x", MontExp_4096_test.c[i]);
        fprintf(f_write,"%s",AliceCS_hex);
    }
	
	

	seconds = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
	if (seconds*1000000.0 > 1000.0)
		printf("Reading 4096 random bits took about %.5f ms\n", seconds*1000.0);
	else 
		printf("Reading 4096 random bits took about %.5f us\n", seconds*1000000.0);


	MontExp_4096_test.sec_calc = 1;	
	clock_gettime(CLOCK_MONOTONIC, &tstart);
	ret_val = ioctl(dd, IOCTL_MWMAC_MONTEXP, &MontExp_4096_test);
	clock_gettime(CLOCK_MONOTONIC, &tend);

	printf("\n\n");
	
	
	seconds = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
	if (seconds*1000000.0 > 1000.0)
		printf("(With sec_calc=1) Reading 4096 random bits took about %.5f ms\n", seconds*1000.0);
	else 
		printf("(With sec_calc=1) Reading 4096 random bits took about %.5f us\n", seconds*1000000.0);

	close_physical (dd);   // close /dev/cryptocore
  
	return 0;
}

int open_physical (int dd)
{
   if (dd == -1)
      if ((dd = open( "/dev/cryptocore", (O_RDWR | O_SYNC))) == -1)
      {
         printf ("ERROR: could not open \"/dev/cryptocore\"...\n");
         return (-1);
      }
   return dd;
}


void close_physical (int dd)
{
   close (dd);
}

void ReadFileContent(FILE *fp)
{	
	char file_data[4096]="";
    	char *del_token;
    	int next_ele = 0;
	int cnt=0;
	
	fscanf(fp,"%s", file_data);
	
	//printf("%s \n", file_data);
	

    	int len = 1 + strlen(file_data) / 2; 
	//printf("%d  %d",strlen(file_data), len);
	
    result  = malloc(len* sizeof(*result));

    if (result == NULL)
        exit(-1);                                
    del_token = strtok(file_data, "");   
	//printf("\n\n token\n %s\n\n", del_token);
    while (cnt<128 && del_token!=NULL) { //because 4096(prec)/32

        if (next_ele >= len)
            exit(-2);                           
        if (1 != sscanf(del_token, "%08x", result + next_ele))
            exit(-3);                           
        next_ele++;
        del_token=del_token+8;
	cnt++;
    }

}
