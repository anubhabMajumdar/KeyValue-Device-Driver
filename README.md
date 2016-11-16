# KeyValue-Device-Driver
Device driver for a simulated device storing key-value. Tested in Ubuntu.

Project Description
-------------------------------------------------
In order to keep the kernel small but also provide the flexibility and generality for machines with different tasks, most modern operating systems support "loadable kernel modules". In this way, the system can boot with a simpler, smaller kernel and then load these modules into kernel space when necessary. 

In this project, you will be implementing a loadable kernel module under Ubuntu Linux. This kernel module will create a pseudo device that maintains a key-value store in your kernel and allow different processes to share data through accessing this device. By doing so, you will learn how to write a kernel module, how to write programs working in the kernel space, how the kernel module interacts with user-space applications and the role of device drivers, library and applications in a system.

How to use
--------------------------------------------------
To begin, you may clone the repo. We have a Makefile under each subdirectory of the provided code. 
In the kernel_module directory, you may try to "make" with root permission (e.g. "sudo make"). If it success, you will see a "keyvalue.ko" under this directory. You can then type "make" using root permission to install the kernel module and the header into system directories. After you are done with this, you can now "insmod keyvalue.ko" using root. If this success, you can navigate to /dev directory and you should see a "keyvalue" device under this directory! However, at this point, this device doesn't really do anything. It's now your responsibility to endow this device with some features! You may need to unload the device by using "rmmod keyvalue" before you want to apply any change to the kernel module.

In the library directory, we provide an API for the application to interact with the device. You don't need to modify anything in the directory. However, you should read the library code to figure out how the library interact with the device to make things happen. To allow your application using this library, you need to "make all install" to have the library build and install in your system.

Finally, you will see a benchmark directory. An automated shell script test_suit.sh is provided. Running that will spwan 30,000 threads to test every aspect of the keyvalue device. It will generate appropriate messages when it finishes to let you know whether the device is working as expected or not.

