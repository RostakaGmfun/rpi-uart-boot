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

def bsd_checksum(bytes):
    checksum = 0
    for b in bytes:
        checksum = ((checksum & 0xFF) >> 1) + ((checksum & 0x1) << 7)
        checksum = (checksum + ord(b)) & 0xFF
    return checksum

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

    print('INIT_ACK recieved')
    num_packets = len(bytes)/255
    if len(bytes)%255>0:
        num_packets+=1
    data_ptr = 0
    print('Image size %d (bytes)' % len(bytes))
    print('Going to send %d packets' % num_packets)
    ser.write('%c' % DATA_SYN) # instead of DATA_END
    for packet in range(0, num_packets):
        sys.stdout.write('\r[%-20s] %d%%' % ("=" * (packet/num_packets*20), packet/num_packets*100))
        sys.stdout.flush()
        ser.write('%c' % DATA_SYN)
        while ser.read(1) != chr(DATA_ACK):
            pass

        ser.write('%c' % DATA_HDR_MAGIC)
        chksum = 0
        if len(bytes) - data_ptr < 255:
            ser.write('%c' % len(bytes)-data_ptr)
            chksum = bsd_checksum(bytes[data_ptr:])
        else:
            chksum = bsd_checksum(bytes[data_ptr:data_ptr+255])
            ser.write('%c' % 255)
       # print('chksum %c' % chksum)
        ser.write('%d' % chksum)
        if len(bytes) - data_ptr < 255:
            ser.write(bytes[data_ptr:])
            data_ptr = len(bytes)
        else:
            ser.write(bytes[data_ptr:data_ptr+255])
            data_ptr+=255


    ser.write('%c' % DATA_END)
    print('Sent %d bytes' % data_ptr)


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

