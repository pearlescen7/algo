from pynput.keyboard import Key, Controller, Listener
from pynput.mouse import Button, Controller
import time

mouse = Controller()
keyboard = Controller()

def hit_mouse():
	mouse.press(Button.left)
	mouse.release(Button.left)

def on_press(key):
	if(key == Key.space):
		time.sleep(3)
		counter = 0
		while(counter < times):
			hit_mouse()
			counter += 1
			time.sleep(seconds)
		print("="*80)
		print("Tıklama tamamlandı!\nÇıkmak için esc'ye basabilirsiniz.\nSpace'e basarak tekrar başlatabilirsiniz.")

def on_release(key):
	if(key == Key.esc):
		print("="*80)
		print("Makro durduruldu.")
		print("="*80)
		time.sleep(3)
		exit()

times = int(input("Tıklama sayısı: "))
seconds = float(input("Saniye aralığı: "))
print("Space tuşuna bastıktan 3 saniye sonra program başlayacak...")

with Listener(on_press=on_press, on_release=on_release) as listener:
    listener.join()