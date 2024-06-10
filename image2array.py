# -*- coding: utf-8 -*-
"""
Created on Thu May  5 13:07:03 2022

@author: Lukas Schnittcher
usage:   Beim ausführen der Datei wird ein GUI-Fenster geöffnet, mit dem es möglich ist 
         ein Grauwert-Bild (jpg / png) einzulesen um die Bildpixel dann als komplexe Werte 
         in einem 1D-Vektor in einem .txt-File zu speichern 
         
         Mit umkehrung 
         
         Feature quadratische Auflösung 
"""

"""
from PySimpleGUI import Text, Input, Button, Exit, Window, WIN_CLOSED
from math import sqrt, log2
from numpy import resize, asarray, uint8
from PIL import Image
"""

from PySimpleGUI import Text, Button, Window, WIN_CLOSED, FileBrowse, Listbox, Popup
from math import sqrt, log2
from numpy import resize, asarray, uint8
from PIL import Image
from subprocess import Popen
import os
from sys import platform


def load_greyscale(path):
    return Image.open(path).convert('L')

def resize_image(image, size):
    if size == 0:
        if(image.size[0] >= image.size[1]):
            powerof2 = log2(image.size[1])
        else:
            powerof2 = log2(image.size[0])
        return image.resize((2**int(powerof2), 2**int(powerof2)), Image.ANTIALIAS)
    else:
        return image.resize((size, size), Image.ANTIALIAS)   

def image_to_array(image):
    return resize(asarray(image.getdata(), dtype= uint8), (image.size[1], image.size[0]))

def write_array(array, path):
    array1D = resize(array, (int(len(array)**2),))
    with open(path, 'w') as f:
        f.write(str(len(array1D)) + "\n")
        for i in range(0, len(array1D)):
            f.write(str(i) + "\t" + str(array1D[i]) + " " + "0\n")
        f.close()

def array_to_image(array):
    if(type(array.size) == int):
        resolution = int(sqrt(len(array)))
        array_2d = resize(array, (resolution, resolution))
        return Image.fromarray(array_2d, mode='L')
    else:
        return Image.fromarray(array, mode='L')
  
def read_array(path):
    array1D = []
    with open(path, "r") as f:
        lines = f.readlines()
        lines.pop(0)
        for line in lines:
            value = round(float(line.split()[1]))
            if(value < 0):
                array1D.append(0)
            elif(value > 255):
                array1D.append(255)
            else:
                array1D.append(value)
        f.close()
    return resize(asarray(array1D, dtype = uint8), (len(array1D),))

#sg.theme('DarkAmber')    # Keep things interesting for your users

layout = [[Text('Transformation eines (Grau-)Bilds (*.bmp, *.png, *.jpg) in eine Datendatei (*.txt) und umgekehrt'), Text('')],      
          [Text("Welches File?"), FileBrowse(key='path')],
          [Text('Bei Transformation in eine Datendatei: Welche Auflösung (quadratisch)?'), Listbox([2**x for x in range(4,13)], size = (7,4), key='resolution')],
          [Button('Transformation'), 
         # [Text(size=(40,1), key = 'Feedback_1')],
           ]]      

window = Window('Bild-Daten-Transformation', layout)      
image_types = ['png', 'peg', 'jpg', 'bmp']

while True:                             # The Event Loop
    event, values = window.read()
                      
    if event == WIN_CLOSED or event == 'Exit':
        break     

    if values['path'] != None:
        path = values['path']
        if path[-1:len(path)-4:-1] == 'txt':
            array = read_array(path)
            image = array_to_image(array)
            path = path[:-4] + "_.png"
            image.save(path)
            Popup('Transformation von Datendatei zu Bild war erfolgreich.', keep_on_top=True, title="Erfolgreich")
            
        elif path[-3:] in image_types:
            size = (values['resolution'])
            image = load_greyscale(path)
            if(size):
                image = resize_image(image, size[0])
            else:
                Popup('Bitte wähle zuerst eine Auflösung aus.', keep_on_top=True, title="Fehler")
                continue
                # image = resize_image(image, 0)    
            array = image_to_array(image)
            path = path[:-3] + "txt"
            write_array(array, path) 
            Popup('Transformation von Bild zu Datendatei war erfolgreich.', keep_on_top=True, title="Erfolgreich")
        else:
            Popup('Bitte wähle zuerst ein Bild oder eine Textdatei aus.', keep_on_top=True, title="Fehler")
            continue


window.close()



        

