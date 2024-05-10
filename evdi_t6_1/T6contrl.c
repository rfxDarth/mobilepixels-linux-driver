#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>

#include "t6usbdongle.h"
char romfile[8388608];

static void usage(FILE *fp, int argc, char **argv)
{
        fprintf(fp,
                 "version 23031501\n"
                 "Usage: sudo %s [options]\n\n"
                 "Options:\n"
                 "-t | --touch         Switch touch [on/off/get] \n"
                 "-b | --brightness    Change screen brightness[0 ~100/get]\n"
                 "-s | --usage time    usage time [get] (unit /min)\n"
                 "-f | --firmware      Get firmware version\n"
                 "-n | --sn            Get serial number\n"
                 //"-r | --rotate        Set rotate state [0,90,180,270]\n"
                 //"-c | --custom state  Set custom state \n"
                 //"-u | --update  	   update chip firmware \n"
                 "-h | --help          Print this message\n",
                
                 //"\n ex:\n sudo T6contrl -t on \n sudo T6contrl -t get \n sudo T6contrl -b 100 \n sudo T6contrl -b get \n sudo T6contrl -s 500 \n sudo T6contrl -s get \n sudo T6contrl -f \n sudo T6contrl -r 90 \n sudo T6contrl -c\n",
                 argv[0]);
}

static const char short_options[] = "t:b:s:r:u:d:fchn";

static const struct option
long_options[] = {
        { "touch",     required_argument, NULL, 't' },
        { "brightness",required_argument, NULL, 'b' },
        { "usagetime" ,required_argument, NULL, 's' },
        { "firmware"  ,no_argument, NULL, 'f' },
        { "sn"        ,no_argument, NULL, 'n' },
        { "rotate"    ,required_argument, NULL, 'r' },
        { "update"    ,required_argument, NULL, 'u' },
        { "custom",    no_argument, NULL, 'c' },
        { "down",      required_argument, NULL, 'd' },
        { "help",      no_argument, NULL, 'h' },
        { 0, 0, 0, 0 }
};

static int is_T6dev( libusb_device *dev)
{
	struct libusb_device_descriptor desc;
	int rc = libusb_get_device_descriptor( dev, &desc );

	if( LIBUSB_SUCCESS == rc && desc.idVendor == 0x0711 && (desc.idProduct == 0x5600 || desc.idProduct == 0x5601 || desc.idProduct == 0x560b || desc.idProduct == 0x5621))
    	return 1;
	else if(LIBUSB_SUCCESS == rc && desc.idVendor == 0x03f0 && desc.idProduct == 0x0182)	
		return 1;
	else 
		return 0;
}

libusb_device_handle* GetT6Device (){

	int i; 
	int busid = 0 , devid = 0 ;
	int ret ;

	
	ssize_t cnt=0;
	libusb_device **list;
    libusb_device *T6dev = NULL;
    libusb_device_handle* t6usbdev = NULL;
	libusb_context *ctx = NULL;
	
	libusb_init(&ctx);
	cnt = libusb_get_device_list(ctx, &list);
	
	// find T6 device
	for(i = 0; i < cnt; i++){
		libusb_device *device = list[i];
		busid = libusb_get_bus_number(device);
		devid = libusb_get_device_address(device);
		if(is_T6dev(device)){	
			ret = libusb_open(device, &t6usbdev);
	
			if(ret != 0) {
				DEBUG_PRINT("T6:  libusb_open failed\n");
				continue ;
			}
			libusb_free_device_list(list, 1);
			return t6usbdev;
		  
		}
  	}
	
	libusb_free_device_list(list, 1);
    return NULL;

	
	
}

int  FileRead(char *filename ,char *ptr )
{
	FILE *file = NULL ;
	int ret = 0 ;
	//printf("func =%s \n",__func__);
	file = fopen(filename,"rb");
	if(file == NULL)
		return -1;
	
	ret = fread(ptr,1,8388608,file);
	if(ret <= 0){
		fclose(file);
		return ret;
	}
	fclose(file);
	return ret;

}



