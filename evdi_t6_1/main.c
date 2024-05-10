#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <turbojpeg.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sched.h>
//#include <alsa/asoundlib.h>
//#include <X11/Xlib.h>
//#include <X11/extensions/Xinerama.h>
//#include <X11/extensions/Xrandr.h>






#include "t6usbdongle.h"
//#include "pulseaudio_list.h"


int g_program_exit = 0;
bool ready_flag = true;
pthread_mutex_t ready_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ready_cond = PTHREAD_COND_INITIALIZER;
pid_t pid ;
libusb_context *ctx = NULL;
//struct T6evdi *g_t6para;

//struct T6evdi *g_t6para;
struct evdi_box* mevdi_list;
static pthread_mutex_t linklist_mutex = PTHREAD_MUTEX_INITIALIZER;


struct T6evdi *head = NULL;
struct T6evdi *curr = NULL;
//int evdi_number[MAX_DISP_USE]={0,1,2,3,4,5,6,7,8};

static const unsigned char generic_edid[]={
	//0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1c, 0xae, 0x73, 0x24, 0x01, 0x01, 0x01, 0x01,
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x34, 0x74, 0x73, 0x24, 0x01, 0x01, 0x01, 0x01,
	0x32, 0x14, 0x01, 0x03, 0x68, 0x30, 0x1b, 0x78, 0x2a, 0xe6, 0x75, 0xa4, 0x56, 0x4f, 0x9e, 0x27,
	0x0f, 0x50, 0x54, 0xbf, 0xef, 0x80, 0xb3, 0x00, 0xa9, 0x40, 0x95, 0x00, 0x81, 0x40, 0x81, 0x80,
	0x95, 0x0f, 0x71, 0x4f, 0x90, 0x40, 0x02, 0x3a, 0x80, 0x18, 0x71, 0x38, 0x2d, 0x40, 0x58, 0x2c,
	0x45, 0x00, 0xde, 0x0d, 0x11, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0xff, 0x00, 0x31, 0x30, 0x31,
	0x32, 0x41, 0x32, 0x39, 0x36, 0x30, 0x31, 0x37, 0x39, 0x31, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x37,
	0x4c, 0x1e, 0x52, 0x11, 0x00, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xfc,
	//0x00, 0x47, 0x65, 0x6e, 0x75, 0x69, 0x6e, 0x65, 0x32, 0x31, 0x2e, 0x35, 0x27, 0x27, 0x00, 0x77
	0x00, 0x47, 0x65, 0x6e, 0x65, 0x72, 0x69, 0x63, 0x32, 0x31, 0x2e, 0x35, 0x27a, 0x27, 0x00, 0xa7
};


void wait_ready()
{
  pthread_mutex_lock(&ready_mutex);
  while(!ready_flag) {
    pthread_cond_wait(&ready_cond, &ready_mutex);
  }
  pthread_mutex_unlock(&ready_mutex);
}


void set_ready(bool ready)
{
  pthread_mutex_lock(&ready_mutex);
  ready_flag = ready;

  pthread_cond_signal(&ready_cond);
// or using pthread_cond_broadcast(&ready_cond);

  pthread_mutex_unlock(&ready_mutex);

}


#if 0
struct T6evdi* create_list(int display_id)
{
    DEBUG_PRINT("\n creating list with headnode as [%d]\n",display_id);
	pthread_mutex_lock(&linklist_mutex);
    struct T6evdi *ptr = (struct T6evdi*)malloc(sizeof(struct T6evdi));
    if(NULL == ptr)
    {
        DEBUG_PRINT("\n Node creation failed \n");
		pthread_mutex_unlock(&linklist_mutex);
        return NULL;
    }
    ptr->display_id = display_id;
    ptr->next = NULL;

    head = curr = ptr;
	pthread_mutex_unlock(&linklist_mutex);
    return ptr;
}

struct T6evdi* add_to_list(int display_id, bool add_to_end)
{
    if(NULL == head)
    {
        return (create_list(display_id));
    }

    if(add_to_end)
        DEBUG_PRINT("\n Adding node to end of list with display_idue [%d]\n",display_id);
    else
        DEBUG_PRINT("\n Adding node to beginning of list with display_idue [%d]\n",display_id);

	pthread_mutex_lock(&linklist_mutex);
    struct T6evdi *ptr = (struct T6evdi*)malloc(sizeof(struct T6evdi));
    if(NULL == ptr)
    {
        DEBUG_PRINT("\n Node creation failed \n");
		pthread_mutex_unlock(&linklist_mutex);
        return NULL;
    }
    ptr->display_id = display_id;
    ptr->next = NULL;

    if(add_to_end)
    {
        curr->next = ptr;
        curr = ptr;
    }
    else
    {
        ptr->next = head;
        head = ptr;
    }
	pthread_mutex_unlock(&linklist_mutex);
    return ptr;
}

struct T6evdi* search_in_list(int display_id, struct T6evdi **prev)
{
    struct T6evdi *ptr = head;
    struct T6evdi *tmp = NULL;
    bool found = false;

    DEBUG_PRINT("\n Searching the list for display_idue [%d] \n",display_id);
    pthread_mutex_lock(&linklist_mutex);
    while(ptr != NULL)
    {
        if(ptr->display_id == display_id)
        {
            found = true;
            break;
        }
        else
        {
            tmp = ptr;
            ptr = ptr->next;
        }
    }

    if(true == found)
    {
        if(prev)
            *prev = tmp;
		pthread_mutex_unlock(&linklist_mutex);
        return ptr;
    }
    else
    {
    	pthread_mutex_unlock(&linklist_mutex);
        return NULL;
    }
}

int delete_from_list(int display_id)
{
    struct T6evdi *prev = NULL;
    struct T6evdi *del = NULL;

    DEBUG_PRINT("\n Deleting display_idue [%d] from list\n",display_id);

    del = search_in_list(display_id,&prev);
	pthread_mutex_lock(&linklist_mutex);
    if(del == NULL)
    {
    	pthread_mutex_unlock(&linklist_mutex);
        return -1;
    }
    else
    {
        if(prev != NULL)
            prev->next = del->next;

        if(del == curr)
        {
        	if(curr == head){
            	curr = prev;
				head = prev;			
			}else{				
				curr = prev;			
			}
        }
        else if(del == head)
        {
            head = del->next;
        }
    }
 
    free(del);
    del = NULL;
	pthread_mutex_unlock(&linklist_mutex);
    return 0;
}

