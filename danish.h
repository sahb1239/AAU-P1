/* Danske karakterer */
/* http://stackoverflow.com/questions/3213037/determine-if-linux-or-windows-in-c */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define ae "\x91"
	#define oe "\x9B"
	#define aa "\x86"
	#define AE "\x92"
	#define OE "\x9D"
	#define AA "\x8F"
#else
	#define ae "æ"
	#define oe "ø"
	#define aa "å"
	#define AE "Æ"
	#define OE "Ø"
	#define AA "Å"
#endif