int main(int argc, char **argv)
{

	//SetT6Device();
	int slen;
	int fun = 0;
	int on  = 0;
	int ret = 0;
	int ustime = 0;
	char bvalue = 0;
	char ver[4];
    char sn[8];
	char rsn[8];
	int i = 0;
	libusb_device_handle* t6usbdev = NULL;

	t6usbdev = GetT6Device();
    if(t6usbdev == NULL){
		printf("T6 device not found \n");
        exit(EXIT_SUCCESS);
    }
	for (;;) {
		int idx;
		int c;
        int rom_size = 0 ;
		c = getopt_long(argc, argv,
						short_options, long_options, &idx);

	   
	    if (-1 == c)
	   		break;

		switch (c) {
		
        
		case 't':
			    printf("active = %s \n",optarg);
				if(strcmp("on", optarg)== 0){
				   ret = t6_libusb_set_touch(t6usbdev,1);
				   if(ret == 0)
				     printf("set Touch state = on \n");
				}else if(strcmp("off", optarg)== 0){
                   ret = t6_libusb_set_touch(t6usbdev,0);
				   if(ret == 0)
				     printf("set Touch state = off  \n");
				}else if(strcmp("get", optarg)== 0){
				   ret = t6_libusb_get_touch(t6usbdev);
			       if(ret < 0)
				   	   break;
				   printf("get Touch state = %d \n",ret);
				}
				break;

		case 'h':
				usage(stdout, argc, argv);
				libusb_close(t6usbdev);
				exit(EXIT_SUCCESS);
        case 'c':
				ret =t6_libusb_set_custom(t6usbdev );
				if(ret == 0)
				 	printf("set custom successful  \n");
				break;
		
		case 'b':
			    printf("active = %s \n",optarg);
				if(strcmp("get", optarg)== 0){
				   ret = t6_libusb_get_brightness(t6usbdev);
			       if(ret < 0)
				   	   break;
				   printf("brightness value = %d \n",ret);

				}else{
					
					bvalue = strtol(optarg, NULL, 0);
					ret = t6_libusb_set_brightness(t6usbdev,bvalue);
					if(ret == 0)
						printf("set brightness = %d \n",bvalue);
				}
				break;

		case 's':

			 printf("active = %s \n",optarg);
			 if(strcmp("get", optarg)== 0){
				ret = t6_libusb_get_usagetime(t6usbdev);
				if(ret < 0)
					break;
				printf("usagetime value = %d \n",ret);

			 }else{
				 
				 ustime = strtol(optarg, NULL, 0);
				 ret = t6_libusb_set_usagetime(t6usbdev,ustime);
				 if(ret == 0)
					 printf("set usagetime = %d \n",ustime);
			 }
			 break;
        case 'r':
			  printf("active = %s \n",optarg);
			  on = strtol(optarg, NULL, 0);
			  if(on == 0)
				ret = t6_libusb_set_rotate(t6usbdev,0);
			  if(on == 90)
				ret = t6_libusb_set_rotate(t6usbdev,1);	
			  if(on == 180)
				ret = t6_libusb_set_rotate(t6usbdev,2);	
			  if(on == 270)
				ret = t6_libusb_set_rotate(t6usbdev,3);	
			  if(ret == 0)
			  	printf("set rotate = %d \n",on);
        case 'f':
			
				ret = t6_libusb_get_version(t6usbdev ,6);
				if(ret < 0)
					break;
				memcpy(ver,&ret,4);
				if(ver[0] == 0)
			  	  printf("HP version =%02x.%02x.%02x \n",ver[3],ver[2],ver[1]);
			  	else
			  	  printf("HP version =%02x.%02x.%02x.%02x \n",ver[3],ver[2],ver[1],ver[0]);
				ret = t6_libusb_get_version(t6usbdev ,2);
				if(ret < 0)
					break;
				memcpy(ver,&ret,4);
			  	printf("MCT Display version =%02x.%02x.%02x.%02x \n",ver[3],ver[2],ver[1],ver[0]);
		        break;
		case 'n':
			    
                ret = t6_libusb_get_sn(t6usbdev ,sn);
				if(ret < 0)
					break;
				slen = strlen(sn);	
				for(i = 0 ; i < slen ; i++){
					rsn[i] = sn[slen - i -1];
				}
				printf("serial number = 2TP%s \n",rsn);
				break;
	        case 'd':
	        		bvalue = strtol(optarg, NULL, 0);
	        		if(bvalue < 0 || bvalue > 1)
	        		   bvalue = 1;	
	        		ret = t6_libusb_set_monitor_power2(t6usbdev,bvalue);
	        		break;			
		case 'u':
			    printf("file name = %s \n",optarg);
			        //system("killall T6evdi");
			        system("systemctl stop T6evdi.service");
				
				t6_libusb_donglereset2(t6usbdev);
				libusb_close(t6usbdev);
				sleep(5);
				t6usbdev = NULL;
				t6usbdev = GetT6Device();
			    if(t6usbdev == NULL){
					printf("T6 device not found \n");
					system("systemctl start T6evdi.service");
			        break;
			    }
				libusb_set_configuration(t6usbdev, 1);
				libusb_claim_interface(t6usbdev, 0);
             	rom_size = FileRead(optarg,romfile);
				if(rom_size < 0){
					printf("Read file failed \n");
					system("systemctl start T6evdi.service");
					break;
				}
				printf("rom_size = %d \n",rom_size);
				if(t6_write_rom_date( t6usbdev,romfile ,rom_size) < 0){
					printf("write rom data failed \n");
					system("systemctl start T6evdi.service");
					break;		
				}
				ShowRomMsg(t6usbdev);
                		t6_libusb_donglereset2(t6usbdev);
                		libusb_close(t6usbdev);
                               //sleep(5);
				//system("systemctl start T6evdi.service");
				return 0;
		default:
				usage(stdout, argc, argv);
				libusb_close(t6usbdev);
				exit(EXIT_FAILURE);
		}
	}
   
	libusb_close(t6usbdev);
	//t6_libusb_test(t6usbdev);
	

}



