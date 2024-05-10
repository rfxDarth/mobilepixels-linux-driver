#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>		/* open */
#include <sys/ioctl.h>		/* ioctl */
#include<stdbool.h>
#include "t6usbdongle.h"

void hex_dump(char *data, int size, char *caption)
{
	int i; // index in data...
	int j; // index in line...
	char temp[8];
	char buffer[128];
	char *ascii;

	memset(buffer, 0, 128);

	printf("---------> %s <--------- (%d bytes from %p)\n", caption, size, data);

	// Printing the ruler...
	printf("        +0          +4          +8          +c            0   4   8   c   \n");

	// Hex portion of the line is 8 (the padding) + 3 * 16 = 52 chars long
	// We add another four bytes padding and place the ASCII version...
	ascii = buffer + 58;
	memset(buffer, ' ', 58 + 16);
	buffer[58 + 16] = '\n';
	buffer[58 + 17] = '\0';
	buffer[0] = '+';
	buffer[1] = '0';
	buffer[2] = '0';
	buffer[3] = '0';
	buffer[4] = '0';
	for (i = 0, j = 0; i < size; i++, j++) {
		if (j == 16) {
			printf("%s", buffer);
			memset(buffer, ' ', 58 + 16);

			sprintf(temp, "+%04x", i);
			memcpy(buffer, temp, 5);

			j = 0;
		}

		sprintf(temp, "%02x", 0xff & data[i]);
		memcpy(buffer + 8 + (j * 3), temp, 2);
		if ((data[i] > 31) && (data[i] < 127))
			ascii[j] = data[i];
		else
			ascii[j] = '.';
	}

	if (j != 0)
		printf("%s", buffer);
}

unsigned long GetTickCount()
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
int t6_libusb_get_rotate(libusb_device_handle *t6usbdev)
{
	char  rotate = 0 ;
	int  ret ,usize;
	usize = sizeof(T6ROMDISPLAYSECTIONHDR);
	PT6ROMDISPLAYSECTIONHDR  Dispsecthdr;
	
	
	Dispsecthdr = (PT6ROMDISPLAYSECTIONHDR)malloc(usize);
	printf("T6ROMDISPLAYSECTIONHDR =%d \n",usize);
#ifdef __i386__
	printf("T6ROMDISPLAYCAPS =%u \n",sizeof(T6ROMDISPLAYCAPS));
	printf("T6ROMDISPLAYINTERFACE =%u \n",sizeof(T6ROMDISPLAYINTERFACE));
#else
	printf("T6ROMDISPLAYCAPS =%lu \n",sizeof(T6ROMDISPLAYCAPS));
	printf("T6ROMDISPLAYINTERFACE =%lu \n",sizeof(T6ROMDISPLAYINTERFACE));
#endif
	ret = libusb_control_transfer(t6usbdev, 0xc0, VENDOR_REQ_QUERY_SECTION_DATA, 0, 0, (UINT8 *)Dispsecthdr, usize, 3000);
	if(ret < 0) {
		printf("%s: libusb_control_transfer fail\n", __FUNCTION__);
		return 0;
	} 
    rotate =(Dispsecthdr->DispFunc >> 4) & 0x0F;
 	
	//hex_dump((char *)Dispsecthdr,ret,(char*)"section");
	free(Dispsecthdr);
	return rotate;

}


int t6_libusb_get_jpegreset(libusb_device_handle *t6usbdev)
{
	int  resetbit = 0 ;
	int  ret ,usize;
	usize = sizeof(T6ROMDISPLAYSECTIONHDR);
	PT6ROMDISPLAYSECTIONHDR  Dispsecthdr;
	
	
	Dispsecthdr = (PT6ROMDISPLAYSECTIONHDR)malloc(usize);
	printf("T6ROMDISPLAYSECTIONHDR =%d \n",usize);
#ifdef __i386__
	printf("T6ROMDISPLAYCAPS =%u \n",sizeof(T6ROMDISPLAYCAPS));
	printf("T6ROMDISPLAYINTERFACE =%u \n",sizeof(T6ROMDISPLAYINTERFACE));
#else
	printf("T6ROMDISPLAYCAPS =%lu \n",sizeof(T6ROMDISPLAYCAPS));
	printf("T6ROMDISPLAYINTERFACE =%lu \n",sizeof(T6ROMDISPLAYINTERFACE));
#endif
	ret = libusb_control_transfer(t6usbdev, 0xc0, VENDOR_REQ_QUERY_SECTION_DATA, 0, 0, (UINT8 *)Dispsecthdr, usize, 3000);
	if(ret < 0) {
		printf("%s: libusb_control_transfer fail\n", __FUNCTION__);
		return 0;
	} 
    resetbit =(Dispsecthdr->DispFunc >> 3) & 0x00000001;
 	
	//hex_dump((char *)Dispsecthdr,ret,(char*)"section");
	free(Dispsecthdr);
	return resetbit;

}


