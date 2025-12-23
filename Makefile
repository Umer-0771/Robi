CC=gcc
CFLAGS=-Wall
LIBS= -lgpiod

SRC=main.c I2C.c PWM.c MotorHat.c Robot.c Sensor.c
TARGET=robi

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
