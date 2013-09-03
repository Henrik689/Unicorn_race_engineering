from new import classobj
import math

class data(object):
	def __init__(self):#,name,ID,active,rounddec,datalength,max,min):
		self.name = "Empty"
		self.ID = 0
		self.active = 1
		self.rounddec = 0
		self.datalength = 0
		self.max = 9999
		self.min = 0
	
	def conv(self,x):
		return round(x,self.rounddec)
		
class fuelPress(data):
	def __init__(self):
		self.name = "fuelPress"
		self.ID = 1
		self.active = 1
		self.rounddec = 2
		self.datalength = 16
	def conv(self,x):
		return round(x,self.rounddec)
		
class statusLapCount(data):
	def __init__(self):
		self.name = "statusLapCount"
		self.ID = 2
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)
		
class statusInJSum(data):
	def __init__(self):
		self.name = "statusInJSum"
		self.ID = 3
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)
		
class lastGearShift(data):
	def __init__(self):
		self.name = "lastGearShift"
		self.ID = 4
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)
			
class motorOilTemp(data):
	def __init__(self):
		self.name = "motorOilTemp"
		self.ID = 5
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)
		
class oilPressure(data):
	def __init__(self):
		self.name = "oilPressure"
		self.ID = 6
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)
		
class statusTime(data):
	def __init__(self):
		self.name = "statusTime"
		self.ID = 7
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)
		
class statusLapTime(data):
	def __init__(self):
		self.name = "statusLapTime"
		self.ID = 8
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)	
	
class gearOilTemp(data):
	def __init__(self):
		self.name = "gearOilTemp"
		self.ID = 9
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)	
		
class statusTraction(data):
	def __init__(self):
		self.name = "statusTraction"
		self.ID = 10
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)	
			
class statusGas(data):
	def __init__(self):
		self.name = "statusGas"
		self.ID = 11
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)	
		
class statusLambdaV2(data):
	def __init__(self):
		self.name = "statusLambdaV2"
		self.ID = 12
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		if(x>32768):
			x = -(65535-x)
		return round((70-x/64.0),self.rounddec)
		
class statusCamTrigP1(data):
	def __init__(self):
		self.name = "statusCamTrigP1"
		self.ID = 13
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)	
			
class statusCamTrigP2(data):
	def __init__(self):
		self.name = "statusCamTrigP2"
		self.ID = 14
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)	
			
class statusChokerAdd(data):
	def __init__(self):
		self.name = "statusChokerAdd"
		self.ID = 15
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)	
			
class statusLambdaPWM(data):
	def __init__(self):
		self.name = "statusLambdaPWM"
		self.ID = 16
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)
			
class waterTemp(data):
	def __init__(self):
		self.name = "waterTemp"
		self.ID = 17
		self.active = 1
		self.rounddec = 0
		self.datalength=16
	
	def conv(self,x):
		return round((x*(-150.0/3840.0)+120),self.rounddec)
			
class airTemp(data):
	def __init__(self):
		self.name = "airTemp"
		self.ID = 18
		self.active = 1
		self.rounddec = 1
		self.datalength=16
	
	def conv(self,x):
		return round((x*(-150.0/3840.0)+120),self.rounddec)
			
class potmeter(data):
	def __init__(self):
		self.name = "potmeter"
		self.ID = 19
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(((x-336)/26.9),self.rounddec)
			
class rpm(data):
	def __init__(self):
		self.name = "rpm"
		self.ID = 20
		self.active = 1
		self.rounddec = 0
		self.datalength=16
	
	def conv(self,x):
		return round((x*0.9408),self.rounddec)
			
class triggerErr(data):
	def __init__(self):
		self.name = "triggerErr"
		self.ID = 21
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)
			
class camAngle1(data):
	def __init__(self):
		self.name = "camAngle1"
		self.ID = 22
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)
			
class camAngle2(data):
	def __init__(self):
		self.name = "camAngle2"
		self.ID = 23
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)
			
class roadSpeed(data):
	def __init__(self):
		self.name = "roadSpeed"
		self.ID = 24
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x*0.01,self.rounddec)
			
class mapSensor(data):
	def __init__(self):
		self.name = "mapSensor"
		self.ID = 25
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x*0.75,self.rounddec)
			
class batteryV(data):
	def __init__(self):
		self.name = "batteryV"
		self.ID = 26
		self.active = 1
		self.rounddec = 1
		self.datalength=16
	
	def conv(self,x):
		return round(x*(1/210.0),self.rounddec)
			
class lambdaV(data):
	def __init__(self):
		self.name = "lambdaV"
		self.ID = 27
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		if(x>32768):
			x=-(65535-x)
		return round((70-x/64.0)/100.0,self.rounddec)
			