int t6_libusb_get_displaysectionheader(libusb_device_handle *t6usbdev)
{
	int  number = 0 ;
	int  ret ,usize;
	usize = sizeof(T6ROMDISPLAYSECTIONHDR);
	PT6ROMDISPLAYSECTIONHDR  Dispsecthdr;
	
	
	Dispsecthdr = (PT6ROMDISPLAYSECTIONHDR)malloc(usize);
	printf("T6ROMDISPLAYSECTIONHDR =%d \n",usize);
#ifdef __i386__
	printf("T6ROMDISPLAYCAPS =%u \n",sizeof(T6ROMDISPLAYCAPS));
	printf("T6ROMDISPLAYINTERFACE =%u \n",sizeof(T6ROMDISPLAYINTERFACE));
#else
	printf("T6ROMDISPLAYCAPS =%lu \n",sizeof(T6ROMDISPLAYCAPS));
	printf("T6ROMDISPLAYINTERFACE =%lu \n",sizeof(T6ROMDISPLAYINTERFACE));
#endif
	ret = libusb_control_transfer(t6usbdev, 0xc0, VENDOR_REQ_QUERY_SECTION_DATA, 0, 0, (UINT8 *)Dispsecthdr, usize, 3000);
	if(ret < 0) {
		printf("%s: libusb_control_transfer fail\n", __FUNCTION__);
		return 0;
	} 
   
	printf(" display section header ver =%d \n", Dispsecthdr->Version);
	printf(" display section header vid =%x \n", Dispsecthdr->vid);
	printf(" display section header pid =%x \n", Dispsecthdr->pid);
	printf(" display section DispFunc =%x \n", Dispsecthdr->DispFunc);
	printf(" Display1Caps.LinkInterfaces =%d \n", Dispsecthdr->Display1Caps.LinkInterfaces);
	printf(" Display2Caps.LinkInterfaces =%d \n", Dispsecthdr->Display2Caps.LinkInterfaces);
	if(Dispsecthdr->Display1Caps.LinkInterfaces != 0)
	    number++;
	if(Dispsecthdr->Display2Caps.LinkInterfaces != 0)
	    number++ ;
	//hex_dump((char *)Dispsecthdr,ret,(char*)"section");
	free(Dispsecthdr);
	return number;

}

int  t6_libusb_dongle_reset(PT6EVDI t6dev)
{
	 t6_libusb_donglereset(t6dev);
	 //libusb_close(t6dev->t6usbdev);
}





static int 
t6_libusb_get_resolution_num(PT6EVDI t6dev )
{
	int ret ;
	int resnum ;
	ret = libusb_control_transfer(t6dev->t6usbdev,0xc0,VENDOR_REQ_GET_RESOLUTION_TABLE_NUM,t6dev->disp_interface ,0,(char *)&resnum,4,3000);
	if(ret < 0)
		return -1;

	//printf("resolution number=%d \n",resnum);
	return resnum;
	
}

int t6_libusb_get_version(libusb_device_handle* t6usbdev ,char index)
{
	int ret = 0 ;
	int  ver = -1;
    ret = libusb_control_transfer(t6usbdev,0xc0,VENDOR_REQ_GET_VERSION,0,index,(unsigned char*)&ver,4,1000);
    if(ret <= 0)
		return -1;
	return ver;


}

int t6_libusb_get_sn(libusb_device_handle* t6usbdev ,char* sn)
{
	int ret = 0 ;
	int  ver = -1;
    ret = libusb_control_transfer(t6usbdev,0xc0,VENDOR_REQ_GET_VERSION,0,5,sn,8,1000);
    if(ret <= 0)
		return -1;
    //printf("ret = %d sn = %s \n",ret,sn);		
	return 0;


}