void print_list(void)
{
    struct T6evdi *ptr = head;
    pthread_mutex_lock(&linklist_mutex);
    DEBUG_PRINT("\n -------Printing list Start------- \n");
    while(ptr != NULL)
    {
        DEBUG_PRINT("\n [%d] \n",ptr->display_id);
        ptr = ptr->next;
    }
    DEBUG_PRINT("\n -------Printing list End------- \n");
    pthread_mutex_unlock(&linklist_mutex);
    return;
}
#endif

void jpg_releses_queue(list_t *restrict l)
{
	int size = list_size(l);
	int i ;
	for(i = 0 ; i < size ; i++){
		struct jpg_packet* packet = (struct jpg_packet*) list_extract_at(l,0);
        free(packet->buffer);
		free(packet);
	}
}




#if 0

void close_mutex(pthread_mutex_t *lock ,int id )
{
	struct T6evdi *ptr = head;
    pthread_mutex_lock(&linklist_mutex);
    while(ptr != NULL)
    {

        if(ptr->lock == lock){
             if(ptr->display_id != id){
                pthread_mutex_unlock(&linklist_mutex);
			 	return ;
             }
        }
        ptr = ptr->next;	
    }
	pthread_mutex_unlock(&linklist_mutex);
    if(lock != NULL){
      pthread_mutex_destroy(lock);
	}
	return 0;

}	

void close_usb_handle(libusb_device_handle  *usbhandle ,int id)
{
	struct T6evdi *ptr = head;
    pthread_mutex_lock(&linklist_mutex);
    while(ptr != NULL)
    {
	if(ptr->t6usbdev == NULL)
		break;
        if(ptr->t6usbdev == usbhandle){
			if(ptr->display_id != id){
              pthread_mutex_unlock(&linklist_mutex);
              return;
			}
        }
        ptr = ptr->next;	
    }
    pthread_mutex_unlock(&linklist_mutex);
	if(usbhandle != NULL){
         libusb_close(usbhandle);
		 usbhandle = NULL;
	}		 

}
#endif



#if 0
int audio_use(libusb_device_handle  *usbhandle ,int id)
{
    int use = 0 ;
	struct T6evdi *ptr = head;
    pthread_mutex_lock(&linklist_mutex);
    while(ptr != NULL)
    {
        if(ptr->t6usbdev == usbhandle){
			if( id != ptr->display_id){
              use = ptr->audio_only ;
			}
			break;
        }
        ptr = ptr->next;
    }
    pthread_mutex_unlock(&linklist_mutex);
	return 	use; 

}

int pullaudio_buffer(char *audiobuffer)
{

	struct T6evdi *ptr = head;
    pthread_mutex_lock(&linklist_mutex);
    while(ptr != NULL)
    {
        if(ptr->audio_queue != NULL){
			if(ptr->disp_set_mode == 1){
				char *audiodata = (char*)malloc(1920);
				if(audiodata != NULL){
					memcpy(audiodata,audiobuffer,1920);
					queue_add(ptr->audio_queue,audiodata);
				}
			}
        }
        ptr = ptr->next;
    }
    pthread_mutex_unlock(&linklist_mutex);


}

#endif


 int is_T6dev( libusb_device *dev)
{
	struct libusb_device_descriptor desc;
	int rc = libusb_get_device_descriptor( dev, &desc );

	if( LIBUSB_SUCCESS == rc && desc.idVendor == 0x0711 && (desc.idProduct == 0x5600 || desc.idProduct == 0x5601 || desc.idProduct == 0x5621 || desc.idProduct == 0x5609 || desc.idProduct == 0x560b || desc.idProduct == 0x562b))
    	return 1;
	else if(LIBUSB_SUCCESS == rc && desc.idVendor == 0x03f0 && desc.idProduct == 0x0182)	
		return 1;
	else 
		return 0;
}


 int GetEvdi_id(struct evdi_box* evdi_list , int mode ){
	//int retry = 3;
	int evdi_id = -1 ;
	int ret ;
	int i ;
	
	do{
		for( i = 0 ; i < MAX_T6_DEVICES ; i++ ){
			ret = evdi_check_device(i);
			DEBUG_PRINT("evdi_check_device(%d) ret= %d\n",i, ret);
			if(ret == 0){
				DEBUG_PRINT("evdi_list->box[i]= %d\n",evdi_list->box[i]);
				if(evdi_list->box[i] == 0){
					evdi_id = i;
					evdi_list->box[i] = 1;
					break;
				}
			}	
	    }
		if(evdi_id < 0){
			ret = evdi_add_device();
			DEBUG_PRINT("add device =%d\n" ,ret);
		}
	}while(evdi_id < 0 );
	
	
	return evdi_id;


}



void evdi_mutex_lock(pthread_mutex_t   *lock)
{
  if(lock !=NULL )
	pthread_mutex_lock(lock);	
}


void evdi_mutex_unlock(pthread_mutex_t  *lock)
{
  if(lock !=NULL )
	pthread_mutex_unlock(lock);	
}

void Dev_destroy(struct T6evdi *g){

	int id = g->display_id;
    struct evdi_box* evdi_list  = g->evdi_list_queue;
    DEBUG_PRINT("Dev_destroy id = %d \n",id);
	evdi_list->box[id] = 0;
	 DEBUG_PRINT("evdi_list->box[%d] = %d \n",id,evdi_list->box[id]);
    if(g->video_buffer != NULL)
		free(g->video_buffer);
    		
    //DEBUG_PRINT("release image_mutex \n");
	//pthread_mutex_destroy(&g->image_mutex);
	jpg_releses_queue(&g->jpg_list_queue);
	list_destroy(&g->jpg_list_queue);
    DEBUG_PRINT("release bulkusb_mutex id = %d\n",id);
	pthread_mutex_destroy(&g->bulkusb_mutex);

	free(g);

	DEBUG_PRINT("exit process id %d \n",id);
	//exit(1);
   // close_usb_handle(g->t6usbdev,g->display_id);
   // close_mutex(g->lock,id);
	//delete_from_list(id);
   

}

