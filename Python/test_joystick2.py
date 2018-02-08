
import pygame
import sys
import time
import serial

ser = serial.Serial('/dev/ttyACM0',115200)
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
value_xaxes_old = 0
value_yaxes_old = 0
old_message = "a0000"
seuil = 10
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
                if axes[0] == 0 and axes[1] == 0:
                    arret = False
        e = event.dict
        axes[e['axis']] = e['value']
        if abs(axes[1]) > 0.2:
            value_axes = int(axes[1]*(axes[3]-1)*50)
            if(abs(value_axes - value_yaxes_old) > seuil):
                message = "a{:4}".format(value_axes)
                value_yaxes_old = value_axes
        elif abs(axes[0]) > 0.2:
            value_axes = int(axes[0]*(axes[3]-1)*50)
            if(abs(value_axes - value_xaxes_old) > seuil):
                message = "r{:4}".format(value_axes)
                value_xaxes_old = value_axes
        else:
            message = "a0000"
                        
    elif event.type in [pygame.JOYBUTTONUP, pygame.JOYBUTTONDOWN ]:
        e = event.dict
        buttons[e['button']] ^= True
        print(e['button'])
        if(buttons[7] == True):
            keep_alive = False
            buttons[7] = False
        if(buttons[0] == True):
            arret = True
            print("STOP")
            message = "s0000"
            ser.write(str.encode(message))
            buttons[0] = False
            print("ENVOI")

    if (message != old_message and not arret ):
        print(message)
        ser.write(str.encode(message))
        old_message = message