int T6CalculatePixelClock( unsigned long ulPixelClock,   unsigned long ulBaseFreq,  unsigned long *pN,  unsigned long *pP,  unsigned long *pQ,  unsigned long *pVideoSelect) {
                                   //kHz                 //Mhz
  double  fMhz;
  UINT32  N, P;
  unsigned char i = 0;
  UINT32 x = 4;
  unsigned char VideoSelectOutput[4] = {3, 1, 2, 0}; //{3, 2, 1, 0};

  if ((ulPixelClock < 25000) || (ulPixelClock > 400000))
    return -1;

  fMhz = (double)ulPixelClock / 1000;

  for (i = 0; i < 4; i++) {

    if ((fMhz * x) >= 800.0) {

      N = (UINT32)((double)((fMhz * x) / (double)ulBaseFreq));
      P = (UINT32)(((double)((fMhz * x) / (double)ulBaseFreq) - N) * 1000);
      *pN = N;
      *pP = P;
      *pQ = 1000;
      *pVideoSelect = VideoSelectOutput[i];
      return 0;

    } else if ((fMhz * x) > 1600.0) {

      *pN = 0;
      *pP = 0;
      *pQ = 0;
      *pVideoSelect = 0;
      return -1;

    } else {

      x = x * 2;

    }
  }
  return -1;
}


int t6_libusb_get_resolution_timing(PT6EVDI t6dev ,int w , int h ,PRESOLUTIONTIMING myres)
{
	int r_number , ret ,usize;
	int i = 0 , index = -1;
	int disp_w =w;
	int disp_h = h;
	
	unsigned char buffer[4096];
	PRESOLUTIONTIMING r_table = (PRESOLUTIONTIMING)buffer ;
	r_number = t6_libusb_get_resolution_num(t6dev);
	if(r_number < 0)
		return -1;
	
	usize = r_number * sizeof(RESOLUTIONTIMING);
	//printf("total size res table =%d \n",usize );
	//printf("RESOLUTIONTIMING =%d \n",sizeof(RESOLUTIONTIMING) );
	//r_table = malloc(usize);
	ret = libusb_control_transfer(t6dev->t6usbdev,0xc0,VENDOR_REQ_GET_RESOLUTION_TIMING_TABLE,t6dev->disp_interface ,0,(char *)r_table,usize,3000);
	if(ret < 0){
		//free(r_table);
		return -1; 
	}
	for(i = 0 ; i < r_number ; i++){
		
		
		//printf("Width = %d , Height = %d , Frequency = %d \n",r_table->HorAddrTime,r_table->VerAddrTime,r_table->Frequency);
		//printf("w = %d , h = %d  \n",w,h);
		if(t6dev->jpg_rotate == 1 || t6dev->jpg_rotate == 3){
             disp_w = h;
			 disp_h = w;
		}
		
		if(disp_w== r_table->HorAddrTime && disp_h== r_table->VerAddrTime){
			unsigned long N, P, Q, VideoSelect;
			int cmd = 40;
			int ver = t6_libusb_get_version(t6dev->t6usbdev,0);
			if(t6dev->jpg_rotate == 1 || t6dev->jpg_rotate == 3){
				r_table->HorAddrTime = w;
				r_table->VerAddrTime = h;
			}
			printf("version = %d \n",ver);
			if(ver == 0) //   == 0 lite
				cmd = 48;
			else         // == 1  super lite
				cmd = 40;
			if(T6CalculatePixelClock(r_table->PixelClock, cmd, &N, &P, &Q, &VideoSelect)== 0) {
				  r_table->FNUM = (UINT16)P;
				  r_table->FDEN = (UINT16)Q;
				  r_table->IDIV = (UINT8)N;
				  r_table->OutputSelect = (UINT8)VideoSelect;
			}
			memcpy((char*)myres,(char*)r_table,sizeof(RESOLUTIONTIMING));
			//printf(" time ok \n");
			break;
		}
		r_table++;
	}
	//free(r_table);
	return 0;

}

int t6_libusb_set_monitor_power(PT6EVDI t6dev,char on)
{
	int ret;
	ret = libusb_control_transfer(t6dev->t6usbdev,0x40,VENDOR_REQ_SET_MONITOR_CTRL,t6dev->disp_interface ,on,NULL,0,3000);
	if(ret < 0)
		return -1; 
	return 0;

}
int t6_libusb_test(libusb_device_handle*	 t6usbdev)
{
	int ret ;
	UINT8 ramsize;
	ret = libusb_control_transfer(t6usbdev, 0xc0, VENDOR_REQ_QUERY_VIDEO_RAM_SIZE, 0, 0, (UINT8 *)&ramsize, 1, 3000);
	if(ret < 0)
		return -1;

	printf("ramsize =%d \n",ramsize);
	return ramsize;
}