/*
void DetectOrientation(struct T6evdi* pt6evdi ,int w , int h)
{
    
	
	int ret ; 
	Display *display = XOpenDisplay(NULL);
	if(display == NULL )
		return;
	
    XRRScreenResources *screens = XRRGetScreenResources(display, DefaultRootWindow(display));
    XRRCrtcInfo *info = NULL;
    int i = 0;
    if(screens == NULL ){
		XCloseDisplay( display );
		return;
    }
   
	
    for (i = 0; i < screens->ncrtc; i++) {
        info = XRRGetCrtcInfo(display, screens, screens->crtcs[i]);
		if(info->width == 0 ||info->height == 0)
			continue;
		
		if(info->width == w && info->height == h){
        	
			//DEBUG_PRINT("set rotation = %d \n",info->rotation);
			//DEBUG_PRINT("mode = %d \n",info->mode);
			 if(info->rotation == 1 ) // 0 
				t6_libusb_set_rotate(pt6evdi->t6usbdev,0);
			 if(info->rotation == 4) // 180
			 	t6_libusb_set_rotate(pt6evdi->t6usbdev,2);
			
				
		}else if(info->width == h && info->height == w){
		    //DEBUG_PRINT("set rotation = %d \n",info->rotation);
			//DEBUG_PRINT("mode = %d \n",info->mode);
            if(info->rotation == 2 ) // 90 
				t6_libusb_set_rotate(pt6evdi->t6usbdev,1);	
			if(info->rotation == 8 ) // 279
                t6_libusb_set_rotate(pt6evdi->t6usbdev,3);	
		}
	
        XRRFreeCrtcInfo(info);
    }
    XRRFreeScreenResources(screens);
}
*/

void mct_dpms_handler(int dpms_mode, void* user_data)
{
	DEBUG_PRINT("%s: dpms_mode = %d\n", __FUNCTION__, dpms_mode);
	struct T6evdi* pt6evdi = (struct T6evdi*)user_data; //g_t6para;//user_data;
	if(dpms_mode == 3){
		t6_libusb_set_monitor_power(pt6evdi,0);
	}

	if(dpms_mode == 0){
		t6_libusb_set_monitor_power(pt6evdi,1);
	}
      	
}


void mct_mode_changed_handler(struct evdi_mode mode, void* user_data)
{
	
	int ret = 0 ;	
	struct T6evdi* pt6evdi = (struct T6evdi*)user_data; //g_t6para;//user_data;
	
	struct evdi_rect ev_rect;
	struct evdi_buffer evBuffer;
    DEBUG_PRINT("%s: width:%d height:%d rate:%d bps:%d format:%d\n",
				__FUNCTION__, mode.width, mode.height, mode.refresh_rate,
				              mode.bits_per_pixel, mode.pixel_format);
     
	
    DEBUG_PRINT("%s: width:%d height:%d rate:%d bps:%d format:%d\n",
				__FUNCTION__, mode.width, mode.height, mode.refresh_rate,
				              mode.bits_per_pixel, mode.pixel_format);
/*
	if(pt6evdi->jpg_rotate > 0)
    	DetectOrientation(pt6evdi,mode.width,mode.height);
*/	
    if(pt6evdi->disp_set_mode == 1){
	 DEBUG_PRINT("pt6evdi->disp_set_mode = %d \n",pt6evdi->disp_set_mode);	
        evdi_unregister_buffer(pt6evdi->ev_handle, pt6evdi->display_id);
		pt6evdi->disp_set_mode = 0 ;
        t6_libusb_set_monitor_power(pt6evdi,0);
 		usleep(500000);
        DEBUG_PRINT("release buffer\n");
			
		free(pt6evdi->video_buffer);
		
    }
	DEBUG_PRINT("set mode 0\n");
	evdi_mutex_lock(pt6evdi->lock);
	DEBUG_PRINT("set mode\n");
	t6_libusb_set_resolution(pt6evdi,mode.width,mode.height);
    ret = t6_libusb_set_monitor_power(pt6evdi,1);
    if(ret < 0){
      	DEBUG_PRINT("set power failed disp_interface =%d \n",pt6evdi->disp_interface);
	}
	evdi_mutex_unlock(pt6evdi->lock);
    DEBUG_PRINT("set mode finish\n");    

	
	pt6evdi->Width = mode.width;
	pt6evdi->Height= mode.height;
	pt6evdi->video_buffer = (unsigned char *)malloc(pt6evdi->Width*pt6evdi->Height*4);
	if(pt6evdi->video_buffer == NULL) {
		DEBUG_PRINT("cannot allocat video memory\n");	
		return;
	}
	ev_rect.x1 = 0;
	ev_rect.y1 = 0;
	ev_rect.x2 = pt6evdi->Width;
	ev_rect.y2 = pt6evdi->Height;
		
	evBuffer.id 		= pt6evdi->display_id;
	evBuffer.buffer 	= (void*)pt6evdi->video_buffer;
	evBuffer.width  	= pt6evdi->Width;
	evBuffer.height 	= pt6evdi->Height;
	evBuffer.stride 	= pt6evdi->Width*4;
	evBuffer.rects  	= &ev_rect;
	evBuffer.rect_count = 1;
    pt6evdi->disp_set_mode = 1;
	evdi_register_buffer(pt6evdi->ev_handle, evBuffer);
	
}

void mct_update_ready_handler(int buffer_to_be_updated, void* user_data)
{
	set_ready(true);
}
void mct_crtc_state_handler(int state, void* user_data)
{
	DEBUG_PRINT("mct_crtc_state_handler state = %d\n", state);
		
}
void mct_cursor_set_handler(struct evdi_cursor_set cursor_set,void *user_data)
{

	DEBUG_PRINT("mct_cursor w =%d y =%d \n",cursor_set.width,cursor_set.height);
}

void mct_cursor_move_handler(struct evdi_cursor_move cursor_move,void *user_data)
{
   DEBUG_PRINT("mct_cursor w =%d y =%d \n",cursor_move.x,cursor_move.y);
}


int mct_customFilter(short *coeffs, tjregion arrayRegion,tjregion planeRegion, int componentIndex,int transformIndex, struct tjtransform *transform)
{
	DEBUG_PRINT("mct_customFilter \n");
	return 0;

}



#if 0
int audio_found_device(){

	int ctr;
        
    	// This is where we'll store the input device list
    pa_devicelist_t pa_input_devicelist[16];

    	// This is where we'll store the output device list
    pa_devicelist_t pa_output_devicelist[16];

	while(!g_program_exit){
		char *pch =NULL;
    	if (pa_get_devicelist(pa_input_devicelist, pa_output_devicelist) < 0) {
			//printf("failed to get device list\n");
			//return -1;
			sleep(1);
			continue ;
    	}

    	for (ctr = 0; ctr < 16; ctr++) {
	   		if (! pa_output_devicelist[ctr].initialized) {
	    		break;
			}
			printf("=======[ Output Device #%d ]=======\n", ctr+1);
			printf("Description: %s\n", pa_output_devicelist[ctr].description);
			pch =strstr(pa_output_devicelist[ctr].description,"Loopback");
			if(pch != NULL)		
				return 1;
		//printf("Name: %s\n", pa_output_devicelist[ctr].name);
		//printf("Index: %d\n", pa_output_devicelist[ctr].index);
		//printf("\n");
    		}
		sleep(1);
	}
	return -1;
}


