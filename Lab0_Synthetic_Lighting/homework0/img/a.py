import os,cv2,PIL
import numpy as np
base_path  = '/home/anti_super/Downloads/CG/Lab0_Synthetic_Lighting/homework0/img'
pics = os.listdir()
temp = []
for i in pics:
    if "jpeg" in i:
        temp.append(i)
size= 250
for i in temp:
    Img = cv2.resize(np.array(cv2.imread(  os.path.join(base_path , i) ),dtype=np.uint8) ,(size,size))
    cv2.imwrite('r'+i, Img) 
