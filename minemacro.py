import pywinauto
import win32api
import threading
import time, os 
from pywinauto.application import Application
from pynput.keyboard import Key, Controller, Listener

def stop_if_stop():
	with Listener(on_press=on_release) as listener:
		listener.join()

print("Mining macro loading")

keyboard = Controller()
stop = False
stop_thread = threading.Thread(target = stop_if_stop)
stop_thread_running = False

try:
	app = Application(backend="win32").connect(best_match='Minecraft 1.8')
	dlg = app.window(best_match='Minecraft 1.8')
except:
	exit()

print("Minecraft window found, waiting for user input capslock (Press esc to stop the macro or to exit now)")

def hold_mouse():
	x, y = win32api.GetCursorPos()
	pywinauto.mouse.press(coords=(x, y))

def on_press(key):
	global stop
	if(key == Key.caps_lock):
		stop_thread.start()
		keyboard.press('w')
		while(stop != True):
			hold_mouse()
			time.sleep(0.01)
		pywinauto.mouse.release()
		keyboard.release('w')
		exit()
	if(key == Key.esc):
		exit()

def on_release(key):
	global stop
	if(key == Key.esc):
		stop = True
		exit()

with Listener(on_press=on_press) as listener:
    listener.join()