void *audio_process_read(void *userdata)
{
	
	int i;
	int err;
	char *buffer;
	int buffer_frames = 480;
	unsigned int rate = 48000;
	short buf[buffer_frames * 2];
	snd_pcm_t *capture_handle;
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
#if 0	
    if (audio_found_device() < 0){
		DEBUG_PRINT (" audio device not found\n") ;
		return;	
	}
#endif	
	if ((err = snd_pcm_open (&capture_handle, "hw:Loopback,1", SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		DEBUG_PRINT ("cannot open audio device \n") ;
		return;	
	}

	DEBUG_PRINT("audio interface opened\n");
		   
	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		DEBUG_PRINT ("cannot allocate hardware parameter structure\n");
		return;	
	}

	DEBUG_PRINT("hw_params allocated\n");
				 
	if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
		DEBUG_PRINT("cannot initialize hardware parameter structure)\n");
		return;	
	}

	DEBUG_PRINT("hw_params initialized\n");

	if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		DEBUG_PRINT("cannot set access type \n");
		return;	
	}

	DEBUG_PRINT("hw_params access setted\n");

	if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, format)) < 0) {
		DEBUG_PRINT ("cannot set sample format \n");
		return;	
	}

	DEBUG_PRINT( "hw_params format setted\n");

	if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0) {
		DEBUG_PRINT("cannot set sample rate\n");
		return;	
	}

	DEBUG_PRINT("hw_params rate setted\n");

	if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 2)) < 0) {
		DEBUG_PRINT ( "cannot set channel count \n");
		return;	
	}

	DEBUG_PRINT("hw_params channels setted\n");

	if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
		DEBUG_PRINT("cannot set parameters \n");
		return;
	}

	DEBUG_PRINT("hw_params setted\n");

	snd_pcm_hw_params_free (hw_params);

	DEBUG_PRINT("hw_params freed\n");

	if ((err = snd_pcm_prepare (capture_handle)) < 0) {
		DEBUG_PRINT ( "cannot prepare audio interface for use \n");
		return;
	}

	DEBUG_PRINT("audio interface prepared\n");
  
    while(!g_program_exit ){
		
		err = snd_pcm_readi(capture_handle,buf,buffer_frames);		
		if(err == -EAGAIN ){			
			DEBUG_PRINT(" capture failed ret = -EAGAIN  \n");			
			snd_pcm_wait(capture_handle, 100);
		
			continue;		
		}else if(err == -EPIPE){		    
			DEBUG_PRINT(" capture failed ret = -EPIPE \n");			
			snd_pcm_prepare(capture_handle);
			
			continue;		
		}else if (err == -ESTRPIPE){ 		    
			DEBUG_PRINT(" capture failed ret = -ESTRPIPE \n");			
			err = snd_pcm_resume(capture_handle);			
			if(err < 0)				
				snd_pcm_prepare(capture_handle);
			
				continue;		
		}else if(err < 0){			
			
			continue; 		
		}
		pullaudio_buffer((char*)buf);
		
		

    }
    snd_pcm_close(capture_handle);

}


void *audio_process(void *userdata)
{
	struct T6evdi* pt6evdi = (struct T6evdi*) userdata;
	char *buf ;
	int  err;
    pt6evdi->audio_work_process = 1;
    while(pt6evdi->audio_work_process){
		
		if(queue_length(pt6evdi->audio_queue) == 0){
            usleep(100);
			continue;
		}
		
		buf = queue_remove(pt6evdi->audio_queue);
		
		
		evdi_mutex_lock(pt6evdi->lock);
		pthread_mutex_lock(&pt6evdi->bulkusb_mutex);
		err = t6_libusb_SendAudio(pt6evdi,buf,1920);
		free(buf);
		pthread_mutex_unlock(&pt6evdi->bulkusb_mutex);
		evdi_mutex_unlock(pt6evdi->lock);
		if(err < 0){
			break;
		}	
		
		

    }
   

}
#endif
#if 0
void *DetectMode(void *userdata)
{
	int ret ; 
	struct T6evdi* pt6evdi = (struct T6evdi*) userdata;
	while(pt6evdi->usb_process){
		int index = 0; 
		int mode[4];
		Display *display = XOpenDisplay(NULL);
		if(display == NULL )
			break;
	    XRRScreenResources *screens = XRRGetScreenResources(display, DefaultRootWindow(display));
	    XRRCrtcInfo *info = NULL;
	    int i = 0;
	    if(screens == NULL ){
			XCloseDisplay( display );
			break;
	    }
		
	    for (i = 0; i < screens->ncrtc; i++) {
	        info = XRRGetCrtcInfo(display, screens, screens->crtcs[i]);
			if(info->width == 0 ||info->height == 0)
				continue;
			
			mode[index] = info->mode;
			/*
			if(index > 0){
			 if(mode[0] == mode[index])
             else if(mode[0] != mode[index])   
			}
			*/
			index++;
			//DEBUG_PRINT("mode = %d \n",info->mode);
			
		
	        XRRFreeCrtcInfo(info);
	    }
	    XRRFreeScreenResources(screens);
	    XCloseDisplay( display );

        
		
		sleep(3);
	}

}
#endif 

void *int_process(void *userdata)
{
    char buf[64];
	int  ret = 0;
	struct T6evdi* pt6evdi = (struct T6evdi*) userdata;
	while(pt6evdi->usb_process){
		ret =t6_libusb_get_interrupt(pt6evdi,buf,64);
	}


}

