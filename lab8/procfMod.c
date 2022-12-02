#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>
#include <linux/kallsyms.h>
#include <linux/kthread.h>  
#include <asm/uaccess.h> 
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
/*
const char* FILEPATH="/proc/version";
char MAPPATH[128] ="/boot/System.map-";*/

long forkCounter=0;
typedef asmlinkage long (*sys_call_ptr_t)(unsigned long p1, void* p2, int* p3,int* p4, unsigned long p5);
static sys_call_ptr_t cloneFork;
sys_call_ptr_t* sysCall;
void ** sys_call_table;


static asmlinkage long forkyy(unsigned long p1, void* p2, int* p3,int* p4, unsigned long p5)
{
    
    forkCounter=forkCounter+1;
   printk("The fork called: %d \n", forkCounter);

long trial;
//return 
	trial= cloneFork(p1, p2, p3, p4, p5);
return trial;
}

/*struct myfile{
struct file* f; 
mm_segment_t fs; 
loff_t pos;
};*/


/*
struct myfile* open_file (const char *filePath) 
{
    struct myfile *mfptr;
    mfptr= kmalloc(sizeof(struct myfile), GFP_KERNEL);
    mfptr->fs = get_fs();
    set_fs(get_ds());
   mfptr-> f = filp_open(filePath,0, 0);
    set_fs(mfptr->fs);
    return mfptr;
}
*/
/*
int read_from_file_until(struct myfile *mfptr, unsigned char* buf,unsigned long  vlen, char c) 
{
   int ret;
   mfptr-> fs = get_fs();
   set_fs(get_ds());
int i;    
    for (i=0;i<vlen;i++){
   	 ret = vfs_read(mfptr->f, buf+i ,1 , &(mfptr->pos));
	 if (buf[i]==c){
	 buf[i]=NULL;
	 break;
	 }
	}
    set_fs(mfptr->fs);
    return ret;
} */  
/*
void close_file(struct myfile *myfileptr) 
{
    filp_close(myfileptr->f, NULL);
}
*/

inline void writerFunc(unsigned long holder2)
{
    asm volatile("mov %0,%%cr0": "+r" (holder2), "+m" (__force_order));
}



static void enableWrite(void)
{
  unsigned long reg_cr0 = read_cr0();
  clear_bit(16, &reg_cr0);
  writerFunc(reg_cr0);
} 


static void disableWrite(void)
{
  unsigned long reg_cr0 = read_cr0();
  set_bit(16, &reg_cr0);
  writerFunc(reg_cr0);
}
/////////////////////////////////////////////////////////





static int forkPrinter(struct seq_file *mfptr, void * ptr){

seq_printf(mfptr, "Fork Counter is now : %d \n", forkCounter);
return 0;
}

static int forkOpener (struct inode *inode, struct file *mf){
return single_open(mf, forkPrinter, NULL);

}

static ssize_t counterResetter(struct file* mf,const char __user *buffer,size_t count,loff_t *mfpos){  ///// BONUS//////
  
  forkCounter=0;
  return 0;
}

static const struct file_operations opHandler ={

.owner= THIS_MODULE,
.open = forkOpener,
.read = seq_read,
.llseek= seq_lseek,
.release= single_release,
.write= counterResetter,
};








//////////////////////////////////////////


 int init (void){
    proc_create("forkCounter", 0 , NULL, &opHandler);
		sysCall= kallsyms_lookup_name("sys_call_table");
    printk("syscall Adress is : %p \n", sys_call_table);
		printk(KERN_ALERT " Fork Address: %px \n", sysCall[2]);
		cloneFork = sysCall[__NR_clone];
    	enableWrite();
    	sysCall[__NR_clone] = (sys_call_ptr_t)forkyy;
		disableWrite();
return 0; 
}


static void clean_module(void){
  remove_proc_entry("fork_count", NULL);
	enableWrite();
    sysCall[__NR_clone] = cloneFork;
disableWrite();
	printk(KERN_INFO " Tamam keda\n");
}



module_init(init);
module_exit(clean_module);