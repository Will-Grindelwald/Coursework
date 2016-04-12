#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
from PIL import Image

n = len(sys.argv)
box = [[438, 312, 438 + 1120, 312 + 840], [1950, 312, 1950 + 1120, 312 + 840], [438, 1326, 438 + 1120, 1326 + 840], [1950, 1326, 1950 + 1120, 1326 + 840]]
j = 1

for i in range(1,n):
    path = sys.argv[i]
    imlist = os.listdir(path)
    os.mkdir(path+'/deal')
    for imname in imlist:
        im = Image.open(path+'/'+imname)
        #im.thumbnail((200, 100))
        buf1 = buf2 = buf3 = buf4 = im
        buf1.crop(box[0]).save(path+'/deal/'+str(j)+'.jpg', 'JPEG')
        j+=1
        buf2.crop(box[1]).save(path+'/deal/'+str(j)+'.jpg', 'JPEG')
        j+=1
        buf3.crop(box[2]).save(path+'/deal/'+str(j)+'.jpg', 'JPEG')
        j+=1
        buf4.crop(box[3]).save(path+'/deal/'+str(j)+'.jpg', 'JPEG')
        j+=1
