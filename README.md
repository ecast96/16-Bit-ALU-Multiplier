# 16-Bit ALU Multiplier

Program uses Booth's multiplication algorithm for unsigned numbers.

Values are stored in vectors of boolean values:
 - MD = Multiplicand
 - MQ = Multiplier
 - AC = Accumulator
 - PR = Product
 
 0th index in vector is 0th spot in 16-bit number.
 
 Counter is used to show steps in algorithm.
 
 ### How to input values
 In [main](SimulationProgram_CSCI113/SimulationProgram_CSCI113/main.cpp):
  - Change vector MD's values to any 16-bit number for the Multiplicand
  - Change vector MQ's values to any 16-bit number for the Multiplier
  
  [Sample Output](SampleOutput.pdf)

