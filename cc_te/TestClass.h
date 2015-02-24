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
    virtual unsigned char methode(void) = 0;
    unsigned char get(void) { return x; }
};

class TestSibling : public TestClass {
    virtual unsigned char methode(void) { return 'h'; }
};

#endif /* TESTCLASS_H_ */