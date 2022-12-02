#include <linux/module.h>	
#include <linux/kernel.h>
#include <linux/init.h>	

char* msg;
int num;
int i;
module_param(num,int,0660);
module_param(msg,char,0660);

int initModule(void)
{
	printk(KERN_INFO "Hello World CSCE-3402 :) \n");
	
	for(i=0;i<num;i++){
	printk(KERN_INFO "%s\n", msg) ;}
	
	return 0;
}



void cleanupModule(void)
{
	printk(KERN_INFO "Bye bye CSCE-3402 :) \n");
	/*for(i=0;i<num;i++){
	printk(KERN_INFO "%s\n", msg) ;}*/
}

module_init(initModule);
module_exit(cleanupModule);
