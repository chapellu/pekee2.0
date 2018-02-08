
import pygame
import sys
import time
import serial

ser = serial.Serial('/dev/ttyACM1',115200)
pygame.init()
pygame.joystick.init()

print (pygame.joystick.get_count())

_joystick = pygame.joystick.Joystick(0)
_joystick.init()
print (_joystick.get_init())
print (_joystick.get_id())
print (_joystick.get_name())
print (_joystick.get_numaxes())
print (_joystick.get_numballs())
print (_joystick.get_numbuttons())
print (_joystick.get_numhats())
print (_joystick.get_axis(0))

axes = [ 0.0 ] * _joystick.get_numaxes()
buttons = [ False ] * _joystick.get_numbuttons()

value_motorG_old = 0
value_motorD_old = 0
value_axes_z_old = 0
old_message = "a0000"

seuil = 5
arret = False
keep_alive=True

while keep_alive:
    event = pygame.event.wait()
    pygame.event.clear()
    if event.type == pygame.QUIT:
        keep_alive = False
    elif event.type == pygame.JOYAXISMOTION: 
        while arret:
            event_arret = pygame.event.wait()
            pygame.event.clear()
            e_arret = event_arret.dict
            if 'value' in e_arret.keys():
                axes[e_arret['axis']] = e_arret['value']
                if axes[0] == 0 and axes[1] == 0 and axes[2] == 0:
                    arret = False
        
        e = event.dict
        axes[e['axis']] = e['value']
        
        if axes[0] == 0 and axes[1] == 0:
            if abs(axes[2]) > 0.1:
                value_axes_z = int(50*axes[2]*(axes[3]-1))
                if abs(value_axes_z_old-value_axes_z)>seuil:
                    message = "r{:4}".format(value_axes_z)
                    value_axes_z_old = value_axes_z
            else:
                message = "a00000000"
            
        elif axes[2]==0:
            if ((abs(axes[0]) > 0.2 or abs(axes[1])) > 0.2):
                value_motorG = int(50*axes[1]*(axes[3]-1)*(1+axes[0]))
                value_motorD = int(50*axes[1]*(axes[3]-1)*(1-axes[0]))
                if((abs(value_motorG_old - value_motorG) > seuil) or (abs(value_motorD_old - value_motorD) > seuil)):
                    message = "m{:4}{:4}".format(value_motorG,value_motorD)
                    value_motorG_old = value_motorG
                    value_motorD_old = value_motorD
            
            elif abs(axes[0]) > 0 and abs(axes[1]) == 0:
                message = "a00000000"
            
            else:
                message = "a00000000"
            
        
                        
    elif event.type in [pygame.JOYBUTTONUP, pygame.JOYBUTTONDOWN ]:
        e = event.dict
        buttons[e['button']] ^= True
        if(buttons[7] == True):
            keep_alive = False
            buttons[7] = False
        if(buttons[0] == True):
            print("STOP")
            arret = True
            message = "s00000000"
            ser.write(str.encode(message))
            buttons[0] = False
            
    if (message != old_message and not arret ):
        print(message)
        ser.write(str.encode(message))
        old_message = message
