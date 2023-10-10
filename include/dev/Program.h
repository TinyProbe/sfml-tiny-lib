#ifndef SFML_DEV_PROGRAM_H_
#define SFML_DEV_PROGRAM_H_

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

#endif // SFML_DEV_PROGRAM_H_
