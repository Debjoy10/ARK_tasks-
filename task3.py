import cv2
import pyautogui
import numpy as np
import time

time.sleep(1)
im0 = pyautogui.screenshot('my_screenshot.png')
img0 =cv2.imread('my_screenshot.png')
img0 = img0[60:,70:]
pyautogui.press('enter')
time.sleep(1)

def extractEdges():
  gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
  edges = cv2.Canny(gray,50,150,apertureSize = 3)
  minLineLength = 100
  maxLineGap = 10
  lines = cv2.HoughLinesP(edges,1,np.pi/180,100,minLineLength,maxLineGap)
  edges= np.zeros((512,512,3), np.uint8)
  for line in lines:
    for x1,y1,x2,y2 in line:
      cv2.line(img,(x1,y1),(x2,y2),(0,255,0),2)
      cv2.line(edges,(x1,y1),(x2,y2),(255,255,255),2)			
      print((x1+x2)/2,(y1+y2)/2)
      print((y2-y1)/(x2-x1))
      print()
  return img 

def maxgreen():
  maxgval=0
  for i in range(rows):
    for j in range(cols):
      if img[i,j,1]>maxgval:
        maxgval=img[i,j,1]

  return maxgval

def findwall():         #0,112,124       #255,173,112
  for i in range(rows):
    for j in range(cols):
      if img[i,j,0]>=0 and img[i,j,0]<5 and img[i,j,1]>110 and img[i,j,1]<115 and img[i,j,2]>122 and img[i,j,2]<130:
        wall[i,j,0]=0
        wall[i,j,1]=0
        wall[i,j,2]=0
      if img[i,j,0]>250 and img[i,j,0]<=255 and img[i,j,1]>170 and img[i,j,1]<180 and img[i,j,2]>108 and img[i,j,2]<120:
        wall[i,j,0]=0
        wall[i,j,1]=0
        wall[i,j,2]=0

def findgreenest():
  freq = 0;
  greenest = 0
  for i in range(rows):
    for j in range(cols):
      if wall[i,j,1]>greenest:
        greenest = wall[i,j,1]
  for i in range(rows):
    for j in range(cols):
      if wall[i,j,1] != greenest:
        wall[i,j,0]=0
        wall[i,j,1]=0
        wall[i,j,2]=0
      if wall[i,j,1] == greenest:
        freq=freq+1
  return freq
          
def showimage(izx):
  cv2.imshow('image',izx)
  cv2.waitKey(500)
  cv2.destroyAllWindows()
  

def find_contours():
  wallgray = cv2.cvtColor(wall,cv2.COLOR_BGR2GRAY)
  ret,thresh = cv2.threshold(wallgray,0,255,0)
  imgcontour, contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
  
def clear_obstacle():
  print("obs")
  pyautogui.keyDown('>')
  time.sleep(1)
  pyautogui.keyUp('>')
  print('>')
  im1 = pyautogui.screenshot('my_screenshot.png')
  img1=cv2.imread('my_screenshot.png')
  img1 = img1[60:,70:]
  rows1,cols1,dim1 = img1.shape
  
  diff1 = 0
  for i in range(rows1):
    for j in range(cols1):
      for k in range(dim1):
        diff1 = diff1+img1[i,j,k]-img0[i,j,k]
  if diff1==0:
    pyautogui.keyDown('<')
    print('<')
    time.sleep(1)
    pyautogui.keyUp('<')

flag = 'q';
nflag = 0;

im = pyautogui.screenshot('my_screenshot.png')
img=cv2.imread('my_screenshot.png')
img = img[60:,70:]
rows,cols,dim = img.shape
img0= np.empty_like (img)

while(True):
  nflag+=1  
  im = pyautogui.screenshot('my_screenshot.png')

  img=cv2.imread('my_screenshot.png')
  img = img[60:,70:]
  rows,cols,dim = img.shape
  
  diff = 0
  for i in range(rows):
    for j in range(cols):
      for k in range(dim):
        diff = diff+img[i,j,k]-img0[i,j,k]
  if diff==0:
    clear_obstacle()
    continue
   
  img0[:] = img
  
  wall = img

  findwall()
  wall1 = wall
  
  cv2.imwrite('zxc.jpg',img0)
  freq = findgreenest()
  wallgray = cv2.cvtColor(wall,cv2.COLOR_BGR2GRAY)
  ret,thresh = cv2.threshold(wallgray,10,255,0)
  imgcontour, contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

  if(freq==(rows*cols)):
     pyautogui.keyDown('d')
     time.sleep(0.2)
     pyautogui.keyUp('d')
  
  MAX_area=0
  center = (0,0)
  radius = 0
  for cnt in contours:
    area = cv2.contourArea(cnt)
    if area > MAX_area:
      MAX_area = area
      (x,y),radius = cv2.minEnclosingCircle(cnt)
      center = (int(x),int(y))
      radius = int(radius)

  img = cv2.circle(wall,center,radius,(0,255,0),2)  

  #center[0] -> column     
  #center[1] -> row

  if center[0]<(int(cols/2)):
    if flag == 'l':
      pyautogui.keyDown('w')
      time.sleep(1)
      pyautogui.keyUp('w')
      flag = 'w'
    else:
      pyautogui.keyDown('a')
      time.sleep(0.1)
      pyautogui.keyUp('a')
      flag = 'r'
 

  if center[0]>(int(cols/2)):
    if flag == 'r':
      pyautogui.keyDown('w')
      time.sleep(1)
      pyautogui.keyUp('w')
      flag = 'w'
    else:
      pyautogui.keyDown('d')
      time.sleep(0.1)
      pyautogui.keyUp('d')
      flag = 'l'
  cv2.imwrite('houghlines5.jpg',img)