void t6_libusb_donglereset(PT6EVDI t6dev)
{
	libusb_control_transfer(t6dev->t6usbdev,0x40,VENDOR_REQ_SET_CANCEL_BULK_OUT,0,0,NULL,0,1000);
}

int t6_libusb_get_monitorstatus(PT6EVDI t6dev) // veiw = 0 hdmi , view =1 vga; 
{
	int ret = 0 ;
	char status = 0;
    ret = libusb_control_transfer(t6dev->t6usbdev,0xc0,VENDOR_REQ_QUERY_MONITOR_CONNECTION_STATUS,t6dev->disp_interface,0,&status,1,1000);
    if(ret < 0)
		return -1;
	//printf("monitor status =%d  ret =%d\n",status,ret);
	return status;

}



static int
EDID_Header_Check(UINT8 *pbuf)
{
	if (pbuf[0] != 0x00 || pbuf[1] != 0xFF || pbuf[2] != 0xFF ||
	    pbuf[3] != 0xFF || pbuf[4] != 0xFF || pbuf[5] != 0xFF ||
	    pbuf[6] != 0xFF || pbuf[7] != 0x00) {
		printf("EDID block0 header error\n");
		return -1;
	}
	return 0;
}

static int
EDID_Version_Check(UINT8 *pbuf)
{
	//printf("EDID version: %d.%d\n", pbuf[0x12], pbuf[0x13]);
	if (pbuf[0x12] != 0x01) {
		//printf("Unsupport EDID format,EDID parsing exit\n");
		return -1;
	}
	if (pbuf[0x13] < 3 && !(pbuf[0x18] & 0x02)) {
		//printf("EDID revision < 3 and preferred timing feature bit "
		//	"not set, ignoring EDID info\n");
		return -1;
	}
	return 0;
}

static int
EDID_CheckSum( unsigned char *buf)
{
	int i = 0, CheckSum = 0;
	unsigned char *pbuf = buf ;

	for (i = 0; i < 128; i++) {
		CheckSum += pbuf[i];
		CheckSum &= 0xFF;
	}

	return CheckSum;
}


int t6_libusb_get_edid(PT6EVDI t6dev )
{
	
	int ret , i ;
	int ucExtendEDID = 0 ;
	int edid_size =128;
    if(t6dev->t6usbdev == NULL)
        return -1;

	ret = libusb_control_transfer(t6dev->t6usbdev,0xc0,VENDOR_REQ_GET_EDID, 0, t6dev->disp_interface,t6dev->edid,128,3000);
    DEBUG_PRINT("EDID_ret= %d \n",ret);
	if(ret < 0)
		return -1;
	
	if(ret < 128)
		return 0;
	
	if(EDID_CheckSum(t6dev->edid) != 0){
		DEBUG_PRINT("EDID_CheckSum error \n");
		return 0;
	}
	
	if (EDID_Header_Check(t6dev->edid) != 0){
		DEBUG_PRINT("EDID_Header_Check error \n");
		return 0;
	}
	
	if (EDID_Version_Check(t6dev->edid) != 0){
		DEBUG_PRINT("EDID_Version_Check error \n");
		return 0;
	}
	//hex_dump(t6dev->edid,edid_size,"EDID1");	

	ucExtendEDID = t6dev->edid[126];
	if (ucExtendEDID > 0)	{//extended EDID
		for(i=1;(i<= ucExtendEDID)&&(i<=3);i++){
			ret = libusb_control_transfer( t6dev->t6usbdev, 0xc0, VENDOR_REQ_GET_EDID, 128*i, t6dev->disp_interface, (UINT8 *)t6dev->edid+128*i, 128, 3000);
			if(ret == 128) 
				edid_size += 128; 
			else
				break;
		}
	}
	

	
	//hex_dump(t6dev->edid,edid_size,"EDID2");		
	return edid_size;
}





int  t6_libusb_get_AudioEngineStatus(PT6EVDI t6dev ,PT6AUD_SETENGINESTATE setEngine )
{
	int ret  = 0;
	unsigned char num ;
	T6AUD_GETENGINESTATE aud_engine;
	ret = libusb_control_transfer(t6dev->t6usbdev,0xc0,VENDOR_REQ_AUD_GET_ENGINE_STATE,0,0,(unsigned char*)&aud_engine,sizeof(aud_engine),3000);
	if(ret < 0)
		return -1;
	
	setEngine->Activity = 0x01;
	setEngine->CyclicBufferSize = aud_engine.CyclicBufferSize;
	setEngine->FormatIndex = aud_engine.FormatIndex;
	setEngine->ReturnSize  = 9600 ; // 10ms
	
	//printf("aud_engine.Activity =%d \n",aud_engine.Activity);
	//printf("aud_engine.CyclicBufferSize =%d \n",aud_engine.CyclicBufferSize);
	//printf("aud_engine.FormatIndex =%d \n",aud_engine.FormatIndex);
	//printf("aud_engine.JackState =%d \n",aud_engine.JackState);
	return ret;
	
}	

