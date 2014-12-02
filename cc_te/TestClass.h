/*
 * TestClass.h
 *
 *  Created on: 24.11.2014
 *      Author: nw
 */

#ifndef TESTCLASS_H_
#define TESTCLASS_H_

class TestClass {
	unsigned char x;
public:
	TestClass();
	virtual ~TestClass();

	unsigned char get(void) { return x; }
};

#endif /* TESTCLASS_H_ */
