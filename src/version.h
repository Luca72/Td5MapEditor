#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{

	//Date Version Types
	static const char DATE[] = "21";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] = "17.04";

	//Software Status
	static const char STATUS[] = "Release";
	static const char STATUS_SHORT[] = "r";

	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 0;
	static const long BUILD = 1;
	static const long REVISION = 3;

	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 0;
	#define RC_FILEVERSION 2,0,1,3b
	#define RC_FILEVERSION_STRING "2, 0, 1, 3b\0"
	static const char FULLVERSION_STRING[] = "2.0.1.3b";

	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;


}
#endif //VERSION_H