int  t6_libusb_set_AudioEngineStatus(PT6EVDI t6dev )
{
    int ret ; 
	T6AUD_SETENGINESTATE setEngine ;
	memset(&setEngine,0,sizeof(setEngine));
	if(t6_libusb_get_AudioEngineStatus(t6dev,&setEngine) < 0){
			return -1;
		}
	//hex_dump((char *) &setEngine,sizeof(setEngine),"set endine status");
	ret = libusb_control_transfer(t6dev->t6usbdev,0x40,VENDOR_REQ_AUD_SET_ENGINE_STATE,0,0,(unsigned char*)&setEngine,sizeof(setEngine),3000);
	if(ret < 0){
		
		printf("set audio engine failed \n");
		return -1;
	}

	
	return 0;

}

int  t6_libusb_set_softready(PT6EVDI t6dev)
{
	int ret = 0 ;
	ret =  libusb_control_transfer(t6dev->t6usbdev,0x40,VENDOR_REQ_SET_SOFTWARE_READY,t6dev->disp_interface,0,NULL,0,3000);
	if(ret < 0)
		return -1;
	 
	return 0;

}


int  t6_libusb_set_resolution(PT6EVDI t6dev, int w,int h)
{
	int ret;
	RESOLUTIONTIMING myres;
	
	if(t6_libusb_get_resolution_timing(t6dev,w,h,&myres) <0 ){
		printf(" t6_libusb_get_resolution_timing failed 1\n");
		return -1;
	}
	ret = libusb_control_transfer(t6dev->t6usbdev,0x40,VENDOR_REQ_SET_RESOLUTION_DETAIL_TIMING,t6dev->disp_interface ,0,(char*)&myres,sizeof(RESOLUTIONTIMING),3000);
	if(ret < 0){
		printf(" t6_libusb_get_resolution_timing failed 2\n");
		return -1;
	}
#if 0
	ret = t6_libusb_set_AudioEngineStatus(t6dev);
	if(ret <0 ){
		printf(" t6_libusb_get_resolution_timing failed 3\n");
		//return -1;
	}

	
	ret =t6_libusb_set_monitor_power(t6dev,1);
    if(ret <0 ){
		printf(" t6_libusb_get_resolution_timing failed 4\n");
		///return -1;
    }	
#endif		
	return t6_libusb_set_softready(t6dev);
}


int t6_libusb_get_ram_size(PT6EVDI t6dev)
{
	int ret ;
	UINT8 ramsize;
	ret = libusb_control_transfer(t6dev->t6usbdev, 0xc0, VENDOR_REQ_QUERY_VIDEO_RAM_SIZE, 0, 0, (UINT8 *)&ramsize, 1, 3000);
	if(ret < 0)
		return -1;

	printf("ramsize =%d \n",ramsize);
	return ramsize;
	
}

