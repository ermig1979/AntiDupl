![AntiDupl screenshot](https://ermig1979.github.io/AntiDupl/data/help/english/files/MainForm.png)

Typically, modern computer users have large collections of images in various formats. 
And then more these collections, then more likely to have the large number of duplicates. 
The natural desire of the user is to get rid of them. However, if the collection is large 
enough, do this manually is a very tedious and unproductive work. AntiDupl.NET program 
will help you automate this process. It can find and display duplicate images in the main 
graphic formats: JPEG, GIF, TIFF, BMP, PNG, EMF, WMF, WEBP, EXIF, ICON, JP2, PSD, DDS, 
HEIF, HEIC, TGA, AVIF and JXL. 
The comparison is based on the contents of the files, so the program can find not only 
almost identical, but similar images. In addition, the program can find images with some 
types of defects.

[AntiDupl.NET](http://ermig1979.github.io/AntiDupl) program is free and open-source software. 
It is simple to use, has high speed and accuracy of work, supports Russian and English interface.

Building AntiDupl.NET
=====================
1. Download Visual Studio 2022
e.g. the Community Edition:

	https://visualstudio.microsoft.com/vs/community/
	
2. In the Visual Studio Installer select following workloads
- .NET Desktop development
- Desktop development with C++
Info: Current latest default toolset is v143
3. Download [vcpkg](https://github.com/Microsoft/vcpkg) dependency manager,
e.g. by using git:
```
	git clone https://github.com/Microsoft/vcpkg.git
```
4. Install [vcpkg](https://github.com/Microsoft/vcpkg):
```
	cd vcpkg
	./bootstrap-vcpkg.bat
	./vcpkg integrate install
```
5. Open Visual solution file
```
	src\AntiDupl.sln
```
6. Start building AntiDupl.NET in Visual Studio.
Depended libraries will be automatically loaded and build by vcpkg

Command line tool AntiDuplX
===========================
There is a command line tool [AntiDuplX](https://github.com/ermig1979/AntiDuplX) to search simular and damaged images. 
This is standalon project which uses similar algorithm of image comparison. It works for Linux and Windows.