//////////////////////////////////////////////////////////////////////
//                             North Carolina State University
//
//
//
//                             Copyright 2016
//
////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or modify it
// under the terms and conditions of the GNU General Public License,
// version 2, as published by the Free Software Foundation.
//
// This program is distributed in the hope it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
//
////////////////////////////////////////////////////////////////////////
//
//   Author:  Hung-Wei Tseng
//
//   Description:
//     Skeleton of KeyValue Pseudo Device
//
////////////////////////////////////////////////////////////////////////

#include "keyvalue.h"

#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/poll.h>



//------------------------------------------------structures--------------------------------------------//

struct node
{
	struct keyvalue_set inp;
	void *actual_data;
	struct node *next;
};

//------------------------------------------------Global variables--------------------------------------------//


unsigned transaction_id;
struct node *head = NULL;

//------------------------------------------------Linked List methods--------------------------------------------//



int add_node (struct keyvalue_set *ukv)
{
	struct node *new_node = kmalloc(sizeof(struct node), GFP_KERNEL);
	
	if (!new_node)
		return -1;
	
	if (access_ok( struct keyvalue_set, ukv, sizeof(struct keyvalue_set) )) {
		printk(KERN_ALERT "access ok in add_node");
		copy_from_user( &(new_node->inp), ukv, sizeof(struct keyvalue_set) );
		
		// copy actual data from user memory
		new_node->actual_data = kmalloc(((new_node->inp).size), GFP_KERNEL);
		copy_from_user( new_node->actual_data, (new_node->inp).data, (new_node->inp).size );
		printk(KERN_ALERT "Key = %d", (new_node->inp).key);
		printk(KERN_ALERT "Size = %d", (new_node->inp).size);
		printk(KERN_ALERT "Data = %s", new_node->actual_data);
		
		// insert in linked list
		new_node->next = head;
		head = new_node;
		
		return 1;
	}
	else
	{
		printk(KERN_ALERT "access NOT ok in add_node");
		return -1;
	}
}


int find_node (struct keyvalue_get *ukv)
{
	struct keyvalue_get *new_node = kmalloc(sizeof(struct keyvalue_get), GFP_KERNEL);
	
	if (!new_node)
	{
		printk(KERN_ALERT "No kernel space in find_node");
		return -1;
	}
	
	if (access_ok( struct keyvalue_get, ukv, sizeof(struct keyvalue_get) )) {
		printk(KERN_ALERT "access ok in find_node");
		copy_from_user( new_node, ukv, sizeof(struct keyvalue_get) );
		
		struct node *temp = head;
		int flag = -1;
		
		while(temp!=NULL)
		{
			if ((temp->inp).key == new_node->key)
			{
				flag = 1;
				break;
			}
			
			temp = temp->next;
		}
		
		if (flag == 1)
		{
			printk(KERN_ALERT "Key matched = %d", (temp->inp).key);
			printk(KERN_ALERT "Size = %d", (temp->inp).size);
			printk(KERN_ALERT "Data = %s", temp->actual_data);
			
			copy_to_user(new_node->size, &((temp->inp).size), sizeof(__u64));
			printk(KERN_ALERT "Size copy success!!!");
			
			copy_to_user(new_node->data, temp->actual_data, (temp->inp).size);
			printk(KERN_ALERT "Data copy success!!!");
			
			new_node->data = (temp->inp).data;
			copy_to_user(ukv, new_node , sizeof(struct keyvalue_get));
			printk(KERN_ALERT "Get struct copy success!!!");
			
			
		}
		
		return flag;
	}
	
	
}

int delete_node (struct keyvalue_delete *ukv)
{
	struct keyvalue_delete *new_node = kmalloc(sizeof(struct keyvalue_delete), GFP_KERNEL);
	
	if (!new_node)
	{
		printk(KERN_ALERT "No kernel space in find_node");
		return -1;
	}
	
    
	if (access_ok( struct keyvalue_delete, ukv, sizeof(struct keyvalue_delete) )) 
    {
		printk(KERN_ALERT "access ok in delete_node");
		copy_from_user( new_node, ukv, sizeof(struct keyvalue_delete) );
		
        printk(KERN_ALERT "Key to be deleted = %d", new_node->key);

		struct node *cur = head;
        struct node *previous = NULL;
		int flag = -1;
		
		while(cur!=NULL)
		{
			if ((cur->inp).key == new_node->key)
			{
                flag = (cur->inp).key ;
				
				if(previous == NULL)
                {
                    //current = current->next;
                    head = cur->next;
					kfree(cur);
                    
                }
                else
                {
                    previous->next = cur->next;
                    kfree(cur);
                    //current = previous->next;
                }
                break;
			}
            else
            {
                previous = cur ;
                cur = cur->next;
            }
		}
		
		return flag;
	}
	
	
}






//------------------------------------------------Main Code--------------------------------------------//

static void free_callback(void *data)
{
}

static long keyvalue_get(struct keyvalue_get __user *ukv)
{
    //struct keyvalue_get kv;
	
	int flag = find_node (ukv);
	
	if (flag == -1)
    	return -1;
    else
    	return transaction_id++;
}

static long keyvalue_set(struct keyvalue_set __user *ukv)
{
    int flag = add_node(ukv);
	
	if (flag != -1)
    	return transaction_id++;
    else
    	return -1;
}

static long keyvalue_delete(struct keyvalue_delete __user *ukv)
{
    int flag = delete_node(ukv);
    if(flag!=-1)
    {
        return transaction_id++;
    }
    else
    {
        return -1;
    }
    
}

//Added by Hung-Wei
     
unsigned int keyvalue_poll(struct file *filp, struct poll_table_struct *wait)
{
    unsigned int mask = 0;
    printk("keyvalue_poll called. Process queued\n");
    return mask;
}

static long keyvalue_ioctl(struct file *filp, unsigned int cmd,
                                unsigned long arg)
{
    switch (cmd) {
    case KEYVALUE_IOCTL_GET:
        return keyvalue_get((void __user *) arg);
    case KEYVALUE_IOCTL_SET:
        return keyvalue_set((void __user *) arg);
    case KEYVALUE_IOCTL_DELETE:
        return keyvalue_delete((void __user *) arg);
    default:
        return -ENOTTY;
    }
}

static int keyvalue_mmap(struct file *filp, struct vm_area_struct *vma)
{
    return 0;
}

static const struct file_operations keyvalue_fops = {
    .owner                = THIS_MODULE,
    
    .unlocked_ioctl       = keyvalue_ioctl,
    .mmap                 = keyvalue_mmap,
//    .poll		  = keyvalue_poll,
};

static struct miscdevice keyvalue_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "keyvalue",
    .fops = &keyvalue_fops,
};

static int __init keyvalue_init(void)
{
    int ret;

    if ((ret = misc_register(&keyvalue_dev)))
        printk(KERN_ERR "Unable to register \"keyvalue\" misc device\n");
    return ret;
}

static void __exit keyvalue_exit(void)
{
    misc_deregister(&keyvalue_dev);
}

MODULE_AUTHOR("Hung-Wei Tseng <htseng3@ncsu.edu>");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
module_init(keyvalue_init);
module_exit(keyvalue_exit);