void *usb_process(void *userdata)
{
	struct T6evdi* pt6evdi = (struct T6evdi*) userdata;
	int ret = 0 ;
	int cmdAddrr = 0;
	int fbAddr1=  (pt6evdi->ramsize - 8) * 1024 * 1024;
	int fbAddr2=  (pt6evdi->ramsize - 4) * 1024 * 1024;
	int cmdoffset = 1024 * 1024 * 1;
	int tmp = pt6evdi->ramsize / 2  ;
	int count = 0 ;
	int resetflag = 0 ;
	int len = 0;
    if(pt6evdi->interface_num > 1){		
		if(pt6evdi->disp_interface == 0){
        	cmdAddrr = 0 ;
			fbAddr1=  (tmp - 8) * 1024 * 1024;
			fbAddr2=  (tmp - 4) * 1024 * 1024;
		}else{
			cmdAddrr = tmp * 1024 * 1024;
		}
		
    }
	printf("number = %d intereface = %d  cmdAddr = %x fbAddr1 =%x fbAddr2= %x \n",pt6evdi->interface_num,
	pt6evdi->disp_interface,cmdAddrr,fbAddr1,fbAddr2);
	
    pt6evdi->cmdAddr = cmdAddrr;
	
    pt6evdi->fbAddr = fbAddr2;
   // t6_libusb_Rgb24_full_block(pt6evdi,fbAddr1);
	//t6_libusb_Rgb24_full_block(pt6evdi,fbAddr2);
	while(pt6evdi->usb_process && pt6evdi->evdi_list_queue->box[pt6evdi->display_id] == 1){
		
        if(list_size(&pt6evdi->jpg_list_queue)== 0){
			usleep(10000);
			continue;
		}
        pthread_mutex_lock(&pt6evdi->bulkusb_mutex);
		struct jpg_packet *jpacket = (struct jpg_packet *)list_extract_at(&pt6evdi->jpg_list_queue,0);//(struct jpg_packet *)queue_remove(pt6evdi->jpg_queue);
        pthread_mutex_unlock(&pt6evdi->bulkusb_mutex);
		if(pt6evdi->disp_set_mode  == 0){
			
			free(jpacket->buffer);
			free(jpacket);
			
			continue ;
        }
		
		
		//usleep(100000);
		
		if(pt6evdi->fbAddr == fbAddr1)
			pt6evdi->fbAddr = fbAddr2;
		else
			pt6evdi->fbAddr = fbAddr1;

        len= jpacket->jpgImageSize + 1024 +48;
		if(len < 0x100000 )
	       	cmdoffset = 0x100000;
		else if(len < 0x200000)
			cmdoffset = 0x200000;
		else
			cmdoffset = 0x300000;
		
		if(pt6evdi->cmdAddr + cmdoffset > fbAddr1){
			pt6evdi->cmdAddr = cmdAddrr;
			resetflag = 0x80;
		}else{
			resetflag = 0;
			
		}
		
		if(++count < 10)
           resetflag = 0x80;
		//if(pt6evdi->jpg_reset_fun == 1 && count >= 200)
		//	resetflag = 0;
	

		//if(pt6evdi->interface_num == 2)
		//	usleep(10000);
		 
	    //printf("number = %d intereface = %d  cmdAddr = %x fbAddr =%x \n",pt6evdi->interface_num,
		// pt6evdi->disp_interface,pt6evdi->cmdAddr,pt6evdi->fbAddr);
		
		
		//count = count +1;
		//if(count % 3 == 0)
		//	resetflag = 0x80;
		//else
		//	resetflag = 0 ;
			
		//DEBUG_PRINT(" jpg len = %d  id =%d \n",jpacket->jpgImageSize,pt6evdi->display_id);
		
		
		evdi_mutex_lock(pt6evdi->lock);
		//t6_save_file(jpacket->buffer,jpacket->jpgImageSize);
        ret = t6_libusb_FilpJpegFrame(pt6evdi,jpacket->buffer,jpacket->jpgImageSize,resetflag);
		evdi_mutex_unlock(pt6evdi->lock);
		//if(pt6evdi->jpg_reset_fun == 1 && count < 200)
		//	count++;
		/*
		if(pt6evdi->interface_num >= 2){
	    	if(cmdoffset >= 0x200000 )
				usleep(40000);
		}
		*/
		if(ret < 0){
			//t6_libusb_dongle_reset(pt6evdi);
			free(jpacket->buffer);
			free(jpacket);
			jpacket->buffer = NULL;
			jpacket	= NULL;
			
			break;
		}
		pt6evdi->cmdAddr += cmdoffset;
	    
		free(jpacket->buffer);
		free(jpacket);
		
		
	}
	
	//pt6evdi->event_process = 0 ;
	pt6evdi->disp_set_mode = 0 ;
	pt6evdi->image_work_process = 0;
	//pt6evdi->jpg_work_process = 0 ;
	pt6evdi->usb_process = 0 ;
	//pt6evdi->audio_work_process = 0 ;
    
    //sleep(1);
    //Dev_destroy(pt6evdi);
}


void *events_process(void *userdata)
{
	fd_set set;
	evdi_selectable evfd = 0;
    //DEBUG_PRINT("userdata = %d  \n",userdata);
	struct T6evdi* pt6evdi = (struct T6evdi*) userdata;
        
	struct evdi_event_context evContext = {(void*) mct_dpms_handler,
										 (void*)mct_mode_changed_handler,
										 (void*)mct_update_ready_handler,
										 (void*)mct_crtc_state_handler,
										  NULL,
										  NULL,
										  NULL,
										 (void*)pt6evdi};
	

	evfd = evdi_get_event_ready(pt6evdi->ev_handle);
	DEBUG_PRINT("ev fd = %d evdi_process id = %d \n",evfd,pt6evdi->display_id);
	
	FD_ZERO (&set);
  	FD_SET (evfd, &set);
	while(pt6evdi->event_process){
	
		struct timeval tv;
		int retval;
			
		tv.tv_sec = 0;
       	tv.tv_usec = 10000;
		
		retval = select(evfd+1, &set, NULL, NULL, &tv);
		if(retval == 0) {//no new update in 2 seconds, timeout
			//DEBUG_PRINT("select timeout\n");
			
			FD_ZERO (&set);
			FD_SET (evfd, &set);
		
			continue;
		}
		else if(retval< 0) {
			break;
		}
		
		evdi_handle_events(pt6evdi->ev_handle, &evContext);

	}
	pt6evdi->event_process = 0;
    pt6evdi->usb_process = 0 ;
    DEBUG_PRINT("leave events_process evdi id = %d  \n",pt6evdi->display_id);
	pt6evdi->evdi_list_queue->box[pt6evdi->display_id] = 0;
    evdi_unregister_buffer(pt6evdi->ev_handle, pt6evdi->display_id);
    usleep(100000);
	evdi_disconnect(pt6evdi->ev_handle);
	usleep(100000);
	evdi_close(pt6evdi->ev_handle);
	Dev_destroy(pt6evdi);
   
}




