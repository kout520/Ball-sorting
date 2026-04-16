import sensor, time
from machine import UART

# 定义颜色的HSV范围
red = (43, 60, 40, 17, 46, 18)# 红色的HSV范围
green = (49, 93, -128, -48, 7, 86)# 绿色的HSV范围
yellow = (68, 90, -14, -37, 81, -45)# 黄色的HSV范围
lan = (0, 100, -26, 28, -128, -21)# 蓝色的HSV范围
pink = (57, 45, 48, 67, -13, 11 ) # 粉色的HSV范围
qianlan =(52, 68, 11, -1, 29, -23) # 浅粉色的HSV范围
orange = (61, 69, 20, -2, 65, 38)     # 橙色的HSV范围

# 初始化UART
uart = UART(3, 115200)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_whitebal(False)
sensor.set_auto_gain(True)

clock = time.clock()

def find_max(blobs):
    max_size = 0
    max_blob = None
    for blob in blobs:
        if blob[2] * blob[3] > max_size:
            max_blob = blob
            max_size = blob[2] * blob[3]
    return max_blob

last_send_time = time.ticks_ms()
sent_no_target = False

# 定义ROL区域的坐标和大小
rol_area = (30, 30, 80, 60)  # (x, y, width, height)

while True:
    clock.tick()
    img = sensor.snapshot()
    max_sizes = {'red': 0, 'green': 0, 'yellow': 0, 'lan': 0, 'pink': 0, 'qianlan': 0, 'orange': 0}
    colors = ['red', 'green', 'yellow', 'lan', 'pink', 'qianlan', 'orange']

    # 在ROL区域内进行颜色检测
    for color in colors:
        blobs = img.find_blobs([eval(color)], roi=rol_area)  # 仅在ROL区域内查找
        if blobs:
            max_blob = find_max(blobs)
            if max_blob:
                area = max_blob[2] * max_blob[3]
                img.draw_rectangle(max_blob.rect(), color=(255, 0, 0) if color == 'red' else
                                  (0, 255, 0) if color == 'green' else
                                  (255, 255, 0) if color == 'yellow' else
                                  (0, 0, 255) if color == 'lan' else
                                  (255, 105, 180) if color == 'pink' else
                                  (0, 255, 255) if color == 'qianlan' else
                                  (255, 165, 0))  # 橙色
                max_sizes[color] = area

    current_time = time.ticks_ms()
    if time.ticks_diff(current_time, last_send_time) >= 1000:
        target_detected = False
        max_area = 0
        max_color = '1'  # 默认值为1，表示未检测到目标
        for color, area in max_sizes.items():
            if area > max_area and area > 2300:
                max_area = area
                if color == 'red':
                    max_color = '3'
                elif color == 'green':
                    max_color = '4'
                elif color == 'yellow':
                    max_color = '5'
                elif color == 'lan':
                    max_color = '2'
                elif color == 'pink':
                    max_color = '6'  # 粉色对应的信号
                elif color == 'qianlan':
                    max_color = '7'  # 浅蓝色对应的信号
                elif color == 'orange':
                    max_color = '8'  # 橙色对应的信号（假设为8，可以根据需要修改）

        if max_color != '1':
            uart.write(max_color)
            print(max_color)
            sent_no_target = False
        elif not sent_no_target:
            uart.write('1')
            print('1')
            sent_no_target = True

        last_send_time = current_time

    # 绘制ROL区域
    img.draw_rectangle(rol_area, color=(0, 0, 255))  # 用蓝色绘制ROL区域边框
    img.draw_string(rol_area[0] + 5, rol_area[1] + 5, "ROL", color=(255, 255, 255), scale=2)  # 在ROL区域内写字

    img.draw_string(0, 0, "fps:%d" % (int(clock.fps())), color=(255, 255, 0), scale=2)
