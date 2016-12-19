# GPIO-Trigger-TEST
A kernel module for GPIO switch trigger interrupt

###Before compiling...

Make sure your system has Linux header
It is usually in the '/lib/modules/(Your Kernel Version)/build' directory

###Build
    
    make
    
###Clean

    make clean
    
###Install module

    sudo insmod gpio_switch.ko
   
###Remove module

    sudo rmmod gpio_switch