void* evdi_process(void *userdata)
{
	
	struct T6evdi* pt6evdi = (struct T6evdi*) userdata;
	int ret = 0 ; 
	int retry = 5;
	int edid_size = 0 ;
	int state= 0;
	int rotate = 1;
	
    pthread_t pthr_event;
	pthread_t pthr_jpg_encode;
	pthread_t pthr_usb , pthr_audio ,pthr_int ,pthr_mode;
    DEBUG_PRINT("evdi_process id = %d \n",pt6evdi->display_id);
    
	evdi_mutex_lock(pt6evdi->lock);
    pt6evdi->ramsize = t6_libusb_get_ram_size(pt6evdi);
	evdi_mutex_unlock(pt6evdi->lock);
	
	do{
		evdi_mutex_lock(pt6evdi->lock);
		DEBUG_PRINT("disp_interface =%d \n",pt6evdi->disp_interface);
		state = t6_libusb_get_monitorstatus(pt6evdi);
		DEBUG_PRINT("monitor state =%d \n",state);
		if(state >= 0)
			edid_size = t6_libusb_get_edid(pt6evdi);
          
		if(state == -1){
			 evdi_mutex_unlock(pt6evdi->lock);
             edid_size = -1;
			 break;
		}
   
		
		DEBUG_PRINT("edid edid_size = %d \n",edid_size);
		evdi_mutex_unlock(pt6evdi->lock);
        if(edid_size > 0 || edid_size < 0)
			break;
		
		sleep(1); 
	}while(1); //--retry > 0);
	
    if(edid_size < 0 ){
        printf("read edid failed \n");
		Dev_destroy(pt6evdi);
		return NULL;
    }
   
	int disp_w = ((((long) pt6evdi->edid[0x3a] << 4) & 0x0f00) + ((long)pt6evdi->edid[0x38]&0x00ff));
	int disp_h  = (((long) pt6evdi->edid[0x3d] << 4) & 0x0f00) + ((long)pt6evdi->edid[0x3b]&0x00ff);
	DEBUG_PRINT("disp_w = %d disp_h =%d \n",disp_w,disp_h);
    if(disp_w > 1920 || disp_h > 1080){
      memcpy(pt6evdi->edid,generic_edid,128);
      edid_size = 128;
    }
    
    if(pt6evdi->jpg_rotate != 0 ) {
        pt6evdi->display_id =  1;
        pt6evdi->evdi_list_queue->box[1] = 1;
    }else
        pt6evdi->display_id =  GetEvdi_id(pt6evdi->evdi_list_queue,pt6evdi->jpg_rotate);

   
    printf("pt6evdi->display_id =%d\n", pt6evdi->display_id);
    sleep(1);
    pt6evdi->ev_handle = evdi_open(pt6evdi->display_id);
    if(pt6evdi->ev_handle == EVDI_INVALID_HANDLE){
		evdi_close(pt6evdi->ev_handle);
		Dev_destroy(pt6evdi);
		return NULL;
    }
	//hex_dump(pt6evdi->edid,edid_size,"edid");
	sleep(1);
	evdi_connect(pt6evdi->ev_handle, (const unsigned char*) pt6evdi->edid,edid_size,0,0); 
	pt6evdi->event_process = 1;
	pt6evdi->image_work_process = 1;
	pt6evdi->usb_process = 1;
	pt6evdi->jpg_work_process = 1;
	pt6evdi->disp_set_mode = 0 ;
	pt6evdi->frameupdate = 0;
	pt6evdi->audio_only = 0 ;
	
	if (pthread_create(&pthr_event, NULL, events_process,pt6evdi) != 0) {
		DEBUG_PRINT ("Create pthread error!\n");
		evdi_disconnect(pt6evdi->ev_handle);
		evdi_close(pt6evdi->ev_handle);
		Dev_destroy(pt6evdi);
		return NULL;
	}	
	
	
	if (pthread_create(&pthr_usb,NULL,usb_process,pt6evdi) != 0) {
		DEBUG_PRINT ("Create pthread error!\n");
		evdi_disconnect(pt6evdi->ev_handle);
		evdi_close(pt6evdi->ev_handle);
		Dev_destroy(pt6evdi);
		return NULL;
	}

    if(pt6evdi->jpg_rotate != 0){
		if (pthread_create(&pthr_int,NULL,int_process,pt6evdi) != 0) {
			DEBUG_PRINT ("Create pthread error!\n");
			evdi_disconnect(pt6evdi->ev_handle);
			evdi_close(pt6evdi->ev_handle);
			Dev_destroy(pt6evdi);
			return NULL;
		}
		/*
		if (pthread_create(&pthr_mode,NULL,DetectMode,pt6evdi) != 0) {
			DEBUG_PRINT ("Create pthread error!\n");
			evdi_disconnect(pt6evdi->ev_handle);
			evdi_close(pt6evdi->ev_handle);
			Dev_destroy(pt6evdi);
			return NULL;
		}
        */
		
    }
	
	
	/*
    evdi_mutex_lock(pt6evdi->lock); 
	if(audio_use(pt6evdi->t6usbdev ,pt6evdi->display_id) == 0){
        t6_libusb_set_AudioEngineStatus(pt6evdi );
        pt6evdi->audio_queue =  queue_create();
		if (pthread_create(&pthr_audio,NULL,audio_process,pt6evdi) != 0) {
			DEBUG_PRINT ("Create pthread error!\n");
			evdi_disconnect(pt6evdi->ev_handle);
			evdi_close(pt6evdi->ev_handle);
			Dev_destroy(pt6evdi);
			evdi_mutex_unlock(pt6evdi->lock);
			return;
		}	
		pt6evdi->audio_only = 1;
	}
	evdi_mutex_unlock(pt6evdi->lock);
	*/
	while(pt6evdi->image_work_process){
	    if(pt6evdi->disp_set_mode == 0){
			usleep(10);
			continue;
	    }
            wait_ready();
			if(evdi_request_update(pt6evdi->ev_handle, pt6evdi->display_id)) {
				
				
				struct evdi_rect rects[MAX_DIRTS];
				int rect_count=0;
				int i = 0 ;
	#if 0			
				pthread_mutex_lock(&pt6evdi->image_mutex);
				if(pt6evdi->jpg_work_process == 1){
					pt6evdi->jpg_work_process = 0 ;
					pthread_mutex_unlock(&pt6evdi->image_mutex);
					continue;
				}	
	#endif
				evdi_grab_pixels(pt6evdi->ev_handle, rects, &rect_count);
		    //DEBUG_PRINT ("evdi_grab_pixels!\n");
				
				#if 0	
				DEBUG_PRINT("rect_count = %d \n",rect_count);
				for(i = 0 ; i < rect_count ; i++){
					DEBUG_PRINT("x1 = %d x2= %d y1=%d y2 =%d\n",rects[i].x1,rects[i].x2,rects[i].y1,rects[i].y2);
				}
				#endif
				//int rgb_size = pt6evdi->Width* pt6evdi->Height* 4; 
				if(rect_count > 0){
		        //DEBUG_PRINT ("evdi_request_update s\n");
				
				if(list_size(&pt6evdi->jpg_list_queue) < 5 ){
						unsigned long jpgImageSize = 0;
						unsigned long tjpgImageSize = 0;
						char *jpgImage = NULL;
						char *tjpgImage = NULL;
						
						struct jpg_packet *jpacket = (struct jpg_packet *)malloc(sizeof(struct jpg_packet));
						if(jpacket != NULL){
					   		tjhandle jpegCompressor = tjInitCompress();
					   		tjCompress2(jpegCompressor,pt6evdi->video_buffer, pt6evdi->Width, 0,pt6evdi->Height, TJPF_BGRA,
			  							(unsigned char**)&jpgImage, &jpgImageSize, TJSAMP_420, 95, TJFLAG_FASTDCT );				
					   		tjDestroy(jpegCompressor);
					      
							if(pt6evdi->jpg_rotate > 0 &&  pt6evdi->jpg_rotate < 4){
							tjhandle jpegtramform = tjInitTransform();
								tjtransform transform;
								if(pt6evdi->jpg_rotate == 1)
							    	transform.op = TJXOP_ROT90; // TJXOP_NONE TJXOP_ROT270 // TJXOP_NONE TJXOP_ROT90;
							   if(pt6evdi->jpg_rotate ==  2)
		                        transform.op = TJXOP_ROT180;
								if(pt6evdi->jpg_rotate == 3)
		                        transform.op = TJXOP_ROT270;
								 
							    transform.options =TJXOPT_TRIM ; 
							    transform.data = NULL;
							    transform.customFilter = NULL;
		                    int rel = tjTransform(jpegtramform, jpgImage, jpgImageSize, 1,(unsigned char**) &tjpgImage, &tjpgImageSize, &transform, TJFLAG_ACCURATEDCT);
								tjDestroy(jpegtramform);
								free(jpgImage);
								if (rel != 0)
								{
								    char* err = tjGetErrorStr();
								    printf("ret = %d error = %s tjpgImageSize = %ld \n",ret,err,tjpgImageSize);
								}else{
								 	
									//t6_save_file(tjpgImage,tjpgImageSize);
									jpacket->jpgImageSize = tjpgImageSize;
								jpacket->buffer = tjpgImage;
									pthread_mutex_lock(&pt6evdi->bulkusb_mutex);
									list_append(&pt6evdi->jpg_list_queue,jpacket);
									pthread_mutex_unlock(&pt6evdi->bulkusb_mutex);	
									
								 }	
								
								
								
								
							}else{
					
							
								jpacket->jpgImageSize = jpgImageSize;
							jpacket->buffer = jpgImage;
								pthread_mutex_lock(&pt6evdi->bulkusb_mutex);
								list_append(&pt6evdi->jpg_list_queue,jpacket);
								pthread_mutex_unlock(&pt6evdi->bulkusb_mutex);
							}
							//queue_add(pt6evdi->jpg_queue,(void *) jpacket);
							
							
							
						}
				}
					//DEBUG_PRINT ("evdi_request_update e\n");
				}
				
				//pthread_mutex_unlock(&pt6evdi->image_mutex);
			}else{
				set_ready(false);
				usleep(1000);
			}

	}
	
	pt6evdi->event_process = 0;
	pthread_join(pthr_usb,NULL);
    pthread_join(pthr_event,NULL);
	DEBUG_PRINT("leave evdi_process \n");

}