int t6_libusb_Rgb24_full_block(PT6EVDI t6dev ,int fbaddr )
{
    int ret;
	int transferred = 0;
    BULK_CMD_HEADER bch ;
	int datasize = t6dev->Width * t6dev->Height *3;
    int videodatasize = sizeof(VIDEO_FLIP_HEADER)+ datasize;
	memset(&bch,0,sizeof(BULK_CMD_HEADER));

    char *videobuf = malloc(videodatasize);
    memset(videobuf,0,videodatasize) ;
    PVIDEO_FLIP_HEADER vfh = (PVIDEO_FLIP_HEADER)videobuf;
    UINT16 Width = t6dev->Width ;
	UINT16 Height = t6dev->Height;
	
	if(t6dev->jpg_rotate == 1 || t6dev->jpg_rotate == 3){
		Width =  t6dev->Height;
		Height = t6dev->Width ;
    }
	
    bch.Signature = 0 ;
    bch.PayloadLength = videodatasize ;
    bch.PayloadAddress = fbaddr ;
    bch.PacketLength  = bch.PayloadLength;

        //hex_dump((char *)&bch,sizeof(BULK_CMD_HEADER),"bulk cmd");
    ret = libusb_bulk_transfer(t6dev->t6usbdev, EP_BLK_OUT_ADDR,(UINT8 *)&bch, 32, &transferred, 5000);
    if(ret < 0){
		printf("bulk out failed 32 =%d \n",ret);
		free(videobuf);
		return -1;
    }
	
   
	if(t6dev->disp_interface  == 0)
		vfh->Command = VIDEO_CMD_FLIP_PRIMARY;
	if(t6dev->disp_interface  == 1)
		vfh->Command = VIDEO_CMD_FLIP_SECONDARY;
	vfh->FenceID = 0 ;
	vfh->TargetFormat = VIDEO_COLOR_RGB24;
	vfh->Y_RGB_Pitch = Width*3;
	vfh->UV_Pitch = 0;
	vfh->Y_RGB_Data_FB_Offset = fbaddr +sizeof(VIDEO_FLIP_HEADER);
	vfh->U_UV_Data_Offset = 0;
	vfh->V_Data_Offset = 0;
	vfh->Flag = 0;
	vfh->SourceFormat = VIDEO_COLOR_RGB24;
	vfh->PayloadSize = datasize;
	//vfh++;
    //memcpy((char*)vfh,pbuf,datasize);
	ret = libusb_bulk_transfer(t6dev->t6usbdev, EP_BLK_OUT_ADDR, videobuf, videodatasize, &transferred, 5000);
    if(ret < 0){
		free(videobuf);
		printf("bulk out failed 3 =%d \n",ret);
		//t6_libusb_donglereset(t6dev);
		return -1;
    }

	free(videobuf);
    return 0;

}

int t6_libusb_FilpJpegFrame(PT6EVDI t6dev,char *jpgimage ,int jpgsize,int flag)
{
    int ret = 0 ;
	int transferred = 0;
	int len = jpgsize + 48 + 1024;
	int tmp = len % 1024;
	int VideoDataSize = len -48 ;
	UINT16 Width = t6dev->Width ;
	UINT16 Height = t6dev->Height;
	
	if(t6dev->jpg_rotate == 1 || t6dev->jpg_rotate == 3){
		Width =  t6dev->Height;
		Height = t6dev->Width ;
    }

	//printf("Width = %d Height = %d \n",Width,Height);
	int Y_BlockSize = (((Width+31)/32)*32) * (((Height+31)/32)*32) +1024 ; 
    

	BULK_CMD_HEADER bch ;
	memset(&bch,0,sizeof(BULK_CMD_HEADER));
	bch.Signature = 0 ;
	bch.PayloadLength =  len ;
	bch.PayloadAddress = t6dev->cmdAddr;
	bch.PacketLength  = bch.PayloadLength;
	ret = libusb_bulk_transfer(t6dev->t6usbdev, EP_BLK_OUT_ADDR,(UINT8 *)&bch, 32, &transferred, 1000);
    if(ret < 0){
		printf("bulk out failed 32 =%d \n",ret);
		return -1;
    }
	char *videobuf = (char*) malloc(len);
	PVIDEO_FLIP_HEADER vfh = (PVIDEO_FLIP_HEADER)videobuf;
    memset(videobuf,0,len) ;
	if(t6dev->disp_interface  == 0)
		vfh->Command = VIDEO_CMD_FLIP_PRIMARY;
	if(t6dev->disp_interface  == 1)
		vfh->Command = VIDEO_CMD_FLIP_SECONDARY;
	vfh->FenceID = 0 ;
	vfh->TargetFormat = VIDEO_COLOR_NV12;
	vfh->Y_RGB_Pitch = (((Width+31)/32)*32) ;
	vfh->UV_Pitch =    (((Width+31)/32)*32);
	vfh->Y_RGB_Data_FB_Offset = t6dev->fbAddr ;
	vfh->U_UV_Data_Offset = vfh->Y_RGB_Data_FB_Offset + Y_BlockSize;
	vfh->V_Data_Offset = 0;
	vfh->Flag = flag;
	vfh->SourceFormat = VIDEO_COLOR_JPEG;
	vfh->PayloadSize = VideoDataSize ;
	
	vfh++;
	memcpy((char*)vfh,jpgimage,jpgsize);
	ret = libusb_bulk_transfer(t6dev->t6usbdev, EP_BLK_OUT_ADDR, videobuf, len, &transferred, 1000);
    if(ret < 0){
		free(videobuf);
		printf("bulk out failed 3 =%d \n",ret);
		t6_libusb_donglereset(t6dev);
		return -1;
    }

	free(videobuf);
	return 0;

}