class load(data):
	def __init__(self):
		self.name = "load"
		self.ID = 28
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)
			
class injectorTime(data):
	def __init__(self):
		self.name = "injectorTime"
		self.ID = 29
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(-0.75*x+120,self.rounddec)
			
class ignitionTime(data):
	def __init__(self):
		self.name = "ignitionTime"
		self.ID = 30
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(-0.75*x+120,self.rounddec)
			
class dwellTime(data):
	def __init__(self):
		self.name = "dwellTime"
		self.ID = 31
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)

class gX(data):
	def __init__(self):
		self.name = "gX"
		self.ID = 32
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		if(x>32768):
			x = -(65535-x)
		return round(x*(1/16384.0),self.rounddec)

class gY(data):
	def __init__(self):
		self.name = "gY"
		self.ID = 33
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		x = x+.0
		if(x>32768):
			x = -(65535-x)
		return round(x*(1/16384.0),self.rounddec)

class gZ(data):
	def __init__(self):
		self.name = "gZ"
		self.ID = 34
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		if(x>32768):
			x = -(65535-x)
		return round(x*(1/16384.0),self.rounddec)

class motorFlags(data):
	def __init__(self):
		self.name = "motorFlags"
		self.ID = 35
		self.active = 1
		self.rounddec = 2
		self.datalength=8
	
	def conv(self,x):
		return round(x,self.rounddec)

class outBits(data):
	def __init__(self):
		self.name = "outBits"
		self.ID = 36
		self.active = 1
		self.rounddec = 2
		self.datalength=8
	
	def conv(self,x):
		return round(x,self.rounddec)

class time(data):
	def __init__(self):
		self.name = "time"
		self.ID = 37
		self.active = 1
		self.rounddec = 2
		self.datalength=8
	
	def conv(self,x):
		return round(x,self.rounddec)

class gearUp(data):
	def __init__(self):
		self.name = "gearUp"
		self.ID = 38
		self.active = 1
		self.rounddec = 2
		self.datalength=8
	
	def conv(self,x):
		return round(x,self.rounddec)

class fWheelL(data):
	def __init__(self):
		self.name = "fWheelL"
		self.ID = 39
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)

class fWheelR(data):
	def __init__(self):
		self.name = "fWheelR"
		self.ID = 40
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)

class bWheelL(data):
	def __init__(self):
		self.name = "bWheelL"
		self.ID = 41
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)

class bWheelR(data):
	def __init__(self):
		self.name = "bWheelR"
		self.ID = 42
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)

class gearBoardTemp(data):
	def __init__(self):
		self.name = "gearBoardTemp"
		self.ID = 52
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		try:
			Resistance = ((1024000.0/(1024.0-x)) - 10000)
			Temp = math.log(Resistance)
			Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp))
			x = Temp - 273.15
		except ValueError:
			print "ERROR!!!!!!!!!"
			print Resistance
			print x
			print "."
			print "."
			print "."
			print "."
			print "."
			print "."
			print "."
			print "."
		return round(x,self.rounddec)

class oilPress(data):
	def __init__(self):
		self.name = "oilPress"
		self.ID = 56
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)

class waterInlet(data):
	def __init__(self):
		self.name = "waterInletTemp"
		self.ID = 57
		self.active = 1
		self.rounddec = 0
		self.datalength=16
	
	def conv(self,x):
		return round(127.5*math.exp(-0.003286*x),self.rounddec)

class waterOutlet(data):
	def __init__(self):
		self.name = "waterOutlet"
		self.ID = 58
		self.active = 1
		self.rounddec = 0
		self.datalength=16
	
	def conv(self,x):
		return round(127.5*math.exp(-0.003286*x),self.rounddec)

class gpsSat(data):
	def __init__(self):
		self.name = "gpsSat"
		self.ID = 59
		self.active = 1
		self.rounddec = 2
		self.datalength=8
	
	def conv(self,x):
		return round(x,self.rounddec)

class gpsSpeed(data):
	def __init__(self):
		self.name = "gpsSpeed"
		self.ID = 60
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)

class gpsTimeH(data):
	def __init__(self):
		self.name = "gpsTimeH"
		self.ID = 61
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)

class gpsTimeM(data):
	def __init__(self):
		self.name = "gpsTimeM"
		self.ID = 62
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)

class gpsTimeS(data):
	def __init__(self):
		self.name = "gpsTimeS"
		self.ID = 63
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)

class gearNeutral(data):
	def __init__(self):
		self.name = "gearNeutral"
		self.ID = 64
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		if(x>100):
			x = 1
		return round(x,self.rounddec)

class gearEst(data):
	def __init__(self):
		self.name = "gearEst"
		self.ID = 65
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)

