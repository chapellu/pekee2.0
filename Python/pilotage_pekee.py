import pygame
import sys
import time
import serial

try:
    ser = serial.Serial('/dev/ttyACM0',115200)
    pygame.init()
    pygame.joystick.init()

    _joystick = pygame.joystick.Joystick(0)
    _joystick.init()
    
    axes = [ 0.0 ] * _joystick.get_numaxes()
    axes[3] = -1
    buttons = [ False ] * _joystick.get_numbuttons()
    
    print ("INITIALISATION DONE")

    value_motorG_old = 0
    value_motorD_old = 0
    value_axes_z_old = 0
    old_message = "a00000000"
    
    friction = 0.2
    seuil = 5
    
    running = False
    keep_alive=True
    stop = False

    while keep_alive:
        event = pygame.event.wait()
        pygame.event.clear()
        if event.type == pygame.QUIT:
            keep_alive = False
        elif (running and event.type == pygame.JOYAXISMOTION): 
            e = event.dict
            axes[e['axis']] = e['value']
            if stop:
                stop = not(axes[0] == 0 and axes[1] == 0 and axes[2]==0)
            else:
                if abs(axes[1]) < friction:
                    if(abs(axes[0]) < friction and abs(axes[2]) > friction ):
                        value_axes_z = int(50*axes[2]*(axes[3]-1))
                        if abs(value_axes_z_old-value_axes_z)>seuil:
                            message = "r{:4}0000".format(value_axes_z)
                            value_axes_z_old = value_axes_z
                    else:
                        message = "s00000000"
                else:
                    if (abs(axes[0]) > friction or abs(axes[1]) > friction):
                        value_motorG = int(50*axes[1]*(axes[3]-1)*(1+axes[0]))
                        value_motorD = int(50*axes[1]*(axes[3]-1)*(1-axes[0]))
                        if((abs(value_motorG_old - value_motorG) > seuil) or (abs(value_motorD_old - value_motorD) > seuil)):
                            message = "m{:4}{:4}".format(value_motorG,value_motorD)
                            value_motorG_old = value_motorG
                            value_motorD_old = value_motorD
                    else:
                        message = "s00000000"
                        
                if message != old_message:
                    print(message)
                    ser.write(str.encode(message))
                    old_message = message
              
        elif event.type in [pygame.JOYBUTTONUP, pygame.JOYBUTTONDOWN]:
            e = event.dict
            buttons[e['button']] ^= True
            if buttons[0] == True:
                stop = True
                message = "s00000000"
                ser.write(str.encode(message))
                print("STOP")
                buttons[0] = False
            elif(buttons[7] == True):
                running = False
                message = "s00000000"
                ser.write(str.encode(message))
                print ("PROGRAM END")
                buttons[7] = False
            elif(buttons[6] == True):
                running = True
                print("START PROGRAM")
                buttons[6] = False
                
except Exception as e:
    print(e)