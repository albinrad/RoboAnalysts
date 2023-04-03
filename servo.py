from adafruit_servokit import ServoKit
import math
import sys,tty,os,termios

kit = ServoKit(channels=16)

def calculate_center_rotation(angle, h_offset, v_offset):
    return math.tan(math.radians(abs(angle))) * h_offset + v_offset

def calculate_outer_wheel_angle(center_rotation, h_offset, v_offset):
    return math.degrees(math.atan((center_rotation + h_offset) / v_offset))

def getkey():
    old_settings = termios.tcgetattr(sys.stdin)
    tty.setcbreak(sys.stdin.fileno())
    try:
        while True:
            b = os.read(sys.stdin.fileno(), 3).decode()
            if len(b) == 3:
                k = ord(b[2])
            else:
                k = ord(b)
            key_mapping = {
                127: 'backspace',
                10: 'return',
                32: 'space',
                9: 'tab',
                27: 'esc',
                65: 'up',
                66: 'down',
                67: 'right',
                68: 'left'
            }
            return key_mapping.get(k, chr(k))
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)

# Actuation range for motors
for i in range(4):
    kit.servo[i].actuation_range = 270

# Pulse Width range for motors
for i in range(4):
    kit.servo[i].set_pulse_width_range(500, 2500)

v_offset = 100
h_offset = 60

center_rotation_dist = 7
inner_angle = 90

def update_inner_angle(change):
    global inner_angle
    inner_angle += change
    kit.servo[0].angle = inner_angle
    center_rotation = calculate_center_rotation(inner_angle, h_offset, v_offset)
    outer_angle = calculate_outer_wheel_angle(center_rotation, h_offset, v_offset)
    kit.servo[1].angle = 270 - (180 - outer_angle)
    kit.servo[2].angle = 180 - outer_angle
    kit.servo[3].angle = 270 - inner_angle

try:
    while True:
        k = getkey()
        if k == 'left':
            if inner_angle < 90:
                update_inner_angle(15)
        elif k == 'right':
            if inner_angle > -90:
                update_inner_angle(-15)
except (KeyboardInterrupt, SystemExit):
    os.system('stty sane')
    print('stopping.')