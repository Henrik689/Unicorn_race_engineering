#!/usr/bin/env python
#-*- coding:utf-8 -*-

from pylab import *
import codecs

f = open('log1.txt', 'r')

data = f.read()
data2 = data.split('\r\n')

data3 = []

vand1 = []
vand2 = []
olie = []

for streng in data2:
    if len(streng) > 10:
        if streng[0] == '1':
            vand1.append(138.34*exp(-0.015*int(str.split(str.split(streng,',')[0],'=')[1])))
            vand2.append(138.34*exp(-0.015*int(str.split(str.split(streng,',')[1],'=')[1])))
            olie.append(int(str.split(str.split(streng,',')[2],'=')[1]))

x = range(len(vand1))

plot(x, vand1, label=u'Vand ud af køler')
plot(x, vand2, label=u'Vand ind i køler')
plot(x, olie, label='Olie tryk')

ylim( 0, 105)
grid()
legend()
title('wireless data log1 mou 21/10-2010 (log1.txt)')


savetxt('wireless_data_log1.csv', flipud(rot90(vstack([vand1,vand2,olie]))), delimiter=',')
f = open('wireless_data_log1.csv', 'r')
tmp = f.read()
f.close()
f = codecs.open('wireless_data_log1.csv', encoding='utf-8', mode='w')
f.write(u'Vand ud af køler, Vand ind i køler, Olie\n')
f.write(tmp)
f.close()

show()

