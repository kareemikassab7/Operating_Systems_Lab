#include <linux/module.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/kfifo.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
MODULE_LICENSE("GPL");


unsigned char MsgBuffer[4096];
unsigned char  outputBuffer[4096];
unsigned char myKey[128];
unsigned char procBuffer[4096];
unsigned char* offensiveMsg= "Go away silly one, you cannot see my key \n";
static int openFileCounter = 0;

struct MsgEncrypter {
    struct cdev cdev;
};

struct MsgEncrypter msgEnc[2];


unsigned char * key= "Password123";
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


static int offensiveMsgPrinter (struct seq_file *m, void *v){

	seq_printf(m, "Go away silly one, you cannot see my key\n");
	return 0;
}

////////////////

static int printFork(struct seq_file *mfp, void * p){

seq_printf(mfp, " %s \n", procBuffer);
return 0;
}
////////////////


static int forkOpener (struct inode *inode, struct file *f){
return single_open(f, offensiveMsgPrinter, NULL);

}
////////////////

static ssize_t resetter(struct file* file,const char __user *buffer,size_t size,loff_t *f_pos){
	
	size_t MsgSize = 4096, ncopied;
    procBuffer[MsgSize] = 0;
    return size;	
}

////////////////

static int openF(struct inode *mip, struct file *mfp)
{
	openFileCounter++;
	return 0;
}
////////////////


static ssize_t readerF(struct file *file, char __user *buffer, size_t size, loff_t *offset){

size=4096;

    if (copy_to_user(buffer, MsgBuffer, size)) {
        return -EFAULT;
    }

return simple_read_from_buffer(buffer, size, offset, outputBuffer, 4096);
}

////////////////
static ssize_t readerF_(struct file *file, char __user *buffer, size_t size, loff_t *offset)
{

size=4096;

    if (copy_to_user(buffer, offensiveMsg, size)) {
        return -EFAULT;
    }

  
size_t datalen= strlen(offensiveMsg);


return simple_read_from_buffer(buffer, size, offset, offensiveMsg,datalen);

	return simple_read_from_buffer(buffer, size, offset, MsgBuffer, 4096);

}

////////////////

static ssize_t Writer(struct file *file, const char __user *buffer, size_t size, loff_t *offset)
{
	
	size_t MsgSize = 4096, ncopied;
   

    printk("Write dev: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));

    if (size < MsgSize) {
        MsgSize = size;
    }



    ncopied = copy_from_user(MsgBuffer, buffer, MsgSize);


MsgBuffer[MsgSize]= 0;
  outputBuffer[MsgSize] = 0;

    printk("Message: %s\n", MsgBuffer);
    printk("output: %s\n", outputBuffer);

    return size;
}

////////////////
static ssize_t Writer_(struct file *file, const char __user *buffer, size_t size, loff_t *offset)
{
	size_t MsgSize = 4096, ncopied;
  

  

    if (size < MsgSize) {
        MsgSize = size;
    }

    ncopied = copy_from_user(myKey, buffer, MsgSize);
    myKey[MsgSize] = 0;

    printk("key: %s\n", myKey);

    return size;
	
}
////////////////

static int procPrinter(struct seq_file *f, void * p){


if (strcmp(myKey, procBuffer)==0)
{
seq_printf(f, "%s\n", MsgBuffer   );}

else   seq_printf(f, "%s\n", outputBuffer  );
 return 0;
}
////////////////

static int procOpener_ (struct inode *inode, struct file *f){
return single_open(f, procPrinter, NULL);

}
////////////////

static ssize_t procWriter_(struct file* file,const char __user *buffer,size_t size,loff_t *f_pos){
return 0;
}
////////////////

static int release(struct inode *mip, struct file *mfp)
{
   rc4(MsgBuffer,key, outputBuffer, 100);
	return 0;
}
////////////////
static struct file_operations CipherHandler = 
{
	.read = readerF,
	.write = Writer,
	.open = openF,
	.release = release,
};
////////////////
static struct file_operations keyCipherHandler = 
{
	.read = readerF_,
	.write = Writer_,
	.open = openF,
	.release = release,
};
////////////////
static const struct file_operations procCipherHandler ={
.owner= THIS_MODULE,
.open = forkOpener,
.read = seq_read,
.llseek= seq_lseek,
.release= single_release,
.write= resetter,
};
////////////////
static const struct file_operations procCipherHandler_ ={
.owner= THIS_MODULE,
.open = procOpener_,
.read = seq_read,
.llseek= seq_lseek,
.release= single_release,
.write= procWriter_,
};
////////////////

static int __init initEncryptMsg(void)
{

int majorNum;
majorNum= register_chrdev_region(MKDEV(1024,0),2,"cipher");

int majorNum2;
majorNum2= register_chrdev_region(MKDEV(1024,1),2,"cipher_key");

cdev_init(&msgEnc[0].cdev, &CipherHandler);
cdev_add(&msgEnc[0].cdev, MKDEV(1024,0),1);

cdev_init(&msgEnc[1].cdev, &keyCipherHandler);
cdev_add(&msgEnc[1].cdev, MKDEV(1024,1),1);

proc_create("cipher_key",0, NULL,&procCipherHandler);
proc_create("cipher",0, NULL,&procCipherHandler_);
return 0;

}
////////////////
static void __exit cleanupEncryptMsg(void)
{

remove_proc_entry("cipher", NULL);
remove_proc_entry("cipher_key", NULL);
cdev_del(&msgEnc[0].cdev);
unregister_chrdev(MKDEV(1024,0), 2);
cdev_del(&msgEnc[1].cdev);
unregister_chrdev(MKDEV(1024,1), 2);
}


module_init(initEncryptMsg);
module_exit(cleanupEncryptMsg);