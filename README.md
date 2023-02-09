# WearableSensor_Macos
run on MacOs only
![image](/fileForSketching/sketch_map.png)
The main program of my UOL Year-2-Project, which links the data from sensors and the browsers for displaying.
websocket frame based on https://github.com/javapretty/websocket.git
socket instructed by https://beej.us/guide/bgnet/html/split/


INSTRUCTION:
1. choose a path that you want to store the repo.  
   ```cd path/you/want```
2. clone the main repo as well as its submodules.  
   ```git clone git@github.com:UOL-Y2P-WearableSensor/WearableSensor_Macos.git --recurse-submodules```  
   if you have authentication failure, refer to "https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent", namely creating a new SSH key and then adding it to your GitHub account.
3. If you have clone the repo correctly, then you can run the main.cpp. Luckily, you will see:  
   ```[info] success to run server!```
4. Then, open the browser and go "127.0.0.1:3490"(localhost), you shall see something different!  
