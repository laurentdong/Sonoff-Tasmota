/*
The MIT License (MIT)

Copyright (c) 2019 Laurent Dong

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
  https://github.com/laurentdong/MutexLocker

	MutexLocker.h - V1.0 - Implementation of mutex lock for Arduino

	Created by Laurent Dong, September, 2019.
	Released into the public domain.

Usage:
  Mutex m1;       //Define a global mutex variable for youself
  int function1()
  {
    ...
    {   //Here you are going to do something which might conflict with other threads (or functions in Interrupt call)
      Locker my_lock(m1);   //Declare a locker on m1, but this does not lock m1
      ...
      //Once you need to lock
      if (my_lock.lock()) { // Locks the mutex and gurantees it will be released automatically
          //Do what you want
          ...
          //You can call my_lock.unlock() to release the lock, but it is not really necessary.
          my_lock.unlock();
      }
    }  //my_lock will be released automatically here by the destruction method of Locker class
    ...
  }

*/

#ifndef MutexLocker_h
#define MutexLocker_h

#include <stddef.h>

#define INTERLOCK_WAIT_TIME 100     //Maximum microseconds waiting for the interlock
unsigned long MICROSECONDS_PASSED(unsigned long start)
{
  unsigned long currentTime = micros();
  if (currentTime > start) {
    return (currentTime - start);
  } else {
    return ()
  }
}
class Mutex
{
private:
  bool interlock;
public:
  Mutex();
  ~Mutex();
private:
  //lock() and unlock() declared as private method to prevent unexpected usages
  bool lock() {
    bool bSucceed = false;
    unsigned long starttime = micros();
    while (MICROSECONDS_PASSED(starttime) < INTERLOCK_WAIT_TIME) {
      noInterrupts();
      if (!this->interlock) {
        bSucceed = true;
        this->interlock = true;
      }
      interrupts();
      if (bSucceed) {
        break;
      }
      yield();
    }
    return bSucceed;
  };

  void unlock() {
    noInterrupts();
    this->interlock = false;
    interrupts();
  };
  friend class Locker;
};

class Locker
{
  Mutex& mutexVar;
public:
  Locker(Mutex& m){
    mutexVar = m;
  };
  ~Locker() {
    mutexVar.unlock();
  }
  bool lock() {
    return mutexVar.lock();
  }
};
