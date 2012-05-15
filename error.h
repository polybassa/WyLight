#ifndef _ERROR_H_
#define _ERROR_H_
// *** ERRORBITS
struct ErrorBits {
		char crc_failure:1;
		char eeprom_failure:1;
};
extern struct ErrorBits gERROR;

#define ErrorInit(x) \
	gERROR.crc_failure = 0; \
	gERROR.eeprom_failure = 0;

void throw_errors();
#endif /* #ifndef _ERROR_H_ */
