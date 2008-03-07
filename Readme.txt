Change [Configuration properties->Debugging->working directory] of TestApp and LayoutEditor to 
../Shared/$(ConfigurationName)
if you want run projects from Visual Studio.

In linux you can now build by typing:

sh bootstrap
./configure
make
make install