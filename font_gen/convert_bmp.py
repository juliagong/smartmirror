# Slightly adapted code from that provided by Chris Gregg.
# Used to generate the font configuration array from an
# inverted bitmap (.bmp) image for use in font.c (see font.c
# for details).

#!/usr/bin/env python3
import sys
DEBUG = False

def combineBytes(data,offset,numBytes):
    # little endian
    val = 0
    for i in range(numBytes):
        val += data[offset+i] << (i*8)
    return(val)

if len(sys.argv) != 2:
    print("usage:\n\t{} bmp_file".format(sys.argv[0]))
    quit()

bmp = sys.argv[1]

with open(bmp,"rb") as f:
    data = f.read()

# make sure we are reading a bmp file
if not (data[0] == ord('B') and data[1] == ord('M')):
    print("Not a recognizable .bmp file!")
    quit()

# read the file size (4 bytes) (possibly incorrect -- do not rely on it)
filesize = combineBytes(data,2,4) 
if DEBUG:
    print("file size:")
    print(filesize)

# skip 4 unused bytes

# read the pixel offset (4 bytes)
pixeloffset = combineBytes(data,10,4)
if DEBUG:
    print("pixel offset:")
    print(pixeloffset)

# number of bytes in the dib header
dib_offset = combineBytes(data,14,4)
if DEBUG:
    print("dib offset:")
    print(dib_offset)

# width
width = combineBytes(data,18,4)
if DEBUG:
    print("width:")
    print(width)

# height
height = combineBytes(data,22,4)
if DEBUG:
    print("height:")
    print(height)

# num of color planes
color_planes = combineBytes(data,26,2)
if DEBUG:
    print("color planes:")
    print(color_planes)

# num of bits per pixel
bitsperpixel = combineBytes(data,28,2)
if DEBUG:
    print("bitsperpixel:")
    print(bitsperpixel)

# compression? Should be 0
compression = combineBytes(data,30,4)
if DEBUG:
    print("compression:")
    print(compression)

# bitmap data size
bitmapSize = combineBytes(data,34,4)
if DEBUG:
    print("bitmapSize:")
    print(bitmapSize)

# horz print resolution
horizRes = combineBytes(data,38,4)
if DEBUG:
    print("horz res:")
    print(horizRes)

# vert print resolution
vertRes = combineBytes(data,42,4)
if DEBUG:
    print("vert res:")
    print(vertRes)

# colors in the palette
numColors = combineBytes(data,46,4)
if DEBUG:
    print("numColors:")
    print(numColors)

# important colors
importantColors = combineBytes(data,50,4)
if DEBUG:
    print("important colors")
    print(importantColors)

# gather data
pixels = data[pixeloffset:]

if DEBUG:
    print("pixel bytes:")
    print(len(pixels))
    print("pixels:")
    for i in range(len(pixels) // (bitsperpixel//8)):
        nextPixel = combineBytes(pixels,i*(bitsperpixel//8),(bitsperpixel//8))
        if (nextPixel & 0xffffff > 0): 
            print("X",end='')
        else:
            print(" ",end='')
        if ((i+1) % width == 0):
            print()
else: # generate the pixel map representations
    numChars = 0x7f-0x21
    print("static const font_t font_default = {")
    print("    // first char is 0x21 ('!'), last char is 0x7f (del)")
    print("    // width {}, height {}".format(width//numChars,height))
    print("        0x21, 0x7f, {}, {}, {{".format(width//numChars,height))

    # we want to compress this into a 1-bit representation
    # so, for each byte that is non-zero, that becomes one bit
    # in the representation

    nextByte = 0
    for i in range(len(pixels) // (bitsperpixel//8)):
        nextPixel = combineBytes(pixels,i*(bitsperpixel//8),(bitsperpixel//8))
        # ignore alpha
        if (nextPixel & 0x00ffffff > 0):
            nextByte |= (1 << ((7-i) % 8))
        if i % 64 == 0:
            print("        ",end='')
        if (i+1) % 8 == 0:
            print("0x{}, ".format(hex(nextByte)[2:].zfill(2)),end='')
            nextByte = 0
            if (i+1) % 64 == 0:
                print()
    print("}")

