import sys
import BaseHTTPServer
import os
from SimpleHTTPServer import SimpleHTTPRequestHandler
import random
import time
from threading import Thread
import parser
import data
import Queue

lastdata = {}
for i in range(70):
	try:
		#print i, data.datatype(i).name
		if(data.datatype(i).active == 1):
			lastdata[i] = [data.datatype(i).name,0]
			print lastdata[i]
	except:
		print "nothing"
		pass

q = Queue.Queue()

HandlerClass = SimpleHTTPRequestHandler
ServerClass = BaseHTTPServer.HTTPServer
Protocol = "HTTP/1.0"

speed = 11

if sys.argv[1:]:
	port = int(sys.argv[1])
else:
	port = 9999
server_adress = ('127.0.0.1', port)
going = 1
count = 0

class MyHandler(SimpleHTTPRequestHandler):

	def do_OPTIONS(self):
		self.send_response(200,"ok")
		self.send_header("Access-Control-Allow-Methods",'GET','OPTIONS')
		self.send_header("Access-Origin-Allow-Origin", '*')
		self.send_header('Access-Control-Allow-Headers', 'X-Request, X-Requested-With')
		self.send_header('Content-Length','0')
		self.end_headers()
		return
	def do_GET(self):
		print "getting!"
		try:
			if self.path == "/test.xml":
				xmlstr = '<?xml version="1.0" encoding="UTF-8" ?>\n<Car><variables><speed>' + str(speed) + '</speed><force><X>' + str(11+random.random()) + '</X><Y>21</Y><Z>9</Z></force><heading>180</heading></variables></Car>'
				self.send_response(200)
				self.send_header('Content-type', 'text/xml')
				self.send_header("Access-Origin-Allow-Origin", '*')
				self.end_headers()
				self.wfile.write(xmlstr)
				return
			if self.path == "/data.xml":
				xmlstr = '<?xml version="1.0" encoding="UTF-8" ?>\n<Car><Variables>'
				for i in lastdata:
					xmlstr = xmlstr +  "<"+ str(lastdata[i][0])+ ">"+str(lastdata[i][1])+ "</" + str(lastdata[i][0]) + ">\n"
				xmlstr = xmlstr + "</Variables></Car>"
				self.send_response(200)
				self.send_header('Content-type', 'text/xml')
				self.send_header("Access-Origin-Allow-Origin", '*')
				self.end_headers()
				self.wfile.write(xmlstr)
				return
			elif self.path.endswith(".xml"):
				f = open(os.curdir + os.sep + self.path)
				self.send_response(200) 
				self.send_header("Content-type", 'text/xml')
				self.end_headers()
				self.wfile.write(f.read())
				f.close()
				return
			elif self.path.endswith(".html"):
				print os.curdir +self.path
				f = open(os.curdir + self.path)
				self.send_response(200) 
				self.send_header("Content-type", 'text/html')
				self.end_headers()
				self.wfile.write(f.read())
				f.close()
				return
			return
		except IOError:
			self.send_error(404,'File Not Found: %s' % self.path)
	def do_POST(self):
		global rootnode

print "Starting..."
p = parser.parser()
HandlerClass.protocol_version = Protocol
httpd = ServerClass(server_adress, MyHandler)
sa = httpd.socket.getsockname()
going = True
def webserve():
	while going:
		httpd.handle_request()
		
Thread(target=webserve).start()

try:
	print "Serving HTTP on", sa[0], ":", sa[1], "..."
	p = parser.parser(queue=q)
	print "Created parser"
	p.daemon = True
	print "made parser into daemon"
	print("what!")
	p.start()
	httpd.blocking = False
	while True:
		try:
			gotten = q.get(False)
			lastdata[gotten[0]][1] = gotten[1]
			print "Queue:", gotten
		except Queue.Empty:
			pass
			#print "Empty"
		#print "Handling request:"
		#httpd.handle_request()
		#print "Handled"
		time.sleep(0.01)	

except (KeyboardInterrupt,SystemExit):
	p.close()
	httpd.socket.close()
	print "Quitting"
	print lastdata
	quit()
	
