from quadstick.rc.frsky import Taranis as Controller

controller = Controller(('Altitude hold', 'Position hold', 'Autopilot'))

while (controller.running()):

    pitch, roll, yaw, climb, switch = controller.poll()

    print(pitch, roll, yaw, climb, switch)
