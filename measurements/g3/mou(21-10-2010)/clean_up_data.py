#!/usr/bin/env python
#-*- coding:utf-8 -*-

from pylab import *
import codecs

sd_data = loadtxt('data_fra_sd.txt')

olie, vand2, vand1 = rot90(sd_data.reshape(len(sd_data)/3, 3))

vand1 = 138.34*exp(-0.015*(vand1/4.))
vand2 = 138.34*exp(-0.015*(vand2/4.))

x = range(len(vand1))

plot(x, vand1, label=u'Vand ud af køler')
plot(x, vand2, label=u'Vand ind i køler')
plot(x, olie, label='Olie tryk')

ylim( 0, 200)
grid()
legend()
title('SD-kort data mou 21/10-2010 (data_fra_sd.txt)')

savetxt('sd_data.csv', flipud(rot90(vstack([vand1,vand2,olie]))), delimiter=',')
f = open('sd_data.csv', 'r')
tmp = f.read()
f.close()
f = codecs.open('sd_data.csv', encoding='utf-8', mode='w')
f.write(u'Vand ud af køler, Vand ind i køler, Olie\n')
f.write(tmp)
f.close()
show()