int t6_libusb_SendAudio(PT6EVDI t6dev,char * data , int len  )
{	int ret ;	
    int transferred = 0;
	BULK_CMD_HEADER bulkhead;	
	memset(&bulkhead,0,sizeof(bulkhead));	
	bulkhead.Signature = SIGNATURE_AUDIO;	
	bulkhead.PayloadLength = len ;	
	bulkhead.PacketLength  = len ;	
	bulkhead.PayloadAddress = 0;	
	ret = libusb_bulk_transfer(t6dev->t6usbdev, EP_BLK_OUT_ADDR,(char *)&bulkhead,32, &transferred,5000);    
	if(ret < 0){		
		printf("bulk out failed 1 =%d \n",ret);	
		t6_libusb_donglereset(t6dev);
		return -1;    
		}	
	ret = libusb_bulk_transfer(t6dev->t6usbdev, EP_BLK_OUT_ADDR,data,len,&transferred,5000);    
	if(ret < 0){		
		printf("bulk out failed 2 =%d \n",ret);	
		t6_libusb_donglereset(t6dev);
		return -1;    
		}	
	return 0 ;		
}

int t6_libusb_get_interrupt(PT6EVDI t6dev,char * data , int len )
{
  int ret = 0;
  int transferred = 0;
  ret = libusb_interrupt_transfer(t6dev->t6usbdev,EP_INT_IN_ADDR,data,len,&transferred,5000);  
  if(ret < 0){	
	printf("interrupt  failed 1 =%d \n",ret);	
	return -1;
  }
  return 0 ;	

}

int t6_libusb_get_touch(libusb_device_handle* t6usbdev)
{
	int ret ;
	char bvalue = 0;
	ret = libusb_control_transfer(t6usbdev, 0xc0, 0x8b, 1, 0, (UINT8 *)&bvalue, 1, 3000);
	if(ret < 0)
		return -1;

	//printf("min =%d \n",min);
	return bvalue;



}

int t6_libusb_set_touch(libusb_device_handle* t6usbdev,char on)
{
	int ret ;

	ret = libusb_control_transfer(t6usbdev, 0x40, 0x16, 1, on, NULL, 0, 3000);
	if(ret < 0)
		return -1;

	//printf("ret =%d \n",ret);
	return 0;



}


int t6_libusb_set_brightness(libusb_device_handle* t6usbdev ,char bvalue)
{
	int ret ;

	ret = libusb_control_transfer(t6usbdev, 0x40, 0x18, bvalue, 0, NULL, 0, 3000);
	if(ret < 0)
		return -1;

	//printf("min =%d \n",min);
	return 0;



}

int t6_libusb_get_brightness(libusb_device_handle* t6usbdev)
{
	int ret ;
	char bvalue = 0;
	ret = libusb_control_transfer(t6usbdev, 0xc0, 0x8c, 0, 0, (UINT8 *)&bvalue, 1, 3000);
	if(ret < 0)
		return -1;

	//printf("min =%d \n",min);
	return bvalue;



}
int t6_libusb_set_usagetime(libusb_device_handle* t6usbdev ,int min)
{
	int ret ;

	ret = libusb_control_transfer(t6usbdev, 0x40, 0x19, 0, 0, (UINT8 *)&min, 4, 3000);
	if(ret < 0)
		return -1;

	//printf("min =%d \n",min);
	return 0;



}

int t6_libusb_get_usagetime(libusb_device_handle* t6usbdev)
{
	int ret ;
	int min = 0;
	ret = libusb_control_transfer(t6usbdev, 0xc0, 0x8f, 0, 0, (UINT8 *)&min, 4, 3000);
	if(ret < 0)
		return -1;

	//printf("min =%d \n",min);
	return min;


}

int t6_libusb_set_custom(libusb_device_handle* t6usbdev)
{
	int ret ;

	ret = libusb_control_transfer(t6usbdev, 0x40, 0x1e, 0, 0, NULL, 0, 3000);
	if(ret < 0)
		return -1;

	//printf("min =%d \n",min);
	return 0;



}

int t6_libusb_set_rotate(libusb_device_handle* t6usbdev ,char rotate)
{
	int ret ;

	ret = libusb_control_transfer(t6usbdev, 0x40, 0x1f, 0, rotate, NULL, 0, 3000);
	if(ret < 0)
		return -1;

	//printf("min =%d \n",min);
	return 0;



}



