#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_

class Program {
 public:
  static void run();

 private:
  Program() = delete;
  Program(Program const &rhs) = delete;
  Program &operator=(Program const &rhs) = delete;
  ~Program() = delete;

  static void loop();
}; // Program

#endif
