# nameCorrector
Simple app to correct names of any objects (files, directories)

## Who needs nameCorrector
Book-keepers, secretaries, and others who has to work with many files. nameCorrector is intended to help to keep order in the file storage.

## How to get
You can use the code below:
    
    git clone https://github.com/VMaksimN/nameCorrector.git
    
Just paste it in the command line and press Enter

You can also download the ZIP file  (just click _**Code**_ button)

## How to get dependencies

### g++
You can use the code below:

Fedora based distributions:
	
	sudo dnf install gcc	
	
Debian based distributions:
	
	sudo apt install gcc
	
Arch based distributions:
	
	sudo pacman -Sy gcc
	
Windows:
	
	https://gcc.gnu.org/

OR

	https://sourceforge.net/projects/mingw-w64

Visit one of these sites and download binaries or get sources and build it yourself

### CMake
You can use the code below:

Fedora based distributions:
	
	sudo dnf install cmake	
	

Debian based distributions:
	
	sudo apt install cmake
	
	
Arch based distributions:
	
	sudo pacman -Sy cmake
	
Windows:
	
	https://cmake.org/install/
	
Visit this website and follow directions

### QT5 libraries
You can use the code below:

Red hat based distributions:
	
	sudo yum groupinstall "C Development Tools and Libraries"
	sudo yum install mesa-libGL-devel	
	

Debian based distributions:
	
	sudo apt install qt5-default
	
Arch based distributions:
	
	sudo pacman -Syu fcitx5-qt
	
Windows:
	
	https://doc.qt.io/qt-5/gettingstarted.html

Visit the getting started page and follow directions
	

## How to build
You would need:

1. **g++**
2. **CMake**
3. **QT5 libraries**

You can use the code below:
	
	cd name_corrector
	mkdir build && cd build
	cmake ..
	make
	

## How to run
Find the **_name_corrector_** file in your build directory and run it, in the terminal type:
				
	./name_corrector
		
and press Enter or use your file manager
See docs/How_to_use for more information