void t6_save_file(char* p,int size)
{
	
	static int i = 1;
	char filename[256];
	sprintf(filename, "frame-%d.jpg", i);
	FILE *fp=fopen(filename,"wb");
	fwrite(p, size, 1, fp);
	fflush(fp);
	fclose(fp);
	i++;
}

void t6_libusb_donglereset2(libusb_device_handle* t6usbdev)
{
	libusb_control_transfer(t6usbdev,0x40,VENDOR_REQ_SET_CANCEL_BULK_OUT,0,0,NULL,0,1000);
}
int t6_libusb_set_monitor_power2(libusb_device_handle* t6usbdev ,char on)
{
	int ret;
	ret = libusb_control_transfer(t6usbdev,0x40,VENDOR_REQ_SET_MONITOR_CTRL,0 ,on,NULL,0,3000);
	if(ret < 0)
		return -1; 
	return 0;

}


int t6_write_rom_date(libusb_device_handle* t6usbdev,char *buf ,int len)
{

	int transferred = 0; 
	int transferred1 = 0; 
	int ret = 0 ;
	T6BULKDMAHDR bulkcmd ;
	bulkcmd.Signature = SIGNATURE_ROM;
	bulkcmd.PayloadAddress = 0 ;
	bulkcmd.PayloadLength = len;
	bulkcmd.PacketSize = len;
	bulkcmd.PacketOffset = 0 ;
	bulkcmd.PacketFlags = T6_PACKET_FLAG_NONE;
	bulkcmd.FuncSpecific.Rom.Flags = T6_ROM_FLAGS_INTERRUPT ;
    bulkcmd.FuncSpecific.Rom.Verb  = T6_ROM_VERB_BURN_IMAGE2;
	bulkcmd.FuncSpecific.Rom.FenceId = ((SIGNATURE_ROM << 8) |0);
	bulkcmd.FuncSpecific.Rom.StartOffset = 0 ;
	ret = libusb_bulk_transfer(t6usbdev, EP_BLK_OUT_ADDR,(char *)&bulkcmd,32, &transferred,5000);	
	if(ret < 0 || transferred != 32){
		printf(" bulk ret = %d \n",ret);
		return -1;
	}
	
   
	ret = libusb_bulk_transfer(t6usbdev, EP_BLK_OUT_ADDR,buf,len, &transferred1,8000);	
	if(ret <0 || transferred1 != len){
		printf(" bulk2 ret = %d \n",ret);
		return -1;
	}

	return 0;
		
}







int t6_libusb_get_interrupt2(libusb_device_handle* t6usbdev,char * data , int len )
{
	  int ret = 0;
	  int transferred = 0;
	  ret = libusb_interrupt_transfer(t6usbdev,EP_INT_IN_ADDR,data,len,&transferred,5000);  
	  if(ret < 0){	
		printf("interrupt  failed 1 =%d \n",ret);	
		return -1;
	  }
	  return 0 ;	

}



void ShowRomMsg(libusb_device_handle* t6usbdev)
{
	int rom_size = 0 ;
    int leave_flag = 0 ;
	int prcent = 0 ;
	char rdata[64];
	char rommsg[1024];
	
	while(!leave_flag){
		if(t6_libusb_get_interrupt2(t6usbdev,rdata,64) < 0)
			return;
		PT6INTERRUPTDATA ptr_int = (PT6INTERRUPTDATA)rdata;
		
		if(ptr_int->FuncMask == 0){
			printf("FuncMask error \n");
			break;
		}
		if(ptr_int->FuncMask != T6INT_FUNC_MASK_ROM)
			continue;
        fflush(stdout);
		switch(ptr_int->RomEvent){
			
			case T6INT_ROM_EVENT_FINISH:
				printf("\rUpdate firmware success\n");
				leave_flag = 1;
				break;

			case T6INT_ROM_EVENT_ERASING:
				printf("\rerasing = %d ",ptr_int->RomProceedSize );
				
				break;
				
			case T6INT_ROM_EVENT_WRITING:
				printf("\rwriting = %d ",ptr_int->RomProceedSize );
				
				break;
				
			case T6INT_ROM_EVENT_VERIFYING:
				printf("\rverifying = %d ",ptr_int->RomProceedSize );
				
				break;
				
			case T6INT_ROM_EVENT_TRANSFERING:
				printf("\rtransfering = %d ",ptr_int->RomProceedSize );
				break;
				
			default:
				printf("RomEvent = %d ",ptr_int->RomEvent );
				leave_flag = 1;
				break;

		}
					
	}

}












