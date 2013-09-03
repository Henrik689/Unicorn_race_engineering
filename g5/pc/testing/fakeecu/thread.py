import time
import datetime
import threading

def compute(x):
    time.sleep(x)
    return x*x

# Include a timestamp in the log message
def log(message):
    now = datetime.datetime.now().strftime("%H:%M:%S")
    print "%s %s" % (now, message)

# Each compute request gets its own thread
class ComputeThread(threading.Thread):
    def __init__(self, value):
        threading.Thread.__init__(self)
        self.value = value
    def run(self):
        result = compute(self.value)
        log("%s -> %s" % (self.value, result))

def main():
    log("starting main thread")
    ComputeThread(3).start()
    ComputeThread(2.5).start()
    ComputeThread(1).start()
    log("ending main thread")
    
if __name__ == "__main__":
    main()

