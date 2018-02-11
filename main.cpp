#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>

#include "rc-switch/RCSwitch.h"

#ifndef DEFAULT_SENDPIN
#define DEFAULT_SENDPIN 7
#endif

#ifndef DEFAULT_RETRANSMIT
#define DEFAULT_RETRANSMIT 0
#endif

#ifndef DEFAULT_PULSELENGTH
#define DEFAULT_PULSELENGTH 0
#endif

char *progname = NULL;
RCSwitch rc;

int is_binary_word(char *word) {
  for(int i = 0; word[i] != '\0'; i++) {
    if(word[i] != '0' && word[i] != '1') return 0;
  }
  return 1;
}

int is_valid_dip(char *dip) {
  if (strlen(dip) == 5 && is_binary_word(dip)) {
    return 1;
  }
  return 0;
}

void setup_rcswitch() {
  if (wiringPiSetup() == -1) {
    exit(1);
  }

  char *clirc_sendpin_env = getenv("CLIRC_SENDPIN");
  int pin = clirc_sendpin_env ? atoi(clirc_sendpin_env) : DEFAULT_SENDPIN;
  char *clirc_retransmit_env = getenv("CLIRC_RETRANSMIT");
  int retransmit = clirc_retransmit_env ? atoi(clirc_retransmit_env) : DEFAULT_RETRANSMIT;
  char *clirc_pulselength_env = getenv("CLIRC_PULSELENGTH");
  int pulse = clirc_pulselength_env ? atoi(clirc_pulselength_env) : DEFAULT_PULSELENGTH;

  rc = RCSwitch();
  rc.enableTransmit(pin);
  
  if (pulse > 0) rc.setPulseLength(pulse);
  if (retransmit > 0) rc.setRepeatTransmit(retransmit);

}

void usage(int code) {
  printf("Example: %s selector 1 3 on\n", progname);
  printf("Example: %s dip 01010 10100 on\n", progname);
  printf("Example: %s intertechno a 2 3 on\n", progname);
  printf("Example: %s raw 000000000000000101010100\n", progname);
  exit(code);
}

int main(int argc, char** argv) {
  progname = argv[0];

  if (argc > 1 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))) {
    usage(0);
  }

  if (argc == 3 && !strcmp(argv[1], "raw")) {
    char *raw = argv[2];
    if (is_binary_word(raw)) {
      setup_rcswitch();
      rc.send(raw);
      exit(0);
    }
  }

  if (argc == 5 && !strcmp(argv[1], "selector")) {
    int group = atoi(argv[2]);
    int device = atoi(argv[3]);
    if (group > 0 && group < 5 && device > 0 && device < 5) {
      if(!strcmp(argv[4], "on")) {
        setup_rcswitch();
        rc.switchOn(group, device);
        exit(0);
      } else if (!strcmp(argv[4], "off")) {
        setup_rcswitch();
        rc.switchOff(group, device);
        exit(0);
      }
    }
  }

  if (argc == 5 && !strcmp(argv[1], "dip")) {
    char *group = argv[2];
    char *device = argv[3];
    if (is_valid_dip(group) && is_valid_dip(device)) {
      if(!strcmp(argv[4], "on")) {
        setup_rcswitch();
        rc.switchOn(group, device);
        exit(0);
      } else if (!strcmp(argv[4], "off")) {
        setup_rcswitch();
        rc.switchOff(group, device);
        exit(0);
      }
    }
  }

  if (argc == 6 && !strcmp(argv[1], "intertechno")) {
    int family_arg_len = strlen(argv[2]);
    char family = argv[2][0];
    int group = atoi(argv[3]);
    int device = atoi(argv[4]);
    if(family_arg_len == 1 && family >= 'a' && family <= 'f' &&
       group > 0 && group < 5 && device > 0 && device < 5) {
      if(!strcmp(argv[5], "on")) {
        setup_rcswitch();
        rc.switchOn(group, device);
        exit(0);
      } else if (!strcmp(argv[5], "off")) {
        setup_rcswitch();
        rc.switchOff(group, device);
        exit(0);
      }
    } 
  }

  usage(-1);
}
