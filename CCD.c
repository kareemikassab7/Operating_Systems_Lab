#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/kfifo.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
MODULE_LICENSE("GPL");

unsigned char messageBuffer[4096];
unsigned char  outputBuffer[4096];
unsigned char keyBuffer[128];
unsigned char proc[4096];
unsigned char encMessage[256];
unsigned char * key;
key= "password123";

struct devData {
    struct cdev cdev;

};
struct devData myDevice[2];

static int opener(struct inode *, struct file *);
static int closer(struct inode *, struct file *);
static ssize_t reader(struct file *, char *, size_t, loff_t *);
static ssize_t reader2(struct file *, char *, size_t, loff_t *);

static ssize_t writer(struct file *file, const char __user *buffer, size_t len, loff_t *offset)
{
        
        size_t Msglen = 4096, ncopied;
   

    printk("Writing device: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));

    if (len < Msglen) {
        Msglen = len;
    }
    ncopied = copy_from_user(messageBuffer, buffer, Msglen);
   rc4(messageBuffer,key, outputBuffer, 100);

    if (ncopied == 0) {
        printk("Copied %zd bytes from the user\n", Msglen);
    } else {
        printk("Could't copy %zd bytes from the user\n", ncopied);
    }

messageBuffer[Msglen]= 0;
  outputBuffer[maxdatalen] = 0;

    printk("Message: %s\n", messageBuffer);
    printk("output: %s\n", outputBuffer);

    return len;
}

static ssize_t writer2(struct file *,  const char *, size_t, loff_t *);

static int fOpenCount=0;


static int forkPrinter(struct seq_file *mfptr, void * ptr){

seq_printf(mfptr, "Fork Counter is now : %d \n", forkCounter);
return 0;
}

static int forkOpener (struct inode *inode, struct file *mf){
return single_open(mf, forkPrinter, NULL);

}

static ssize_t counterResetter(struct file* mf,const char __user *buffer,size_t count,loff_t *mfpos){  ///// BONUS//////
  
  forkCounter=0;
}



static int procOpener (struct inode *inode, struct file *f){
return single_open(f, procDisplayer, NULL);

}


static int procDisplayer(struct seq_file *f, void * p){


if (strcmp(keyy, proc_in)==0)
{
seq_printf(f, "%s\n", databuf   );}

else   seq_printf(f, "%s\n", out  );

}

static ssize_t procWriter(struct file* file,const char __user *buffer,size_t count,loff_t *f_pos){

        
}


static struct file_operations cipherHandler = 
{
        .read = reader,
        .write = writer,
        .open = opener,
        .release = closer,
};

static struct file_operations keyCipherHandler = 
{
        .read = reader2,
        .write = writer2,
        .open = opener,
        .release = closer,
};

static const struct file_operations procCipherHandler ={

.owner= THIS_MODULE,
.open = forkOpener,
.read = seq_read,
.llseek= seq_lseek,
.release= single_release,
.write= counterResetter,
};

static const struct file_operations procCipherHandler0 ={

.owner= THIS_MODULE,
.open = procOpener,
.read = seq_read,
.llseek= seq_lseek,
.release= single_release,
.write= procWriter,
};




void rc4(unsigned char * p, unsigned char * k, unsigned char * c,int l)
{
        unsigned char s [256];
        unsigned char t [256];
        unsigned char temp;
        unsigned char kk;
        int i,j,x;
        for ( i  = 0 ; i  < 256 ; i ++ )
        {
                s[i] = i;
                t[i]= k[i % 4];
        }
j = 0 ;
        for ( i  = 0 ; i  < 256 ; i ++ )
        {
                j = (j+s[i]+t[i])%256;
                temp = s[i];
                s[i] = s[j];
                s[j] = temp;
        }
        i = j = -1;
        for ( x = 0 ; x < l ; x++ )
        {
                i = (i+1) % 256;
                j = (j+s[i]) % 256;
                temp = s[i];
                s[i] = s[j];
                s[j] = temp;
                kk = (s[i]+s[j]) % 256;
                c[x] = p[x] ^ s[kk];
        }
}

static int __init encdev_init(void)
{

int majorNum;
majorNum= register_chrdev_region(MKDEV(1024,0),2,"cipher");

int majorNum2;
majorNum2= register_chrdev_region(MKDEV(1024,1),2,"cipher_key");



cdev_init(&devs[0].cdev, &cipher_fops);
cdev_add(&devs[0].cdev, MKDEV(1024,0),1);

cdev_init(&devs[1].cdev, &key_cipher_fops);
cdev_add(&devs[1].cdev, MKDEV(1024,1),1);

proc_create("cipher_key",0, NULL,&proc_cipher_fops);
proc_create("cipher",0, NULL,&proc_cipher_fops_original);
return 0;

}