void create_wording_thread(int busid ,int devid)
{
	int evdi_id = - 1;
	int i = 0 ;
	int ret;
	pthread_t pthr_evdi[2];
	pthread_mutex_t *foo = NULL;
	key_t key= 1634;
	int shm_id = -1;
    struct evdi_box* evdi_list;
	libusb_context *ctx = NULL;
    libusb_init(&ctx);
   
     
	ssize_t cnt=0;
    libusb_device **list;
    libusb_device *T6dev = NULL;
    libusb_device_handle* t6usbdev = NULL;
		 
	DEBUG_PRINT("ENTER %s\n",__FUNCTION__);
	 cnt = libusb_get_device_list(ctx, &list);
	 
	 // find T6 device
	 for(i = 0; i < cnt; i++){
		 libusb_device *device = list[i];
		 int mybusid = libusb_get_bus_number(device);
		 int mydevid = libusb_get_device_address(device);
		 if(mybusid == busid && mydevid == devid){
		 	ret = libusb_open (device, &t6usbdev);
	
			if(ret != 0) {
				DEBUG_PRINT("T6:  libusb_open failed\n");
				continue ;
			}

			if((ret = libusb_set_configuration(t6usbdev, 1)) != 0) {
				DEBUG_PRINT("T6: %s: libusb_set_configuration failed(%s)\n", __func__, libusb_strerror(ret));
				 libusb_close(t6usbdev);
				continue ;
			}
	
			if((ret = libusb_claim_interface(t6usbdev, 0)) != 0) {
				DEBUG_PRINT("T6: %s: libusb_claim_interface 0 failed(%s)\n", __func__, libusb_strerror(ret));
				libusb_close(t6usbdev);
				continue ;
			}
			
			break;
		 }
	}
    libusb_free_device_list(list, 1);
	if(t6usbdev == NULL)
		return;
	
	if ((shm_id = shmget(key, sizeof(struct evdi_box), 0666|IPC_CREAT)) < 0) {
        perror("shmget");
        return ;
    }
	
	if ((evdi_list = (struct evdi_box*)shmat(shm_id, NULL, 0)) < 0) {
        perror("shmat");
        return ;
    }


		
	int number = t6_libusb_get_displaysectionheader(t6usbdev);
    if(number > 1){
		foo = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(foo, NULL);
    }

	
	DEBUG_PRINT("interface num =%d\n", number);

    int jpgrst = t6_libusb_get_jpegreset(t6usbdev);
	int rotate =  t6_libusb_get_rotate(t6usbdev);
	
	DEBUG_PRINT("jpgrst =%d\n", jpgrst);
	DEBUG_PRINT("rotate =%d\n", rotate);
	
	for(i = 0 ; i < number ; i++ ){
    	//evdi_id =  GetEvdi_id(evdi_list);
		//DEBUG_PRINT("evdi_id =%d\n", evdi_id);
		struct T6evdi *g = (struct T6evdi *) malloc(sizeof(struct T6evdi));//add_to_list(evdi_id,true);
		if(g == NULL)
			continue;
		//memset(g,0,sizeof(g));
		//g_t6para = g;
		g->jpg_reset_fun = jpgrst;
		g->jpg_rotate    = rotate;
		g->disp_interface = i;
		g->t6usbdev = t6usbdev;
		g->display_id = 0;
		g->lock = foo;
		g->interface_num = number;
		//g->image_queue = queue_create();
		//g->jpg_queue   = queue_create();
		list_init(&g->jpg_list_queue);
		//g->audio_queue =  NULL;
        //g->video_buffer =  NULL;
		g->evdi_list_queue = evdi_list;
		//pthread_mutex_init(&g->image_mutex, NULL);
		pthread_mutex_init(&g->bulkusb_mutex, NULL);
		if (pthread_create(&pthr_evdi[i], NULL, evdi_process,g) != 0) {
			DEBUG_PRINT ("Create pthread error!\n");
			Dev_destroy(g);
			continue;
		}
		sleep(2);
		
	}
	if(number == 1){
    	pthread_join(pthr_evdi[0],NULL);
	}else{
    	pthread_join(pthr_evdi[0],NULL);
		pthread_join(pthr_evdi[1],NULL);
	}
	libusb_close(t6usbdev);
    libusb_exit(ctx);  
	shmdt(evdi_list); 
	if(foo != NULL)
		pthread_mutex_destroy(foo);
	printf("leave create_wording_thread !\n");
	//exit(1);
}




