
import pygame
import sys
import time

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

keep_alive=True

while keep_alive:
    event = pygame.event.wait()
    if event.type == pygame.QUIT:
        keep_alive = False
    elif event.type == pygame.JOYAXISMOTION:
        e = event.dict
        axes[e['axis']] = e['value']
        print(axes)
    elif event.type in [pygame.JOYBUTTONUP, pygame.JOYBUTTONDOWN ]:
        e = event.dict
        print(e)
        buttons[e['button']] ^= True
