#include "mbed.h"


Serial pc(USBTX, USBRX);

Serial xbee(D12, D11);


void reply_message(char *xbee_reply, char *messange){

  xbee_reply[0] = xbee.getc();

  xbee_reply[1] = xbee.getc();

  xbee_reply[2] = xbee.getc();

  if(xbee_reply[1] == 'O' && xbee_reply[2] == 'K'){

   pc.printf("%s\r\n", messange);

   xbee_reply[0] = '\0';

   xbee_reply[1] = '\0';

   xbee_reply[2] = '\0';

  }

}


void check_addr(char *xbee_reply, char *messenger){

  xbee_reply[0] = xbee.getc();

  xbee_reply[1] = xbee.getc();

  xbee_reply[2] = xbee.getc();

  xbee_reply[3] = xbee.getc();

  pc.printf("%s = %c%c%c\r\n", messenger, xbee_reply[1], xbee_reply[2], xbee_reply[3]);

  xbee_reply[0] = '\0';

  xbee_reply[1] = '\0';

  xbee_reply[2] = '\0';

  xbee_reply[3] = '\0';

}


int main(){

  int i=0;

  pc.baud(9600);


  char xbee_reply[4];


  // XBee setting

  xbee.baud(9600);

  xbee.printf("+++");

  xbee_reply[0] = xbee.getc();

  xbee_reply[1] = xbee.getc();

  if(xbee_reply[0] == 'O' && xbee_reply[1] == 'K'){

   pc.printf("enter AT mode.\r\n");

   xbee_reply[0] = '\0';

   xbee_reply[1] = '\0';

  }


  xbee.printf("ATMY 0x240\r\n");

  reply_message(xbee_reply, "set MY : 0x240");


  xbee.printf("ATDL 0x140\r\n");

  reply_message(xbee_reply, "set DL : 0x140");


  xbee.printf("ATWR\r\n");

  reply_message(xbee_reply, "write config");


  xbee.printf("ATMY\r\n");

  check_addr(xbee_reply, "MY");


  xbee.printf("ATDL\r\n");

  check_addr(xbee_reply, "DL");


  xbee.printf("ATCN\r\n");

  reply_message(xbee_reply, "exit AT mode");

  xbee.getc();


  // start

  pc.printf("start\r\n");

  char buf[100] = {0};


  while(1){

    i = 0;


    xbee.getc();

    while (i < 4){

      buf[i] = xbee.getc();

      i++;

      buf[i] = '\0';

    }

    pc.printf("Get : %s\r\n", buf);

    xbee.printf("%s", buf);

    wait(0.1);


    i = 0;

    while(buf[i] != '\0'){

      buf[i] = 0;

      i++;

    }

  }

}