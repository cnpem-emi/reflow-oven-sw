#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from serial import Serial
from time import sleep

class MBTemp:

    def __init__(self, addr, port = "/dev/ttyUSB0", spd= 115200):
        self.address = addr
        self.connection = Serial(port, spd, write_timeout=0.2, timeout=0.2)


    def send_data(self, var, func):

        cs = 0
        message_received = ''

        for i in var:
            cs += i

        msg = var + [(0x100 - (cs % 0x100)) & 0xFF]

        # Prepares message to be sent
        message_to_send = bytes(msg)

        while (len(message_received) != func or (cs % 0x100) != 0):
            cs = 0

            # Send the message to MBTemp
            self.connection.flushInput()
            self.connection.write(message_to_send)

            # Read the message from MBTemp
            message_received = self.connection.read(func)
            # Verify Checksum
            for i in message_received:
                cs += i

        return(message_received)

    def ReadTemp(self, channel): #Read temperature per channel

        msg = [self.address, 0x10, 0x00, 0x01, channel]
        data = self.send_data(msg, 7)

        return((data[4]*256 + data[5])/100.0)

    def ReadAlpha(self):
        alphas = []
        for end in [0x08, 0x0d, 0x10]:
            msg = [self.address, 0x10, 0x00, 0x01, end]
            data = self.send_data(msg, 7)

            alphas.append((data[4]*256 + data[5]))
        return(alphas)

    def ReadAngularCoef(self):
        angs = []
        for ang in [0x09, 0x0e, 0x11]:
            msg = [self.address, 0x10, 0x00, 0x01, ang]
            data = self.send_data(msg, 7)

            angs.append((data[4]*256 + data[5])/100.0)
        return(angs)

    def ReadLinearCoef(self):
        lins = []
        for lin in [0x0a, 0x0f, 0x12]:
            msg = [self.address, 0x10, 0x00, 0x01, lin]
            data = self.send_data(msg, 7)

            lins.append((data[4]*256 + data[5])/100.0)
        return(lins)


    def ReadTemp_All(self): # Read temperature of all channels

        msg = [self.address, 0x12, 0x00, 0x01, 0x01]
        data = self.send_data(msg, 21)

        answer = []
        for i in range(8):
            answer.append((data[4 + 2 * i] * 256 + data[5 + 2 * i]) / 100.0)

        return (answer)

    def WriteAlpha(self, valor):

        msg = [self.address, 0x20, 0x00, 0x03, 0x08, int(valor//256), int(valor%256)]
        data = self.send_data(msg, 5)

        return (self.ReadAlpha())

    def WriteAngularCoef(self, valor):
        angular = valor*100
        msg = [self.address, 0x20, 0x00, 0x03, 0x09, int(angular//256), int(angular%256)]
        data = self.send_data(msg, 5)

        return(self.ReadAngularCoef())

    def WriteLinearCoef(self, valor):

        linear = valor * 100
        msg = [self.address, 0x20, 0x00, 0x03, 0x0a, int(linear//256), int(linear%256)]
        data = self.send_data(msg, 5)

        return (self.ReadLinearCoef())

    def close(self):
        self.connection.close()