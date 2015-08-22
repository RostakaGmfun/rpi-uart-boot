#!/usr/bin/python

import sys
import os.path
import serial
INIT_SYN = 0x6D
INIT_ACK = 0x65
DATA_HDR_MAGIC = 0xEA
DATA_SYN = 0xAC
DATA_ACK = 0xDC
DATA_END = 0xDE
DATA_ERR = 0xEE

def dump_serial(port):
    port.flush()
    while(1):
        c = port.read(1)
        sys.stdout.write(c)
        sys.stdout.flush()

def upload(filename):
    if not os.path.isfile(filename):
        print ("No such file: %s" % filename)
        exit()
    
    bytes = open(filename, "rb").read()
    
    ser = serial.Serial('/dev/ttyUSB0', 115200)
    
    print('Sending INIT_SYN')
    ser.write('%c' % INIT_SYN)
    
    while ser.read(1)!=chr(INIT_ACK):
        pass

    print('INIT_ACK received')
    num_packets = len(bytes)/255
    if len(bytes)%255>0:
        num_packets+=1
    data_ptr = 0
    
    print('Image size %d (bytes)' % len(bytes))
    print('Going to send %d packets' % num_packets)
    
    for packet in range(0, num_packets):
        sys.stdout.write('\r')
        sys.stdout.write('[%-50s] %d%%' % ("=" * ((packet+1)*50/num_packets), (packet+1)*100/num_packets))
        sys.stdout.flush()
        ser.write('%c' % DATA_SYN)

        ser.write('%c' % DATA_HDR_MAGIC)
        
        if len(bytes) - data_ptr < 255:
            ser.write('%c' % (len(bytes)-data_ptr))
        else:
            ser.write('%c' % 255)
    
        if len(bytes) - data_ptr < 255:
            ser.write(bytes[data_ptr:])
            data_ptr = len(bytes)
        else:
            ser.write(bytes[data_ptr:data_ptr+255])
            data_ptr+=255
        
        while True:
            c = ser.read(1)
            if c == chr(DATA_ACK):
                break
            elif c == chr(DATA_ERR):
                print "\nData Error!"
                exit()
            pass

    ser.write('%c' % DATA_END)
    print('\nSent %d bytes\n' % data_ptr)
    print '='*50
    print 'Serial output'
    print '='*50
    dump_serial(ser)


def usage():
    print('rpi-uart-boot client tool')
    print('usage:')
    print('rpi-uart-boot.py [filename]')
    print('Developed by RostakaGmfun, rostawesomeg@gmail.com')

if __name__ == "__main__":
    if len(sys.argv) < 2:
        usage()
        exit()
    
    upload(sys.argv[1])