static void EnumT6Device (){

	bool t6found = false ;
	int i; 
	int busid = 0 , devid = 0 ;
	int evdi_id = -1 ;
	int ret ;
	int disp_interface_number = 0;
	
	ssize_t cnt=0;
	libusb_device **list;
    libusb_device *T6dev = NULL;
    libusb_device_handle* t6usbdev = NULL;
	
	//DEBUG_PRINT("ENTER %s\n",__FUNCTION__);
	cnt = libusb_get_device_list(ctx, &list);
	
	// find T6 device
	for(i = 0; i < cnt; i++){
		libusb_device *device = list[i];
		busid = libusb_get_bus_number(device);
		devid = libusb_get_device_address(device);
		if(is_T6dev(device)){	
			ret = libusb_open (device, &t6usbdev);
	
			if(ret != 0) {
				DEBUG_PRINT("T6:  libusb_open failed\n");
				continue ;
			}
			
		/*
			if((ret = libusb_set_configuration(t6usbdev, 1)) != 0) {
				 DEBUG_PRINT("T6: %s: libusb_set_configuration failed(%s)\n", __func__, libusb_strerror(ret));
				 //t6_libusb_test(t6usbdev);
				 libusb_close(t6usbdev);
				continue ;
			}
        */ 
			if((ret = libusb_claim_interface(t6usbdev, 0)) != 0) {
				//DEBUG_PRINT("T6: %s: libusb_claim_interface 0 failed(%s)\n", __func__, libusb_strerror(ret));
				libusb_close(t6usbdev);
				continue ;
			}
	
			
			libusb_close(t6usbdev);
			t6found = true;
			break;
		}
  	}
	
	libusb_free_device_list(list, 1);
	//libusb_close(t6usbdev);
	if(t6found == false)
		return;

	
	
	DEBUG_PRINT("%s: suucess\n", __FUNCTION__);
   
	
	pid = fork();
	if(!pid){	
     create_wording_thread(busid,devid);
	}
	
	
    
  
	
    //DEBUG_PRINT("disp_interface_number = %d \n", disp_interface_number);
	
	
	

}


void SearchT6DeviceReset (){

	int i; 
	int busid = 0 , devid = 0 ;
	int ret ;

	
	ssize_t cnt=0;
	libusb_device **list;
    libusb_device *T6dev = NULL;
    libusb_device_handle* t6usbdev = NULL;

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
			t6_libusb_set_monitor_power2(t6usbdev,0);
			//t6_libusb_donglereset2(t6usbdev);
			libusb_close(t6usbdev);
			break;
		  
		}
  	}
	
	libusb_free_device_list(list, 1);

	
}


void Loaddriver(){
	
    system("modprobe evdi");
    //system("modprobe snd-aloop");
  	
}

void intHandler(int dummy) 
{
	DEBUG_PRINT("single = %d  \n",dummy);
        g_program_exit = 1;
	SearchT6DeviceReset();	
	memset(mevdi_list->box,0,128);
	//kill(pid,SIGKILL);	
	//all_list_leave();
  
} 





int main(int argc, char** argv){

	int rc = 0, i=0;
	int shm_id;
	key_t key;
 	key = 1634;
	
	libusb_hotplug_callback_handle hp[2];
	struct timeval tm_wait;
	pthread_t  pthr_audio_read;
        
	libusb_init(&ctx);

	signal(SIGUSR1, intHandler);
	signal(SIGINT, 	 intHandler);
	signal(SIGKILL,  intHandler);
	signal(SIGTERM,  intHandler);
	signal(SIGHUP, 	 intHandler);
    Loaddriver();
	//create two display
	//evdi_add_device();
	//evdi_add_device();
	//T6UsbReset();
	if ((shm_id = shmget(key,sizeof(struct evdi_box), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        return 0;
    }

	if ((mevdi_list = shmat(shm_id, NULL, 0)) < 0) {
        perror("shmat");
        return 0;
    }
	
	memset(mevdi_list->box,0,128);
    
/*
	if (pthread_create(&pthr_audio_read,NULL,audio_process_read,NULL) != 0) {
		libusb_exit(ctx);
		return;
	}
*/
	
	while (!g_program_exit) {
		EnumT6Device();
		sleep(3);

	}
	//all_list_leave();
	libusb_exit(ctx);
	shmdt(mevdi_list);
 	shmctl(shm_id , IPC_RMID , NULL);
	return 0;
}