class debug(data):
	def __init__(self):
		self.name = "debug"
		self.ID = 66
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)

class valueIdLength(data):
	def __init__(self):
		self.name = "valueIdLength"
		self.ID = 67
		self.active = 1
		self.rounddec = 2
		self.datalength=16
	
	def conv(self,x):
		return round(x,self.rounddec)










	
def class_with_method(conv):
	class klass: pass
	setattr(klass, conv.__name__, conv)
	print conv.__name__
	print "made it"
	return klass
	
	
#Foo = class_with_method('say_foo': lambda print 'foo')
#Foo = class_with_method({"sho": {lambda self, x: x*2}})
modes = dict()
foo ={'func': lambda x: x*2}

'''
class Fuelpress(data):
	def __init__(self):
		self.name = "Fuel Press"
		self.ID = 1
		self.active = 1
		self.rounddec = 2
	def conv(self,x):
		return round(x,self.rounddec)*2
'''
		
class datatype(object):

	def __init__(self,key):
		
		if(key == 0):
			pass
		elif(key == 1):
			self.__class__ = fuelPress
		elif(key == 2):
			self.__class__ = statusLapCount
		elif(key == 3):
			self.__class__ = statusInJSum
		elif(key == 4):
			self.__class__ = lastGearShift
		elif(key == 5):
			self.__class__ = motorOilTemp
		elif(key == 6):
			self.__class__ = oilPressure
		elif(key == 7):
			self.__class__ = statusTime
		elif(key == 8):
			self.__class__ = statusLapTime
		elif(key == 9):
			self.__class__ = gearOilTemp
		elif(key == 10):
			self.__class__ = statusTraction
		elif(key == 11):
			self.__class__ = statusGas
		elif(key == 12):
			self.__class__ = statusLambdaV2
		elif(key == 13):
			self.__class__ = statusCamTrigP1
		elif(key == 14):
			self.__class__ = statusCamTrigP2
		elif(key == 15):
			self.__class__ = statusChokerAdd
		elif(key == 16):
			self.__class__ = statusLambdaPWM
		elif(key == 17):
			self.__class__ = waterTemp
		elif(key == 18):
			self.__class__ = airTemp
		elif(key == 19):
			self.__class__ = potmeter
		elif(key == 20):
			self.__class__ = rpm
		elif(key == 21):
			self.__class__ = triggerErr
		elif(key == 22):
			self.__class__ = camAngle1
		elif(key == 23):
			self.__class__ = camAngle2
		elif(key == 24):
			self.__class__ = roadSpeed
		elif(key == 25):
			self.__class__ = mapSensor
		elif(key == 26):
			self.__class__ = batteryV
		elif(key == 27):
			self.__class__ = lambdaV
		elif(key == 28):
			self.__class__ = load
		elif(key == 29):
			self.__class__ = injectorTime
		elif(key == 30):
			self.__class__ = ignitionTime
		elif(key == 31):
			self.__class__ = dwellTime
		elif(key == 32):
			self.__class__ = gX
		elif(key == 33):
			self.__class__ = gY
		elif(key == 34):
			self.__class__ = gZ
		elif(key == 35):
			self.__class__ = motorFlags
		elif(key == 36):
			self.__class__ = outBits
		elif(key == 37):
			self.__class__ = time
		elif(key == 38):
			self.__class__ = gearUp
		elif(key == 39):
			self.__class__ = fWheelL
		elif(key == 40):
			self.__class__ = fWheelR
		elif(key == 41):
			self.__class__ = bWheelL
		elif(key == 42):
			self.__class__ = bWheelR
		elif(key == 52):
			self.__class__ = gearBoardTemp
		elif(key == 56):
			self.__class__ = oilPress
		elif(key == 57):
			self.__class__ = waterInlet
		elif(key == 58):
			self.__class__ = waterOutlet
		elif(key == 59):
			self.__class__ = gpsSat
		elif(key == 60):
			self.__class__ = gpsSpeed
		elif(key == 61):
			self.__class__ = gpsTimeH
		elif(key == 62):
			self.__class__ = gpsTimeM
		elif(key == 63):
			self.__class__ = gpsTimeS
		elif(key == 64):
			self.__class__ = gearNeutral
		elif(key == 65):
			self.__class__ = gearEst
		elif(key == 66):
			self.__class__ = debug
		elif(key == 67):
			self.__class__ = valueIdLength
			
		
		
		if(key is not 0):
			self.__init__()
	
		
kl = datatype(3)
#m = Foo()
if __name__ == "__main__":
	for i in range(67):
		try:
			print i,"\t", datatype(i).conv(2.12321)
		except:
			print "Nothing"