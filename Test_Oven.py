from datetime import datetime 
from matplotlib.animation import FuncAnimation
from itertools import count
from matplotlib.axis import YAxis 
from serial import Serial
from tkinter.filedialog import askdirectory
import time
import matplotlib.pyplot as plt 
import serial 
import random
import numpy as np
import csv
from MBTempCoef import MBTemp
from tkinter import *

def Animate(*args):
    value_temp = column2_temp[0]
    list_inf = int(len(total_ports) - 1) 
    inf = 0
    if not (column1_time[-1] == "time") and float(column1_time[-1]) >= read_duration : 
        pause = False
        ax.clear()
        save_csv_file(column1_time, column2_temp)
    else:
        ax.clear()
        ax.set_yscale("log")
        plt.ylabel('Temperature [°C]')
        plt.xlabel('Time [sec]')
        list_inf = [0]
        column1_time.append((time.time() - start_time))
        for x in range (len(column2_temp)):
            time.sleep(time_sec)       
            column2_temp[x].append(float(mb.ReadTemp(x)))
        for z in range (len(column2_temp)):
            ax.plot(column1_time[1:], column2_temp[z][1:], label = 'temp {}'.format(z))
        plt.legend()
        plt.grid()

def save_csv_file(column1_time, column2_temp):
    name = path + file_name+'.csv'
    with open(name , 'w') as f: 
        a = ''
        print((len(column1_time)), (len(column2_temp[0])))
        for index in range(len(column1_time)):
            a += f"{column1_time[index]},{','.join(str(column2_temp[i][index]) for i in range(len(column2_temp)))}\n"
        f.write(a)
        f.close()
    plot_graph(column1_time, column2_temp, name) 
    time.sleep(time_sec)
    return "Graph file named '{}' saved successfully!".format(name)

def plot_graph(column1_time, column2_temp, name):
    fig, ax = plt.subplots(1, 1, figsize=(20, 10))
    ax.locator_params(axis='y', tight=True, nbins=15)
    ax.locator_params(axis='x', tight=True, nbins=20)
    ax.set_yscale("log")
    for z in range (len(column2_temp)):
        ax.plot(column1_time[1:], column2_temp[z][1:], label = 'temp {}'.format(z))
        plt.legend()
    plt.xlabel('Time [s]')
    plt.ylabel('Temperature [°C]')
    plt.grid()
    plt.savefig(name+'.jpg')
    plt.close()
    end()

def end ():
    print('Acsition save as sucefull')
    exit()


if __name__ == '__main__':
    print('Step 1 - Insert ports module andress.')
    address = (int(input('Insert module andress:')))
    mb = MBTemp(address)

    
    
    print('Step 2 - Insert ports that will be measured and duration test.')
    total_ports = []
    read_duration = int(input('Duration time test:'))
    ports_conected = (input('Insert ports:'))
    while ports_conected != '' : 
        total_ports.append(int(ports_conected))
        ports_conected = input('Insert ports:')
        print(ports_conected)
    column1_time = ['time']
    column2_temp = []
    print('Setep 3 - Insert folder location and name file')
    path = (askdirectory(title='Select Folder')+'/')
    file_name = input('Insert file name:')
    name = path + file_name+'.csv'
    Tk().destroy
    for i in total_ports:
        column2_temp.append(['temperature%d'%i])  
    time_sec = 0.3
    fig, ax = plt.subplots(1, 1, figsize=(10, 5))
    ani = FuncAnimation(fig=fig, func=Animate, interval=100, repeat = True)
    start_time = time.time() 
    plt.show()
    Animate ()