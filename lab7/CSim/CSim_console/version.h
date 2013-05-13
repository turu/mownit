#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "03";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.01";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 5;
	static const long BUILD = 8;
	static const long REVISION = 42;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 93;
	#define RC_FILEVERSION 0,5,8,42
	#define RC_FILEVERSION_STRING "0, 5, 8, 42\0"
	static const char FULLVERSION_STRING[] = "0.5.8.42";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 8;
	

}
#endif //VERSION_H
