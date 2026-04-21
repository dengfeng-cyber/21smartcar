import pyb
import sensor, image, time, math
import os, tf

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)
sensor.set_auto_exposure(False, exposure_us=200)
sensor.skip_frames(time=1000)

sensor.set_hmirror(True)
sensor.skip_frames(time=300)
sensor.set_vflip(True)
clock = time.clock()

net_path = "new51.tflite"
labels = [line.rstrip() for line in open("/sd/21smcar.txt")]
net = tf.load(net_path, load_to_fb=True)

while(True):
    img = sensor.snapshot()
    clock.tick()

    # 直接对整个画面进行分类，不框矩形、不裁剪 ROI
    for obj in tf.classify(net, img, min_scale=1.0, scale_mul=0.5, x_overlap=0.0, y_overlap=0.0):
        sorted_list = sorted(zip(labels, obj.output()), key = lambda x: x[1], reverse = True)
        # 只打印准确率最高的结果
        print("%s = %f" % (sorted_list[0][0], sorted_list[0][1]))

    print("FPS:", clock.fps())
