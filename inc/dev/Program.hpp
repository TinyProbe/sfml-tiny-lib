#pragma once
#ifndef PROGRAM_HPP
#define PROGRAM_HPP

class Program {
  Program();
  Program(Program const &rhs);
  Program &operator=(Program const &rhs);
  ~Program();

public:
  static void run();

private:
  static void loop();

};

#endif
