#ifndef STARMODE_H
#define STARMODE_H
    
#if ARDUINO >= 100
 #include <Arduino.h>
 #include <string.h>
#else
 #include <WProgram.h>
#endif

class StarMode {
    public:
        StarMode();
        ~StarMode();
        void setPins();
        
        void setRegistersNull();
        void setRegistersUnits();
        void doFourProgram();
        void doFiveProgram();
        void doThreeProgram();
        void doTwoProgram();
     private:
        void _setDataInRegisters(byte *data);
};

#endif
