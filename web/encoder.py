import Adafruit_BBIO.GPIO as bbio
import Adafruit_BBIO.PWM as pwm
import time
import numpy as np

class Encoder(object):
	def __init__(self):

		self.t_s_l = 0
		self.t_s_r = 0			#initialize time span

		self.pos_current_l = 0		#the current displacement of the wheel
		self.pos_prev_l = 0		#storage for regressive wheel position

		self.pos_current_r = 0
		self.pos_prev_r = 0

		self.speed_l = 0		#linear speed (assume no slipping)
		self.speed_r = 0

		self.d = 0.042		#diameter of wheel (m)		

		self.div = 20		#encoder divisions (for shitty encoder, make sure you
			#use a capacitor or there will be "bounce" in the
			#signal, doubling the encoder hits

		self.t_current_l = 0
		self.t_prev_l = 0

		self.t_current_r = 0
		self.t_prev_r = 0

	def encoder_setup(self):
	# Set up beaglebone pins for encoder functionality

		bbio.setup("P9_11", bbio.IN)
		bbio.setup("P9_12", bbio.IN)

		bbio.add_event_detect("P9_11", bbio.RISING)
		bbio.add_event_detect("P9_12", bbio.RISING)

	def run(self):
	# Begin reading values from the encoders

			

			if(bbio.event_detected("P9_11")):
                		t_prev_l = t_current_l
                		t_current_l = time.clock()
                		t_s_l = t_current_l - t_prev_l
                		pos_prev_l = pos_current_l
                		pos_current_l = 360/div + pos_prev

                		if (t_s_l == 0):
                        		vel_l = 900
                	else:
                        	vel_l = ( pos_current_l - pos_prev_l)/(t_s_l)

				print pos_current_l
				speed_l = vel_l * 2*np.pi / 180 * d / 2
                		
				print speed_l

			if(bbio.event_detected("P9_12")):
				t_prev_r = t_current_r
				t_current_r = time.clock()
				t_s_r = t_current_r - t_prev_r
				pos_prev_r = pos_current_r
				pos_current_r = 360/div + pos_prev_r 

				if (t_s_r == 0):
					vel_r = 900
				else:
					vel_r = (pos_current_r-pos_prev_r)/(t_s_r)

				print pos_current_r
		
				speed_r = vel_r * 2*np.pi / 180 * d / 2

				print speed_r
