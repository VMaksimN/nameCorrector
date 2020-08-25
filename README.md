# nameFixer
Simple app to fix names of any objects (files, directories)

## How to get
You can use the code below:
    
    git clone https://github.com/VMaksimN/nameFixer.git
    
Just paste it in the command line and press Enter

You can also download the ZIP file  (just click _**Code**_ button)

## How to get dependencies

### g++
You can use the code below:

Fedora based distributions:
	
	sudo dnf install gcc	
	
Debian based distributions:
	
	sudo apt install gcc
	
Arch:
	
	sudo pacman -Sy gcc
	
	
### CMake
You can use the code below:

Fedora based distributions:
	
	sudo dnf install cmake	
	

Debian based distributions:
	
	sudo apt install cmake
	
	
Arch:
	
	sudo pacman -Sy cmake
	

### QT5 libraries
You can use the code below:

Red hat based distributions:
	
	sudo yum groupinstall "C Development Tools and Libraries"
	sudo yum install mesa-libGL-devel	
	

Debian based distributions:
	
	sudo apt install qt5-default
	
Arch:
	
	sudo pacman -Syu fcitx5-qt
	
## How to build
You would need:

1. **g++**
2. **CMake**
3. **QT5 libraries**

You can use the code below:
	
	cd yourFolder/nameFixer
	mkdir build && cd build
	cmake ..
	make
	
Just paste it in the command line and press Enter

## How to use
Find **_name_fixer_** file in your build directory and run it, in the terminal type:
		
	./name_fixer			
		
and press Enter or if you use a file manager with GUI then just click right button above the file icon and select **Run** option


Use _**New rule**_ button to add a pattern according to which the program will correct names

Then press _**Fix names**_ button to select a directory. 

Check out the result

If you not sure about correction just press _**Reset**_ button

_**Replace**_ usage: enter two strings (or symbols), the first will be replaced with the second

_**Remove**_ usage: enter the string (or symbol) and all occurrences of this string will be removed from the object name

_**RemoveFromTo**_ usage: enter the string and two numbers, the first and the last positions, then program remove all occurrences of the string from the object name

_**AddTo**_ usage: enter the string and one number, the position of the addition, the program add your string to the position 


